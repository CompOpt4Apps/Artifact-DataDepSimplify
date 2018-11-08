/*!
 *\mainpage IEGen C++ Rewrite
 *\authors Michelle Strout
 *\authors Joe Strout
 *\authors Alan LaMielle
 *\authors Catherine Olschanowsky
 *\authors Barbara Kreaseck
 *\authors Mark Heim
 *\authors Ian Craig
 *\authors Nicholas Jeanette
 *\date Date Started: 5/17/10
 *
 *
 *\section Intro
 *  IEGenLib is a library with data structures and routines that
 *  can represent, parse, and visit integer tuple sets and relations with affine
 *  constraints and uninterpreted function symbol equality constraints. See
 *  src/parser/parser_test.cc for examples of strings that can be parsed.
 *
 *  The run_tests.cc driver will execute all tests identified with the TEST
 *  macro. In each of the src/ subdirectories, the *_test.cc files define
 *  regression tests.
 *
 *  The iegenlib_calc binary will execute tests that you input yourself and will
 *  return the interpreted version of the input along with a dotty file to the
 *  file set_relation.dot in the directory where the program is executed.
 *
 *

 *\section COPYING
 *\verbinclude COPYING

 *\section README
 *\verbinclude README

 */

/*
 * Copyright (c) 2009, 2010, 2011, 2012, 2013 Colorado State University <br>
 * All rights reserved. <br>
 * See COPYING for details. <br>
 $Revision:: 805                $: last committed revision\n
 $Date:: 2013-09-09 03:27:10 -0#$: date of last committed revision\n
 $Author:: mstrout              $: author of last committed revision\n
*/

#include <iostream>
#include <algorithm>
#include <fstream>

#include <iegenlib.h>

using iegenlib::Set;
using iegenlib::Relation;
using iegenlib::parse_exception;

#define SET_RELATION_DOT_FILE_NAME "set_relation.dot"
#define AST_DOT_FILE_NAME "ast.dot"

/*! lowercase
 *
 *  Converts the given string to lowercase.
 *
 * @param str -- string to convert
 * @return lowercase version of str
 */
std::string lowercase(const std::string& str)
{
  std::string upper;
  transform(str.begin(), str.end(), std::back_inserter(upper), tolower);
  return upper;
}

/*! contains
 *
 *  This is a small helper that reports whether one string contains a
 *  substring of interest.  It is preferred over directly using the
 *  s.find() != string::npos idiom because (1) it is shorter, and (2)
 *  it more clearly expresses the intent to the human reader.  Also,
 *  this method can now act in a case-insensitive manner, which find
 *  doesn't naturally do.
 *
 * @param s -- string to search
 * @param substring -- substring to search for
 * @param caseSensitive -- if true, do a case-sensitive search (defaults to false)
 * @return true if s contains substring, false otherwise
 */
bool contains(std::string s, std::string substring, bool caseSensitive=false)
{
  if (not caseSensitive) {
    s = lowercase(s);
    substring = lowercase(substring);
  }
  return s.find(substring) != std::string::npos;
}

/*! isRelation
 *
 *  Determine whether the given input is a relation, by searching
 *  for the arrow token ("->"), which appears only in a relation
 *  (and never in a set).
 *
 * @param input -- string to examine
 * @return true iff input appears to be a relation
 */
bool isRelation(std::string input) {
  return contains(input, "->");
}

/*! newRelationOrNull
 *
 *  This factory function returns a new Relation object, if the input
 *  string appears to describe a relation; otherwise it returns NULL.
 *
 * @param input -- string to examine
 * @return new Relation constructed with input, or NULL
 */
Relation *newRelationOrNull(std::string input) {
  if (isRelation(input)) return new Relation(input);
  return NULL;
}

/*! newSetOrNull
 *
 *  This factory function returns a new Set object, if the input
 *  string appears to describe a Set; otherwise it returns NULL.
 *
 * @param input -- string to examine
 * @return new Set constructed with input, or NULL
 */
Set *newSetOrNull(std::string input) {
  if (not isRelation(input)) return new Set(input);
  return NULL;
}

/*! handleSet
 *
 *  Handle the case of a set input.
 */
void handleSet(std::string set_str)
{
  //Create a set from the given set string
  Set *set=new Set(set_str);

  std::cout << "  set->prettyPrintString(): "
            << set->prettyPrintString() << std::endl;
  
  //Print the result of the toDotString() method of the set
  //to the file SET_RELATION_DOT_FILE_NAME
  std::ofstream dot_file;
  dot_file.open(SET_RELATION_DOT_FILE_NAME);
  std::cout << "  Writing output of set->toDotString() to file '"
            << SET_RELATION_DOT_FILE_NAME << "'..." << std::endl;
  dot_file << set->toDotString();
  dot_file.close();

  //Delete the allocated set
  delete set;
}

/*! handleRelation
 *
 *  Handle the case of a relation input.
 */
void handleRelation(std::string relation_str)
{
  //Create a relation from the given relation string
  Relation *relation=new Relation(relation_str);

  std::cout << "  relation->prettyPrintString(): "
            << relation->prettyPrintString() << std::endl;
  
  //Print the result of the toDotString() method of the relation
  //to the file SET_RELATION_DOT_FILE_NAME
  std::ofstream dot_file;
  dot_file.open(SET_RELATION_DOT_FILE_NAME);
  std::cout << "  Writing output of relation->toDotString() to file '"
            << SET_RELATION_DOT_FILE_NAME << "'..." << std::endl;
  dot_file << relation->toDotString();
  dot_file.close();

  //Delete the allocated relation
  delete relation;
}

/*! handleInverse
 *
 *  Handle the case of an Inverse input.
 */
void handleInverse(std::string set_relation_str)
{
  size_t opLoc = lowercase(set_relation_str).find("inverse");
  std::string opRight = set_relation_str.substr(opLoc+7, std::string::npos);
  Relation *relation = newRelationOrNull(opRight);
  if (relation) {
    std::string str = relation->Inverse()->prettyPrintString();
    std::cout << "  result: " << str << std::endl;
    delete relation;
  } else {
    throw parse_exception("Inverse requires one relation!\n"
          "Usage: inverse [relation]");
  }
}

/*! handleCompose
 *
 *  Handle the case of a Compose input.
 */
void handleCompose(std::string set_relation_str)
{
  size_t opLoc = lowercase(set_relation_str).find("compose");
  std::string opLeft = set_relation_str.substr(0, opLoc);
  std::string opRight = set_relation_str.substr(opLoc+7, std::string::npos);

  Relation *relation = newRelationOrNull(opLeft);
  Relation *relation2 = newRelationOrNull(opRight);

  bool done = false;
  if (relation and relation2) {
    std::string str = relation->Compose(relation2)->prettyPrintString();
    std::cout << "  result: " << str << std::endl;
    done = true;
  }

  if (relation2) delete relation2;
  if (relation) delete relation;

  if (not done) {
    throw parse_exception("Compose requires two relations!\n"
            "Usage: [relation] compose [relation]");
  }
}

/*! handleApply
 *
 *  Handle the case of an Apply input.
 */
void handleApply(std::string set_relation_str)
{
  size_t opLoc = lowercase(set_relation_str).find("apply");
  std::string opLeft = set_relation_str.substr(0, opLoc);
  std::string opRight = set_relation_str.substr(opLoc+5, std::string::npos);

  Relation *relation = newRelationOrNull(opLeft);
  Set *set = newSetOrNull(opRight);

  bool done = false;
  if (relation and set) {
      std::string str = relation->Apply(set)->prettyPrintString();
      std::cout<<"  result: " << str << std::endl;
      done = true;
  }
  
  if (set) delete set;
  if (relation) delete relation;
  
  if (not done) {
    throw parse_exception("Apply requires one element of each same type!\n"
          "Usage: [relation] apply [set]");
  }

}

/*! handleUnion
 *
 *  Handle the case of a Union input.
 */
void handleUnion(std::string set_relation_str) {
  size_t opLoc = lowercase(set_relation_str).find("union");
  std::string opLeft = set_relation_str.substr(0, opLoc);
  std::string opRight = set_relation_str.substr(opLoc+5, std::string::npos);

  bool done = false;  
  if (isRelation(opLeft)) {
    // expect two relations
    Relation *relation = new Relation(opLeft);
    Relation *relation2 = newRelationOrNull(opRight);
    if (relation and relation2) {
      std::string str = relation->Union(relation2)->prettyPrintString();
      std::cout<<"  result: "<< str << std::endl;
      done = true;
    }
    if (relation2) delete relation2;
    if (relation) delete relation;
  } else {
    // expect two sets
    Set *set = new Set(opLeft);
    Set *set2 = newSetOrNull(opRight);
    if (set and set2) {
      Set *set2 = new Set(opRight);
      std::string str = set->Union(set2)->prettyPrintString();
      std::cout<<"  result: "<< str << std::endl;
      done = true;
    }
    if (set2) delete set2;
    if (set) delete set;
  }

  if (not done) {
    throw parse_exception("Union requires two elements of the same type!\n"
          "Usage: [set|relation] union [set|relation]");
  }
}

int main(int ac, char **av)
{
  std::string set_relation_str;

  //Print a startup message
  std::cout<<std::endl;
  std::cout<<"==============================================================="<<std::endl;
  std::cout<<"This is a calculator for IEGenLib."<<std::endl<<std::endl;
  std::cout<<"Please enter a set, relation, or operation string.  Some examples of sets are:";
  std::cout <<std::endl<<std::endl;
  std::cout <<"  {[a]}" << std::endl << "  {[a]: a=10}" << std::endl;
  std::cout <<"  {[a,b]}" << std::endl;
  std::cout <<"  {[a,b]: a=f(b)}" << std::endl<<std::endl;
  std::cout <<"and some examples of relations are:"<<std::endl<<std::endl;
  std::cout <<"  {[a]->[b]}" << std::endl << "  {[a]->[b]: a=b}" << std::endl;
  std::cout <<"  {[a,b]->[c]: 0<=a and a<10 and b=10 and b=c}" << std::endl;
  std::cout <<"  {[a]->[b,c]: 0<=a and a<10 and b=c}"<<std::endl<<std::endl;
  std::cout <<"and some examples of operations are:"<<std::endl<<std::endl;
  std::cout <<"  {[i,j]: 0<=i and i<n and 5<=j and j<m} "
              "union {[i,j]: 0<=i and i<n and m+5<=j and j<m+10}" 
              << std::endl << std::endl;
  std::cout <<"  {[i,j]->[ip,j]:f(ip)=i} apply "
              "{[i,j]: 0<=i and i<n and 5<=j and j<m}" 
              << std::endl << std::endl;
  std::cout <<"  {[i,j]->[ip,jp]: ip=f(i) and jp=2j} "
              "compose {[q,r]->[q,r]}"<< std::endl <<std::endl;
  std::cout <<"  inverse {[i,j]->[ip,jp]: "
              "ip=f(i) and jp=2j}" << std::endl << std::endl;
              
  std::cout <<"Examples using the environment to declare function inverses:"
            << std::endl;
  std::cout <<"  f() = inverse f_inv()" << std::endl; 
  std::cout <<"  {[i,j]->[ip,j]:ip=f(i) and j=j} apply "
              "{[i,j]: 0<=i and i<n and 5<=j and j<m}" 
              << std::endl << std::endl;

  std::cout<<"This version does not have variable assignment."<<std::endl;
  std::cout<<"Enter an empty string or Ctrl-D to quit..."<<std::endl<<std::endl;
  std::cout<<"==============================================================="<<std::endl;
  std::cout<<std::endl;

  while (true) {
  
    //Read a set/relation string
    std::cout << "-------------" << std::endl;
    std::cout << "Enter Set/Relation> ";
    getline(std::cin, set_relation_str);
    if ("" == set_relation_str) break;
    
    //Print the string we received as input
    //std::cout << "  read: " << set_relation_str << std::endl;

    try {
        if (contains(set_relation_str, "union")) {
            handleUnion(set_relation_str);
        } else if (contains(set_relation_str, "apply")) {
            handleApply(set_relation_str);
        } else if (contains(set_relation_str, "compose")) {
            handleCompose(set_relation_str);
        // FIXME: need to update this syntax
        //} else if (contains(set_relation_str, "()")) { // func inverse decl
        //    iegenlib::setCurrEnv(set_relation_str);
        } else if (contains(set_relation_str, "inverse")) {
            handleInverse(set_relation_str);
        } else if (isRelation(set_relation_str)) {
            handleRelation(set_relation_str);
        } else {
            handleSet(set_relation_str);
        }
        
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

  }

  std::cout << std::endl << "Exiting..." << std::endl;

  return 0;
}
