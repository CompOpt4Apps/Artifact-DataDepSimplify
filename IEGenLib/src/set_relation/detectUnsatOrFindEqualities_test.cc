/*!
 * \file complexityForPartialParallel_test.cc
 *
 * \brief complexity For Partial Parallel tests.
 *
 * \date Started: 2/14/18
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2015-2018, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "set_relation.h"
#include "expression.h"
#include "UFCallMap.h"
#include "Visitor.h"

#include <util/util.h>
#include <gtest/gtest.h>

#include <utility>
#include <fstream>
#include <iostream>

// Exp and Term classes
using iegenlib::Exp;
using iegenlib::Term;
using iegenlib::UFCallTerm;
using iegenlib::TupleVarTerm;
using iegenlib::VarTerm;
using iegenlib::TupleDecl;

// set and relation classes
using iegenlib::Conjunction;
using iegenlib::SparseConstraints;
using iegenlib::Set;
using iegenlib::Relation;
using iegenlib::Environment;
using iegenlib::UninterpFunc;
using iegenlib::SubMap;
using iegenlib::UniQuantRule;
using iegenlib::currentEnv;
using iegenlib::UFCallMap;
using iegenlib::UniQuantRuleType;
using iegenlib::ruleInstantiation;
using iegenlib::instantiate;


#pragma mark ruleInstantiationTEST
TEST(detectUnsatOrFindEqualitiesTest, ruleInstantiationTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("rowptr",
     new Set("{[i]:0<=i &&i<m}"), 
     new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);

  // Adding universially quantified rule
  UniQuantRule *uqRule1;
  // Forall e1,e2,  e1 = e2 => rowptr(e1) <= diagptr(e2)
  uqRule1 = new UniQuantRule(std::string("CoMonotonicity"), 
            std::string("[e1,e2]"), 
            std::string("e1 = e2"), std::string("rowptr(e1) <= diagptr(e2)") );
  currentEnv.addUniQuantRule( uqRule1 );


  //So now we have two rules in the environment:
  // (1) forall e1,e2: e1 < e2 => rowptr(e1) <= rowptr(e2) 
                   // Non-strict monotonicity
  // (2) Forall e1,e2: e1 = e2 => rowptr(e1) <= diagptr(e2) 
                   // User added

  // Now, we must create set of expressions that 
  // we are going to use to instantiate the two rules.
  std::set<Exp> instExps;
  Exp *e = new Exp();
  e->addTerm(new TupleVarTerm(1, 0));  // i
  instExps.insert( *(e) );
  delete e;
  e = new Exp();
  e->addTerm(new TupleVarTerm(1, 1)); // j
  instExps.insert( *(e) );
  delete e;

  // ufcmap and tupleDecl are used in detectUnsatOrFindEqualities
  UFCallMap *ufcmap = new UFCallMap();
  Set *s = new Set("{[i,j]: 0 < i < j}");
  TupleDecl tupleDecl = s->getTupleDecl();
  // useRule is used to indicate which type of rule to instantiate, 
  // refer to environment.cc for details
  bool* useRule = new bool[ 5 ];
  for(int i = 0 ; i < 5 ; i++ ){ useRule[i] = 1; } // Use all of them
  // Instantiate the rules
  std::set<std::pair <std::string,std::string>> instantiations;
  instantiations = ruleInstantiation(instExps, useRule, tupleDecl, ufcmap);

  // Instantiations
//  int i=0;
//  for (std::set<std::pair <std::string,std::string>>::iterator 
//       it=instantiations.begin(); it!=instantiations.end(); it++){
//    std::cout<<"\n Inst #"<<++i<<"  "<<(*it).first<<" -> "<<(*it).second;
//  }

}


#pragma mark detectUnsatOrFindEqualitiesTEST
TEST(detectUnsatOrFindEqualitiesTest, detectUnsatOrFindEqualitiesTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
  iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

  // Adding universially quantified rules
  UniQuantRule *uqRule1;
  // Forall e1,e2,  e1 = e2 => rowptr(e1) <= diagptr(e2)
  uqRule1 = new UniQuantRule(std::string("CoMonotonicity"), 
                std::string("[e1,e2]"), 
                std::string("e1 = e2"), std::string("rowptr(e1) <= diagptr(e2)") );
  currentEnv.addUniQuantRule( uqRule1 );
  UniQuantRule *uqRule2;
  // Forall e1,e2,  e1 < e2 => diagptr(e1) < rowptr(e2)
  uqRule2 = new UniQuantRule(std::string("CoMonotonicity"), 
                std::string("[e1,e2]"), 
                std::string("e1 < e2"), std::string("diagptr(e1) < rowptr(e2)") );
  currentEnv.addUniQuantRule( uqRule2 );

  Set *A1 = new Set("[m] -> {[i,k,j1,j2,ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m1"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && k = kp }");

    // expected output  (for testing purposes)
  Set *ex_A1 = NULL;

  Set *A1_result = A1->detectUnsatOrFindEqualities();
  // Verify the results 
  if ( ex_A1 != NULL && A1_result != NULL) {
    EXPECT_EQ(ex_A1->toISLString(), A1_result->toISLString());
  } else {
    EXPECT_EQ(ex_A1, A1_result);
  }

  Set *A8 = new Set("[m] -> {[i,k,j1,j2,ip,kp,j1p,j2p]: ip < i"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                   " && k < j1 && j1 < rowptr(1+i)"
                                 " && kp < j1p && j1p < rowptr(1+ip)"
                  " && diagptr(colidx(k)) < j2 && j2 < rowptr(1+colidx(k))"
                " && diagptr(colidx(kp)) < j2p && j2p < rowptr(1+colidx(kp))"
                             " && colidx(j1) = colidx(j2)"
                             " && colidx(j1p) = colidx(j2p)"

                                     " && j1 = j2p}");

  Set *ex_A8 = new Set("[m] -> {[i,k,j1,j2,ip,kp,j1p,j2p]: "
                                   "  j1 = j2p"
                                "&& i = colidx(kp)   }");

  Set *A8_result = A8->detectUnsatOrFindEqualities();
  // Verify the results 
  if ( ex_A8 != NULL && A8_result != NULL) {
    EXPECT_EQ(ex_A8->toISLString(), A8_result->toISLString());
  } else {
    EXPECT_EQ(ex_A8, A8_result);
  }
}

