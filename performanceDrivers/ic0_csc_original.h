
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
using namespace std;

extern double sqrt(int);
void ic0_csc_original(int n, double *val, int * colPtr, int *rowIdx)
{
 int i, k,l, m;
 for (i = 0; i < n ; i++){
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

