//
// Created by kazem on 11/4/18.
//

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

 if(argc < 2){
  cout<<"Input missing arguments, you need to specify input list file\n";
 }

 std::string f1 = argv[1];
 int *colA;
 int *rowA;
 double *valA;
 size_t n, nnzA, ncol;

 std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double durationSym = 0, duration3 = 0, duration2 = 0, duration1 = 0;



 if (!readMatrix(f1, n, nnzA, colA, rowA, valA))
  return -1;

 int numThread = atoi(argv[2]);
 int innerPart = atoi(argv[3]);//Inner parts
 int divRate = atoi(argv[4]);
 int chunk = innerPart/numThread + 1;
 int levelParam = 5;// level distance
 int blasThreads = 1;

 cout<<f1<<","<<numThread<<","<<innerPart<<","<<divRate<<";";
 /*
  * Calling Cholesky to generate blocked triangular matrix
  */

 omp_set_num_threads(numThread);
 mkl_set_num_threads(numThread);

 // MKL_Set_Num_Threads(1);
 MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);


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
 * ********************* Triangular Solve
 */


/* std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double duration4 = 0 ,duration3 = 0, duration2=0, duration1=0;*/

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
 std::cout<<L->xsize<<";"<<counts<<";";
 delete []ia;
 delete []ja;
 delete []a;
#endif
//Running LBC here

 int *HLevelPtr = NULL, *HLevelSet = NULL, *parPtr = NULL,
   *partition =NULL;
 int *levelPtr = NULL, *levelSet = NULL;
 int nLevels=0, nPar=0, levels=0;
 double *nodeCost;
 int iterNo=1;

 //Computing node cost
 nodeCost = new double[n];
 int *xi = new int[2*n]();
 for (int s = 0; s < n; ++s) {
  //nodeCost[s]=A2->p[s+1]-A2->p[s];
  nodeCost[s]=1;
 }
 delete []xi;
 //A2 = Amat;
 start = std::chrono::system_clock::now();
 int avg = getCoarseLevelSet_DAG_CSC03(n, A2->p, A2->i,
                                       nLevels, HLevelPtr,
                                       HLevelSet, nPar,
                                       parPtr, partition,
                                       innerPart, levelParam, divRate, nodeCost);
 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 double HD = elapsed_seconds.count();
// std::cout <<avg<<"," <<duration1 << ",";
 delete[]nodeCost;
#ifdef TRUE


std::cout<<"\n\n-- Running Serial:\n";
 //***************Serial
 for (int l = 0; l < iterNo; ++l) {
  rhsInit(n, A2->p, A2->i, A2->x, x);
  start = std::chrono::system_clock::now();
  lsolve(n, A2->p, A2->i, A2->x, x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  duration1 = elapsed_seconds.count();
   std::cout << "\n       Duration = "<< duration1 << "\n\n";
 }

std::cout<<"\n\n-- Running wavefront parallel using normal level set:\n";
 //****************Parallel CSC
 for (int l = 0; l < iterNo; ++l) {


  start = std::chrono::system_clock::now();
  levels = buildLevelSet_CSC(n, A2->nzmax, A2->p, A2->i,
                             levelPtr, levelSet);

  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  double duration3 = elapsed_seconds.count();


  rhsInit(n, A2->p, A2->i, A2->x, x);
  start = std::chrono::system_clock::now();
  lsolvePar(n, A2->p, A2->i, A2->x, x, levels, levelPtr, levelSet, chunk);
  //lsolvePar2(n,col,row,val,x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  duration2 = elapsed_seconds.count();
std::cout <<"\n       Duration (inspector) = "<< duration3<< "\n       duration (executor) = "<<duration2<< "\n       duration (executor+inspector) = "<<(duration2+duration3) << "\n       Exe+Inp performance relative to serial = "<< (duration1/(duration2+duration3)) <<"\n";
 }



std::cout<<"\n\n-- Running wavefront parallel using H level set:\n";
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
std::cout << "\n       Inspector Duration = "<< (HD)<<"\n       Executor Duration = "<< (duration2)<<"\n       duration (executor+inspector) = "<<(duration2+HD) <<"\n       Relative speed up = "<< (duration1/(HD+duration2)) << "\n";
 }
 cout<< nLevels <<";;";
#endif

 if (HLevelPtr != NULL)
  delete[]HLevelPtr;
 if (HLevelPtr != NULL)
  delete[]HLevelSet;
 if (parPtr != NULL)
  delete[]parPtr;
 if (partition != NULL)
  delete[]partition;
 //delete []contribs;
 //delete []map;
// delete[]valL;
 //delete []colL;
 //delete []li_ptr;
 delete[]timingChol;
 allocateAC(A2, 0, 0, 0, FALSE);
 return 0;

}
