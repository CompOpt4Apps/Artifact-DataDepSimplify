//
// Created by kazem on 11/14/18.
//

#include <stdio.h>
#include <cholmod.h>
#include <cholmod_function.h>
#define MKL_ILP64
#define FLEXCOMPLEX
typedef int  INT;
#define MKL_INT INT

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <mkl.h>
#include <serial_simplicial_Cholesky_01.h>
#include <symbolic_analysis_CSC.h>
#include <hlevel_simplicial_cholesky.h>
#include <inspection_DAG03.h>

#include "util.h"


//#define DEBUG 1
#define VERIFY
#define CPUTIME (SuiteSparse_time ( ))

//TODO this file uses a hack for computing the sparsity pattern of L, non-blocked version.
int main(int argc, char *argv[]) {

 std::string f1 = argv[1];
 int *colA, *rowA;
 double *valL_nonblock;
 double *valA;
 int maxSupWid, maxCol;
 size_t n, nnzA;

 //std::vector<profilingInfo> piArray;

 if (!readMatrix(f1, n, nnzA, colA, rowA, valA))
  return -1;


 int numThread = atoi(argv[2]);
 int innerParts = atoi(argv[3]);//Inner parts
 int divRate = atoi(argv[4]);
 int chunk = innerParts/numThread + 1;
 int levelParam = 5;// level distance
 int blasThreads = 1;

/* int numThread = atoi(argv[2]);
 int innerParts = atoi(argv[4]);//Inner parts
 int divRate = atoi(argv[3]);// level distance
 int chunk = innerParts/numThread + 1;
 int blasThreads = 1;
 int levelParam = 5;*/
 std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double duration = 0;
 omp_set_num_threads(numThread);

 MKL_Set_Num_Threads(numThread);
 // MKL_Set_Num_Threads(1);
 MKL_Domain_Set_Num_Threads(blasThreads, MKL_DOMAIN_BLAS);

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

 for (int i = 0; i < L->xsize; ++i) {
  valL_nonblock[i] = 0.0;
 }
 start = std::chrono::system_clock::now();

 cholesky_left_01_hlevel(n, A2->p, A2->i, A2->x, L->p, L->i, valL_nonblock,
                        prunePtr,pruneSet,
                         nLevels, levelPtr, levelSet,
                         nPar, parPtr, partition,
                         chunk);

 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 duration = elapsed_seconds.count();
 std::cout<<f1<<": " <<duration<<"\n";



 allocateAC(A2, 0, 0, 0, FALSE);
 allocateAC(A1, 0, 0, 0, FALSE);
 allocateAC(Amat, 0, 0, 0, FALSE);
 delete []prunePtr;
 delete []pruneSet;
 delete []valL_nonblock;
 delete []levelPtr;
 delete []levelSet;

 return 1;
}

