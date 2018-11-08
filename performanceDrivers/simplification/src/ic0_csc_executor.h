
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
using namespace std;

extern double sqrt(int);

void ic0_csc_executor(int n, double *val, int * colPtr, int *rowIdx,
               int levels, int *levelPtr, int *levelSet, int chunk){

 int li=0;

// for (i = 0; i < n - 1; i++){
 for (int l = 0; l < levels; ++l) {
  #pragma omp parallel for default(shared) private(li) schedule(static)
  for (li = levelPtr[l]; li < levelPtr[l + 1]; ++li) {
   int i = levelSet[li];

   val[colPtr[i]] = sqrt(val[colPtr[i]]);//S1

   for (int m = colPtr[i] + 1; m < colPtr[i+1]; m++){
    val[m] = val[m] / val[colPtr[i]];//S2
   }

   for (int m = colPtr[i] + 1; m < colPtr[i+1]; m++) {
    for (int k = colPtr[rowIdx[m]] ; k < colPtr[rowIdx[m]+1]; k++){
     for (int l = m; l < colPtr[i+1] ; l++){
      if (rowIdx[l] == rowIdx[k] && rowIdx[l+1] <= rowIdx[k]){
       #pragma omp atomic
       val[k] -= val[m]* val[l]; //S3
      }
     }
    }
   }
  }
 }

}

