//
// Created by kazem on 11/15/18.
//


#ifndef PROJECT_GS_BCSR_H
#define PROJECT_GS_BCSR_H

#define BS 2

void guass_seidel_bcsr(double *values[BS][BS], double *y[BS],
                      const double *b[BS], double *idiag[BS][BS],
                      int *rowptr, int *colidx) {

 double sum[BS];
 int n;
 for (int i = 0; i < n; ++i) {
  for (int ii = 0; ii < BS; ++ii) {
   sum[ii] = b[i][ii];//S0
  }
  for (int j = rowptr[i]; j < rowptr[i + 1]; ++j) {
   for (int jj = 0; jj < BS; ++jj) {
    for (int ii = 0; ii < BS; ++ii) {
     sum[ii] -= values[j][jj][ii]*y[colidx[j]][jj];//S1
    }
   }
  }

  for (int ii = 0; ii < BS; ++ii) {
   y[i][ii] = 0;  //S2
  }

  for (int jj = 0; jj < BS; ++jj) {
   for (int ii = 0; ii < BS; ++ii) {
    y[i][ii] += idiag[i][jj][ii]*sum[jj];//S3
   }
  }
 } // for each row
}



void guass_seidel_bcsr_(int n, double ***values, double **y,
                        double **b, double ***idiag,
                       int *rowptr, int *colidx) {

 double sum[BS];
 for (int i = 0; i < n; ++i) {
  for (int ii = 0; ii < BS; ++ii) {
   sum[ii] = b[i][ii];//S0
  }
  for (int j = rowptr[i]; j < rowptr[i + 1]; ++j) {
   for (int jj = 0; jj < BS; ++jj) {
    for (int ii = 0; ii < BS; ++ii) {
     sum[ii] -= values[j][jj][ii]*y[colidx[j]][jj];//S1
    }
   }
  }

  for (int ii = 0; ii < BS; ++ii) {
   y[i][ii] = 0;  //S2
  }
  for (int jj = 0; jj < BS; ++jj) {
   for (int ii = 0; ii < BS; ++ii) {
    y[i][ii] += idiag[i][jj][ii]*sum[jj];//S3
   }
  }
 } // for each row
}


void guass_seidel_bcsr_wf(int n, double ***values, double **y,
                        double **b, double ***idiag,
                        int *rowptr, int *colidx) {

 double sum[BS];
 for (int i = 0; i < n; ++i) {
  for (int ii = 0; ii < BS; ++ii) {
   sum[ii] = b[i][ii];//S0
  }
  for (int j = rowptr[i]; j < rowptr[i + 1]; ++j) {
   for (int jj = 0; jj < BS; ++jj) {
    for (int ii = 0; ii < BS; ++ii) {
     sum[ii] -= values[j][jj][ii]*y[colidx[j]][jj];//S1
    }
   }
  }

  for (int ii = 0; ii < BS; ++ii) {
   y[i][ii] = 0;  //S2
  }
  for (int jj = 0; jj < BS; ++jj) {
   for (int ii = 0; ii < BS; ++ii) {
    y[i][ii] += idiag[i][jj][ii]*sum[jj];//S3
   }
  }
 } // for each row
}














void guass_seidel_bcsr_H2(int n, double ***values, double **y,
                        double **b, double ***idiag,
                       int *rowptr, int *colidx,
                 int levels, int *levelPtr, int *levelSet,
                 int parts,  int *parPtr, int *partition,
                 int chunk){

 double sum[BS];



 for (int i1 = 0; i1 < levels ; ++i1) {
#pragma omp parallel //shared(lValues)//private(map, contribs)
  {
#pragma omp  for schedule(auto)
   for (int j1 = levelPtr[i1]; j1 < levelPtr[i1 + 1]; ++j1) {
    for (int k1 = parPtr[j1]; k1 < parPtr[j1 + 1]; ++k1) {
     int i = partition[k1];


  for (int ii = 0; ii < BS; ++ii) {
   sum[ii] = b[i][ii];//S0
  }
  for (int j = rowptr[i]; j < rowptr[i + 1]; ++j) {
   for (int jj = 0; jj < BS; ++jj) {
    for (int ii = 0; ii < BS; ++ii) {
     sum[ii] -= values[j][jj][ii]*y[colidx[j]][jj];//S1
    }
   }
  }

  for (int ii = 0; ii < BS; ++ii) {
   y[i][ii] = 0;  //S2
  }
  for (int jj = 0; jj < BS; ++jj) {
   for (int ii = 0; ii < BS; ++ii) {
    y[i][ii] += idiag[i][jj][ii]*sum[jj];//S3
   }
  }

}}}}

// } // for each row
}











#endif //PROJECT_GS_BCSR_H
