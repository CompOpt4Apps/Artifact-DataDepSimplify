#include "LevelSchedule.hpp"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

#define BS 2
void forwardSolveRef_(double *values[BS][BS], double *y[BS], const double *b[BS], double *idiag[BS][BS], int *rowptr, int *colidx)
{

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

int main(){
  int n =500;
  int rowptr[501];
  int diagptr[500];
  int colidx[5000];
  double values[5000];
  double b[5000];
  double y[5000];

  forwardSolveRef(n, rowptr, colidx, diagptr, values, y, b);

  return 0;
}
