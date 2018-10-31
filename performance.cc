/*!
 * \file simplification.cc
 *
 * \author Mahdi Soltan Mohammadi 
 *
 * Copyright (c) 2018, University of Arizona <br>
 * All rights reserved. <br>
 * See ./LICENSE for details. <br>
 *
 * This file is a driver for reproducing the result for the paper titled:
   "Sparse Matrix Code Dependence Analysis Simplification at Compile Time"
   that has been submitted to arXiv.
 *

 ** For building the dependencies see README.md

>> After build all the dependenciues you can build the driver (in root directory run):

g++ -O3 -o performance performance.cc -I IEGenLib/src IEGenLib/build/src/libiegenlib.a -lisl -std=c++11

>> Run the driver (in root directory):

./performance list.txt

*/

#include <iostream>
#include "iegenlib.h"
#include "parser/jsoncons/json.hpp"
#include<cstdlib>
#include<fstream>
#include <map>

using jsoncons::json;
using namespace iegenlib;
using namespace std;

// The data structure that holds evaluation result for a dependence relation
typedef struct deprel{

  deprel(){
    bl = mono = coMono = tri = combo = false;
    origComplexity = simpComplexity = "";
    rel = NULL; simpRel = NULL; subSetOf=-1;
  }

  Relation * rel;
  Relation * simpRel;
  bool bl;    // Is it part of baseline? (is it MaySat considering linear inconsistency?)
  bool mono;  // Is it MaySat after considering mopnotonicity domain information? 
  bool coMono;
  bool tri;
  bool combo;
  string origComplexity;
  string simpComplexity;
  int subSetOf;
}depRel;


void driver(string list);

// Utility functions
void initComplexities(map<string,int> &complexities);
void restComplexities(map<string,int> &complexities);
void printCompString(string category, map<string,int> &complexities, ofstream &out);
void printComplexities(map<string,int> &complexities, string stage, ofstream &out);
string adMissingInductionConstraints(string str,json &missingConstraints);
void setDependencesVal(std::vector<depRel> &dependences, int relNo, int rule, bool val);
string getPrettyComplexity(string comp);
string giveCompWithOrd(int ord);
int compCompare(string comp1, string comp2);
string int2str(int i);
string trimO(string str);
string b2s(bool cond){ if(cond){ return string("Yes");} return string("No");} 
void genChillScript(json &analysisInfo);



//------------------------------------------ MAIN ----------------------------------------
int main(int argc, char **argv)
{
  if (argc == 1)
  {
    cout<<"\n\nYou need to specify the input file. The input file should contain name of input JSON files:"
          "\n./simplifyDriver list.txt\n\n";
  } else if (argc >= 2){
    // Parsing command line arguments and reading given files.
    for(int arg = 1; arg < argc ; arg++){
      driver(string(argv[arg]));
    }
  }

  return 0;
}


// The driver that gathers the results for arXiv submission
void driver(string list)
{

  string inputFileName="";

  // Read name of json files from input list
  ifstream inL(list);

 // Looping over examples listed in the input file (JSON files)
 for(; getline( inL, inputFileName );){ 

  int totalDeps = 0, unSatFound = 0, maySatFound = 0, simplyUnSat = 0, i = 0, 
      ct, nUniqueRels, origCompLess=0, withEqLess=0;
  string line="";
  std::pair<std::set<Relation>::iterator,bool> uniqRel;
  std::set<Relation> uniqueRelations;
  Relation* rel;
  std::vector<depRel> dependences (100);

  iegenlib::setCurrEnv();
  std::set<int> parallelTvs;

  // Read the data from inputFileName
  ifstream in(inputFileName);
  json data;
  in >> data;

  // Introduce the uninterpreted function symbols to environment, and 
  // indicate their domain, range, whether they are bijective, or monotonic.
  json ufcs = data[0][0];
  // Read UFCs' data for code No. p, from ith relation
  addUFCs(ufcs);
  // Add defined domain information to environment
  json uqCons = data[0][0]["User Defined"];
  addUniQuantRules(uqCons);


  // Generating the CHILL script
  json analysisInfo = data[0][0]["CHILL analysis info"];
  genChillScript(analysisInfo);

  // Extracting dependence relations with CHILL
  string chillCommand = "./chill/build/chill " + analysisInfo[0]["Script file"].as<string>() + " 2> /dev/null"; 
  int chillErr = system (chillCommand.c_str());

  ifstream depf((analysisInfo[0]["Output file"].as<string>()).c_str());

  for (ct = 0, i=0; getline( depf, line ) ; ct++){
    // Adding missing constraints related to induction variables (only applies to ILU0)
    json missingConstraints = data[0][0]["Missing induction iterator constraints"];
    if( missingConstraints.size() > 0 ){
      line = adMissingInductionConstraints(line, missingConstraints);
    }
    
    // If the relation is not unique, ignore it
    rel = new Relation(line); 
    uniqRel = uniqueRelations.insert(*rel);
    if( !(uniqRel.second) ){
      delete rel;
      continue;
    }

    // Specify loops that are going to be parallelized, 
    // so we are not going to project them out.
    json npJ = data[0][0]["Do Not Project Out"];
    parallelTvs.clear();
    notProjectIters( rel, parallelTvs, npJ);

    dependences[i].rel = rel;
    i++;
  }
  nUniqueRels = i;

  std::vector<int> superSets;

  int nBL=0;
  for( i=0; i < nUniqueRels; i++){    //Loop over all unique relations
      // Use all the known index array properties
      bool* useRule = new bool[ TheOthers ];
      for(int j = 0 ; j < TheOthers ; j++ ) useRule[j] = 1;

      // Specify loops that are going to be parallelized, so we are not going to project them out.
      json npJ = data[0][0]["Do Not Project Out"];
      parallelTvs.clear();
      notProjectIters( dependences[i].rel, parallelTvs, npJ);
      Relation* rel_sim = dependences[i].rel->simplifyForPartialParallel(parallelTvs);

      if( rel_sim ){
        dependences[i].bl = true; // the relation is trivially satisfiable
      } else {
        dependences[i].bl = false; // the relation is trivially UNsatisfiable
        continue;
      }

      Relation* result = rel_sim->detectUnsatOrFindEqualities(useRule);

      if( !result ){
        setDependencesVal(dependences, i, FuncConsistency, true);

        unSatFound++;
      } else{
        setDependencesVal(dependences, i, FuncConsistency, false);
          dependences[i].simpRel = result;
          maySatFound++;

        // Find supbet of relations that are superset of all the relations
        bool notSubSet=true;
        // ...  r1->setRelationship(r2)
        for(int ct=0; ct < superSets.size() ; ct++){
          int t = (dependences[superSets[ct]].rel)->setRelationship(dependences[i].rel);
          if( t == iegenlib::SuperSetEqual || t == iegenlib::SetEqual){
            dependences[i].subSetOf = superSets[ct];
            notSubSet = false;
            break;
          }
        }
        if( notSubSet ){
          superSets.push_back(i);
        }
      }
      delete useRule;
  }



 } // End of input json file list loop

}
// ----------- End of driver function --------------------------------------








// Generate CHILL scripts using analysis info from json file 
void genChillScript(json &analysisInfo){

  ofstream outf;
  outf.open((analysisInfo[0]["Script file"].as<string>()).c_str(), std::ofstream::out);

  outf<<"from chill import *\n";
  outf<<"source(\'"<<analysisInfo[0]["Source"].as<string>()<<"\')\n";
//  outf<<"destination(\'"<<analysisInfo[0]["Destination"].as<string>()<<"\')\n";
  outf<<"procedure(\'"<<analysisInfo[0]["Procedure"].as<string>()<<"\')\n";
  outf<<"loop("<<analysisInfo[0]["Loop"].as<string>()<<")\n";
  outf<<"print_dep_ufs(\'"<<analysisInfo[0]["Output file"].as<string>()
                   <<"\',\'"<<analysisInfo[0]["Private Arrays"].as<string>()
                   <<"\',\'"<<analysisInfo[0]["Reduction Statements"].as<string>()
                          <<"\')\n";
}

string adMissingInductionConstraints(string str,json &missingConstraints){
  bool notChanged=true;
  int inArrity = 5, outArity = 5;
  string newStr, inTupleVars[20], outTupleVars[20];
  srParts parts = getPartsFromStr(str);
  std::queue<std::string> tupVars = tupVarsExtract(parts.tupDecl, inArrity, outArity);
  for(int i=0; i < inArrity; i++){
    inTupleVars[i] = tupVars.front(); 
    tupVars.pop();
  }
  for(int i=0; i < outArity; i++){
    outTupleVars[i] = tupVars.front(); 
    tupVars.pop();
  }
  for(int i=0; i < missingConstraints.size(); i++){
    string it = missingConstraints[i]["Indunction iterator"].as<string>();
    if( str.find(it) != std::string::npos ){
       notChanged = false;
       parts.constraints += " && " + 
                            missingConstraints[i]["Constraints to add"].as<string>();
       for(int j=0;j<missingConstraints[i]["Iterators to add"].size();j++){
         string itToAdd = missingConstraints[i]["Iterators to add"][j].as<string>();
         if( itToAdd[(itToAdd.size()-1)] == 'p' ) {
           outTupleVars[outArity] = itToAdd;
           outArity++;
         } else {
           inTupleVars[inArrity] = itToAdd;
           inArrity++;
         }
       }
    }
  }

  if(notChanged) return str;

  string newTupleDecl = "["+inTupleVars[0];
  for(int i=1; i < inArrity; i++) newTupleDecl += "," + inTupleVars[i];
  newTupleDecl += "] -> ["+outTupleVars[0];
  for(int i=1; i < outArity; i++) newTupleDecl += "," + outTupleVars[i];
  parts.tupDecl = newTupleDecl + "]";
  
  newStr = getFullStrFromParts(parts);

  return newStr;
}

void setDependencesVal(std::vector<depRel> &dependences, int relNo, int rule, bool val){

  if(rule == Monotonicity)          dependences[relNo].mono = val;
  else if(rule == CoMonotonicity)   dependences[relNo].coMono = val;
  else if(rule == Triangularity)    dependences[relNo].tri = val;
  else if(rule == FuncConsistency)  dependences[relNo].combo = val;
}

string getPrettyComplexity(string comp){
  string pComp;
  if(comp == "O(0)")              pComp = "O(0)";
  else if(comp == "O(1)")         pComp = "O(1)";
  else if(comp == "O(n^1)")       pComp = "O(n)";
  else if(comp == "O(nnz^1)")     pComp = "O(nnz)";
  else if(comp == "O(nnz^2/n^1)") pComp = "O(nnz*(nnz/n))";
  else if(comp == "O(nnz^3/n^2)") pComp = "O(nnz*(nnz/n)^2)";
  else if(comp == "O(nnz^4/n^3)") pComp = "O(nnz*(nnz/n)^3)";
  else if(comp == "O(nnz^5/n^4)") pComp = "O(nnz*(nnz/n)^4)";
  else if(comp == "O(n^2)")       pComp = "O(n^2)";
  else if(comp == "O(n^1*nnz^1)") pComp = "O(n*nnz)";
  else if(comp == "O(nnz^2)")     pComp = "O(nnz^2)";
  else if(comp == "O(nnz^3/n^1)") pComp = "O((nnz^2)*(nnz/n))";
  else if(comp == "O(nnz^4/n^2)") pComp = "O((nnz^2)*(nnz/n)^2)";
  else if(comp == "O(nnz^5/n^3)") pComp = "O((nnz^2)*(nnz/n)^3)";
  else                            pComp = "NaN";
  return pComp;
}

void initComplexities(map<string,int> &complexities){
  complexities[string("O(0)")] = 0;
  complexities[string("O(1)")] = 0;
  complexities[string("O(n^1)")] = 0;
  complexities[string("O(nnz^1)")] = 0;
  complexities[string("O(nnz^2/n^1)")] = 0;
  complexities[string("O(nnz^3/n^2)")] = 0;
  complexities[string("O(nnz^4/n^3)")] = 0;
  complexities[string("O(nnz^5/n^4)")] = 0;
  complexities[string("O(n^2)")] = 0;
  complexities[string("O(n^1*nnz^1)")] = 0;
  complexities[string("O(nnz^2)")] = 0;
  complexities[string("O(nnz^3/n^1)")] = 0;
  complexities[string("O(nnz^2/n^2)")] = 0;
  complexities[string("O(nnz^5/n^3)")] = 0;
}


int compCompare(string comp1, string comp2){
  map<string,int> complexities;
  complexities[string("O(0)")] = 0;
  complexities[string("O(1)")] = 1;
  complexities[string("O(n^1)")] = 2;
  complexities[string("O(nnz^1)")] = 3;
  complexities[string("O(nnz^2/n^1)")] = 4;
  complexities[string("O(nnz^3/n^2)")] = 5;
  complexities[string("O(nnz^4/n^3)")] = 6;
  complexities[string("O(nnz^5/n^4)")] = 7;
  complexities[string("O(n^2)")] = 8;
  complexities[string("O(n^1*nnz^1)")] = 9;
  complexities[string("O(nnz^2)")] = 10;
  complexities[string("O(nnz^3/n^1)")] = 11;
  complexities[string("O(nnz^4/n^2)")] = 12;
  complexities[string("O(nnz^5/n^3)")] = 13;
  if( complexities[comp1] < complexities[comp2] )       return -1;
  else if( complexities[comp1] == complexities[comp2] ) return 0;
  else if( complexities[comp1] > complexities[comp2] )  return 1;

  return -1;
}

string giveCompWithOrd(int ord){
  string comp="";
  if(ord == 0) comp = "O(0)";
  else if(ord == 1) comp = "O(1)";
  else if(ord == 2) comp = "O(n^1)";
  else if(ord == 3) comp = "O(nnz^1)";
  else if(ord == 4) comp = "O(nnz^2/n^1)";
  else if(ord == 5) comp = "O(nnz^3/n^2)";
  else if(ord == 6) comp = "O(nnz^4/n^3)";
  else if(ord == 7) comp = "O(nnz^5/n^4)";
  else if(ord == 8) comp = "O(n^2)";
  else if(ord == 9) comp = "O(n^1*nnz^1)";
  else if(ord == 10) comp = "O(nnz^2)";
  else if(ord == 11) comp = "O(nnz^3/n^1)";
  else if(ord == 12) comp = "O(nnz^4/n^2)";
  else if(ord == 13) comp = "O(nnz^5/n^3)";
  else              comp = "NaN";
  return comp;  
}

void restComplexities(map<string,int> &complexities){
  for (map<string,int>::iterator it = complexities.begin(); it!=complexities.end(); it++)
    it->second=0;
}

void printComplexities(map<string,int> &complexities, string stage, ofstream &out){
  out<<"\n"<<stage;
  for (map<string,int>::iterator it = complexities.begin(); it!=complexities.end(); it++)
    out<<"  "<<it->second<<"  ";
}

string trimO(string str){
  return str.erase(0,1);
}

string int2str(int i){
  char buf[50];
  sprintf (buf, "%d",i);
  return string(buf);
}

