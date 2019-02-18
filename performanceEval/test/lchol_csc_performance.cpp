//
// Created by kazem on 11/14/18.
//

#include <stdio.h>
//#include <cholmod.h>
//#include <cholmod_function.h>
#define MKL_ILP64
#define FLEXCOMPLEX
typedef int  INT;
#define MKL_INT INT

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
//#include <mkl.h>
#include <symbolic_analysis_CSC.h>
#include <inspection_DAG03.h>

#include "../src/lchol_csc_original.h"
#include "../src/lchol_csc_inspector.h"
#include "../src/lchol_csc_executor.h"

#include "util.h"

using namespace std;



//#define DEBUG 1
#define VERIFY
#define CPUTIME (SuiteSparse_time ( ))


int main(int argc, char *argv[]) {



  if(argc != 2){
    cout<<"\n\nMust be invoked like:"
          "\n  ./fs_csc_per matList&Params.txt"
          "\n  matlist.txt's first line must contain NThreads and NRuns (separated with comma),"
          "\n  the subsequent line should be the list of input matricies."
          "\n  NThreads being maximum number of threads to be used,"
          "\n  NRuns being number of runs to be averged\n\n";
    return 1;
  }

  ofstream outInsp("results/insp.csv", std::ofstream::out | std::ofstream::app);
  ofstream outExec("results/exec.csv", std::ofstream::out | std::ofstream::app);
  outInsp<<"\nL. Chol";
  outExec<<"\nL. Chol";

  std::string inputMatrix;
  int numThread=8;
  int nRuns=1;
  string parameters;
  ifstream inF(argv[1]);
  getline( inF, parameters );
  sscanf(parameters.c_str(), "%d, %d",&numThread,&nRuns);


 int innerParts = 16;//atoi(argv[3]);//Inner parts
 int divRate = 5;//atoi(argv[4]);
 int chunk = innerParts/numThread + 1;
 int levelParam = 5;// level distance
 int blasThreads = 1;


  // Looping over input matricies
  for( ; getline( inF, inputMatrix ); ){ 

    if(inputMatrix=="") break;


 std::string f1 = argv[1];
 int *colA, *rowA;
 double *valL_nonblock;
 double *valA;
 int maxSupWid, maxCol;
 size_t n, nnzA;

 //std::vector<profilingInfo> piArray;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    std::chrono::duration<double> elapsed_secondsT;
    double durationID[20] = {0.0}, durationIL[20] = {0.0}, durationE[20] = {0.0}, durationTT[20] = {0.0}, inspTT;
    double serialMedTT = 0.0, serialMedE = 0.0, serialAvgTT = 0.0, serialAvgE = 0.0;
    double durationSym = 0, duration3 = 0, duration2 = 0, duration1 = 0;

    std::cout <<"\n\n---------- Started Reading Matrix: "<< inputMatrix;
    if (!readMatrix(inputMatrix, n, nnzA, colA, rowA, valA)){
      return 1;
    }

/* int numThread = atoi(argv[2]);
 int innerParts = atoi(argv[4]);//Inner parts
 int divRate = atoi(argv[3]);// level distance
 int chunk = innerParts/numThread + 1;
 int blasThreads = 1;
 int levelParam = 5;
 std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double duration = 0;
*/
 omp_set_num_threads(numThread);

// MKL_Set_Num_Threads(numThread);
 // MKL_Set_Num_Threads(1);
// MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);

 int *prunePtr, *pruneSet;
 int *levelPtr = NULL, *levelSet = NULL, *parPtr = NULL,
   *partition = NULL;
 int nLevels = 0, nPar = 0;
 double *nodeCost;

 double *timingChol = new double[4 + numThread]();//for time measurement
 double orderingTime = 0;
 int nrelax[3] = {4, 16, 48};//TODO
 double zrelax[3] = {0.8, 0.1, 0.05};
 int status = 0;
 CSC *Amat = new CSC;
 Amat->nzmax = nnzA;
 Amat->ncol = Amat->nrow = n;
 Amat->stype = -1;
 Amat->xtype = CHOLMOD_REAL;
 Amat->packed = TRUE;
 Amat->p = colA;
 Amat->i = rowA;
 Amat->x = valA;
 Amat->nz = NULL;
 Amat->sorted = TRUE;
 BCSC *L = analyze_DAG_CSC(1, Amat, NULL, NULL, nrelax, zrelax,
                           n, prunePtr, pruneSet,
                           nLevels, levelPtr, levelSet,
                           nPar, parPtr, partition,
                           innerParts, levelParam, divRate,
                           status, maxSupWid, maxCol, orderingTime);

 valL_nonblock = new double[L->xsize]();


 std::cout<<"\n\nRunning "<<f1<<"\n";



        // # Inspector:
        std::vector<std::vector<int>> DAG;
        DAG.resize(n);
        //for(int i = 0 ; i < n ; i++ ) DAG[i].push_back(i);
        std::vector<std::set<int>> DAG_s;
        DAG_s.resize(n);
        start = std::chrono::system_clock::now();
        // Creating the DAG with generated
        /*fs_csc_inspector(n,A2->p, A2->i, DAG_s);

        int *v, *edg;
        v = new int[n+1]();
        edg = new int[nnzA]();
        int ct=0, edges=0;
        for(ct = 0, edges = 0; ct < n; ct++){
          v[ct] = edges;
          std::set<int> tms = DAG_s[ct];
          edg[edges++] = ct; 
          for (std::set<int>::iterator it= tms.begin(); it!=tms.end(); ++it)
            edg[edges++] = *it;
        }*/
        lchol_csc_inspector(n,Amat->p, Amat->i, DAG);

        int *v, *edg;
        v = new int[n+1]();
        edg = new int[nnzA]();
        int cti,edges=0;
        for(cti = 0, edges = 0; cti < n; cti++){
          v[cti] = edges;
          edg[edges++] = cti; 
          for (int ctj = 0; ctj < DAG[cti].size(); ctj++)
            edg[edges++] = DAG[cti][ctj];
        }
        v[cti] = edges;
        end = std::chrono::system_clock::now();
        elapsed_secondsT = end - start;
        inspTT = elapsed_secondsT.count();


 std::cout<<"\n\nInspector time = "<<inspTT<<"\n";


 start = std::chrono::system_clock::now();
 // build H- level set
 //Computing cost
 nodeCost = new double[n];
 int *xi = new int[2*n]();
 for (int s = 0; s < n; ++s) {
  nodeCost[s]=L->ColCount[s];
 }
 delete[]L->ColCount;
 //HACK to avoid conversion
 int *LP = new int[n+1];
 for (int m = 0; m <= n; ++m) {
  LP[m] = static_cast<int>(L->p[m]);
 }
 nLevels = getCoarseLevelSet_DAG_CSC03(n,LP,L->i,nLevels, levelPtr, levelSet,
                             nPar, parPtr, partition,
                             innerParts, levelParam, divRate,
                             nodeCost);

 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 double HD = elapsed_seconds.count();


 delete []LP;
#if 0
 bool *to_check = new bool[n]();
 for (int k = 0; k < nLevels; ++k) {
  for (int i = levelPtr[k]; i < levelPtr[k+1]; ++i) {
   assert(levelSet[i]<n);
   to_check[levelSet[i]] = true;
  }
 }
 for (int l = 0; l < n; ++l) {
  assert(to_check[l]);
 }
#endif
 CSC *A1 = ptranspose(Amat, 2, L->Perm, NULL, 0, status);
 CSC *A2 = ptranspose(A1, 2, NULL, NULL, 0, status);


    // ------ Sequentially Run
    std::cout <<"-- Running the algorithm sequentially for #"<<nRuns<<" times:\n";
    for (int l = 0; l < nRuns; ++l) {

 for (int i = 0; i < L->xsize; ++i) {
  valL_nonblock[i] = 0.0;
 }
 start = std::chrono::system_clock::now();

 lchol_csc_original(n, A2->p, A2->i, A2->x, L->p, L->i, valL_nonblock,
   prunePtr,pruneSet);

      end = std::chrono::system_clock::now();
      elapsed_secondsT = end - start;
      durationE[l] = elapsed_secondsT.count();
    }
    // Calculating Median and Average of execution times for plotting
    std::sort(durationE, durationE+nRuns);
    double medE;
    if(nRuns%2) {
      medE = durationE[(int(nRuns/2))];
    } else {
      medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
    }
    serialMedE = medE;
    std::cout <<"\n>>>>>>>>>>>> Median of Execution times = "<<medE <<"\n";


    // --------- Wavefront run
    std::cout<<"\n\n--Running the algorithm with #"
          <<numThread<<" threads in parallel for #"<<nRuns<<" times:";
    //****************Parallel H2 CSC
    for (int l = 0; l < nRuns; ++l) {

 for (int i = 0; i < L->xsize; ++i) {
  valL_nonblock[i] = 0.0;
 }
 start = std::chrono::system_clock::now();

 lchol_csc_executor(n, A2->p, A2->i, A2->x, L->p, L->i, valL_nonblock,
                        prunePtr,pruneSet,
                         nLevels, levelPtr, levelSet,
                         nPar, parPtr, partition,
                         chunk);

      end = std::chrono::system_clock::now();
      elapsed_secondsT = end-start;
        durationE[l] = elapsed_secondsT.count();
    }
    // Calculating Median and Average of execution times for plotting
    std::sort(durationE, durationE+nRuns);
    if(nRuns%2) {
      medE = durationE[(int(nRuns/2))];
    } else {
      medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
    }

    std::cout <<"\n>>>>>>>>>>>> Median of Inspector Dependence times = "<<inspTT <<"\n";
    std::cout <<">>>>>>>>>>>> Median of Inspector Build Level Set times = "<<HD <<"\n";
    std::cout <<">>>>>>>>>>>> Median of Execution times = "<<medE <<"\n";
    std::cout <<"\n>>>>>>> Exec run break point = "<< (HD+inspTT)/(serialMedE-medE) <<"\n";
    std::cout <<">>>>>>> Executor Speed up = "<< (serialMedE/medE) <<"\n";

    // Making Figures
    // Making stacked histogram number of executor run needed to make executor worth it
    outInsp<<", "<<(HD+inspTT)/(serialMedE-medE);
    // Making stacked histogram for executor speed up
    outExec<<", "<<(serialMedE/medE);


 allocateAC(A2, 0, 0, 0, FALSE);
 allocateAC(A1, 0, 0, 0, FALSE);
 allocateAC(Amat, 0, 0, 0, FALSE);
 delete []prunePtr;
 delete []pruneSet;
 delete []valL_nonblock;
 delete []levelPtr;
 delete []levelSet;

  }


  outInsp.close();
  outExec.close();

 return 1;
}

