//
//


#include <cstdio>
#include <vector>
#include <assert.h>
#include<set>

#undef  MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#undef  MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))



/*
 * Computes the DAG of dependency after simplification
*/
void lchol_csc_inspector(int n, int *prunePtr, int *pruneSet, std::vector<std::vector<int>>& DAG){


#define pruneSet(Out_2,Out_4) pruneSet[Out_4]
#define prunePtr(Out_2) prunePtr[Out_2]
#define prunePtr_(Out_2) prunePtr[Out_2 + 1]
#define s0(Out_2,Out_4,In_2) DAG[In_2].push_back( Out_2 );

// Omega generated Code Generated 
#pragma omp parallel for schedule(auto)
for(int t1 = 1; t1 <= n-1; t1++) {
  for(int t2 = prunePtr(t1); t2 <= prunePtr_(t1)-1; t2++) {
    if (pruneSet(t1,t2) >= 0 && t1 >= pruneSet(t1,t2)+1) {
      int t3=pruneSet(t1,t2);
      s0(t1,t2,t3);
    }
  }
}

}




/*
 * Computes the DAG of dependency after simplification

void lch_csc_inspector(int n, int *prunePtr, int *pruneSet, std::vector<std::set<int>>& DAG){


#define pruneSet(Out_2,Out_4) pruneSet[Out_4]
#define prunePtr(Out_2) prunePtr[Out_2]
#define prunePtr_(Out_2) prunePtr[Out_2 + 1]
#define s0(Out_2,Out_4,In_2) DAG[In_2].insert( Out_2 );

// Omega generated Code Generated 
#pragma omp parallel for schedule(auto)
for(int t1 = 1; t1 <= n-1; t1++) {
  for(int t2 = prunePtr(t1); t2 <= prunePtr_(t1)-1; t2++) {
    if (pruneSet(t1,t2) >= 0 && t1 >= pruneSet(t1,t2)+1) {
      int t3=pruneSet(t1,t2);
      s0(t1,t2,t3);
    }
  }
}

}

*/


