//
// Created by kazem on 4/5/18.
//

#ifndef SIMPLIFICATION_LEVELSET_H
#define SIMPLIFICATION_LEVELSET_H

#include <vector>
#include <assert.h>

#define NOEDGE 1
/*
 *Builds the levelset from the CSC format
 */
int buildLevelSet_CSC(int n, int nnz, int *Lp, int *Li, int *&levelPtr,
                      int *&levelSet){
 int begin=0,end=n-1;
 int curLevel=0, curLevelCol=0;
 levelPtr = new int[n+1]();
 levelSet = new int[n]();
 int *inDegree = new int[n]();
 bool *visited = new bool[n]();
 for (int i = 0; i < Lp[n]; ++i) {//O(nnz)
  inDegree[Li[i]]++;
 }
#if 0
 for (int k = 0; k < n; ++k) {
        std::cout<<inDegree[k]<<",";
    }
    std::cout<<"\n";
#endif
 while(begin <= end){
  for (int i = begin; i <= end; ++i) {//For level curLevel
   if (inDegree[i] == 1 && !visited[i]) {//if no incoming edge
    visited[i] = true;
    levelSet[curLevelCol] = i; //add it to current level
    curLevelCol++;//Adding to level-set
   }
  }
  curLevel++;//all nodes with zero indegree are processed.
  levelPtr[curLevel]=curLevelCol;
  while(inDegree[begin]==1)
   begin++;
  while(inDegree[end]==1 && begin <= end)
   end--;
  //Updating degrees after removing the nodes
  for (int l = levelPtr[curLevel-1]; l < levelPtr[curLevel]; ++l) {
   int cc=levelSet[l];
   for (int j = Lp[cc] + 1; j < Lp[cc + 1]; ++j) {
    inDegree[Li[j]]--;//removing corresponding edges
   }
  }
#if 0
  for (int k = 0; k < n; ++k) {
                std::cout<<inDegree[k]<<",";
            }
            std::cout<<"\n";
#endif

 }
#ifdef VERIFY
 bool *toCheck = new bool[n];
 for (int j = 0; j < n; ++j) {
  toCheck[j]=false;
 }
 for (size_t i = 0; i < n; ++i) {
  for (int j = levelPtr[i]; j < levelPtr[i+1]; ++j) {
   toCheck[levelSet[j]]=true;
  }
 }
 for (int j = 0; j < n; ++j) {
  assert(toCheck[j]);
 }
#endif
 return curLevel;//return number of levels
}


/*
 *
 */
int buildLevelSet_DAG(int n, std::vector<std::vector<int>>& DAG, int *&levelPtr,
                      int *&levelSet){
 int begin=0,end=n-1;
 int curLevel=0, curLevelCol=0;
 levelPtr = new int[n+1]();
 levelSet = new int[n]();
 int *inDegree = new int[n]();
 bool *visited = new bool[n]();
 for (int i = 0; i < DAG.size(); ++i) {//O(nnz)
  for (int j = 0; j < DAG[i].size(); ++j) {
   inDegree[DAG[i][j]]++;
  }
 }
 // Extra to make self-edges
/* for (int i = 0; i < DAG.size(); ++i) {
   inDegree[i]++;
 }*/
#if 0
 for (int k = 0; k < n; ++k) {
        std::cout<<inDegree[k]<<",";
    }
    std::cout<<"\n";
#endif
 while(begin <= end){
  for (int i = begin; i <= end; ++i) {//For level curLevel
   if (inDegree[i] == NOEDGE && !visited[i]) {//if no incoming edge
    visited[i] = true;
    levelSet[curLevelCol] = i; //add it to current level
    curLevelCol++;//Adding to level-set
   }
  }
  curLevel++;//all nodes with zero indegree are processed.
  levelPtr[curLevel]=curLevelCol;
  while(inDegree[begin] == NOEDGE)
   begin++;
  while(inDegree[end] == NOEDGE && begin <= end)
   end--;
  //Updating degrees after removing the nodes
  for (int l = levelPtr[curLevel-1]; l < levelPtr[curLevel]; ++l) {
   int cc=levelSet[l];
   for (int j = 1; j < DAG[cc].size(); ++j) {
    inDegree[DAG[cc][j]]--;//removing corresponding edges
   }
  }
#if 0
  for (int k = 0; k < n; ++k) {
                std::cout<<inDegree[k]<<",";
            }
            std::cout<<"\n";
#endif
 }
 return curLevel;//return number of levels
}

/*
 * Build DAG from CSC
 */
void buildDAG_CSC(int n, int *Lp, int *Li,
                 std::vector<std::vector<int>>& DAG){
 for (int k = 0; k < n; ++k) {
  for (int i = Lp[k]; i < Lp[k+1]; ++i) {
   DAG[k].push_back(Li[i]);
  }
 }

#ifdef VERIFY
 bool *toCheck = new bool[n];
 for (int j = 0; j < n; ++j) {
  toCheck[j]=false;
 }
 for (size_t i = 0; i < n; ++i) {
  assert(DAG[i].size() >= 1);
  for (int j = 0; j < DAG[i].size(); ++j) {
   toCheck[DAG[i][j]]=true;
  }
 }
 for (int j = 0; j < n; ++j) {
  assert(toCheck[j]);
 }
#endif
}

#endif //SIMPLIFICATION_LEVELSET_H
