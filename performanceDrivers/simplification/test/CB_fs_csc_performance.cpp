/*
 Mahdi:
 Should be build by running cmake then make
 the executable would be fs_csc_performance, 
 Run the driver with:
 ./fs_csc_performance matList&Params.txt
      matlist.txt's first line must contain NThreads and NRuns (separated with space),
      the subsequent line should be the list of input matricies.
      NThreads being maximum number of threads to be used,
      NRuns being number of runs to be averged.
*/
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <mkl.h>
#include "omp.h"
#include "transpose.h"
#include "def.h"
#include <metis.h>
#include <triangularSolve.h>
#include "../src/ic0_csc.h"
#include "../src/iChol.h"
#include "../src/util.h"
#include "../src/levelSet.h"
#include "../src/triangularSolve_inspector.h"
#include "../src/inspection_DAG03.h"
#define TRUE 1
#undef DEBUG

using namespace std;



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
  size_t n, nnzA, ncol;

  std::chrono::time_point<std::chrono::system_clock> startT, endT;
  std::chrono::duration<double> elapsed_secondsT;
  double durationDR[20] = {0.0}, durationE[20] = {0.0}, durationTT[20] = {0.0};
  double serialMedTT = 0.0, serialMedE = 0.0, serialAvgTT = 0.0, serialAvgE = 0.0;


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

    std::cout <<"\n\n---------- Started Reading Matrix: "<< inputMatrix <<" \n\n";
    startT = std::chrono::system_clock::now();
    if (!readMatrix(inputMatrix, n, nnzA, colA, rowA, valA)) return -1;
    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationDR[0] = elapsed_secondsT.count();
    std::cout <<"\n>>>>>>>>>>>> Data Read Total Duration = "<< durationDR[0] <<"\n";


//------------ Parsy Related Code -----------------------------------------------------------------
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

std::cout <<"\n\nHi1!\n\n";

   delete checkOrder;
std::cout <<"\n\nHi1.5!\n\n";
#endif
   CSC *A1 = ptranspose(Amat, 2, Perm, NULL, 0, status);
std::cout <<"\n\nHi1.51!\n\n";
   CSC *A2 = ptranspose(A1, 2, NULL, NULL, 0, status);
std::cout <<"\n\nHi1.52!\n\n";
#if 0
 for (int i = 0; i < n; ++i) {
  for (int j = A2->p[i]; j < A2->p[i+1]; ++j) {
   std::cout<<A2->i[j]<<";";
  }
  std::cout<<"\n";
 }
#endif
   allocateAC(Amat, 0, 0, 0, FALSE);
std::cout <<"\n\nHi1.53!\n\n";
   allocateAC(A1, 0, 0, 0, FALSE);
std::cout <<"\n\nHi1.54!\n\n";
   double *x = new double[n]();
std::cout <<"\n\nHi1.55!\n\n";

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

std::cout <<"\n\nHi2!\n\n";
    delete []ia;
std::cout <<"\n\nHi3!\n\n";
    delete []ja;
std::cout <<"\n\nHi4!\n\n";
    delete []a;
std::cout <<"\n\nHi5!\n\n";
#endif
//Running LBC here

std::cout <<"\n\nHi5.51!\n\n";
    int *HLevelPtr = NULL, *HLevelSet = NULL, *parPtr = NULL, *partition =NULL;
    int *levelPtr = NULL, *levelSet = NULL;
    int nLevels=0, nPar=0, levels=0;
    double *nodeCost;
  
    //Computing node cost
    nodeCost = new double[n];
std::cout <<"\n\nHi5.52!\n\n";
    int *xi = new int[2*n]();
std::cout <<"\n\nHi5.53!\n\n";
    for (int s = 0; s < n; ++s) {
      //nodeCost[s]=A2->p[s+1]-A2->p[s];
      nodeCost[s]=1;
    }
std::cout <<"\n\nHi5.54!\n\n";
    delete []xi;
std::cout <<"\n\nHi5.55!\n\n";
    //A2 = Amat;
//    start = std::chrono::system_clock::now();
std::cout <<"\n\nHi5.56!\n\n";
    int avg = getCoarseLevelSet_DAG_CSC03(n, A2->p, A2->i,
                                       nLevels, HLevelPtr,
                                       HLevelSet, nPar,
                                       parPtr, partition,
                                       innerPart, levelParam, divRate, nodeCost);
//    end = std::chrono::system_clock::now();
//    elapsed_seconds = end - start;
//    duration1 = elapsed_seconds.count();
//    std::cout <<avg<<"," <<duration1 << ",";

std::cout <<"\n\nHi6!\n\n";
    delete[]nodeCost;
//------------ END of Parsy Related Code ----------------------------------------------------------



    std::cout <<"\n\n\nMatrix: "<<inputMatrix<<"  MaxNumThreads = "<<maxTC<<"  NRuns = "<<nRuns<<"\n\n";

    std::cout <<"-- Running the algorithm sequentially for #"<<nRuns<<" times:\n";
    mkl_set_num_threads(1);
    MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);
    omp_set_num_threads(1);

    for (int r = 0; r < nRuns; ++r) {

      durationDR[r] = durationDR[0];

      rhsInit(n, A2->p, A2->i, A2->x, x);

      //std::cout <<"\n\n---------- Serial: \n\n";
      startT = std::chrono::system_clock::now();
      lsolve(n, A2->p, A2->i, A2->x, x);
      endT = std::chrono::system_clock::now();

      elapsed_secondsT = endT - startT;
      durationE[r] = elapsed_secondsT.count();
      //std::cout <<"\n>>>>>>>>>>>> Serial Total Duration = "<< durationT <<"\n"; 

      if (!testTriangular(n, x)){
        std::cout << "\n Serial run failed!\n\n";
        exit(1);
      }

      durationTT[r] = durationDR[r] + durationE[r];
      std::cout <<"\n>>>>>> Run #"<<r+1<<":\n";
      std::cout <<">>>>>> Data Read time = "<< durationDR[r] <<"\n";
      std::cout <<">>>>>> Execution time (without data read) = "<<durationE[r] <<"\n";
      std::cout <<">>>>>> Execution time (totally) = "<<durationTT[r] <<"\n";
    }
  
    std::sort(durationE, durationE+nRuns);
    std::sort(durationTT, durationTT+nRuns);
    double medE, medTT;
    if(nRuns%2) {
      medE = durationE[(int(nRuns/2))];
      medTT = durationTT[(int(nRuns/2))];
    } else {
      medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
      medTT = (durationTT[(int(nRuns/2))]+durationTT[(int(nRuns/2))-1])/2.0;
    }
    double avgE=0.0, avgTT=0.0;
    for(int i=0; i<nRuns; i++){
      avgE += durationE[i];
      avgTT += durationTT[i];
    }
    avgE /= nRuns;
    avgTT /= nRuns;

    serialMedE = medE;
    serialAvgE = avgE;
    serialMedTT = medTT;
    serialAvgTT = avgTT;
    std::cout <<"\n>>>>>>>>>>>> Median of Execution times (without data read) = "<<medE <<"\n";
    std::cout <<">>>>>>>>>>>> Averaged Execution time (without data read) = "<<avgE <<"\n";
    std::cout <<">>>>>>>>>>>> Median of Execution times (totally) = "<<medTT <<"\n";
    std::cout <<">>>>>>>>>>>> Averaged Execution time (totally) = "<<avgTT <<"\n\n\n";


#if 0


    int chunk = innerPart/numThread + 1;
    mkl_set_num_threads(1);
    MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);
    omp_set_num_threads(1);

 //****************Parallel CSC
 for (int l = 0; l < iterNo; ++l) {
  levels = buildLevelSet_CSC(n, A2->nzmax, A2->p, A2->i,
                             levelPtr, levelSet);
  rhsInit(n, A2->p, A2->i, A2->x, x);
  start = std::chrono::system_clock::now();
  lsolvePar(n, A2->p, A2->i, A2->x, x, levels, levelPtr, levelSet, chunk);
  //lsolvePar2(n,col,row,val,x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  duration2 = elapsed_seconds.count();
  if (!testTriangular(n, x) )
   std::cout << "##LevelSet,";
  else
   std::cout << duration2 << ",";
 }
 cout<< levels << ";;";

 //****************Parallel H2 CSC
 for (int l = 0; l < iterNo; ++l) {
  rhsInit(n,A2->p,A2->i,A2->x,x);
  start = std::chrono::system_clock::now();
  lsolveParH2(n,A2->p,A2->i,A2->x,x,nLevels,HLevelPtr,HLevelSet,
              nPar,parPtr,partition, chunk);
  //lsolvePar2(n,col,row,val,x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  duration2=elapsed_seconds.count();
  if(!testTriangular(n,x))
   std::cout<<"##HlevelSet,";
  else
   std::cout <<duration2<<",";
 }
 cout<< nLevels <<";;";
#endif


    if (HLevelPtr != NULL) delete[]HLevelPtr;
    if (HLevelPtr != NULL) delete[]HLevelSet;
    if (parPtr != NULL)    delete[]parPtr;
    if (partition != NULL) delete[]partition;
    //delete []contribs;
    //delete []map;
    // delete[]valL;
    //delete []colL;
    //delete []li_ptr;
    allocateAC(A2, 0, 0, 0, FALSE);

  }


  return 0;
}
