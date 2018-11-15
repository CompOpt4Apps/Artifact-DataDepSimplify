// Mahdi:
// Inside test directory compile the code with:
// g++ -o ic0 ic0.cpp -std=c++11 -fopenmp -O3
// Run the code with:
// ./ic0 smatList.txt > result.txt
// Note, smatList.txt should contains list of input matrices.

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <mkl.h>

#include "omp.h"
#include "transpose.h"
#include "def.h"
#include <metis.h>
#include "../src/util.h"
#include "../src/levelSet.h"
#include "../src/inspection_DAG03.h"
#include "../src/gs_csr_original.h"
#include "../src/gs_csr_inspector.h"
#include "../src/gs_csr_executor.h"

#define TRUE 1
#undef DEBUG

using namespace std;



int main(int argc, char *argv[]) {

  if(argc != 2){
    cout<<"\n\nMust be invoked like:"
          "\n  ./gs_csr_per matList&Params.txt"
          "\n  matlist.txt's first line must contain NThreads and NRuns (separated with comma),"
          "\n  the subsequent line should be the list of input matricies."
          "\n  NThreads being maximum number of threads to be used,"
          "\n  NRuns being number of runs to be averged\n\n";
    return 1;
  }

  ofstream outInsp("results/insp.csv", std::ofstream::out | std::ofstream::app);
  ofstream outExec("results/exec.csv", std::ofstream::out | std::ofstream::app);
  outInsp<<"\nG-S CSR";
  outExec<<"\nG-S CSR";

  std::string inputMatrix;
  int numThread=8;
  int nRuns=1;
  string parameters;
  ifstream inF(argv[1]);
  getline( inF, parameters );
  sscanf(parameters.c_str(), "%d, %d",&numThread,&nRuns);

//  int numThread = atoi(argv[2]);
  int innerPart = 16;// = atoi(argv[3]);//Inner parts
  int divRate = 5; //atoi(argv[4]);
  int chunk = innerPart/numThread + 1;
  int levelParam = 5;// level distance
  int blasThreads = 1;

  // Looping over input matricies
  for( ; getline( inF, inputMatrix ); ){ 

    if(inputMatrix=="") break;

    int *colA;
    int *rowA;
    double *valA;
    size_t n, nnzA, ncol;

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
 
    //cout<<f1<<","<<numThread<<","<<innerPart<<","<<divRate<<";\n\n";
 /*
  * Calling Cholesky to generate blocked triangular matrix
  */

    omp_set_num_threads(numThread);
    mkl_set_num_threads(numThread);

    // MKL_Set_Num_Threads(1);
    MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);


//------------------------------  Ordering
 int *Perm = new int[n]();

 double *timingChol = new double[4 + numThread]();//for time measurement
 double orderingTime = 0;
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
 ncol = Amat->ncol;

 //Ordering

 start = std::chrono::system_clock::now();
#ifdef GIVEN
 //pastix_data_t **pastix_data;
 L->ordering = CHOLMOD_METIS;
 for (int l = 0; l < A->nrow; ++l) {
  Lperm[l] = inPerm[l];
 }

#elif METIS
 CSC *ATrans;
 unsigned long nnzFull = Amat->nzmax * 2;//Symmetric case
 ATrans = ptranspose(Amat, 0, NULL, NULL, 0, status);
#if 0
 for (int i = 0; i < ncol; ++i) {
  for (int j = A->p[i]; j < A->p[i+1]; ++j) {
   std::cout<<A->i[j]<<";";
  }
  std::cout<<"\n";
 }
 std::cout<<"---\n";
 for (int i = 0; i < ncol; ++i) {
  for (int j = ATrans->p[i]; j < ATrans->p[i+1]; ++j) {
   std::cout<<ATrans->i[j]<<";";
  }
  std::cout<<"\n";
 }
 std::cout<<"==\n";
#endif

 //Making the graph for passing it to metis, it should have
 //both upper and lower parts
 //allocateAC(AFull,ncol,nnzFull,0,TRUE);
 idx_t options1[METIS_NOPTIONS];
 METIS_SetDefaultOptions(options1);

 idx_t *AFullp = new idx_t[ncol + 1]();
 idx_t *AFulli = new idx_t[nnzFull]();
 idx_t ncolIDXT = ncol;
 idx_t *weigt = new idx_t[ncol];
 idx_t *LpermIDX = new idx_t[ncol];
 idx_t *ILpermIDX = new idx_t[ncol];
 for (int i = 0; i < ncol; ++i) {
  LpermIDX[i] = 0;
  ILpermIDX[i] = 0;
  weigt[i] = 1;
 }
 AFullp[0] = 0;
 for (int i = 0; i < ncol; ++i) {
  int nnzOfCurCol = ATrans->p[i + 1] - ATrans->p[i] - 1;
  nnzOfCurCol += Amat->p[i + 1] - Amat->p[i] - 1;
  AFullp[i + 1] = (long int) AFullp[i] + nnzOfCurCol;
  //copying Upper part, ignoring diagonal since it is in L already
  int base = AFullp[i];
  for (int j = ATrans->p[i], k = 0; j < ATrans->p[i + 1] - 1; ++j, ++k) {
   AFulli[base + k] = (long int) ATrans->i[j];
  }
  //copying L part
  base += ATrans->p[i + 1] - ATrans->p[i] - 1;
  for (int j = Amat->p[i] + 1, k = 0; j < Amat->p[i + 1]; ++j, ++k) {
   AFulli[base + k] = (long int) Amat->i[j];
  }
 }
#if 0
 for (int i = 0; i < ncol; ++i) {
  for (int j = AFullp[i]; j < AFullp[i+1]; ++j) {
   std::cout<<AFulli[j]<<";";
  }
  std::cout<<"\n";
 }
 std::cout<<"\n";
#endif

 int retMet = METIS_NodeND(&ncolIDXT, AFullp, AFulli, NULL, options1,
                           LpermIDX, ILpermIDX);
 assert(retMet == METIS_OK);
 if (retMet != METIS_OK) {
  std::cout << " " << retMet << "\n";
  exit(10);
 }
 for (int i = 0; i < ncol; ++i) {
  Perm[i] = LpermIDX[i];
  //std::cout<<Lperm[i];
 }
 allocateAC(ATrans, ATrans->nrow, ATrans->nzmax, ATrans->stype, false);
 METIS_Free(AFullp);
 METIS_Free(AFulli);
 METIS_Free(weigt);
 METIS_Free(LpermIDX);
 METIS_Free(ILpermIDX);
#endif
 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 orderingTime = elapsed_seconds.count();
//printf("ddddd %f ddd \n",orderingTime);
 elapsed_seconds = end - start;
 durationSym = elapsed_seconds.count();
#ifdef VERIFY
 auto checkOrder = new bool[ncol]();
 for (int i = 0; i < ncol; ++i) checkOrder[i] = false;
 for (int i = 0; i < ncol; ++i) {
  checkOrder[Perm[i]] = true;
 }
 for (int i = 0; i < ncol; ++i) {
  assert(checkOrder[i] == true);
 }
 delete checkOrder;
#endif


 CSC *A1 = ptranspose(Amat, 2, Perm, NULL, 0, status);
 CSC *A2 = ptranspose(A1, 2, NULL, NULL, 0, status);
#if 0
 for (int i = 0; i < n; ++i) {
  for (int j = A2->p[i]; j < A2->p[i+1]; ++j) {
   std::cout<<A2->i[j]<<";";
  }
  std::cout<<"\n";
 }
#endif

 allocateAC(Amat, 0, 0, 0, FALSE);
 allocateAC(A1, 0, 0, 0, FALSE);
/*
 * ********************* 
 */


/* std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double duration4 = 0 ,duration3 = 0, duration2=0, duration1=0;*/

 double *x = new double[n]();

#ifdef FLOPCNT
 //***************
 int *ia = new int[n + 1];
 int *ja = new int[L->xsize];
 double *a = new double[L->xsize];
 bcsc2csc(n, L->nsuper, L->p, L->s, L->i_ptr, L->super, valL, ia, ja, a);
 unsigned long counts=0;
 rhsInit(n,ia,ja,a,x);
 counts = flopCoutLSolve(n,ia,ja,a,x);
 std::cout<<L->xsize<<";"<<counts<<";";
 delete []ia;
 delete []ja;
 delete []a;
#endif
//------------------------------ End of ordering



//  Running Inspector 

    int *HLevelPtr = NULL, *HLevelSet = NULL, *parPtr = NULL,
   *partition =NULL;
 int *levelPtr = NULL, *levelSet = NULL;
 int nLevels=0, nPar=0, levels=0;
 double *nodeCost;
 int nRuns=10;

 //Computing node cost
 nodeCost = new double[n];
 int *xi = new int[2*n]();
 for (int s = 0; s < n; ++s) {
  //nodeCost[s]=A2->p[s+1]-A2->p[s];
  nodeCost[s]=1;
 }
 delete []xi;
 //A2 = Amat;


        // # Inspector:
        std::vector<std::vector<int>> DAG;
        DAG.resize(n);
        //for(int i = 0 ; i < n ; i++ ) DAG[i].push_back(i);
        std::vector<std::set<int>> DAG_s;
        DAG_s.resize(n);
        start = std::chrono::system_clock::now();
        // Creating the DAG with generated
        /*gs_csr_inspector(n,A2->p, A2->i, DAG_s);

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
        gs_csr_inspector(n,A2->p, A2->i, DAG);

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

 start = std::chrono::system_clock::now();
 int avg = getCoarseLevelSet_DAG_CSC03(n, v, edg,
                                       nLevels, HLevelPtr,
                                       HLevelSet, nPar,
                                       parPtr, partition,
                                       innerPart, levelParam, divRate, nodeCost);
 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 double HD = elapsed_seconds.count();
// std::cout <<avg<<"," <<duration1 << ",";
 delete[]nodeCost;
/////////////////////////////////////////
/*
    copyVal = new double[A2->nzmax];
    copyIDiag = new int [n+1];
    copyPtr = new int [n+1];
    copyIdx = new int [A2->nzmax];

    for (int i = 0; i < n; i++) {
      copyIDiag[i] = A2->x[A2->p[i]]
      for (int j = A2->p[i]+1; j < A2->p[i+1]; j++) {
        copyIdx[j-i] = A2->i[j];
        copyVal[j-i] = A2->x[j];
      }
      copyPtr[i] = A2->p[i]-1;
    }  
*/
    int *idiag = new int [n+1];
    for (int i = 0; i < n; i++) {
      idiag[i] = int(A2->x[A2->p[i]]);
    }  


    // ------ Sequentially Run
    std::cout <<"-- Running the algorithm sequentially for #"<<nRuns<<" times:\n";
    //***************Serial
    for (int l = 0; l < nRuns; ++l) {
      rhsInit(n, A2->p, A2->i, A2->x, x);
      start = std::chrono::system_clock::now();
      gs_csr_original(n, A2->p, A2->i, idiag, A2->x, x, x);
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



    std::cout<<"\n\n--Running the algorithm with #"
          <<numThread<<" threads in parallel for #"<<nRuns<<" times:";
    //****************Parallel H2 CSC
    for (int l = 0; l < nRuns; ++l) {
      rhsInit(n,A2->p,A2->i,A2->x,x);

      start = std::chrono::system_clock::now();
      gs_csr_executor_H2(n,A2->p,A2->i, idiag, A2->x,x, x,nLevels,HLevelPtr,HLevelSet,
              nPar,parPtr,partition, chunk);
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


    if (HLevelPtr != NULL) delete[]HLevelPtr;
    if (HLevelPtr != NULL) delete[]HLevelSet;
    if (parPtr != NULL)    delete[]parPtr;
    if (partition != NULL) delete[]partition;
 //delete []contribs;
 //delete []map;
// delete[]valL;
 //delete []colL;
 //delete []li_ptr;
    delete[]timingChol;
    allocateAC(A2, 0, 0, 0, FALSE);

  }


  outInsp.close();
  outExec.close();

 return 0;

}
