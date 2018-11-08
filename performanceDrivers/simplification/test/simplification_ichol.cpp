

#include <iostream>
#include <mkl.h>
#include "../src/util.h"
#include "../src/iChol.h"
#include "../src/levelSet.h"

#define DEBUG 0
int main(int argc, char *argv[]) {
 std::string f1 = argv[1];
 int *colA, *rowA;
 double *valA;
 size_t n, nnzA;


 if (!readMatrix(f1, n, nnzA, colA, rowA, valA))
  return -1;

 MKL_INT job[7] = {1, 0, 0, 0, 0, 1, 1};
 MKL_INT info = 0;
 const MKL_INT size_csr = n;
 double *csrVal = (double *) mkl_calloc(nnzA, sizeof(double), 64);
 MKL_INT *csrColInd = (MKL_INT *) mkl_calloc(nnzA, sizeof(MKL_INT), 64);
 MKL_INT *csrRowPtr = (MKL_INT *) mkl_calloc(n + 1, sizeof(MKL_INT), 64);
 int *levelPtr = new int[n+1](), *levelSet = new int[n]();
 int *levelPtr_opt = new int[n+1](), *levelSet_opt = new int[n]();
 std::vector<std::vector<int>> DAG;
 DAG.resize(n);

 mkl_dcsrcsc(job, &size_csr, csrVal, csrColInd, csrRowPtr, valA, rowA, colA, &info);

 computeDAG_simplified(n,csrRowPtr,csrColInd,DAG);

 buildLevelSet_DAG(n,DAG,levelPtr,levelSet);

 buildLevelSet_CSC(n,nnzA,colA,rowA,levelPtr_opt,levelSet_opt);

#if DEBUG==1
 for (size_t i = 0; i < n; ++i) {
  for (int j = levelPtr_opt[i]; j < levelPtr_opt[i+1]; ++j) {
   if(levelSet[j] != levelSet_opt[j]){
    std::cout<<i<<";"<<j<<";"<<levelSet[j]<<";"<<levelSet_opt[j]<<";";
   }
  }
 }
#endif

 delete []levelPtr;
 delete []levelSet;
 delete []levelPtr_opt;
 delete []levelSet_opt;
 delete []colA;
 delete []rowA;
 delete []valA;
 mkl_free(csrVal);
 mkl_free(csrColInd);
 mkl_free(csrRowPtr);
 return 0;
}