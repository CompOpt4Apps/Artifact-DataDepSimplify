
extern double sqrt(double in);

int staticleftChol_csc(int n, int* c, int* r, double* values,
                   int* lC, int* lR, double* lValues, int *prunePtr, int *pruneSet, double *f) {
 /*
  * Performs a Cholesky decomposition on a given matrix (c, r, values), i.e.
  * stored in compressed column format, and produces L, which are
  * stored in column compressed format.
  * (n, c, r, values) : IN : input matrix
  * (lC, lR) : IN : The column and rwo sparsity patterns of L
  * (lValues) : OUT : the nonzero values of the L factor
  * (pruneSet, prunePtr) : IN : the row sparsity pattern of the factor L
  */

  int colNo, nzNo, i, l, j, sw = 0;
  double tmp, temp, tmpSqrt;

  for (colNo = 0; colNo < n; colNo++) {
//    memset(f, 0, n*sizeof(double));
    //Uncompressing a col into a 1D array
    for (nzNo = c[colNo]; nzNo < c[colNo + 1]; nzNo++) {
      f[r[nzNo]] = values[nzNo];//Copying nonzero of the col
    }
    for (i = prunePtr[colNo]; i < prunePtr[colNo + 1]; i++) {
//      spCol = pruneSet[i];
      sw=0;
      for (l = lC[pruneSet[i]]; l < lC[pruneSet[i] + 1]; l++) {
//        if (lR[l] == colNo){
          if(sw == 0) {
            tmp = lValues[l];
            sw=1;
          }
//        }
        if(sw == 1){
          f[lR[l]] -= lValues[l] * tmp;
        }
      }
    }
//    if (f[colNo] <= 0)  return 0; //The matrix is not SPD
    temp = f[colNo];
    tmpSqrt = sqrt(temp);
    f[colNo] = 0;
    lValues[lC[colNo]] = tmpSqrt;
    for (j = lC[colNo] + 1; j < lC[colNo + 1]; j++) {
      lValues[j] = f[lR[j]] / tmpSqrt;
      f[lR[j]] = 0;
    }
  }
//  free(f);
  return 1;
}
