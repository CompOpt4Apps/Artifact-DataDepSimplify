//
// Created by kazem on 11/15/18.
//

#ifndef PROJECT_GS_CSR_H
#define PROJECT_GS_CSR_H


void guass_seidel_csr(int n, int *rowptr, int *colidx, double *idiag,
                      double *values, double *y, const double *b)
{
 int i,j;
 // double sum;

 for (i = 0; i < n; i++) {
  double sum = b[i];
  for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
   sum -= values[j]*y[colidx[j]];
  }
  y[i] = sum*idiag[i];
 } // for each row
}

#endif //PROJECT_GS_CSR_H
