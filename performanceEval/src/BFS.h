//
// Created by kazem on 10/27/18.
//

#ifndef ICHOL_HLEVEL_BFS_H
#define ICHOL_HLEVEL_BFS_H
/*
 * Mixed BFS and topological sort for CSC format.
 */
int modifiedBFS_CSC(int n,
                    int *Lp,
                    int* Li,
                    int *inDegree,
                    bool *visited,
                    int *node2partition,
                    int* &levelPtr,
                    int* levelSet,
                    int bfsLevel,
                    std::vector<std::vector<int>> &newLeveledParList){

 std::vector<int> queue;
 //Let's do BFS for every leaf node of the
 for (int ii = levelPtr[bfsLevel]; ii < levelPtr[bfsLevel+1]; ++ii) {
  int curNode=levelSet[ii];
  assert(node2partition[curNode]>=0);
  queue.push_back(curNode);
  while (!queue.empty()){
   int popedNode=queue[0];
   queue.erase(queue.begin());
   visited[popedNode]=true;
   newLeveledParList[node2partition[popedNode]].push_back(popedNode);
   //Find the adjacent nodes
   for (int r = Lp[popedNode]; r < Lp[popedNode+1]; ++r) {
    int cn=Li[r];
    inDegree[cn]--;
    if(inDegree[cn]==1 && !visited[cn]){
     queue.push_back(cn);
    }
   }
  }
 }

}
#endif //ICHOL_HLEVEL_BFS_H
