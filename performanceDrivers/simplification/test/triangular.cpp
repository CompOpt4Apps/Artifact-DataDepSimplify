//
// Created by kazem on 30/07/18.
//

//
// Created by kazem on 7/12/18.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <omp.h>
#include "../src/triangularSolve.h"
#include "../src/util.h"

using namespace std;



int main(int argc, char *argv[]) {

 if(argc < 4){
  cout<<"Input missing arguments, you need three args: inputFile numThread numIteration\n";
 }

 std::string f1 = argv[1];
 int *colA;
 int *rowA;
 double *valA;
 size_t n, nnzA;

 std::chrono::time_point<std::chrono::system_clock> start, end;
 std::chrono::duration<double> elapsed_seconds;
 double durationSym = 0, duration3 = 0, duration2 = 0, duration1 = 0;

 if (!readMatrix(f1, n, nnzA, colA, rowA, valA))
  return -1;
 int numThread = atoi(argv[2]);
 int iterNo = atoi(argv[3]);

 cout<<f1<<","<<numThread<<";";

 omp_set_num_threads(numThread);



/*
 * ********************* Triangular Solve
 */


 double *x = new double[n]();



 //***************Serial
 for (int l = 0; l < iterNo; ++l) {
  rhsInit(n, colA, rowA, valA, x);
  start = std::chrono::system_clock::now();
  lsolve(n, colA, rowA, valA, x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  duration1 = elapsed_seconds.count();
  if (!testTriangular(n, x))
   std::cout << "##serial,";
  else
   std::cout << duration1 << ",";
 }
 cout<< ";;";

 //***************Parallel Inner loop
 for (int l = 0; l < iterNo; ++l) {
  rhsInit(n, colA, rowA, valA, x);
  start = std::chrono::system_clock::now();
  lsolve_parInner(n, colA, rowA, valA, x);
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  duration1 = elapsed_seconds.count();
  if (!testTriangular(n, x))
   std::cout << "##serial,";
  else
   std::cout << duration1 << ",";
 }
 cout<< ";;";



 delete []x;




}
