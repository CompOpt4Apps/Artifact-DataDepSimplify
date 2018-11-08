
// sparse matrix vector multiply (SpMV) for CSR format

void spMV_Mul_csr(int n, int* rowPtr, int* col, double* val, double *x, double *y)
{
  int i,k, tmp;

  for (i=0; i<n; i++) {
    tmp = y[i];
    for(k=rowPtr[i]; k<rowPtr[i+1]; k++){
      y[i] = tmp + val[k]*x[col[k]];
    }
  }

}

