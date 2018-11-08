//
// Created by kazem on 4/5/18.
//

#ifndef SIMPLIFICATION_ICHOL_H
#define SIMPLIFICATION_ICHOL_H

#include <cstdio>
#include <vector>
#include <assert.h>

#undef  MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#undef  MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))

/*
 * Computes the DAG of dependency before simplification
 */
int computeDAG(int n, int *row, int *col, std::vector<std::vector<int>>& DAG){
 for(int i = 0 ; i < n ; i++ ){
  for(int  m = row[i]+1 ; m < row[i+1] ; m++ ){
   for(int l = m ; l < row[i+1] ; l++ ){
    int ip = col[m];
    for(int mp = row[ip]+1 ; mp < row[ip+1] ; mp++ ){
     int k = mp;
     if( row[col[m]] <= k && k < row[col[m]+1] ){
      for(int kp = row[col[mp]] ; kp < row[col[mp]+1] ; kp++ ){
       for(int lp = m ; lp < row[ip+1] ; lp++ ){
        if ( col[lp+1] <= col[kp] && col[l+1] <= col[k] && col[l] == col[k] && col[lp] == col[kp] ){
         if( i < ip ){
          // ip depends on i; So add an edge from ip to i in dependency graph.
          DAG[i].push_back(ip);
         }
         else if (ip < i){
          // i depends on ip So add an edge from i to ip in dependency graph.
          DAG[ip].push_back(i);
         }
        }
       }
      }
     }
    }
   }
  }
 }
#ifdef VERIFY
 // Make sure that all nodes exist in the computed DAG
 bool *toCheck = new bool[n];
 for (int j = 0; j < n; ++j) {
  toCheck[j]=false;
 }
 for (int j = 0; j < DAG.size(); ++j) {
  for (int i = 0; i < DAG[j].size(); ++i) {
   toCheck[DAG[j][i]]=true;
  }
 }
 for (int j = 0; j < n; ++j) {
  assert(toCheck[j]);
 }
#endif
 return 1;
}

/*
 * Computes the DAG of dependency after simplification
 */
int computeDAG_simplified(int n, int *row, int *col, std::vector<std::vector<int>>& DAG){
 for(int i = 0 ; i < n ; i++ ){
  for(int m = row[i]+1 ; m < row[i+1] ; m++ ){
   int ip = col[m];
   for(int mp = MAX(row[col[m]],row[ip]+1) ; mp < MIN(row[col[m]+1],row[ip+1]) ; mp++ ){
    if(row[col[mp]] < row[col[mp]+1]){
     if( i < ip ){
      // ip depends on i: So add an edge from ip to i in dependency graph.
      DAG[i].push_back(ip);
     }
     else if (ip < i){
      // i depends on ip: So add an edge from i to ip in dependency graph.
      DAG[ip].push_back(i);
     }
    }
   }
  }
 }
#ifdef VERIFY
 // Make sure that all nodes exist in the computed DAG
bool *toCheck = new bool[n];
 for (int j = 0; j < n; ++j) {
  toCheck[j]=false;
 }
 for (int j = 0; j < DAG.size(); ++j) {
  for (int i = 0; i < DAG[j].size(); ++i) {
   toCheck[DAG[j][i]]=true;
  }
 }
 for (int j = 0; j < n; ++j) {
  assert(toCheck[j]);
 }
#endif
 return 1;
}
















/*
 * Computes the DAG of dependency after simplification
 */
int computeDAG_Test(int n, int *colPtr, int *rowIdx, std::vector<std::vector<int>>& DAG){

//std::cout <<"---------- computeDAG_Test: Hi1!\n\n";

  // Inspector from r3
  for(int i = 0 ; i <= n-1 ; i++ ){
            DAG[i].push_back(i);
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

//std::cout <<"---------- computeDAG_Test: Hi2!\n\n";

  //Inspectro from r22
  for(int i = 0; i <= n-1; i++) {
    for(int m = colPtr[(i)]+1; m <= colPtr[(i+1)]-1; m++) {
      if (rowIdx[(m)] >= i+1 && n >= rowIdx[(m)]) {//if (rowIdx[(m)] >= i+1 && n >= rowIdx[(m)]+2) {
        for(int k = colPtr[rowIdx[m]]; k <= colPtr[rowIdx[m]+1]-1; k++) {
          for(int l = m; l <= colPtr[(i+1)]-1; l++) {
            if (rowIdx[(l)] == rowIdx[(k)] && rowIdx[(l + 1)] <= rowIdx[(l)] ) {
              int ip=rowIdx[(m)];
              if (colPtr[(ip+1)]+1 >= k+1 && k >= colPtr[(ip)] ) {
                DAG[i].push_back(ip);
              }
            }
          }
        }
      }
    }
  }

//std::cout <<"---------- computeDAG_Test: Hi3!\n\n";
/*

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
 return 1;
}

#endif //SIMPLIFICATION_ICHOL_H
