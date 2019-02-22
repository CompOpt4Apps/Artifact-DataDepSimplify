//
// Created by kazem on 4/5/18.
//

#ifndef SIMPLIFICATION_UTIL_H
#define SIMPLIFICATION_UTIL_H

#include <iostream>
#include <sstream>
#include <fstream>

/*
 * reading a CSC matrix from a coordinate file, stored col-ordered
 */
bool readMatrix(std::string fName, size_t &n, size_t &NNZ, int* &col,
                int* &row, double* &val){
 /*This function reads the input matrix from "fName" file and
  * allocate memory for matrix A, L and U.
  * - The input file is a coordinate version and e
  * ach row of the file shows (col, row, nnz)
  * - The matrices are zero-indexed
  */

 std::ifstream inFile;
 inFile.open(fName);
 std::string line,banner, mtx, crd, arith, sym;
 /*  File format:
  *    %%MatrixMarket matrix coordinate real general/symmetric/...
  *    % ...
  *    % (optional comments)
  *    % ...
  *    #rows    #non-zero
  *    Triplet in the rest of lines: row    col    value
  */
 std::getline(inFile,line);
 for (unsigned i=0; i<line.length(); line[i]=tolower(line[i]),i++);
 std::istringstream iss(line);
 if (!(iss >> banner >> mtx >> crd >> arith >> sym)){
  std::cout<<"Invalid header (first line does not contain 5 tokens)\n";
  return false;
 }

 if(banner.compare("%%matrixmarket")) {
  std::cout<<"Invalid header (first token is not \"%%%%MatrixMarket\")\n";
  return false;
 }
 if(mtx.compare("matrix")) {
  std::cout<<"Not a matrix; this driver cannot handle that.\"\n";
  return false;
 }
 if(crd.compare("coordinate")) {
  std::cout<<"Not in coordinate format; this driver cannot handle that.\"\n";
  return false;
 }
 if(arith.compare("real")) {
  if(!arith.compare("complex")) {
   std::cout<<"Complex matrix; use zreadMM instead!\n";
   return false;
  }
  else if(!arith.compare("pattern")) {
   std::cout<<"Pattern matrix; values are needed!\n";
   return false;
  }
  else {
   std::cout<<"Unknown arithmetic\n";
   return false;
  }
 }
 while (!line.compare(0,1,"%"))
 {
  std::getline(inFile, line);
 }
 std::istringstream issDim(line);
 if (!(issDim >> n >> n >> NNZ)){
  std::cout<<"The matrix dimension is missing\n";
  return false;
 }
 if(n <= 0 || NNZ <= 0)
  return false;
 col = new int[n + 1]();
 row = new int[NNZ];
 val = new double[NNZ];
 if(!val || !col || !row)
  return false;
 //Initializing the result vector
 int y, x, colCnt=0, nnzCnt=0;
 double value;

 col[0]=0;
 for (int i = 0; nnzCnt<NNZ; ) {//Reading from file row by row
  inFile>>x;x--;
  inFile>>y;y--;//zero indexing
  inFile>>value;
  if(y > n)
   return false;
  if(y==i){
   val[nnzCnt]=value;
   row[nnzCnt]=x;
   colCnt++; nnzCnt++;
  }
  else{//New col
   col[i+1]=col[i]+colCnt;
   i++;//next iteration
   colCnt=1;
   val[nnzCnt]=value;
   row[nnzCnt]=x;
   nnzCnt++;
  }
 }
 col[n]= col[n - 1] + colCnt;//last col
 return true;
}








/*
 *
 */

void rhsInit(int n, int *Ap, int *Ai, double *Ax, double *b){
 /*generating a rhs that produces a result of all 1 vector*/
 for (int j = 0; j < n; ++j) {
  b[j]=0;
 }
 for (int c = 0; c < n ; ++c) {
  for (int cc = Ap[c]; cc < Ap[c + 1]; ++cc) {
   b[Ai[cc]]+=Ax[cc];
  }
 }
}

void rhsInit_csr(int n, int *Ap, int *Ai, double *Ax, double *b){
 /*generating a rhs that produces a result of all 1 vector*/
 for (int j = 0; j < n; ++j) {
  b[j]=0;
 }
 for (int c = 0; c < n ; ++c) {
  for (int cc = Ap[c]; cc < Ap[c + 1]; ++cc) {
   b[c]+=Ax[cc];
  }
 }
}
/*
 * Testing lower triangular solve to make sure all x elements are ONE.
 */

int testTriangular(size_t n, const double *x) {//Testing
 int test=0;
 for (int i = 0; i < n; ++i) {
  if(std::abs(1-x[i])<0.001)
   test++;
  /*else
   std::cout<<i<<";";*/
 }
 if(n-test>0){
  return false;
 }
 return true;
}
#endif //SIMPLIFICATION_UTIL_H
