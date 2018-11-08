
// sparse matrix vector mult (SpMV) for CSR format
for (i=0; i<n; i++) {
  for(k=rowptr[i]; k<rowptr[i+1]; k++){
S1: y[i] = y[i] + spMatrix[k]*x[col[k]];
  }
}
