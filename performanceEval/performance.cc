/*!
 * \file performance.cc
 *
 * \author Mahdi Soltan Mohammadi 
 *
 * Copyright (c) 2018, University of Arizona <br>
 * All rights reserved. <br>
 * See ./LICENSE for details. <br>
 *
 * This file is one of the drivers for reproducing the result for the paper titled:
   "Sparse Matrix Code Dependence Analysis Simplification at Compile Time"
   .
 *

 ** For building the dependencies see README.md

>> After build all the dependenciues you can build the driver (in root directory run):

g++ -O3 -o performance performance.cc -std=c++11

>> Run the driver (in root directory):

./performance.cpp execlist.txt

*/


#include <iostream>
#include<cstdlib>
#include<fstream>
#include <map>
using namespace std;

string getSMatName(string extName);


//------------------------------------------ MAIN ----------------------------------------
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cout<<"\n\nYou need to specify the input file. The input file should contain name of input matrix list and executables for experiments"
          "\n./performance.cpp execlist.txt\n\n";
  }

  ofstream outSerT("results/table4.csv", std::ofstream::out);
  ofstream outInsp("results/fig8.csv", std::ofstream::out);
  ofstream outExec("results/fig7.csv", std::ofstream::out);
  outSerT<<"Benchmark";
  outInsp<<"Benchmark";
  outExec<<"Benchmark";

  string execName;
  string execList = string(argv[1]);
  // Read name of json files from input list
  ifstream inF(execList);

  string inputMatrix;
  string inputMatrixList;
  getline( inF, inputMatrixList );

  ifstream matL(inputMatrixList);
  getline( matL, inputMatrix ); // Discard run parameters
  for(; getline( matL, inputMatrix );){ 
    outSerT<<", "<<getSMatName(inputMatrix);
    outInsp<<", "<<getSMatName(inputMatrix);
    outExec<<", "<<getSMatName(inputMatrix);
  }
  outSerT.close();
  outInsp.close();
  outExec.close();

  for(; getline( inF, execName );){
    std::cout<<"\n---- Processing "<<execName<<"\n";
    string comd = "./"+execName+" "+inputMatrixList;
    system(comd.c_str());
  }
//    std::cout<<"\n---- Processing "<<inputFileName<<"\n";

  outSerT.close();
  outInsp.close();
  outExec.close();

  return 0;
}




string getSMatName(string extName){
  std::size_t slp = extName.find_last_of('/');
  std::size_t dotp = extName.find_last_of('.');
  return extName.substr( slp+1 , (dotp-slp-1) );
}

