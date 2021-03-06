/*
 Mahdi:
 Should be build by running cmake then make
 the executable would be fs_csc_performance, 
 Run the driver with:
 ./ic0_csc_performance matList&Params.txt
      matlist.txt's first line must contain NThreads and NRuns (separated with space),
      the subsequent line should be the list of input matricies.
      NThreads being maximum number of threads to be used,
      NRuns being number of runs to be averged.
*/
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "omp.h"
#include "transpose.h"
#include "def.h"
#include <metis.h>
#include "../src/util.h"
#include "../src/levelSet.h"

#include "../src/ic0_csc_original.h"
#include "../src/ic0_csc_inspector.h"
#include "../src/ic0_csc_executor.h"

#define TRUE 1
#undef DEBUG
#define METIS 1

using namespace std;


void ordering(int* colA, int* rowA, double* valA, CSC* &reOrdMat, int nnzA, int n);
string getSMatName(string extName);
void dataCopy(CSC *mat, double* &copyVal, int* &copyPtr, int* &copyIdx, int n);


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

  int levelParam = 5;// level distance
  int innerPart  = 16;//atoi(argv[3]);//Inner parts
  int divRate    = 8;//atoi(argv[4]);
  int blasThreads = 8;
  std::string inputMatrix;
  int *colA;
  int *rowA;
  double *valA;
  size_t n, nnzA;

  std::chrono::time_point<std::chrono::system_clock> startT, endT;
  std::chrono::duration<double> elapsed_secondsT;
  double durationID[20] = {0.0}, durationIL[20] = {0.0}, durationE[20] = {0.0}, durationTT[20] = {0.0}, durationDR;
  double serialMedTT = 0.0, serialMedE = 0.0, serialAvgTT = 0.0, serialAvgE = 0.0;


  ofstream outSerT("results/table4.csv", std::ofstream::out | std::ofstream::app);
  ofstream outInsp("results/fig8.csv", std::ofstream::out | std::ofstream::app);
  ofstream outExec("results/fig7.csv", std::ofstream::out | std::ofstream::app);
  outSerT<<"\nI. Cholesky CSC";
  outInsp<<"\nI. Cholesky CSC";
  outExec<<"\nI. Cholesky CSC";


  int maxTC=1;
  int nRuns=1;
  string parameters;
  ifstream inF(argv[1]);
  getline( inF, parameters );
  sscanf(parameters.c_str(), "%d, %d",&maxTC,&nRuns);
  // Looping over input matricies
  for( ; getline( inF, inputMatrix ); ){ 
 
    if(inputMatrix=="") break;
    serialMedTT = serialMedE = serialAvgTT = serialAvgE = 0.0;

    std::cout <<"\n\n---------- Started Reading Matrix: "<< inputMatrix;

    startT = std::chrono::system_clock::now();
    if (!readMatrix(inputMatrix, n, nnzA, colA, rowA, valA)) return -1;
    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationDR = elapsed_secondsT.count();
   // std::cout <<"\n>>>>>>>>>>>> Data Read Total Duration = "<< durationDR <<"\n";

    // Reordering the matrix
    CSC *reOrdMat;
    ordering(colA, rowA, valA, reOrdMat, nnzA, n);


    // ------ Sequentially Run
    std::cout <<"\n-- Running the algorithm sequentially for #"<<nRuns<<" times:\n";
    double* serialCopyVal;
    int *serialCopyPtr, *serialCopyIdx;
    omp_set_num_threads(1);

    for (int r = 0; r < nRuns; ++r) {

      // Copying data since arrays get manipulated inplace in the computation
      dataCopy(reOrdMat, serialCopyVal, serialCopyPtr, serialCopyIdx, n);

      //std::cout <<"\n\n---------- Serial: \n\n";
      startT = std::chrono::system_clock::now();
      ic0_csc_original(n, serialCopyVal, serialCopyPtr, serialCopyIdx);
      endT = std::chrono::system_clock::now();

      elapsed_secondsT = endT - startT;
      durationE[r] = elapsed_secondsT.count();
      //std::cout <<"\n>>>>>>>>>>>> Serial Total Duration = "<< durationT <<"\n";
//      std::cout <<"\n>>>>>> Run #"<<r+1<<":\n";
//      std::cout <<">>>>>> Execution time = "<<durationE[r] <<"\n";
      if(r < nRuns-1){
        delete serialCopyVal;
        delete serialCopyPtr;
        delete serialCopyIdx;
      }
    }
  
    // Calculating Median and Average of execution times for plotting
    std::sort(durationE, durationE+nRuns);
    double medE, medTT;
    if(nRuns%2) {
      medE = durationE[(int(nRuns/2))];
    } else {
      medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
    }

    serialMedE = medE;
    std::cout <<"\n>>>>>>>>>>>> Median of Execution times = "<<medE <<"\n";




    // ------ Wavefront Parallel Run
    //for(int tc=2; tc <=maxTC; tc *=2){  // To get results with varying number of cores
    for(int tc=maxTC; tc <=maxTC; tc *=2){

      omp_set_num_threads(tc);

      std::cout<<"\n-- Running the algorithm with #"
               <<tc<<" threads in parallel for #"<<nRuns<<" times:\n";
      for(int j=0;j<20;j++){
        durationID[j] = durationIL[j] = durationE[j] = durationTT[j] = 0.0f;
      }

      for(int r=0; r<nRuns; r++){

        // Copying data since arrays get manipulated inplace in the computation
        double* parallelCopyVal;
        int *parallelCopyPtr, *parallelCopyIdx;
        dataCopy(reOrdMat, parallelCopyVal, parallelCopyPtr, parallelCopyIdx, n);

        // # Inspector:
//        std::vector<std::set<int>> DAG_s;
//        DAG_s.resize(n);
        std::vector<std::vector<int>> DAG;
        DAG.resize(n);
        int *levelPtr, *levelSet, levels;
        startT = std::chrono::system_clock::now();
        // Creating the DAG with generated
/*        ic0_csc_inspector(n,parallelCopyPtr, parallelCopyIdx, DAG_s);
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
        }
        v[ct] = edges;
*/
        ic0_csc_inspector_omega(n,parallelCopyPtr, parallelCopyIdx, DAG);
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

        endT = std::chrono::system_clock::now();
        elapsed_secondsT = endT - startT;
        durationID[r] = elapsed_secondsT.count();
        // Building the level sets
        startT = std::chrono::system_clock::now();
//        levels = buildLevelSet_DAG(n, DAG, levelPtr, levelSet);
        levels = buildLevelSet_CSC(n, nnzA, v, edg,
                                  levelPtr, levelSet);
        endT = std::chrono::system_clock::now();
        elapsed_secondsT = endT - startT;
        durationIL[r] = elapsed_secondsT.count();
        //std::cout <<"\n>>>>>>>>>>>> inspector Total Duration = "<< durationT <<"\n";
    

        // # Executor:
        startT = std::chrono::system_clock::now();
        ic0_csc_executor(n, parallelCopyVal, parallelCopyPtr, parallelCopyIdx, levels, levelPtr, levelSet, 1 );
        endT = std::chrono::system_clock::now();
        elapsed_secondsT = endT - startT;
        durationE[r] = elapsed_secondsT.count();
        //std::cout <<"\n>>>>>>>>>>>> Executor Duration = "<< durationE[i] <<"\n";

        durationTT[r] = durationID[r] + durationIL[r] + durationE[r];
        std::cout <<"\n>>>>>> Run #"<<r+1<<":\n";
        std::cout <<">>>>>> Inspector (Dependence) time = "<< durationID[r] <<"\n";
        std::cout <<">>>>>> Inspector (Build Level Set) time = "<< durationIL[r] <<"\n";
        std::cout <<">>>>>> Executor time  = "<<durationE[r] <<"\n";
        std::cout <<">>>>>> Execution time (totally) = "<<durationTT[r] <<"\n";
       

        // Testing parallel results for correctness
        if( !parallelCopyVal || !serialCopyVal){
          std::cout<<"\n\nWrong early memory deallocation!!\n\n";
          exit(1);
        }
        bool failed=false;
        for (int i = 0; i < reOrdMat->nzmax; ++i) {
          if(parallelCopyVal-serialCopyVal > .01){
            failed=true;
          }
        }
        if(failed){
          std::cout<<"\n\nParallel run failed!!\n\n";
          exit(1);
        }

        delete parallelCopyVal;
        delete parallelCopyPtr;
        delete parallelCopyIdx;
      }

      std::sort(durationID, durationID+nRuns);
      std::sort(durationIL, durationIL+nRuns);
      std::sort(durationE, durationE+nRuns);
      std::sort(durationTT, durationTT+nRuns);
      double medID, medIL, medE, medTT;
      if(nRuns%2) {
        medID = durationID[(int(nRuns/2))];
        medIL = durationIL[(int(nRuns/2))];
        medE = durationE[(int(nRuns/2))];
        medTT = durationTT[(int(nRuns/2))];
      } else {
        medID = (durationID[(int(nRuns/2))]+durationID[(int(nRuns/2))-1])/2.0;
        medIL = (durationIL[(int(nRuns/2))]+durationIL[(int(nRuns/2))-1])/2.0;
        medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
        medTT = (durationTT[(int(nRuns/2))]+durationTT[(int(nRuns/2))-1])/2.0;
      }

      std::cout <<"\n>>>>>>>>>>>> Median of Inspector Dependence times = "<<medID <<"\n";
      std::cout <<">>>>>>>>>>>> Median of Inspector Build Level Set times = "<<medIL <<"\n";
      std::cout <<">>>>>>>>>>>> Median of Execution times (without inspector) = "<<medE <<"\n";
      std::cout <<"\n>>>>>>> Exec run break point = "<< (medID+medIL)/(serialMedE-medE) <<"\n";
      std::cout <<">>>>>>> Executor Speed up = "<< (serialMedE/medE) <<"\n";

      // Outputing serial execution times (table 4)
      outSerT<<", "<<serialMedE;
      // Making Figures
      // Making stacked histogram number of executor run needed to make executor worth it
      outInsp<<", "<<(medID+medIL)/(serialMedE-medE);
      // Making stacked histogram for executor speed up
      outExec<<", "<<(serialMedE/medE);
    } // End tc loop

  }   // End of reading input matrix loop


  outSerT.close();
  outInsp.close();
  outExec.close();

  return 0;
}















string getSMatName(string extName){
  std::size_t slp = extName.find_last_of('/');
  std::size_t dotp = extName.find_last_of('.');
  return extName.substr( slp+1 , (dotp-slp-1) );
}

void dataCopy(CSC *mat, double* &copyVal, int* &copyPtr, int* &copyIdx, int n){
    copyVal = new double[mat->nzmax];
    copyPtr = new int [n+1];
    copyIdx = new int [mat->nzmax];

    for (int i = 0; i < n+1; ++i) {
      copyPtr[i] = mat->p[i];
    }
    for (int i = 0; i < mat->nzmax; ++i) {
      copyIdx[i] = mat->i[i];
      copyVal[i] = mat->x[i];
    }
}





void ordering(int* colA, int* rowA, double* valA, CSC* &reOrdMat, int nnzA, int n){
    
    size_t ncol;
    int *Perm = new int[n]();
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
    // Ordering
//    start = std::chrono::system_clock::now();
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
//    std::cout << " " << retMet << "\n";
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
//   end = std::chrono::system_clock::now();
//   elapsed_seconds = end - start;
//   orderingTime = elapsed_seconds.count();
//printf("ddddd %f ddd \n",orderingTime);
//   elapsed_seconds = end - start;
//   durationSym = elapsed_seconds.count();
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
   reOrdMat = ptranspose(A1, 2, NULL, NULL, 0, status);
#if 0
 for (int i = 0; i < n; ++i) {
  for (int j = reOrdMat->p[i]; j < reOrdMat->p[i+1]; ++j) {
   std::cout<<reOrdMat->i[j]<<";";
  }
  std::cout<<"\n";
 }
#endif
   allocateAC(Amat, 0, 0, 0, FALSE);
   allocateAC(A1, 0, 0, 0, FALSE);
   double *x = new double[n]();

#ifdef FLOPCNT
    //***************Serial
    int *ia = new int[n + 1];
    int *ja = new int[L->xsize];
    double *a = new double[L->xsize];
    bcsc2csc(n, L->nsuper, L->p, L->s, L->i_ptr, L->super, valL, ia, ja, a);
    unsigned long counts=0;
    rhsInit(n,ia,ja,a,x);
    counts = flopCoutLSolve(n,ia,ja,a,x);
//   std::cout<<L->xsize<<";"<<counts<<";";
    delete []ia;
    delete []ja;
    delete []a;
#endif
//------------ END of Parsy Related Code ----------------------------------------------------------


}
