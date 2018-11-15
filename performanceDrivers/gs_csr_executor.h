


void gs_csr_executor(int n, int *rowptr, int *colidx,int *idiag, double *values, double *y, const double *b,
               int levels, int *levelPtr, int *levelSet, int chunk){

 for (int l = 0; l < levels; ++l) {
  int li=0;
#pragma omp parallel for default(shared) private(li)  schedule(auto)
  for ( li = levelPtr[l]; li < levelPtr[l + 1]; ++li) {
   int i = levelSet[li];

    int sum = b[i];
    for (int j = rowptr[i]; j < rowptr[i + 1]; j++) {
      sum -= values[j]*y[colidx[j]];
    }
    y[i] = sum*idiag[i];
  } // for each row
 }
}

void fs_csr_executor_H2 (int n, int* rowPtr, int* colIdx, double* val, double *b, double *x,
                 int levels, int *levelPtr, int *levelSet,
                 int parts,  int *parPtr, int *partition,
                 int chunk){

 for (int i1 = 0; i1 < levels ; ++i1) {
#pragma omp parallel //shared(lValues)//private(map, contribs)
  {
#pragma omp  for schedule(auto)
   for (int j1 = levelPtr[i1]; j1 < levelPtr[i1 + 1]; ++j1) {
    for (int k1 = parPtr[j1]; k1 < parPtr[j1 + 1]; ++k1) {
     int i = partition[k1];

      int tmp = b[i];
      for (int j=rowPtr[i]; j<rowPtr[i+1]-1;j++) {
        tmp -= val[j]*x[colIdx[j]];
      }
      x[i] = tmp / val[rowPtr[i+1]-1];
    }
   }
  }
 }

}


void gs_csr_executor_H2(int n, int *rowptr, int *colidx,int *idiag, double *values, double *y, const double *b,
                 int levels, int *levelPtr, int *levelSet,
                 int parts,  int *parPtr, int *partition,
                 int chunk){

 for (int i1 = 0; i1 < levels ; ++i1) {
#pragma omp parallel //shared(lValues)//private(map, contribs)
  {
#pragma omp  for schedule(auto)
   for (int j1 = levelPtr[i1]; j1 < levelPtr[i1 + 1]; ++j1) {
    for (int k1 = parPtr[j1]; k1 < parPtr[j1 + 1]; ++k1) {
     int i = partition[k1];

     int sum = b[i];
     for (int j = rowptr[i]; j < rowptr[i + 1]; j++) {
      sum -= values[j]*y[colidx[j]];
     }
     y[i] = sum*idiag[i];
    } // for each row
   }
  }
 }

}

