//
// Created by kazem on 30/07/18.
//

#ifndef TRIANGULAR_TRIANGULARSOLVE_H
#define TRIANGULAR_TRIANGULARSOLVE_H

/*
 ****** Serial implementation
 */
int lsolve (int n, int* Lp, int* Li, double* Lx, double *x){
 int j;
 if (!Lp || !Li || !x) return (0) ;                     /* check inputs */
 for (j = 0 ; j < n ; j++)
 {
  x [j] /= Lx [Lp [j]] ;
  for ( int p = Lp [j]+1 ; p < Lp [j+1] ; p++)
  {
   x [Li [p]] -= Lx [p] * x [j] ;
  }
 }
 return (1) ;
}


/*
 ****** Parallel inner loop implementation
 */
int lsolve_parInner (int n, int* Lp, int* Li, double* Lx, double *x){
 int  j;
 if (!Lp || !Li || !x) return (0) ;                     /* check inputs */
 for (j = 0 ; j < n ; j++)
 {
  x [j] /= Lx [Lp [j]] ;
#pragma omp parallel for schedule(auto)
  for (int p = Lp [j]+1 ; p < Lp [j+1] ; p++)
  {
   x [Li [p]] -= Lx [p] * x [j] ;
  }
 }
 return (1) ;
}


/*
 ****** Parallel outer loop implementation using level set
 */
int lsolvePar (int n, int* Lp, int* Li, double* Lx, double *x,
               int levels, int *levelPtr, int *levelSet, int chunk){
 if (!Lp || !Li || !x) return (0) ;                     /* check inputs */
 for (int l = 0; l < levels; ++l) {
  int li=0;
#pragma omp parallel for \
   default(shared) private(li)  \
   schedule(auto)
  for ( li = levelPtr[l]; li < levelPtr[l + 1]; ++li) {
   int j = levelSet[li];
   x [j] /= Lx [Lp [j]] ;
   for (int p = Lp [j]+1 ; p < Lp [j+1] ; p++)
   {
    double tmp =  Lx [p] * x [j] ;
    int idx = Li[p];
#pragma omp atomic
    x [idx] -= tmp ;
   }
  }
 }
 return (1) ;
}

/*
 ****** Parallel outer loop implementation using ParSy paper (LBC)
 */
int lsolveParH2 (int n, int* Lp, int* Li, double* Lx, double *x,
                 int levels, int *levelPtr, int *levelSet,
                 int parts,  int *parPtr, int *partition,
                 int chunk){
 if (!Lp || !Li || !x) return (0) ;                     /* check inputs */
 for (int i1 = 0; i1 < levels ; ++i1) {
#pragma omp parallel //shared(lValues)//private(map, contribs)
  {
#pragma omp  for schedule(auto)
   for (int j1 = levelPtr[i1]; j1 < levelPtr[i1 + 1]; ++j1) {
    for (int k1 = parPtr[j1]; k1 < parPtr[j1 + 1]; ++k1) {
     int j = partition[k1];
     x[j] /= Lx[Lp[j]];
     //    #pragma omp critical
     for (int p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      double tmp = Lx[p] * x[j];
      int idx = Li[p];
#pragma omp atomic
      x[idx] -= tmp ;
     }
    }
   }
  }
 }
 return (1) ;
}


#endif //TRIANGULAR_TRIANGULARSOLVE_H
