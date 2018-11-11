//
//


#include <cstdio>
#include <vector>
#include <assert.h>

#undef  MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#undef  MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))




/*
 * Computes the DAG of dependency after simplification
*/
void ic0_csc_inspector(int n, int *colPtr, int *rowIdx, std::vector<std::vector<int>>& DAG){


  // Inspector from r3
  #pragma omp parallel for schedule(auto)
  for(int i = 0 ; i <= n-1 ; i++ ){
    for(int m = colPtr[i]+1 ; m < colPtr[i+1] ; m++ ){
      if( i < rowIdx[m] ){
        int ip = rowIdx[m];
        int k = colPtr[ip];
        for(int l = m ; l < colPtr[i+1] ; l++ ){
          if(rowIdx[(l)] == rowIdx[(k)] && rowIdx[(l + 1)] <= rowIdx[(l)] ){
            DAG[i].push_back(ip);
          }
        }
      }
    }
  }
/*
  //Inspectro from r22
  #pragma omp parallel for schedule(auto)
  for(int i = 0; i <= n-1; i++) {
    for(int m = colPtr[(i)]+1; m <= colPtr[(i+1)]-1; m++) {
      if (rowIdx[(m)] >= i+1 && n >= rowIdx[(m)]) {//if (rowIdx[(m)] >= i+1 && n >= rowIdx[(m)]+2) {
        for(int k = colPtr[rowIdx[m]]; k <= colPtr[rowIdx[m]+1]-1; k++) {
          for(int l = m; l <= colPtr[(i+1)]-1; l++) {
            if (rowIdx[(l)] == rowIdx[(k)] && rowIdx[(l + 1)] <= rowIdx[(l)] ) {
              int ip=rowIdx[(m)];
              if (colPtr[(ip+1)]+1 >= k+1 && k >= colPtr[(ip)] ) {
                DAG[i].push_back(ip);
//std::cout <<"---------- computeDAG_Test: DAG["<<i<<"]["<<ip<<"] \n";
              }
            }
          }
        }
      }
    }
  }

*/

// Omega generated Code Generated for r22 
#define rowIdx(i1,i2) rowIdx[i1]
#define rowIdx_(i1,i2,i3) rowIdx[i3]
#define rowIdx__(i1,i2,i3,i4) rowIdx[i4]
#define rowIdx___(i1,i2,i3,i4) rowIdx[i4 + 1]
#define colPtr(i1) colPtr[i1]
#define colPtr_(i1) colPtr[i1 + 1]
#define colPtr__(i,m,k,l,ip) colPtr[ip]
#define colPtr___(i1,i2,i3,i4,o1) colPtr[o1 + 1]
#define colPtr____(i1,i2) colPtr[rowIdx[i2]]
#define colPtr_____(i1,i2) colPtr[rowIdx[i2] + 1]

int t1,t2,t3,t4,t5;
#pragma omp parallel for schedule(auto)
for(t1 = 0; t1 <= n-1; t1++) {
  for(t2 = colPtr(t1)+1; t2 <= colPtr_(t1)-1; t2++) {
    if (rowIdx(t1,t2) >= t1+1 && n >= rowIdx(t1,t2)+2) {
      for(t3 = colPtr____(t1,t2); t3 <= colPtr_____(t1,t2)-1; t3++) {
        for(t4 = t2; t4 <= colPtr_(t1)-1; t4++) {
          if (rowIdx__(t1,t2,t3,t4) == rowIdx_(t1,t2,t3) && rowIdx__(t1,t2,t3,t4) >= rowIdx___(t1,t2,t3,t4)) {
            t5=rowIdx(t1,t2);
            if (colPtr___(t1,t2,t3,t4,t5) >= t3+1 && t3 >= colPtr__(t1,t2,t3,t4,t5)+1) {
              DAG[t1].push_back(t5);
            }
          }
        }
      }
    }
  }
}


}





void ic0_csc_inspector_app(int n, int *colPtr, int *rowIdx, std::vector<std::vector<int>>& DAG){

  // Inspector from r3
  #pragma omp parallel for schedule(auto)
  for(int i = 0 ; i <= n-1 ; i++ ){
    for(int m = colPtr[i]+1 ; m < colPtr[i+1] ; m++ ){
      if( i < rowIdx[m] ){
        int ip = rowIdx[m];
        if(colPtr[ip] < colPtr[rowIdx[m] + 1] && colPtr[rowIdx[m]] < colPtr[ip]+1 ){
            DAG[i].push_back(ip);
        }
      }
    }
  }
/*
  //Inspectro from r22
  for(int i = 0 ; i <= n-1 ; i++ ){
    for(int m = colPtr[i]+1 ; m < colPtr[i+1] ; m++ ){
      if( i < rowIdx[m] ){
        int ip = rowIdx[m];
        if(colPtr[rowIdx[m]] < colPtr[rowIdx[m] + 1] && colPtr[ip] < colPtr[ip + 1] && 
           colPtr[ip] < colPtr[rowIdx[m] + 1] && colPtr[rowIdx[m]] < colPtr[ip + 1] ){
            DAG[i].push_back(ip);
        }
      }
    }
  }

/*

// Omega generated Code Generated for r22 
#define rowIdx(i,m) rowIdx[m]
#define rowIdx_(i,m,k) rowIdx[k]
#define rowIdx__(i,m,k,l) rowIdx[l]
#define rowIdx___(i1,i2,i3,i4) rowIdx[i4 + 1]
#define colPtr(i1) colPtr[i1]
#define colPtr_(i1) colPtr[i1 + 1]
#define colPtr__(i,m,k,l,ip) colPtr[ip]
#define colPtr___(i1,i2,i3,i4,o1) colPtr[o1 + 1]
#define colPtr____(i1,i2) colPtr[rowIdx[i2]]
#define colPtr_____(i1,i2) colPtr[rowIdx[i2] + 1]

int t1,t2,t3,t4,t5;

for(t1 = 0; t1 <= n-3; t1++) {
  for(t2 = colPtr(t1)+1; t2 <= colPtr_(t1)-1; t2++) {
    if (rowIdx(t1,t2) >= t1+1 && n >= rowIdx(t1,t2)+2) {
      for(t3 = colPtr____(t1,t2); t3 <= colPtr_____(t1,t2)-1; t3++) {
        for(t4 = t2; t4 <= colPtr_(t1)-1; t4++) {
          if (rowIdx__(t1,t2,t3,t4) == rowIdx_(t1,t2,t3) && rowIdx__(t1,t2,t3,t4) >= rowIdx___(t1,t2,t3,t4)) {
            t5=rowIdx(t1,t2);
            if (colPtr___(t1,t2,t3,t4,t5) >= t3+1 && t3 >= colPtr__(t1,t2,t3,t4,t5)+1) {
              DAG[t1].push_back(t5);
            }
          }
        }
      }
    }
  }
}

*/
}

