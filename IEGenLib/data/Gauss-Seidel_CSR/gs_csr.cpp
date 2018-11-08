#include "LevelSchedule.hpp"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

void forwardSolveRef(int n, int *rowptr, int *colidx,int *idiag, double *values, double *y, const double *b)
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

