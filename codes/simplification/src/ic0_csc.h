
#ifndef SIMPLIFICATION_ICHOL0_H
#define SIMPLIFICATION_ICHOL0_H
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
using namespace std;

extern double sqrt(int);
void ic0_csr(int n, double *val, int * colPtr, int *rowIdx)
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

void ic0_csr_parInner(int n, double *val, int * colPtr, int *rowIdx)
{
 int i, m;
 for (i = 0; i < n - 1; i++){
  val[colPtr[i]] = sqrt(val[colPtr[i]]);//S1

  //#pragma omp parallel for schedule(auto)
  for (m = colPtr[i] + 1; m < colPtr[i+1]; m++){
   val[m] = val[m] / val[colPtr[i]];//S2
  }
  #pragma omp parallel for schedule(auto)
  for (m = colPtr[i] + 1; m < colPtr[i+1]; m++) {
   //#pragma omp parallel for schedule(auto)
   for (int k = colPtr[rowIdx[m]] ; k < colPtr[rowIdx[m]+1]; k++){
    //#pragma omp parallel for schedule(auto)
    for (int l = m; l < colPtr[i+1] ; l++){
     if (rowIdx[l] == rowIdx[k] && rowIdx[l+1] <= rowIdx[k]){
      val[k] -= val[m]* val[l]; //S3
     }
    }
   }
  }
 }
}



void ic0_csr_wave_parallel(int n, double *val, int * colPtr, int *rowIdx,
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



void ic0_csr_hlevel_parallel(int n, double *val, int * colPtr, int *rowIdx,
                             int levels, int *levelPtr, int *levelSet,
                             int parts,  int *parPtr, int *partition,
                             int chunk) {

 int li = 0;

// for (i = 0; i < n - 1; i++){
 for (int i1 = 0; i1 < levels; ++i1) {
#pragma omp parallel //shared(lValues)//private(map, contribs)
  {
#pragma omp  for schedule(auto)
   for (int j1 = levelPtr[i1]; j1 < levelPtr[i1 + 1]; ++j1) {
    for (int k1 = parPtr[j1]; k1 < parPtr[j1 + 1]; ++k1) {
     int i = partition[k1];

/* for (int l = 0; l < levels; ++l) {
#pragma omp parallel for default(shared) private(li) schedule(static)
  for (li = levelPtr[l]; li < levelPtr[l + 1]; ++li) {
   int i = levelSet[li];*/

     val[colPtr[i]] = sqrt(val[colPtr[i]]);//S1

     for (int m = colPtr[i] + 1; m < colPtr[i + 1]; m++) {
      val[m] = val[m] / val[colPtr[i]];//S2
     }

     for (int m = colPtr[i] + 1; m < colPtr[i + 1]; m++) {
      for (int k = colPtr[rowIdx[m]]; k < colPtr[rowIdx[m] + 1]; k++) {
       for (int l = m; l < colPtr[i + 1]; l++) {
        if (rowIdx[l] == rowIdx[k] && rowIdx[l + 1] <= rowIdx[k]) {
#pragma omp atomic
         val[k] -= val[m] * val[l]; //S3
        }
       }
      }
     }
    }
   }
  }

 }
}

#endif
