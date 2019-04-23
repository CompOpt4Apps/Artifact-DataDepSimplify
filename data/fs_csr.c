// Forward-Solve CSR 
// From:
//      Richard Vuduc, Shoaib Kamil, Jen Hsu, Rajesh Nishtala, James W Demmel, 
//      and Katherine A Yelick. 2002. Automatic performance tuning
//      and analysis of sparse triangular solve. ICS.


void fs_csr(int n, int* rowPtr, int* colIdx, double* val, double *b, double *x)
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

