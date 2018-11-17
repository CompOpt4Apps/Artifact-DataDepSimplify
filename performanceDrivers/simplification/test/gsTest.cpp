//
// Created by kazem on 11/16/18.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <cholmod.h>
#include <cholmod_function.h>
#include <omp.h>

#include <Transpose.h>
#include <metis.h>
#include <gs_csr.h>
#include <gs_bcsr.h>
#include <set>
#include <util.h>
#include <inspection_DAG03.h>


#include "mkl.h"

#include "MKL_Utils.h"

#define CPUTIME (SuiteSparse_time ( ))
#define CSC_TRNG
#undef DEBUG
//#define FLOPCNT

#define BS 2
using namespace std;



int main(int argc, char *argv[]) {

 std::string f1 = argv[1];
 int *colA;
 int *rowA;
 double *valL;
 double *valA;
 int maxSupWid, maxCol;
 size_t n, nnzA, ncol;

 std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double durationSym = 0, duration3 = 0, duration2 = 0, duration1 = 0;
 long totalIntraContribNNZs = 0, totalInterCotribNNZs = 0, numOfEdgeCuts = 0;
 int numberOfIntraCore = 0, numberOfInterCore = 0;

 if (!readMatrix(f1, n, nnzA, colA, rowA, valA))
  return -1;

/* if(wasteFile.fail())
  return -1;*/
 int numThread = atoi(argv[2]);
 int innerParts = atoi(argv[3]);//Inner parts
 int divRate = atoi(argv[4]);
 int chunk = innerParts/numThread + 1;
 int levelParam = 5;// level distance
 int blasThreads = 1;


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

#elif SCOTCH
 L->ordering = CHOLMOD_METIS;
 CSC *ATrans;
 unsigned long nnzFull = A->nzmax*2;//Symmetric case
 ATrans = ptranspose(A, 0, NULL, NULL, 0, status) ;

 SCOTCH_Num baseVal=0;
 SCOTCH_Num          vertnbr;                    /* Number of vertices */
 SCOTCH_Graph        grafdat;                    /* Source graph       */
 SCOTCH_Ordering     ordedat;                    /* Graph ordering     */
 SCOTCH_Strat        stradat;                    /* Ordering strategy  */
 SCOTCH_Num          straval;
 //Making the graph for passing it to metis, it should have
 //both upper and lower parts
 //allocateAC(AFull,ncol,nnzFull,0,TRUE);
 SCOTCH_Num *permtab = new SCOTCH_Num[ncol]();                    /* Permutation array  */
 SCOTCH_Num *AFullp = new SCOTCH_Num[ncol+1]();
 SCOTCH_Num *AFulli = new SCOTCH_Num[nnzFull]();
 SCOTCH_Num ncolIDXT = ncol;

 AFullp[0]=0;
 for (int i = 0; i < ncol; ++i) {
  int nnzOfCurCol = ATrans->p[i+1]-ATrans->p[i]-1;
  nnzOfCurCol += A->p[i+1]-A->p[i]-1;
  AFullp[i+1] =(long int) AFullp[i]+nnzOfCurCol;
  //copying Upper part, ignoring diagonal since it is in L already
  int base=AFullp[i];
  for (int j = ATrans->p[i],k=0; j < ATrans->p[i+1]-1; ++j,++k) {
   AFulli[base+k] =(long int) ATrans->i[j];
  }
  //copying L part
  base+=ATrans->p[i+1]-ATrans->p[i]-1;
  for (int j = A->p[i]+1,k=0; j < A->p[i+1]; ++j, ++k) {
   AFulli[base+k] =(long int) A->i[j];
  }
 }
 if(SCOTCH_stratInit (&stradat))
  return NULL;
 char straStr[550];
 sprintf(straStr, SCOTCH_STRAT_DIRECT);
 if(SCOTCH_stratGraphOrder(&stradat,straStr))
  return NULL;
 nnzFull=AFullp[ncol];
 if(SCOTCH_graphBuild (&grafdat,baseVal,ncol,AFullp,NULL,NULL,NULL,
                   nnzFull,AFulli,NULL))
  return NULL;
 if(SCOTCH_graphCheck(&grafdat))
  return NULL;
 /*if(SCOTCH_graphOrderList(&grafdat, ncol, NULL, &stradat,
                          permtab, NULL, NULL, NULL, NULL)) { *//* Create ordering *//*
  return NULL;
 }*/
 /*if(SCOTCH_graphOrderInit    (&grafdat, &ordedat, permtab, NULL, NULL, NULL, NULL)) {  Create ordering
  return NULL;
 }
 if(SCOTCH_graphOrderCompute (&grafdat, &ordedat, &stradat)) {  Perform ordering
  return NULL;
 }*/

 if(SCOTCH_graphOrderInit    (&grafdat, &ordedat, permtab, NULL, NULL, NULL, NULL)) {  // Create ordering
  return NULL;
 }
 if(SCOTCH_graphOrderCompute (&grafdat, &ordedat, &stradat)) { // Perform ordering
  return NULL;
 }

 if (SCOTCH_graphOrderCheck (&grafdat, &ordedat) != 0)
  return NULL;
 for (int i = 0; i < ncol; ++i) {
  Lperm[i]=permtab[i];
  //std::cout<<Lperm[i];
 }
 SCOTCH_graphOrderExit (&grafdat, &ordedat);
 SCOTCH_stratExit      (&stradat);
 SCOTCH_graphExit      (&grafdat);

 allocateAC(ATrans,ATrans->nrow,ATrans->nzmax,ATrans->stype,false);
 delete [] AFullp;
 delete [] AFulli;
 delete [] permtab;

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
#else
 double info[20]={0};
 double Control[2];
 Control [0] = 10; //TODO check later //AMD_Dense
 Control [1] = TRUE; //AMD_AGGRESSIVE
 L->ordering = CHOLMOD_AMD;
 amd_order(ncol,A->p,A->i,Lperm,NULL,info);
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
// allocateAC(A1, 0, 0, 0, FALSE);
/*
 * ********************* Triangular Solve
 */



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
 int iterNo=5;

 //Computing node cost
 nodeCost = new double[n];
 int *xi = new int[2*n]();
 for (int s = 0; s < n; ++s) {
  //nodeCost[s]=A2->p[s+1]-A2->p[s];
  nodeCost[s]=1;
 }
 delete []xi;



/* int *col2blk = new int[n];
 for (int i1 = 0, kk=0; i1 < n; i1+=BS, kk++) {
  for (int i = i1; i < i1+BS; ++i) {
   col2blk[i] = kk;
  }
 }

 int *Tp = new int[n];
 Tp[0]=0;
 int *Ti = new int[n];
 std::set<int> tmp;
 int redCol=0;
 for (int i1 = 0; i1 < n ; ++i1) {
  tmp.clear();
  for (int j1 = A2->p[i1]; j1 < A2->p[i1 + 1]; ++j1) {
   tmp.insert(col2blk[A2->i[j1]]);
  }
  redCol++;
  Tp[redCol] = Tp[redCol-1] + tmp.size();
  std::set<int>::iterator it = tmp.begin();
  for (int i = Tp[redCol-1]; i < Tp[redCol]; ++i, ++it) {
   Ti[i] = *it;
  }
 }*/


 start = std::chrono::system_clock::now();
 int avg = getCoarseLevelSet_DAG_CSC03(n, A2->p, A2->i,
                                       nLevels, HLevelPtr,
                                       HLevelSet, nPar,
                                       parPtr, partition,
                                       innerParts, levelParam, divRate,
                                       nodeCost);
 end = std::chrono::system_clock::now();
 elapsed_seconds = end - start;
 duration1 = elapsed_seconds.count();
 std::cout <<avg<<"," <<duration1 << ",";
 delete[]nodeCost;
 sparse_matrix_t *csrA;



 ////----------CSR
 printf(";*;");

 double *csrVal = A2->x;
 MKL_INT *csrColInd = A2->i;
 MKL_INT *csrRowPtr = A2->p;

 double *x_s = new double[n]();
 double *y_s = new double[n]();
 double * idiag = new double[n];
 //populating idiag
 for (int l = 0; l < n; ++l) {
  idiag[l] = csrVal[csrRowPtr[l]];
 }
 for (int j = 0; j < n; ++j) {
  x_s[j] = 0.0;
  y_s[j] = 1.0;
 }

 guass_seidel_csr(n, csrRowPtr, csrColInd, idiag,
                  csrVal, x_s, y_s);


 //converting to bcsr for blocked GS
 const int blockSize = 2;

 CRSArrays csrL;
 BCRSArrays bcrL;
 int m = n;

 //Converting to normal BCSR
 csrL.a = csrVal;
 csrL.ia = csrRowPtr;
 csrL.ja = csrColInd;
 csrL.m = n;
 csrL.nnz = csrRowPtr[n];
 CRS_to_BCRS(csrL, &bcrL, blockSize);
 //Converting to this weird BCSR!
/* std::cout<<"\n";
 for (int k1 = 0; k1 < n; ++k1) {
  for (int i = csrRowPtr[k1]; i < csrRowPtr[k1+1]; ++i) {
   std::cout<<csrVal[i]<<";";
  }
  std::cout<<"\n";
 }
 std::cout<<"\n";

 for (int k1 = 0; k1 < bcrL.nbBlocks*BS*BS; ++k1) {
  std::cout<<bcrL.a[k1]<<";";
 }
 std::cout<<"\n";*/

 double ***idiag_blk = new double**[bcrL.nbBlockRows];
 double ***bsr_val;
 double **x_b, **y_b;
 y_b = new double*[bcrL.nbBlockRows];
 x_b = new double*[bcrL.nbBlockRows];
 for (int l1 = 0; l1 < bcrL.nbBlockRows; ++l1) {
  y_b[l1] = new double[BS]();
  x_b[l1] = new double[BS]();
 }
 bsr_val = new double**[bcrL.nbBlocks];
 for (int l1 = 0; l1 < bcrL.nbBlocks; ++l1) {
  bsr_val[l1] = new double*[BS]();
 }

 for (int  j1=0; j1 < bcrL.nbBlockRows; ++j1) {
  for (int j = 0; j < BS; ++j) {
   y_b[j1][j] = 1.0;
  }
 }

 for (int m1 = 0; m1 < bcrL.nbBlocks; ++m1) {
  for (int i = 0; i < BS; ++i) {
   bsr_val[m1][i] = bcrL.a + (m1*(BS*BS) + i*BS);
  }
 }

 //populating idiag
 for (int l = 0; l < bcrL.nbBlockRows ; l++) {
  for (int i = bcrL.ia[l]; i < bcrL.ia[l+1]; ++i) {
   if(bcrL.ja[i] == l)
    idiag_blk[l] = bsr_val[i];
  }
 }

 guass_seidel_bcsr_(bcrL.nbBlockRows, bsr_val, x_b,
 y_b, idiag_blk, bcrL.ia, bcrL.ja);

/* for (int k1 = 0; k1 < bcrL.nbBlocks*BS*BS; ++k1) {
  std::cout<<bcrL.a[k1]<<";";
 }
 std::cout<<"\n";
 for (int k1 = 0; k1 < A2->nzmax; ++k1) {
  std::cout<<csrVal[k1]<<";";
 }
 std::cout<<"\n";*/
 int k2=0;
 for (int  j1=0; j1 < bcrL.nbBlockRows; ++j1) {
  for (int j = 0; j < BS; ++j) {
   //assert(x_b[j1][j] - x_s[k2] < 1);
   //std::cout<<x_b[j1][j]<<";"<< x_s[k2] <<";";
   k2++;
  }
 }
 delete []x_s;
 delete []y_s;
 delete []idiag;
 delete []idiag_blk;
 for (int l1 = 0; l1 < bcrL.nbBlockRows; ++l1) {
  delete[] y_b[l1];
  delete[] x_b[l1];
 }
 delete []x_b;
 delete []y_b;

/* for (int l1 = 0; l1 < bcrL.nbBlocks; ++l1) {
  delete[] bsr_val[l1];
 }
 delete []bsr_val;*/
 /*delete []idiag_blk;*/
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
 allocateAC(A1, 0, 0, 0, FALSE);
}
