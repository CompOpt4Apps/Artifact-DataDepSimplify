#include "LevelSchedule.hpp"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

extern double sqrt(int);
void ic0_csr(int n, double *val, int * colPtr, int *rowIdx)
{
  int i, k,l, m;
  for (i = 0; i < n - 1; i++){
    val[colPtr[i]] = sqrt(val[colPtr[i]]);//S1

    for (m = colPtr[i] + 1; m < colPtr[i+1]; m++){
      val[m] = val[m] / val[colPtr[i]];//S2
    }

    for (m = colPtr[i] + 1; m < colPtr[i+1]; m++) {
      for (k = colPtr[rowIdx[m]] ; k < colPtr[rowIdx[m]+1]; k++){
        for ( l = m; l < colPtr[i+1] ; l++){
          if (rowIdx[l] == rowIdx[k] && rowIdx[l+1] <= rowIdx[k]){
            val[k] -= val[m]* val[l]; //S3
          }
        }
      }
    }
  }
}

int main(){
  int n =500;
  int rowptr[501];
  int diagptr[500];
  int colidx[5000];
  double values[5000];
  double b[5000];
  double y[5000];

  ic0_csr(n, values, rowptr, colidx);

  return 0;
}
