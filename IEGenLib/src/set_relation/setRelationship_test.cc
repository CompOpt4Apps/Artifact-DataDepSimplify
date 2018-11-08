/*!
 * \file setRelationship_test.cc
 *
 * \brief test cases for detecting set relationship between Sets and Relations.
 *
 * \date Started: 9/06/18
 *
 * \authors Mahdi Soltan Mohammadi
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


#pragma mark setEqualityTEST
TEST(setRelationshipTest, setEqualityTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("rowIdx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("colPtr",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);

  // R3 and R8
  Relation *r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 0, 0, 0, 0, 0, 0, 0] : colPtr(Out_2) = In_6 && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n }");

  Relation *r2 = new Relation("{ [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] -> [0, In_2, 0, 0, 0, 0, 0, 0, 0] : colPtr(In_2) = Out_6 && rowIdx(Out_8) = rowIdx(Out_6) && 0 <= Out_2 && 0 <= In_2 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && Out_2 < In_2 && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) && In_2 + 1 < n }");

  EXPECT_EQ( iegenlib::SetEqual, r1->setRelationship(r2) );


  // R26 and R27
  r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] : Out_6 = In_6 && rowIdx(Out_8) = rowIdx(Out_6) && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) }");

  r2 = new Relation("{ [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] -> [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] : In_6 = Out_6 && rowIdx(In_8) = rowIdx(In_6) && rowIdx(Out_8) = rowIdx(Out_6) && 0 <= Out_2 && 0 <= In_2 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && Out_2 < In_2 && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;


  // R17 and R22
  r1 = new Relation("{ [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] -> [0, In_2, 1, In_4, 0, 0, 0, 0, 0] : In_4 = Out_6 && rowIdx(Out_8) = rowIdx(Out_6) && 0 <= Out_2 && 0 <= In_2 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && Out_2 < In_2 && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 }");

  r2 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 1, Out_4, 0, 0, 0, 0, 0] : Out_4 = In_6 && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;
}


#pragma mark setEqualityTEST
TEST(setRelationshipTest, setTrivialSuperSetEqualityTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("rowIdx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("colPtr",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);

  iegenlib::appendCurrEnv("lR",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("lC",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);


  iegenlib::appendCurrEnv("pruneSet",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("prunePtr",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);



  // R3 and R24
  // r2 has all the constraints of the r1, plus some other constraints, in this case 
  // we say that r1 is SuperSetEqual of r2, which is trivially to see.
  Relation *r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 0, 0, 0, 0, 0, 0, 0] : colPtr(Out_2) = In_6 && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n }");

  Relation *r2 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 1, Out_4, 0, 0, 0, 0, 0] : colPtr(Out_2) = In_6 && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SuperSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;


  // lChol R4 and R0
  // r2 has all the constraints of the r1, plus some other constraints, in this case 
  // we say that r1 is SuperSetEqual of r2, which is trivially to see.
  r1 = new Relation("{ [0, In_2, 2, 0, 0, 0, 0] -> [0, Out_2, 1, Out_4, 1, Out_6, 1] : lC(In_2) = Out_6 && 0 <= In_2 && 0 <= Out_2 && prunePtr(Out_2) <= Out_4 && lC(pruneSet(Out_4)) <= Out_6 && In_2 < Out_2 && In_2 < n && Out_2 < n && Out_4 < prunePtr(Out_2 + 1) && Out_6 < lC(pruneSet(Out_4) + 1) }");  // && 1 = sw

  r2 = new Relation("{ [0, In_2, 2, 0, 0, 0, 0] -> [0, Out_2, 1, Out_4, 1, Out_6, 0] : lR(Out_6) = Out_2 && lC(In_2) = Out_6 && 0 <= In_2 && 0 <= Out_2 && prunePtr(Out_2) <= Out_4 && lC(pruneSet(Out_4)) <= Out_6 && In_2 < Out_2 && In_2 < n && Out_2 < n && Out_4 < prunePtr(Out_2 + 1) && Out_6 < lC(pruneSet(Out_4) + 1) }");  // 0 = sw && 

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SuperSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;



  // lChol R6 and R2
  // r2 has all the constraints of the r1, plus some other constraints, in this case 
  // we say that r1 is SuperSetEqual of r2, which is trivially to see.
  r1 = new Relation("{ [0, In_2, 3, In_4, 0, 0, 0] -> [0, Out_2, 1, Out_4, 1, Out_6, 1] : Out_6 = In_4 && 0 <= In_2 && 0 <= Out_2 && prunePtr(Out_2) <= Out_4 && lC(pruneSet(Out_4)) <= Out_6 && In_2 < Out_2 && In_2 < n && In_4 < lC(In_2 + 1) && lC(In_2) < In_4 && Out_2 < n && Out_4 < prunePtr(Out_2 + 1) && Out_6 < lC(pruneSet(Out_4) + 1) }");  // && 1 = sw

  r2 = new Relation("{ [0, In_2, 3, In_4, 0, 0, 0] -> [0, Out_2, 1, Out_4, 1, Out_6, 0] : Out_6 = In_4 && lR(Out_6) = Out_2 && 0 <= In_2 && 0 <= Out_2 && prunePtr(Out_2) <= Out_4 && lC(pruneSet(Out_4)) <= Out_6 && In_2 < Out_2 && In_2 < n && In_4 < lC(In_2 + 1) && lC(In_2) < In_4 && Out_2 < n && Out_4 < prunePtr(Out_2 + 1) && Out_6 < lC(pruneSet(Out_4) + 1) }");  // 0 = sw && 

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SuperSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;
}


#pragma mark setEqualityTEST
TEST(setRelationshipTest, setTrivialSubSetEqualityTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("rowIdx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("colPtr",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);

  // Mahdi: Self note: R28 and R17: needs isl normalization
  // r1 has all the constraints of the r2, plus some other constraints, in this case 
  // we say that r1 is SubSetEqual of r2, which is trivially to see.
  Relation *r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] : Out_4 = In_6 && rowIdx(Out_8) = rowIdx(Out_6) && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) }");

  Relation *r2 = new Relation("{ [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] -> [0, In_2, 1, In_4, 0, 0, 0, 0, 0] : In_4 = Out_6 && rowIdx(Out_8) = rowIdx(Out_6) && 0 <= Out_2 && 0 <= In_2 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && Out_2 < In_2 && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SubSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;


  // R28 and R22
  // r1 has all the constraints of the r2, plus some other constraints, in this case 
  // we say that r1 is SubSetEqual of r2, which is trivially to see.
  r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] : Out_4 = In_6 && rowIdx(Out_8) = rowIdx(Out_6) && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) }");

  r2 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 1, Out_4, 0, 0, 0, 0, 0] : Out_4 = In_6 && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SubSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;
}


#pragma mark setEqualityTEST
TEST(setRelationshipTest, setComplicatedSuperSetEqualityTEST){

  // Introduce the UFCalls to enviroment, and indicate their domain, range
  // whether they are bijective, or monotonic.
  iegenlib::setCurrEnv();
  iegenlib::appendCurrEnv("rowIdx",
            new Set("{[i]:0<=i &&i<nnz}"),      // Domain 
            new Set("{[j]:0<=j &&j<m}"),        // Range
            false,                              // Bijective?!
            iegenlib::Monotonic_NONE            // monotonicity
            );
  iegenlib::appendCurrEnv("colPtr",
   new Set("{[i]:0<=i &&i<m}"), 
   new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Nondecreasing);


  // R22 and R30
  // r2 has all the constraints of the r1, except for one different equality
  // (alos some extra other constraints), in this case we look into that 
  // different equality, if the equality of r1 subsumes the equality in r2,
  // we say that r1 is SuperSetEqual of r2, which is trivially to see.
  Relation *r1 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] : Out_4 = In_6 && rowIdx(Out_8) = rowIdx(Out_6) && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) }");

  Relation *r2 = new Relation("{ [0, In_2, 2, In_4, 0, In_6, 0, In_8, 0] -> [0, Out_2, 2, Out_4, 0, Out_6, 0, Out_8, 0] : Out_8 = In_6 && rowIdx(Out_8) = rowIdx(Out_6) && rowIdx(In_8) = rowIdx(In_6) && 0 <= In_2 && 0 <= Out_2 && In_4 <= In_8 && colPtr(rowIdx(In_4)) <= In_6 && Out_4 <= Out_8 && colPtr(rowIdx(Out_4)) <= Out_6 && rowIdx(Out_8 + 1) <= rowIdx(Out_6) && rowIdx(In_8 + 1) <= rowIdx(In_8) && In_2 < Out_2 && In_2 + 1 < n && In_4 < colPtr(In_2 + 1) && colPtr(In_2) < In_4 && In_6 < colPtr(rowIdx(In_4) + 1) && In_8 < colPtr(In_2 + 1) && Out_2 + 1 < n && Out_4 < colPtr(Out_2 + 1) && colPtr(Out_2) < Out_4 && Out_6 < colPtr(rowIdx(Out_4) + 1) && Out_8 < colPtr(Out_2 + 1) }");

  // Testing r1->setRelationship(r2) 
  EXPECT_EQ( iegenlib::SuperSetEqual, r1->setRelationship(r2) );

  delete r1;
  delete r2;
}
