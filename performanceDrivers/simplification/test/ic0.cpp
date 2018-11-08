// Mahdi:
// Inside test directory compile the code with:
// g++ -o ic0 ic0.cpp -std=c++11 -fopenmp -O3
// Run the code with:
// ./ic0 smatList.txt > result.txt
// Note, smatList.txt should contains list of input matrices.

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "omp.h"
#include "../src/ic0_csc.h"
#include "../src/iChol.h"
#include "../src/util.h"
#include "../src/levelSet.h"
#include "../src/triangularSolve_inspector.h"

#undef DEBUG

using namespace std;



int main(int argc, char *argv[]) {

  if(argc < 2){
   cout<<"Input missing arguments, you need to specify input list file\n";
  }

  std::string inputMatrix;
  int *colA;
  int *rowA;
  double *valA;
  size_t n, nnzA;
  double *x;
  int nRuns = 5;

  std::chrono::time_point<std::chrono::system_clock> startT, endT;//,startTT, endTT;
  std::chrono::duration<double> elapsed_secondsT;
  double durationDR[20] = {0.0}, durationE[20] = {0.0}, durationTT[20] = {0.0};
  double serialMedTT = 0.0, serialMedE = 0.0, serialAvgTT = 0.0, serialAvgE = 0.0;

  int maxTC;
  int iterNo;

 string parameters;
 ifstream inL(argv[1]);
 getline( inL, parameters );
 sscanf(parameters.c_str(), "%d, %d, %d",&maxTC, &iterNo,&nRuns);
 // Looping over inout matricies
 for(; getline( inL, inputMatrix );){ 
 
  if(inputMatrix=="") break;
  serialMedTT = serialMedE = serialAvgTT = serialAvgE = 0.0;

  std::cout <<"\n\n\n\n\nMatrix: "<<inputMatrix<<"  MaxNumThreads = "<<maxTC<<"  NumIter = "<<iterNo<<"\n\n";


  std::cout <<"-- Running the algorithm sequentially for #"<<nRuns<<" times:\n";

  for(int j=0;j<20;j++){
    durationDR[j] = durationE[j] = durationTT[j] = 0.0f;
  }
//  startTT = std::chrono::system_clock::now();

  for(int i=0; i<nRuns; i++){

    //std::cout <<"\n\n---------- Reading Data: \n\n";
    startT = std::chrono::system_clock::now();

    if (!readMatrix(inputMatrix, n, nnzA, colA, rowA, valA)) return -1;
    x = new double[n]();
#ifdef DEBUG
std::cout <<"\n\n---------- ValA Original:\n";
for(int cc = 0; cc <nnzA; cc++)
  std::cout <<"val["<<cc<<"] = "<<valA[cc]<<"\n";
std::cout <<"---------- Val\n\n";
#endif
    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationDR[i] = elapsed_secondsT.count();
    //std::cout <<"\n>>>>>>>>>>>> Data Read Total Duration = "<< durationT <<"\n";

    //std::cout <<"\n\n---------- Serial: \n\n";
    startT = std::chrono::system_clock::now();
    for (int l = 0; l < iterNo; ++l) {
      ic0_csr(n,  valA, colA, rowA);
    }
#ifdef DEBUG
std::cout <<"\n\n---------- ValA After Serial run:\n";
for(int cc = 0; cc <nnzA; cc++)
  std::cout <<"val["<<cc<<"] = "<<valA[cc]<<"\n";
std::cout <<"---------- Val\n\n";
#endif
    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationE[i] = elapsed_secondsT.count();
    //std::cout <<"\n>>>>>>>>>>>> Serial Total Duration = "<< durationT <<"\n";

    durationTT[i] = durationDR[i] + durationE[i];
    std::cout <<"\n>>>>>> Run #"<<i+1<<":\n";
    std::cout <<">>>>>> Data Read time = "<< durationDR[i] <<"\n";
    std::cout <<">>>>>> Execution time (without data read) = "<<durationE[i] <<"\n";
    std::cout <<">>>>>> Execution time (totally) = "<<durationTT[i] <<"\n";
  }
  
  std::sort(durationE, durationE+nRuns);
  std::sort(durationTT, durationTT+nRuns);
  double medE, medTT;
  if(nRuns%2) {
    medE = durationE[(int(nRuns/2))];
    medTT = durationTT[(int(nRuns/2))];
  } else {
    medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
    medTT = (durationTT[(int(nRuns/2))]+durationTT[(int(nRuns/2))-1])/2.0;
  }
  double avgE=0.0, avgTT=0.0;
  for(int i=0; i<nRuns; i++){
    avgE += durationE[i];
    avgTT += durationTT[i];
  }
  avgE /= nRuns;
  avgTT /= nRuns;

  serialMedE = medE;
  serialAvgE = avgE;
  serialMedTT = medTT;
  serialAvgTT = avgTT;
  std::cout <<"\n>>>>>>>>>>>> Median of Execution times (without data read) = "<<medE <<"\n";
  std::cout <<">>>>>>>>>>>> Averaged Execution time (without data read) = "<<avgE <<"\n";
  std::cout <<">>>>>>>>>>>> Median of Execution times (totally) = "<<medTT <<"\n";
  std::cout <<">>>>>>>>>>>> Averaged Execution time (totally) = "<<avgTT <<"\n\n\n";

 int *colA1;
 int *rowA1;
 double *valA1;

// for(int tc=2; tc <=maxTC; tc *=2){
 for(int tc=maxTC; tc <=maxTC; tc *=2){

  omp_set_num_threads(tc);

  std::cout <<"\n-- Running the algorithm with #"<<tc<<" threads in parallel for #"<<nRuns<<" times:\n";
  for(int j=0;j<20;j++){
    durationDR[j] = durationE[j] = durationTT[j] = 0.0f;
  }

  for(int i=0; i<nRuns; i++){

    //std::cout <<"\n\n---------- Reading Data: \n\n";
    startT = std::chrono::system_clock::now();

    if (!readMatrix(inputMatrix, n, nnzA, colA1, rowA1, valA1)) return -1;
    x = new double[n]();
#ifdef DEBUG
std::cout <<"\n\n---------- ValA1 Original:\n";
for(int cc = 0; cc <nnzA; cc++)
  std::cout <<"val["<<cc<<"] = "<<valA1[cc]<<"\n";
std::cout <<"---------- Val1\n\n";
#endif
    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationDR[i] = elapsed_secondsT.count();
    //std::cout <<"\n>>>>>>>>>>>> Data Read Total Duration = "<< durationT <<"\n";


    //std::cout <<"\n\n---------- Parallel: \n\n";
    startT = std::chrono::system_clock::now();

    std::vector<std::vector<int>> DAG;
    //std::vector<std::vector<int>> DAG_simpl;
    DAG.resize(n);
    //DAG_simpl.resize(n);
    int *levelPtr, *levelSet, levels;
    // Running the inspector for one time
    computeDAG_Test(n,colA1,rowA1, DAG);

    //buildDAG_CSC(n,colA1,rowA1, DAG);
    //lsolve_insector(n, colA1, rowA1, DAG);
//#define VERIFY
//#ifdef DEBUG
   for (int j = 0; j < DAG.size(); ++j) {
    for (int i = 0; i < DAG[j].size(); ++i) {
     //assert([DAG[j][i]]==DAG_simpl[j][i]);
std::cout <<"DAG["<<j<<"]["<<i<<"] = "<<DAG[j][i]<<"\n";
    }
   }
//#endif
    // Building the level sets
    levels = buildLevelSet_DAG(n, DAG, levelPtr, levelSet);

    for (int l = 0; l < iterNo; ++l) {
      ic0_csr_wave_parallel(n,  valA1, colA1, rowA1, levels, levelPtr, levelSet, 1 );
//      ic0_csr(n,  valA1, colA1, rowA1);
    }

#ifdef DEBUG
std::cout <<"\n\n---------- ValA1 After parallel run:\n";
for(int cc = 0; cc <nnzA; cc++)
  std::cout <<"val["<<cc<<"] = "<<valA1[cc]<<"\n";
std::cout <<"---------- Val1\n\n";
#endif


    endT = std::chrono::system_clock::now();
    elapsed_secondsT = endT - startT;
    durationE[i] = elapsed_secondsT.count();
    //std::cout <<"\n>>>>>>>>>>>> Parallel Total Duration = "<< durationT <<"\n";

    durationTT[i] = durationDR[i] + durationE[i];
    std::cout <<"\n>>>>>> Run #"<<i+1<<":\n";
    std::cout <<">>>>>> Data Read time = "<< durationDR[i] <<"\n";
    std::cout <<">>>>>> Execution time (without data read) = "<<durationE[i] <<"\n";
    std::cout <<">>>>>> Execution time (totally) = "<<durationTT[i] <<"\n";
  }
  
  std::sort(durationE, durationE+nRuns);
  std::sort(durationTT, durationTT+nRuns);
  double medE, medTT;
  if(nRuns%2) {
    medE = durationE[(int(nRuns/2))];
    medTT = durationTT[(int(nRuns/2))];
  } else {
    medE = (durationE[(int(nRuns/2))]+durationE[(int(nRuns/2))-1])/2.0;
    medTT = (durationTT[(int(nRuns/2))]+durationTT[(int(nRuns/2))-1])/2.0;
  }
  double avgE=0.0, avgTT=0.0;
  for(int i=0; i<nRuns; i++){
    avgE += durationE[i];
    avgTT += durationTT[i];
  }
  avgE /= nRuns;
  avgTT /= nRuns;

  std::cout <<"\n>>>>>>>>>>>> Median of Execution times (without data read) = "<<medE <<"\n";
  std::cout <<">>> Normalized = "<< (serialMedE/medE) <<"\n";
  std::cout <<">>>>>>>>>>>> Averaged Execution time (without data read) = "<<avgE <<"\n";
  std::cout <<">>> Normalized = "<< (serialAvgE/avgE) <<"\n";
  std::cout <<">>>>>>>>>>>> Median of Execution times (totally) = "<<medTT <<"\n";
  std::cout <<">>> Normalized = "<< (serialMedTT/medTT) <<"\n";
  std::cout <<">>>>>>>>>>>> Averaged Execution time (totally) = "<<avgTT <<"\n";
  std::cout <<">>> Normalized = "<< (serialAvgTT/avgTT) <<"\n\n\n";

  
  bool failed=false;

  for (int i = 0; i < nnzA; ++i) {
    if(valA[i]-valA1[i] > 10e-4){
     failed=true;
     std::cout<<"== "<<i<<"\n";
    }
  }
  if(failed) std::cout<<"\n\nTC = "<<"  faild!\n";
  delete valA1;
  delete colA1;
  delete rowA1;
 }
  delete valA;
  delete colA;
  delete rowA;

 }

  return 0;

}
