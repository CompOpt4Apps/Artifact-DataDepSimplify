#include "LevelSchedule.hpp"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

/* y = A*x+y */
int cs_gaxpy (double *A, int n, int *Ap, int *Ai, double *x, double *y)
{
    int p, j;
    for (j = 0 ; j < n ; j++)
    {
        for (p = Ap[j] ; p < Ap[j+1] ; p++)
        {
            y [Ai[p]] += A[p] * x[j] ;
        }
    }
    return (1) ;
}


int main(){
  int n =500;
  int row[1000];
  int col[500];
  double values[10000];
  double vec[500];
  double y[500];

  cs_gaxpy(values, n, col, row, vec, y);

  return 0;
}
