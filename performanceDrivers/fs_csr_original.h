

/*
 ****** Serial implementation
 */
void fs_csr_original(int n, int* rowPtr, int* colIdx, double* val, double *b, double *x)
{
  int i,j,tmp;

  for(i=0;i<n;i++) {
    tmp = b[i];
    for (j=rowPtr[i]; j<rowPtr[i+1]-1;j++) {
      tmp -= val[j]*x[colIdx[j]];
    }
    x[i] = tmp / val[rowPtr[i+1]-1];
  }
}

