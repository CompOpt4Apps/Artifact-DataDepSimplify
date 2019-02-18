
/*#define FLEXCOMPLEX
typedef long int  INT;
#define MKL_INT INT*/
#include <stdlib.h>
#include <cmath>

bool lchol_csc_original(int n, int* c, int* r, double* values,
                      size_t* lC, MKL_INT* lR, double* &lValues,
                      int *prunePtr, int *pruneSet) {
 /*
  * Performs a Cholesky decomposition on a given matrix (c, r, values), i.e.
  * stored in compressed column format, and produces L, which are
  * stored in column compressed format.
  * (n, c, r, values) : IN : input matrix
  * (lC, lR) : IN : The column and rwo sparsity patterns of L
  * (lValues) : OUT : the nonzero values of the L factor
  * (pruneSet, prunePtr) : IN : the row sparsity pattern of the factor L
  */
 double *f = new double[n]();
 for (int colNo = 0; colNo < n; ++colNo) {
  //Uncompressing a col into a 1D array
  for (int nzNo = c[colNo]; nzNo < c[colNo + 1]; ++nzNo) {
   f[r[nzNo]] = values[nzNo];//Copying nonzero of the col
  }
  for (int i = prunePtr[colNo]; i < prunePtr[colNo + 1]; ++i) {
   double tmp=0;
   int spCol = pruneSet[i];
   bool sw=false;
   for (int l = lC[spCol]; l < lC[spCol + 1]; ++l) {
    if (lR[l] == colNo && !sw) {
     tmp = lValues[l];
     sw=true;
    }
    if(sw){
     f[lR[l]] -= lValues[l] * tmp;
    }
   }
  }
  if (f[colNo] <= 0)
   return false; //The matrix is not SPD
  double tmpSqrt = sqrt(f[colNo]);
  f[colNo] = 0;
  lValues[lC[colNo]] = tmpSqrt;
  for (int j = lC[colNo] + 1; j < lC[colNo + 1]; ++j) {
   lValues[j] = f[lR[j]] / tmpSqrt;
   f[lR[j]] = 0;
  }
 }
 delete[]f;
 return true;
}

