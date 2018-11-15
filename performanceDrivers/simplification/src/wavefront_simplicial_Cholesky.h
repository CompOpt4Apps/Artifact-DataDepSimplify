//
// Created by kazem on 11/14/18.
//

#ifndef PROJECT_SERIAL_SIMPLICIAL_CHOLESKY_WAVEFRONT_H
#define PROJECT_SERIAL_SIMPLICIAL_CHOLESKY_WAVEFRONT_H
/*#define FLEXCOMPLEX
typedef long int  INT;
#define MKL_INT INT*/
#include <stdlib.h>
#include <cmath>

bool cholesky_left_01_level(int n, int* c, int* r, double* values,
                      size_t* lC, MKL_INT* lR, double* &lValues,
                      int *prunePtr, int *pruneSet,
                      int nLevels, int *levelPtr, int *levelSet,
                       int chunk) {
 /*
  * Performs a Cholesky decomposition on a given matrix (c, r, values), i.e.
  * stored in compressed column format, and produces L, which are
  * stored in column compressed format.
  * (n, c, r, values) : IN : input matrix
  * (lC, lR) : IN : The column and rwo sparsity patterns of L
  * (lValues) : OUT : the nonzero values of the L factor
  * (pruneSet, prunePtr) : IN : the row sparsity pattern of the factor L
  */
 double *f ;//= new double[n]();
 //for (int colNo = 0; colNo < n; ++colNo) {
 for (int lev = 0; lev < nLevels; ++lev) {
#pragma omp parallel private(f)
  {
   f = new double[n]();
#pragma omp for \
            schedule(dynamic, chunk)
   for (int lIter = levelPtr[lev]; lIter < levelPtr[lev + 1]; ++lIter) {
    int colNo = levelSet[lIter];
    assert(colNo < n);
    //Uncompressing a col into a 1D array
    for (int nzNo = c[colNo]; nzNo < c[colNo + 1]; ++nzNo) {
     f[r[nzNo]] = values[nzNo];//Copying nonzero of the col
    }
    for (int i = prunePtr[colNo]; i < prunePtr[colNo + 1]; ++i) {
     double tmp=0;
     int spCol = pruneSet[i];
     bool sw = false;
     for (int l = lC[spCol]; l < lC[spCol + 1]; ++l) {
      if (lR[l] == colNo && !sw) {
       tmp = lValues[l];
       sw = true;
      }
      if (sw) {
       f[lR[l]] -= lValues[l] * tmp;
      }
     }
    }
    assert(f[colNo]>0);
    if (f[colNo] <= 0)
     exit(-1); //The matrix is not SPD
    double tmpSqrt = sqrt(f[colNo]);
    f[colNo] = 0;
    lValues[lC[colNo]] = tmpSqrt;
    for (int j = lC[colNo] + 1; j < lC[colNo + 1]; ++j) {
     lValues[j] = f[lR[j]] / tmpSqrt;
     f[lR[j]] = 0;
    }
   }


  delete []f;
  }
 }
 return true;
}

#endif //PROJECT_SERIAL_SIMPLICIAL_CHOLESKY_WAVEFRONT_H
