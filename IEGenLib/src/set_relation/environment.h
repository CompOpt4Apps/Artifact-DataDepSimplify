/*!
 * \file environment.h
 *
 * \brief Declarations for the Environment class.
 *
 * The Environment class keeps track of information about function
 * inverses, domains, ranges, and symbolic constant constraints.
 *
 * \date Started: 5/1/12
 * # $Revision:: 819                $: last committed revision
 * # $Date:: 2014-01-07 16:27:03 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2012-2013, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>

#include "UninterpFunc.h"
#include <util/util.h>

namespace iegenlib{

class UniQuantRule;
class Environment;

/*  FIXME: Might want to resurrect this to parse Symbolic declarations
    of uninterpreted functions.
    possible syntax: uninterpfunc NNZ, domain=S, range={[k]:0 <= k < M},
        property=bijective, generator=ERG_count(domain,&M);
namespace parser{
extern Environment* parse_env(std::string set_string);
}
*/

//! Empties out the current environment.
void setCurrEnv();
    
//! Sets the global environment after creating one with given 
//! uninterpreted function declaration.
//! Will create and add to global environment a funcName_inv if funcName 
//! is bijective.
void setCurrEnv(std::string funcName, Set* domain, Set* range, bool bijective);

//! Append the following uninterpreted function to this environment.
//! Will create and add to global environment a funcName_inv if funcName 
//! is bijective.
void appendCurrEnv(std::string funcName, Set* domain, Set* range, 
                   bool bijective, MonotonicType monoType);

//! search this environment for a function inverse
std::string queryInverseCurrEnv(const std::string funcName);

//! search this environment for a function domain
//! returned Set escapes
Set* queryDomainCurrEnv(const std::string funcName);

//! search this environment for a function range
//! returned Set escapes
Set* queryRangeCurrEnv(const std::string funcName);

//! Returns the monotonicity type of the given function.
MonotonicType queryMonoTypeEnv(const std::string funcName);

//! search this environment for a function's domain arity
unsigned int queryRangeArityCurrEnv(const std::string funcName);

//! search this environment for a function's range arity
unsigned int queryDomainArityCurrEnv(const std::string funcName);

//! add an universially quantified Rule to environment
//! The environment is going to own uqRule object (user should not delete it)
void addUniQuantRule(UniQuantRule *uqRule);

// Return the number of available universially quantified Rules
int queryNoUniQuantRules();

// Returns the universially quantified Rule No. idx stored in the enviroment
//! The environment still owns returned object (user should not delete it)
UniQuantRule* queryUniQuantRuleEnv(int idx);

//! Get an UniQuantRule representing Domain and Range of an UF Symbol
UniQuantRule* getUQRForFuncDomainRange(std::string func);

class Environment {
public:

    //! Constructs an empty environment.
    Environment() {}

    //! Construct an environment and use given UninterpFunc as first member.
    //! The Environment owns the UninterpFunc pointer.
    Environment(UninterpFunc*);
    
    //! Copy constructor for Environment.
    Environment(const Environment& other)
        { *this = other; }
    
    //! Assignment operator for Environment.
    Environment& operator=(const Environment& other);
    
    //! destructor
    ~Environment();
    
    //! append an environment to this one (adopts)
    void append(Environment* other);
    
    //! Reset the Environment to empty
    void reset();
    
    //! Define the inverse for the given function.
    void setInverse(std::string funcName, std::string inverseName);

    //! Get the name of the inverse of the given function (or "" if none).
    std::string funcInverse(const std::string funcName) const;
	
    //! Check whether the given function has a known inverse.
    bool hasInverse(const std::string funcName) const 
		{ return not funcInverse(funcName).empty(); }
	
    //! Returns a clone of the function's domain Set or NULL.
    Set* funcDomain(const std::string funcName) const;

    //! Returns a clone of the function's range Set or NULL.
    Set* funcRange(const std::string funcName) const;
	
    //! Returns whether a function is monotonistic or not and how.
    MonotonicType funcMonoType(const std::string funcName) const;

    std::string toString() const;

    //! Add a universially quantified Rule to the environment.
    //! The environment is going to own uqRule object (user should not delete it
    void addUniQuantRule(UniQuantRule *uqRule);

    //! Get the No. of universially quantified Rules
    // available in the environment
    int getNoUniQuantRules();

    //! Get the universially quantified Rule No. idx from 
    // the environment
    UniQuantRule* getUniQuantRule(int idx);

private:
    std::map<std::string, UninterpFunc*> mUninterpFuncMap;
    std::map<std::string, std::string> mInverseMap;
    std::vector<UniQuantRule*>  uniQuantRules;
};

extern Environment currentEnv;

// Mahdi: FIXME: Want to have FuncConsistency to be the first
// Monotonicity should always be the first and TheOthers the last type
// This convention is used inside drivers 
typedef enum {Monotonicity, CoMonotonicity, Triangularity, 
              DomainRange, FuncConsistency, TheOthers} UniQuantRuleType;

/*!
** This is the basic structure for storing different domain information
** about uninterpreted function symbols in the environment, these 
** information can be of the types:

    Forall e1, e2, ...   Set => Set
    * Set: can be a conjunction of equality and inequality expressions

 * \class UniQuantRule
 */
class UniQuantRule {
public:
  UniQuantRule(string type, string tupleDecl, string leftSide, string rightSide);
  ~UniQuantRule();
  //! Copy constructor.
  UniQuantRule( const UniQuantRule& other );//{ *this = other; }
  //! Assignment operator.
  UniQuantRule& operator=( const UniQuantRule& other );
  //! helper function for implementing copy-and-swap
  void swap(UniQuantRule& second) throw();

  //! prints the content of the map into a std::string, and returns it
  std::string toString();
  UniQuantRuleType getType();
  // Get left side of the rule as a Set:
  //     Forall e1, e2,  p => q ( this functions returns { [e1, e2] : p } )
  Set* getLeftSide();
  // Get right side of the rule as a Set:
  //     Forall e1, e2,  p => q ( this functions returns { [e1, e2] : q } )
  Set* getRightSide();

  std::string getZ3Form(std::set<std::string> &relevantUFSs, 
                        std::set<std::string> &glVarSyms, int cc);

private:

  UniQuantRuleType mUniQuantRuleType; 
  Set *mLeftSide;
  Set *mRightSide;
};

}//end namespace iegenlib

#endif /* ENVIRONMENT_H_ */
