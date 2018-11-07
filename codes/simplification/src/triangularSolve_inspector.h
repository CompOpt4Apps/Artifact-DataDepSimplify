//
// Created by Mahdi on 25/09/18.
//

#include<vector>
#include <cassert>
//#include<set>

#ifndef triangularSolve_inspector_H
#define triangularSolve_inspector_H

// Makes an edge inside dependence graph
void connect(int v, int w, std::vector<std::vector<int>> &DAG){
  DAG[v].push_back( w );
}

/*
 ****** Inspector for level set parallelization of Forward Solve CSC's outer most loop
 */
int lsolve_insector (int n, int* Lp, int* Li, std::vector<std::vector<int>> &DAG){
  int In_2, In_4, Out_2;
  if (!Lp || !Li) return (0) ;                     /* check inputs */

  // Inspector
  for(In_2 = 0; In_2 < n; In_2++){
    for(In_4 = Lp[In_2]; In_4 < Lp[In_2+1]; In_4++){
      Out_2 = Li[In_4]; 
      if( In_2 <= Out_2){  //Mehdi, I replaced '<' with '<='
        connect(In_2,Out_2, DAG);
      }
    }
  }

  return (1) ;
}

#endif //triangularSolve_inspector_H
