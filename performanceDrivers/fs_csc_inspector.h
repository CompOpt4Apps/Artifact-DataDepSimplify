
#include<vector>
#include <cassert>
//#include<set>



// Makes an edge inside dependence graph
inline void connect(int v, int w, std::vector<std::vector<int>> &DAG){
  DAG[v].push_back( w );
}

/*
 ****** Inspector for level set parallelization of Forward Solve CSC's outer most loop
 */
void fs_csc_inspector(int n, int* Lp, int* Li, std::vector<std::vector<int>> &DAG){
  int In_2, In_4, Out_2;

  // Inspector
  for(In_2 = 0; In_2 < n; In_2++){
    for(In_4 = Lp[In_2]; In_4 < Lp[In_2+1]; In_4++){
      Out_2 = Li[In_4]; 
      if( In_2 < Out_2){ 
        connect(In_2,Out_2, DAG);
      }
    }
  }

}
