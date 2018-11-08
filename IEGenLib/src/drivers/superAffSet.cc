/*!
 * \file superAffSet.cc
 *

>> Build IEGenLib (run in the root directory):

./configure
make

>> The driver, superAffSet, should be at build/bin/

>> Build the driver separately (in root directory run):

g++ -O3 -o superAffSet src/drivers/superAffSet.cc 
    -I src build/src/libiegenlib.a -lisl -std=c++11

>> Run the driver (in root directory):

./superAffSet SOME_EXAMPLE.json

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

void superAff(string inputFile);

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
          "\n./superAffSet file1.json file2.json\n\n";
  } else if (argc >= 2){
    // Parsing command line arguments and reading given files.
    for(int arg = 1; arg < argc ; arg++){
      superAff(string(argv[arg]));
    }
  }

    return 0;
}

// Reads information from a JSON file (inputFile), and outputs
// superaffine version of the Relations found in the file. 
void superAff(string inputFile)
{
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
      json uqCons = data[p][i]["User Defined"];
      addUniQuantRules(uqCons);
    }

    bool* useRule = new bool[ TheOthers ];
    for(int i = 0 ; i < TheOthers ; i++ ){ useRule[i] = 1; }

    //iegenlib::UFCallMap* uf_call_map = new UFCallMap();

    Relation* result = rel->superAffineRelation();


    cout<<"\n>>> Super Affine Relation No. "<<i<<": "<<result->toISLString()<<"\n\n";
  }

 } // End of p loop


}

void EXPECT_EQ(Relation *result, Relation *expected){

    if( result == NULL && expected == NULL ){
        cout<<"\n\nResult MATCH Expected: Not Satisfiable.\n\n";
    }

    else if( result != NULL && expected == NULL ){
        cout<<"\n\nResult DOES NOT MATCH Expected:\n"
              "Expected: Not Satisfiable\nResult:\n"<<result->toISLString()<<"\n\n";
    }

    else if( result == NULL && expected != NULL ){
        cout<<"\n\nResult DOES NOT MATCH Expected:\n"
              "Expected:\n"<<expected->toISLString()<<"\nResult: Not Satisfiable\n\n";
    }


    else if( result != NULL && expected != NULL ){
      // Replace uf calls with the variables to create an affine superset.
      Relation * supAffRes = result->superAffineRelation();
      Relation * supAffExp = expected->superAffineRelation();

      isl_ctx* ctx = isl_ctx_alloc();
      isl_map* resISL =  isl_map_read_from_str(ctx, (supAffRes->toISLString().c_str()));
      isl_map* expISL =  isl_map_read_from_str(ctx, (supAffExp->toISLString().c_str()));

      //if( result->toISLString() == expected->toISLString() ){
      if( isl_map_plain_is_equal( resISL , expISL ) ){
        cout<<"\n\nResult MATCH Expected: Which is:\n"<<result->toISLString()<<"\n\n";
      }
      else {
        cout<<"\n\nResult DOES NOT MATCH Expected:\n"
            "Expected:\n"<<expected->toISLString()<<
             "\nResult:\n"<<result->toISLString()<<"\n\n";
      }
    }

}



void EXPECT_EQ(string a, string b){

    if( a != b ){

        cout<<"\n\nExpected: "<<a;
        cout<<"\n\nActual:"<< b <<"\n\n";
    }
}

bool printRelation(string msg, Relation *rel){

    if ( rel ) {

        cout<<"\n\n"<<msg<<rel->toISLString()<<"\n\n";
    } else {

        cout<<"\n"<<msg<<"Not Satisfiable"<<"\n";
    }

    return true;
}

int str2int(string str){
  int i;
  sscanf (str.c_str(),"%d",&i);
  return i;
}

