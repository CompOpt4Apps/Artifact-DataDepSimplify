/*!
 * \file simplifyForPartialParallelDriver.cc
 *
 * This file is a driver for using simplifyForPartialParallel 
 * and pertaining functions in IEgenLib.
 * These functions can be used to carry out simplification of 
 * data dependence relation in non-affine relations containing
 * uninterpreted function calls.
 *
 * To compile, after building IEgenLib with:

     ./configure    
     make

   Run the following command with YOUR OWN ADDRESSES:

     g++ -o EXECUTABLE simplifyDriver.cc -I IEGENLIB_HOME/src \
         IEGENLIB_HOME/build/src/libiegenlib.a -lisl -std=c++11

   You can also run following command after running "make install" for IEgenLIB:

       g++ -o EXECUTABLE simplifyDriver.cc \
           -I INSTALLATION_FOLDER/include/iegenlib cpp_api_example.cc \
              INSTALLATION_FOLDER/lib/libiegenlib.a -lisl -std=c++11
   
   IEGENLIB_HOME indicates where you have your copy of IEgenLIB.
   For example if you are compiling this file in its original location that is 
   IEGENLIB_HOME/src/drivers, FROM IEGENLIB_HOME run following to compile:

     g++ -o simplifyDriver src/drivers/simplifyDriver.cc -I src build/src/libiegenlib.a -lisl -std=c++11

 * Now to run the driver, you should put your dependence relations inside
 * JSON files and give them as inputs to the driver, one or more files
 * at a time. JSON file format is demonstrated by example sample.json 
 * in the same directory as this driver as well as several *.json files 
 * in IEGENLIB_HOME/data directory. The json input files include comment 
 * fields that have the application code, and says where in the code each
 * dependence relation is getting extracted from. So you can run following
 * after compiling the driver to get the simplified relations:
 
   build/bin/simplifyDriver sample.json

 * Note: the driver can read one or as many input json files as given.

 *     
 *
 * \date Date Started: 4/16/2017
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2016, University of Arizona <br>
 * All rights reserved. <br>
 * See COPYING for details. <br>
 * 
 *
 * HOW TO USE INTERFACE TO SIMPLIFICATION ALGORITHM for
 * simplifying constraints relation containing Uninterpreted Function Calls.
 * the overall process is explained in the 7 steps bellow, the simplify function
 * implementes these steps using IEGenLib's Functions:
 * 
 * (1) You need to define uninterpreted function calls (UFCs) that appear
 *        in constraints for iegenlib environment. These informations are 
 *        the first set of information that simplify function reads from 
 *        input json files and puts them in the IEGenLib enviroment.
 *        Note that, you need to do this only once for 
 *        relations that have same UFSs during one run.
 *
 * (2) You need to put constraints in iegenlib Relation (or Set).
 * 
 * (3) Determing unsatisfiability
 * 
 * (4) Create a std::set that includes which tuple variables (loop iterators)
 *     we are not going to project out. 
 *
 * (5) Apply a heuristic to remove expensive constraints that
 *     is keeping us from projecting out an iterator:
         for instance: 
                        col(j) < n  would keep us from projecting 'j'
       We only remove constraints up to a maximum number determined by user.
 * 
 * (6) Use simplifyForPartialParallel function that is main interface for the
 *     algorithm. If relation is not satisfiable the function would return NULL.
 *     Otherwise it would return the simplified result as iegenlib Relation.
 * 
 * (7) Print out result (if not NULL) using toISLString() function.  
 
 *  We have demonstrated these steps in the simplify function
 *  This function reads information from a JSON file (inputFile), and applies
 *  the simplification algorithm to the sets found in the file. 
 */


#include <iostream>
#include "iegenlib.h"
//#include "src/set_relation/Visitor.h"
#include "parser/jsoncons/json.hpp"

using jsoncons::json;
using namespace iegenlib;
//using iegenlib::Set;
//using iegenlib::Relation;
//using iegenlib::UFCallTerm;
using namespace std;


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! If you wish not to see the output, change the value: verbose = false;
bool verbose=true;

void simplify(string inputFile);

// Utility function
bool printRelation(string msg, Relation *rel);
void EXPECT_EQ(string a, string b);
void EXPECT_EQ(Relation *a, Relation *b);
int str2int(string str);


//----------------------- MAIN ---------------
int main(int argc, char **argv)
{

//CSR_Gauss_Seidel_Example();

  if (argc == 1)
  {
    cout<<"\n\nYou need to specify the input JSON files (one or more) that contain dependence relations:"
          "\n./simplifyDriver file1.json file2.json\n\n";
  } else if (argc >= 2){
    // Parsing command line arguments and reading given files.
    for(int arg = 1; arg < argc ; arg++){
      simplify(string(argv[arg]));
    }
  }

    return 0;
}

// Reads information from a JSON file (inputFile), and applies
// the simplification algorithm to the sets found in the file. 
void simplify(string inputFile)
{
/*
  iegenlib::setCurrEnv();
  std::set<int> parallelTvs;
  // (0)
  // Read the data from inputFile
  ifstream in(inputFile);
  json data;
  in >> data;

 for(size_t p = 0; p < data.size(); ++p){    // Dependence relations (DR) found in the file

  cout<<"\n\n"<<data[p][0]["Name"].as<string>()<<"\n\n";

  for (size_t i = 0; i < data[p].size(); ++i){// Conjunctions found for one DR in the file

    // (1) Putting constraints in an iegenlib::Relation
    // Reading original set.
    Relation* rel = new Relation(data[p][i]["Relation"].as<string>());

    // (2) Introduce the uninterpreted function symbols to environment, and 
    // indicate their domain, range, whether they are bijective, or monotonic.
    if( i == 0 ){  // Read these data only once. 
                   // They are stored in the first conjunction.

      json ufcs = data[p][i];
      // Read UFCs' data for code No. p, from ith relation
      addUFCs(ufcs);

      // Add defined domain information to environment
      json uqCons = data[p][0]["User Defined"];
      adduniQuantConstraints(uqCons);
    }

//    // Reading expected outputs
//    Relation *ex_rel = NULL;
//    string expected_str = data[p][i]["Expected"].as<string>();
//    if ( expected_str != string("Not Satisfiable") ){
//      ex_rel = new Relation(expected_str);
//    }

//  Commenting out these until we implement adding domain info based on
//  rule instantiation
//    // (3) Determining unsatisfiability
//    Relation* copyRelation = rel->boundDomainRange();
//    Relation* relAf = copyRelation->determineUnsat();

    char msg[100];
//    sprintf(msg, "@@@ Relation No. %d: ", int(i+1) );
//    printRelation( string(msg) , rel);
 
//    if(  relAf->isUnsat() ){
//      std::cout<<"\nIs unsatisfiable!\n";
//      continue;    
//    } else {
//      std::cout<<"\nMight be satisfiable!!\n";      
//    }

//    //sprintf(msg, "@@@ Relation No. %d After: ", int(i+1) );
//    //printRelation( string(msg) , relAf);

    // If conjunction is satisfiable at compile time try to simplify it

    // (4) 
    // Specify loops that are going to be parallelized, 
    // so we are not going to project them out.
    json npJ = data[p][0]["Do Not Project Out"];
    parallelTvs.clear();
    notProjectIters( rel, parallelTvs, npJ);


    // (5) 
    // Applying heuristic for removing expensive iterators
    int numConstToRemove = str2int(data[p][0]["Remove Constraints"].as<string>());
    Relation* relWithDR = rel->boundDomainRange();
    std::set<iegenlib::Exp> ignore = relWithDR->constraintsDifference(rel);
    relWithDR->removeExpensiveConstraints(parallelTvs, numConstToRemove, ignore);

//  Commenting out these until we implement adding domain info based on
//  rule instantiation
//    // Adding extra domain information
//    delete copyRelation;
//    copyRelation = relWithDR->determineUnsat();
//    delete relWithDR;
//    relWithDR = copyRelation;

    // (6)
    // Simplifyng the constraints relation
    Relation* rel_sim = relWithDR->simplifyForPartialParallel(parallelTvs);

    // (7)
    // Print out results: if not satisfiable returns NULL
    sprintf(msg, "@@@ Simplified relation No. %d: ", int(i+1) );
    printRelation( string(msg) , rel_sim);

//    delete copyRelation;
    delete rel;   
    delete relWithDR;
    delete rel_sim;
//    delete relAf;
  }

 } // End of p loop
*/
}

bool printRelation(string msg, Relation *rel){

    if ( rel ) {

        cout<<"\n\n"<<msg<<rel->toISLString()<<"\n\n";
    } else {

        cout<<"\n"<<msg<<"Not Satisfiable"<<"\n";
    }

    return true;
}

void EXPECT_EQ(string a, string b){

    if( a != b ){

        cout<<"\n\nExpected: "<<a;
        cout<<"\n\nActual:"<< b <<"\n\n";
    }
}

void EXPECT_EQ(Relation *a, Relation *b){

    if( a != b ){
        cout<<"\n\nIncorrect results: Expected or Actual is NULL.\n\n";
    }
}

int str2int(string str){
  int i;
  sscanf (str.c_str(),"%d",&i);
  return i;
}

