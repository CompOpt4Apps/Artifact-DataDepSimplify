// Gauss-Seidel CSR
// From Intel MKL library:
//   Endong Wang, Qing Zhang, Bo Shen, Guangyong Zhang, Xiaowei Lu, Qing Wu, and Yajuan Wang. 2014. 
//   Intel math kernel library. In High-Performance Computing on the Intel® Xeon Phi. Springer, 167–188.     


void gs_csr(int n, int *rowptr, int *colidx,int *idiag, double *values, double *y, const double *b)
{
  int i,j;
  double sum; 

  for (i = 0; i < n; i++) {
    sum = b[i];
    for (j = rowptr[i]; j < rowptr[i + 1]; j++) {
      sum -= values[j]*y[colidx[j]];
    }
    y[i] = sum*idiag[i];
  } // for each row
}
