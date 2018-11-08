/*!
 * \file expression_test.cc
 *
 * \brief Test for the Expression classes and Term classes.
 *
 * This file is to test all of the Expression and Term classes, along with the
 * related classes: EqExp, IneqExp, UFCallTerm, VarTerm, and TupleVarTerm.
 *
 * \authors Michelle Strout and Joe Strout
 *
 * Copyright (c) 2012, Colorado State University <br>
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "expression.h"
#include "set_relation.h"
#include "SubMap.h"
#include "../util/util.h"

#include <gtest/gtest.h>
#include <utility>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;

using iegenlib::Exp;
using iegenlib::Term;
using iegenlib::UFCallTerm;
using iegenlib::TupleVarTerm;
using iegenlib::VarTerm;
using iegenlib::TupleDecl;
using iegenlib::TupleExpTerm;
using iegenlib::SubMap;

// All the below used to test Exp::invertFuncToExposeFactor
using iegenlib::UninterpFunc;
using iegenlib::Set;


// This is just a test setup class that is used in the other tests in this
// file.  We could create a data structure that many of the tests use.
class ExpTest:public::testing::Test {
public:
    // data structure declaration
    std::string root_dir;
    std::string dot_data;
protected:
    // method that sets up the data structure
    void SetUp();

    void TearDown() {
        //delete st;
    }
};

void ExpTest::SetUp() { 

  char* temp = getenv ("IEGEN_HOME");
  if (temp == NULL) {
    throw iegenlib::assert_exception("Please set the IEGEN_HOME "
         "environment variable to the distribution root directory "
         "(see README for more details).\n");
  } else {
    root_dir = temp;
  }
  dot_data = root_dir + "/test_data/dotTest/";
}

// Test the Term methods
TEST_F(ExpTest, TermMethods) {

    // Test construction and conversion to string
    Term t1(42);
    EXPECT_EQ(t1.coefficient(), 42);
    EXPECT_EQ(t1.toString(), "42");
    
    // Test copy-construction and assignment
    Term t2(t1);
    EXPECT_EQ(t2.coefficient(), 42);
    EXPECT_EQ(t2.toString(), "42");

    Term t3(0);
    t3 = t1;
    EXPECT_EQ(t3.coefficient(), 42);
    EXPECT_EQ(t3.toString(), "42");
    
    t2 = Term(1);
    EXPECT_EQ("1", t2.toString());
    EXPECT_TRUE(t1.factorMatches(t2));
    EXPECT_TRUE(t2.factorMatches(t1));
    EXPECT_TRUE(t1.combine(t2.clone()));
    EXPECT_EQ(1+42, t1.coefficient());
    
    // Test multiply
    t1.multiplyBy(2);
    EXPECT_EQ(2*43, t1.coefficient());
}

// Test the VarTerm methods
TEST_F(ExpTest, VarTermMethods) {

    // Test construction and conversion to string
    VarTerm t1(42, "N");
    EXPECT_EQ(t1.coefficient(), 42);
    EXPECT_EQ(t1.toString(), "42 N");
    
    // Test copy-construction and assignment
    VarTerm t2(t1);
    EXPECT_EQ(t2.coefficient(), 42);
    EXPECT_EQ(t2.toString(), "42 N");

    VarTerm t3("M");
    EXPECT_EQ("M", t3.toString());
    t3 = t1;
    EXPECT_EQ(t3.coefficient(), 42);
    EXPECT_EQ(t3.toString(), "42 N");
    
    // Test combination of terms (that should not combine)
    Term t4(1);        // (just a constant)
    EXPECT_FALSE(t4.factorMatches(t1));
    EXPECT_FALSE(t1.factorMatches(t4));
    EXPECT_FALSE(t4.combine(t1.clone()));
    EXPECT_EQ(1, t4.coefficient());
    EXPECT_FALSE(t1.combine(t4.clone()));
    EXPECT_EQ(42, t1.coefficient());
    t3 = VarTerm(2, "M");
    EXPECT_FALSE(t1.factorMatches(t3));
    EXPECT_FALSE(t3.factorMatches(t1));
    EXPECT_FALSE(t1.combine(t3.clone()));
    EXPECT_FALSE(t3.combine(t1.clone()));
    
    // Test combination of terms that can combine
    t2 = VarTerm(1234, "N");
    EXPECT_TRUE(t1.factorMatches(t2));
    EXPECT_TRUE(t2.factorMatches(t1));
    EXPECT_TRUE(t1.combine(t2.clone()));
    EXPECT_EQ(42+1234, t1.coefficient());
    EXPECT_EQ(1234, t2.coefficient());
}

// Test the TupleVarTerm methods
TEST_F(ExpTest, TupleVarTermMethods) {

    // Test construction and conversion to string
    TupleVarTerm t1(42, 0);
    EXPECT_EQ(t1.coefficient(), 42);
    EXPECT_EQ(t1.toString(), "42 __tv0");
    
    // Test copy-construction and assignment
    TupleVarTerm t2(t1);
    EXPECT_EQ(t2.coefficient(), 42);
    EXPECT_EQ(t2.toString(), "42 __tv0");

    TupleVarTerm t3(0, 0);
    t3 = t1;
    EXPECT_EQ(t3.coefficient(), 42);
    EXPECT_EQ(t3.toString(), "42 __tv0");
    
    // Test combination of things that should not be able to combine
    t3 = TupleVarTerm(2, 1);
    EXPECT_FALSE(t1.factorMatches(t3));
    EXPECT_FALSE(t3.factorMatches(t1));
    Term t4(1);        // (just a constant)
    EXPECT_FALSE(t4.factorMatches(t1));
    EXPECT_FALSE(t1.factorMatches(t4));

    // Test combinations that should actually combine
    t2 = TupleVarTerm(1234, 0);
    EXPECT_TRUE(t1.factorMatches(t2));
    EXPECT_TRUE(t2.factorMatches(t1));
    EXPECT_TRUE(t1.combine(t2.clone()));
    EXPECT_EQ(42+1234, t1.coefficient());
    EXPECT_EQ(1234, t2.coefficient());
}

// Test the UFCallTerm methods
TEST_F(ExpTest, UFCallTermMethods) {

    // Test construction and conversion to string
    Exp *e1 = new Exp();  // 42 __tv0 + 7
    e1->addTerm(new TupleVarTerm(42, 0));
    e1->addTerm(new Term(7));
    
    Exp *e2 = new Exp();  // N + 3 __tv3
    e2->addTerm(new VarTerm("N"));
    e2->addTerm(new TupleVarTerm(3, 3));
    
    UFCallTerm u1(5, "foo", 2);
    u1.setParamExp(0,e1);
    u1.setParamExp(1,e2);
    EXPECT_EQ("5 foo(42 __tv0 + 7, 3 __tv3 + N)", u1.toString());

    UFCallTerm u2(u1);
    EXPECT_EQ("5 foo(42 __tv0 + 7, 3 __tv3 + N)", u2.toString());
    
    // operator<, factorMatches, and combine are all tested
    // (indirectly) in the ExpMethods test.
}

#pragma mark ExpMethods
// Test the basic Exp methods
TEST_F(ExpTest, ExpMethods) {
    // simple case of adding terms that can't combine
    Exp e1;
    e1.addTerm(new Term(21));
    EXPECT_EQ("21", e1.toString());
    e1.addTerm(new VarTerm(1, "N"));
    EXPECT_EQ("N + 21", e1.toString());
    e1.multiplyBy(2);
    EXPECT_EQ("2 N + 42", e1.toString());
    
    // now let's add in a term that can combine with an existing one
    e1.addTerm(new VarTerm(3, "N"));
    EXPECT_EQ("5 N + 42", e1.toString());
    // and similar for some tuple terms
    e1.addTerm(new TupleVarTerm(7, 3));
    e1.addTerm(new TupleVarTerm(12, 0));
    EXPECT_EQ("12 __tv0 + 7 __tv3 + 5 N + 42", e1.toString());
    e1.addTerm(new TupleVarTerm(2, 3));
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 5 N + 42", e1.toString());   
    
    // next, let's eliminate the "5 N" term by adding -5 N
    e1.addTerm(new VarTerm(-5, "N"));
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 42", e1.toString());   
    
    // test copy-construction and assignment
    Exp e2(e1);
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 42", e2.toString());
    Exp e3;
    e3 = e2;
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 42", e3.toString());
    
    // add in a UFCall
    UFCallTerm* uf_call = new UFCallTerm(3, "foo", 2);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e1.addTerm(uf_call); // 3 foo(5 N, 42)
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 3 foo(5 N, 42) + 42", e1.toString());

    // add in the same UFCall, differing only in coefficient;
    // it should combine with the last one
    uf_call = new UFCallTerm(6, "foo", 2);
    arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call->setParamExp(0,arg1);
    arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e1.addTerm(uf_call); // 6 foo(5 N, 42)
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 9 foo(5 N, 42) + 42", e1.toString());   

    // add in a UFCall with different arguments; it should NOT combine
    uf_call = new UFCallTerm(2, "foo", 2);
    arg1 = new Exp();
    arg1->addTerm(new VarTerm(4, "N"));
    uf_call->setParamExp(0,arg1);
    arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e1.addTerm(uf_call); // 2 foo(4 N, 42)
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 2 foo(4 N, 42) + 9 foo(5 N, 42) + 42", e1.toString());   

    // add in a UFCall with the same arguments but a different name;
    // it should NOT combine either
    uf_call = new UFCallTerm(3, "bar", 2);
    arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call->setParamExp(0,arg1);
    arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e1.addTerm(uf_call); // 3 bar(5 N, 42)
    EXPECT_EQ("12 __tv0 + 9 __tv3 + 3 bar(5 N, 42) + 2 foo(4 N, 42) + 9 foo(5 N, 42) + 42", e1.toString());   
    
    // test normalization for equality
    Exp e4;
    e4.addTerm(new Term(5));
    e4.addTerm(new VarTerm(-2, "N"));
    EXPECT_EQ("-2 N + 5", e4.toString());
    e4.normalizeForEquality();
    EXPECT_EQ("2 N - 5", e4.toString());
    e4.normalizeForEquality();
    EXPECT_EQ("2 N - 5", e4.toString());
    
}

#pragma mark ExposeUFCallNotElimBug
// Trying to see the f(i)-f(i)
// like what Mark found in { [x] -> [d] : f(x) - f(x) = 0 && d - f(x) = 0 }.
// Actually, this is not where it is happening, the below works.
TEST_F(ExpTest, ExposeUFCallNotElimBug) {
    
    // create a UFCall
    Exp e1;
    UFCallTerm* uf_call = new UFCallTerm("f", 1);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm("i"));
    uf_call->setParamExp(0,arg1);
    e1.addTerm(uf_call); // f(i)
    EXPECT_EQ("f(i)", e1.toString());

    // add in the same UFCall, but negated
    uf_call = new UFCallTerm(-1,"f", 1);
    arg1 = new Exp();
    arg1->addTerm(new VarTerm("i"));
    uf_call->setParamExp(0,arg1);
    e1.addTerm(uf_call); // -f(i)
    EXPECT_EQ("0", e1.toString());   
}


#pragma mark ExpMethodsAdvanced
// Test the more advanced Exp methods: adding expressions, substitution, etc.
TEST_F(ExpTest, ExpMethodsAdvanced) {
    // Set up two expressions...
    Exp e1;
    e1.addTerm(new Term(21));
    e1.addTerm(new VarTerm(1, "N"));
    EXPECT_EQ("N + 21", e1.toString());

    Exp e2;
    e2.addTerm(new VarTerm(2, "M"));
    e2.addTerm(new VarTerm(-1, "N"));
    e2.addTerm(new Term(-21));
    EXPECT_EQ("2 M - N - 21", e2.toString());
    
    // Add the two expressions together
    e1.addExp(new Exp(e2));
    EXPECT_EQ("2 M", e1.toString());
    
    // Now e2 is still "2 M - 1 N - 21";
    // substitute e1 (which is "2 M") for "N",
    // and it should cancel out everything except -21.
    SubMap subMap;
    subMap.insertPair(new VarTerm("N"), new Exp(e1));
    e2.substitute(subMap);
    EXPECT_EQ("-21", e2.toString());
    EXPECT_EQ("2 M", e1.toString());

    // Let's add in an unintepreted function call,
    // and try another substitution.
    UFCallTerm* uf_call = new UFCallTerm(3, "foo", 2);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e2.addTerm(uf_call); // 3 foo(5 N, 42)
    e2.addTerm(new VarTerm(2, "N"));
    EXPECT_EQ("2 N + 3 foo(5 N, 42) - 21", e2.toString());
    EXPECT_EQ("2 M", e1.toString());

    SubMap subMap2;
    subMap2.insertPair(new VarTerm("N"), new Exp(e1));
    e2.substitute(subMap2);
    EXPECT_EQ("2 M", e1.toString());
    EXPECT_EQ("4 M + 3 foo(10 M, 42) - 21", e2.toString());
}

#pragma mark ExpSubstitute
// Test en masse substitutions.
TEST_F(ExpTest, ExpSubstitute) {
    // Set up two expressions...
    Exp * e1 = new Exp();
    e1->addTerm(new Term(21));
    e1->addTerm(new VarTerm(1, "N"));
    EXPECT_EQ("N + 21", e1->toString());

    Exp e2;
    e2.addTerm(new VarTerm(2, "M"));
    e2.addTerm(new VarTerm(-1, "N"));
    e2.addTerm(new Term(-21));
    EXPECT_EQ("2 M - N - 21", e2.toString());
    
    // Add the two expressions together
    e1->addExp(new Exp(e2));
    EXPECT_EQ("2 M", e1->toString());
    
    // Now e2 is still "2 M - 1 N - 21";
    // substitute e1 (which is "2 M") for "N",
    // and it should cancel out everything except -21.
    VarTerm * vartermN = new VarTerm("N");
    SubMap subMap;
    subMap.insertPair(vartermN, e1);
    e2.substitute(subMap);
    EXPECT_EQ("-21", e2.toString());
    EXPECT_EQ("2 M", e1->toString());

    // Let's add in an unintepreted function call,
    // and try the same substitution.
    UFCallTerm* uf_call = new UFCallTerm(3, "foo", 2);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(new Term(42));
    uf_call->setParamExp(1,arg2);
    e2.addTerm(uf_call); // 3 foo(5 N, 42)
    e2.addTerm(new VarTerm(2, "N"));
    EXPECT_EQ("2 N + 3 foo(5 N, 42) - 21", e2.toString());
    EXPECT_EQ("2 M", e1->toString());

    e2.substitute(subMap);
    EXPECT_EQ("2 M", e1->toString());
    EXPECT_EQ("4 M + 3 foo(10 M, 42) - 21", e2.toString());
}

#pragma mark ExpSubstituteSwap
// Test en masse substitutions where swapping
// two tuple variables and therefore
// substitution in one pass is really needed.
TEST_F(ExpTest, ExpSubstituteSwap) {
    // Set up two expressions...
    Exp * e1 = new Exp();
    e1->addTerm(new Term(21));
    e1->addTerm(new TupleVarTerm(1, 0)); // __tv0
    EXPECT_EQ("__tv0 + 21", e1->toString());

    Exp e2;
    e2.addTerm(new TupleVarTerm(2, 1)); // 2 __tv1
    e2.addTerm(new TupleVarTerm(-1, 0));
    e2.addTerm(new Term(-21));
    EXPECT_EQ("-__tv0 + 2 __tv1 - 21", e2.toString());
    
    // Add the two expressions together
    e1->addExp(new Exp(e2));
    EXPECT_EQ("2 __tv1", e1->toString());
    
    // Now e2 is still "2 __tv1 - __tv0 - 21";
    // substitute e1 (which is "2 __tv1") for "__tv0"
    // and substitute 3 __tv0 for __tv1
    // and the result should be 6 __tv0 - 2 __tv1 - 21.
    Exp* e3 = new Exp();
    e3->addTerm( new TupleVarTerm(3, 0) ); // 3 __tv0
    TupleVarTerm * tv0 = new TupleVarTerm(0);
    TupleVarTerm * tv1 = new TupleVarTerm(1);
    SubMap subMap;
    subMap.insertPair(tv0, e1);
    subMap.insertPair(tv1, e3);
    e2.substitute(subMap);
    EXPECT_EQ("6 __tv0 - 2 __tv1 - 21", e2.toString());
    EXPECT_EQ("2 __tv1", e1->toString());

    // Let's add in an unintepreted function call,
    // and try the same substitution.
    UFCallTerm* uf_call = new UFCallTerm(3, "foo", 2);
    Exp *arg1 = new Exp();
    arg1->addTerm(new TupleVarTerm(5, 1));
    uf_call->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(new TupleVarTerm(0));
    uf_call->setParamExp(1,arg2);
    e2.addTerm(uf_call); // 3 foo(5 __tv1, __tv0)
    e2.addTerm(new TupleVarTerm(5, 1)); // 2 __tv1
    EXPECT_EQ("6 __tv0 + 3 __tv1 + 3 foo(5 __tv1, __tv0) - 21", e2.toString());

    e2.substitute(subMap);
    EXPECT_EQ("9 __tv0 + 12 __tv1 + 3 foo(15 __tv0, 2 __tv1) - 21", e2.toString());
}

#pragma mark ExpSolving
// Test solving equality expressions
TEST_F(ExpTest, ExpSolving) {
    Exp e1;
    e1.addTerm(new VarTerm(-2, "N"));
    e1.addTerm(new VarTerm(10, "M"));
    e1.addTerm(new Term(8));
    e1.addTerm(new TupleVarTerm(16, 3));
    EXPECT_EQ("16 __tv3 + 10 M - 2 N + 8", e1.toString());
    EXPECT_TRUE(e1.isDivisible(2));
    EXPECT_FALSE(e1.isDivisible(3));
    
    Exp *solution = e1.solveForFactor(new VarTerm("N"));
    EXPECT_TRUE(solution != NULL);
    EXPECT_EQ("8 __tv3 + 5 M + 4", solution->toString());
    delete solution;
}

#pragma mark ExpDependsOn
// Test dependsOn
TEST_F(ExpTest, ExpDependsOn) {
    Exp e1;
    e1.addTerm(new VarTerm(-2, "N"));
    e1.addTerm(new VarTerm(10, "M"));
    e1.addTerm(new Term(8));
    e1.addTerm(new TupleVarTerm(16, 3));

    UFCallTerm* uf_call_g = new UFCallTerm(7, "g", 1); // 7 g(3 __tv0)
    Exp *g0 = new Exp();
    g0->addTerm(new TupleVarTerm(3, 0));
    uf_call_g->setParamExp(0,g0);

    UFCallTerm* uf_call_foo = new UFCallTerm(3, "foo", 2); // 3 foo(5 N, 42)
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call_foo->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(uf_call_g);
    uf_call_foo->setParamExp(1,arg2);
    e1.addTerm(uf_call_foo); // 3 foo(5 N, 42)

    EXPECT_EQ("16 __tv3 + 10 M - 2 N + 3 foo(5 N, 7 g(3 __tv0)) + 8", e1.toString());
    TupleVarTerm tv0(0);
    EXPECT_TRUE(e1.dependsOn(tv0));
    TupleVarTerm tv2(2);
    EXPECT_FALSE(e1.dependsOn(tv2));
    VarTerm N("N");
    EXPECT_TRUE(e1.dependsOn(N));

    UFCallTerm gcall("g",1);    // g(3 __tv0)
    g0 = new Exp();
    g0->addTerm(new TupleVarTerm(3, 0));
    gcall.setParamExp(0,g0);
    EXPECT_TRUE(e1.dependsOn(gcall));
}

#pragma mark ExpTermCompare
// Test the comparison of Exp and Term when both are const
TEST_F(ExpTest, ExpTermCompare) {
    Exp e1;
    e1.addTerm(new Term(42));
    Term t1(42);
    EXPECT_TRUE(e1==t1);

    Term t2(7);
    EXPECT_FALSE(e1==t2);
    
    e1.addTerm(new TupleVarTerm(99));
    EXPECT_FALSE(e1==t2);
}    

#pragma mark JIRA_85
// Illustrates root cause for IEGRONE-85.
// Exp::operator== was not considering an empty Exp as equal to 0.
// Test the comparison of Exp and Term when both are const
TEST_F(ExpTest, JIRA_85) {
    Exp e1;
    e1.addTerm(new Term(0));
    Term t1(0);
    EXPECT_TRUE(e1==t1);
}    

#pragma mark ExpRemapLocs
// Test remapping of tuple variables
TEST_F(ExpTest, ExpRemapLocs) {
    Exp e1;
    e1.addTerm(new Term(42));
    e1.addTerm(new VarTerm(5, "N"));
    e1.addTerm(new TupleVarTerm(7, 3));
    e1.addTerm(new TupleVarTerm(12, 0));
    EXPECT_EQ("12 __tv0 + 7 __tv3 + 5 N + 42", e1.toString());

    UFCallTerm* uf_call = new UFCallTerm("g", 1); // g(__tv2)
    Exp *g0 = new Exp();
    g0->addTerm(new TupleVarTerm(2));
    uf_call->setParamExp(0,g0);
    e1.addTerm(uf_call);
    EXPECT_EQ("12 __tv0 + 7 __tv3 + 5 N + g(__tv2) + 42", e1.toString());

    std::vector<int> map(4);
    map[0] = 4;
    map[1] = 1;
    map[2] = 0;
    map[3] = 2;
    e1.remapTupleVars(map);
    EXPECT_EQ("12 __tv4 + 7 __tv2 + 5 N + g(__tv0) + 42", e1.toString());  
}

#pragma mark ExpIsConst
// Test that even an expression with the value of zero is considered const.
TEST_F(ExpTest, ExpIsConst) {
    Exp* e1 = new Exp();
    EXPECT_TRUE( e1->isConst() );   // default value of Exp is 0
    
    e1->addTerm(new Term(0));
    EXPECT_TRUE( e1->isConst() );   // should also work with explicit 0
    
    e1->addTerm(new Term(42));
    EXPECT_TRUE( e1->isConst() );   // adding in another constant

    e1->addTerm(new VarTerm(5, "N"));   // term is no longer constant
    EXPECT_FALSE( e1->isConst() );   // adding in another constant
    
    delete e1;
}

TEST_F(ExpTest, PrettyPrintString){

    TupleDecl tupleDecl(8);
    tupleDecl.setTupleElem(0,"A");
    tupleDecl.setTupleElem(1,"B");
    tupleDecl.setTupleElem(2,"C");
    tupleDecl.setTupleElem(3,"D");
    tupleDecl.setTupleElem(4,5);
    tupleDecl.setTupleElem(5,5);
    tupleDecl.setTupleElem(6,0);
    tupleDecl.setTupleElem(7,-5);

    Exp *e1 = new Exp();
    e1->addTerm(new Term(42));
    e1->addTerm(new VarTerm(5, "N"));
    e1->addTerm(new TupleVarTerm(7, 3));
    e1->addTerm(new TupleVarTerm(12, 0));
    EXPECT_EQ("12 A + 7 D + 5 N + 42", e1->prettyPrintString(tupleDecl));
    delete e1;

    Exp *e2 = new Exp();
    e2->addTerm(new Term(5));
    e2->addTerm(new VarTerm(5, "Y"));
    e2->addTerm(new TupleVarTerm(-2, 1));
    e2->addTerm(new TupleVarTerm(18, 2));
    EXPECT_EQ("-2 B + 18 C + 5 Y + 5", e2->prettyPrintString(tupleDecl));
    delete e2;

    Exp *e5 = new Exp;
    e5->addTerm(new Term(42));
    e5->addTerm(new VarTerm(5, "N"));
    e5->addTerm(new TupleVarTerm(7, 3));
    e5->addTerm(new TupleVarTerm(12, 0));
    UFCallTerm* uf_call = new UFCallTerm("g", 1); // g(B+C)
    Exp *g0 = new Exp();
    g0->addTerm(new TupleVarTerm(2));
    g0->addTerm(new TupleVarTerm(1));
    uf_call->setParamExp(0,g0);
    e5->addTerm(uf_call);
    EXPECT_EQ("12 A + 7 D + 5 N + g(B + C) + 42",
        e5->prettyPrintString(tupleDecl));
    delete e5;
}

TEST_F(ExpTest, ToDotString){
    int e1count = 0;
    Exp e1;
    e1.addTerm(new VarTerm(-2, "N"));
    e1.addTerm(new VarTerm(10, "M"));
    e1.addTerm(new Term(8));
    e1.addTerm(new TupleVarTerm(16, 3));

    UFCallTerm* uf_call = new UFCallTerm(3, "foo", 2);
    UFCallTerm* uf_call_g = new UFCallTerm(7, "g", 1);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5,"N"));
    uf_call->setParamExp(0,arg1);
    Exp* garg1 = new Exp();
    garg1->addTerm( new TupleVarTerm(3, 0) );
    uf_call_g->setParamExp(0,garg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(uf_call_g);
    uf_call->setParamExp(1,arg2);
    e1.addTerm(uf_call); // 3 foo(5 N, 7 g(3 __tv0) )

//std::cout << "e1 = " << e1.toString() << std::endl;
//std::cout << "e1 = " << e1.toDotString(e1count) << std::endl;
    string testFile = dot_data + "testExpToDotStringTest1.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              e1.toDotString(e1count)));

    // Test construction and conversion to string
    Exp *e2 = new Exp();  // 42 __tv0 + 7
    e2->addTerm(new TupleVarTerm(42, 0));
    e2->addTerm(new Term(7));
    
    Exp *e3 = new Exp();  // N + 3 __tv3
    e3->addTerm(new VarTerm("N"));
    e3->addTerm(new TupleVarTerm(3, 3));

    uf_call = new UFCallTerm(5, "foo", 2);    
    uf_call->setParamExp(0,e2);
    uf_call->setParamExp(1, e3);
    int e4count = 0;
    Exp *e4 = new Exp();
    e4->addTerm(uf_call);


    testFile = dot_data + "testExpToDotStringTest2.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              e4->toDotString(e4count)));

    delete e4;

}

TEST_F(ExpTest, SymbolIterator){

    // Make a suitable tuple declaration
    TupleDecl tupleDecl(8);
    tupleDecl.setTupleElem(0,"A");
    tupleDecl.setTupleElem(1,"B");
    tupleDecl.setTupleElem(2,"C");
    tupleDecl.setTupleElem(3,"D");
    tupleDecl.setTupleElem(4,5);
    tupleDecl.setTupleElem(5,5);
    tupleDecl.setTupleElem(6,0);
    tupleDecl.setTupleElem(7,-5);

    Exp *e1 = new Exp();
    e1->addTerm(new Term(42));
    e1->addTerm(new VarTerm(5, "N"));
    e1->addTerm(new TupleVarTerm(7, 3));
    e1->addTerm(new TupleVarTerm(12, 0));
    e1->addTerm(new VarTerm(-5, "P"));
    EXPECT_EQ("12 A + 7 D + 5 N - 5 P + 42", e1->prettyPrintString(tupleDecl));
    
    // Check the expected set of symbols against the actual set
    std::set<std::string> expectedSet;
    expectedSet.insert("N");
    expectedSet.insert("P");
    std::set<std::string> actualSet;
    iegenlib::StringIterator * siter = e1->getSymbolIterator();
    while (siter->hasNext()) {
        actualSet.insert(siter->next());
    }
    EXPECT_EQ(expectedSet, actualSet);
    delete siter;
    delete e1;

    Exp *e2 = new Exp();
    e2->addTerm(new Term(5));
    e2->addTerm(new VarTerm(5, "Y"));
    e2->addTerm(new TupleVarTerm(-2, 1));
    e2->addTerm(new TupleVarTerm(18, 2));
    EXPECT_EQ("-2 B + 18 C + 5 Y + 5", e2->prettyPrintString(tupleDecl));

    // Check the expected set of symbols against the actual set
    expectedSet.clear();
    actualSet.clear();
    expectedSet.insert("Y");
    siter = e2->getSymbolIterator();
    while (siter->hasNext()) {
        actualSet.insert(siter->next());
    }
    EXPECT_EQ(expectedSet, actualSet);
    delete siter;
    delete e2;

    Exp *e5 = new Exp;
    e5->addTerm(new Term(42));
    e5->addTerm(new VarTerm(5, "N"));
    e5->addTerm(new TupleVarTerm(7, 3));
    e5->addTerm(new TupleVarTerm(12, 0));
    UFCallTerm* uf_call = new UFCallTerm("g", 1); // g(C - 3 Q)
    Exp *g0 = new Exp();
    g0->addTerm(new TupleVarTerm(2));
    g0->addTerm(new VarTerm(-3, "Q"));
    uf_call->setParamExp(0,g0);
    e5->addTerm(uf_call);
    EXPECT_EQ("12 A + 7 D + 5 N + g(C - 3 Q) + 42",
        e5->prettyPrintString(tupleDecl));
        
    // Check the expected set of symbols against the actual set
    expectedSet.clear();
    actualSet.clear();
    expectedSet.insert("Q");
    expectedSet.insert("N");
    siter = e5->getSymbolIterator();
    while (siter->hasNext()) {
        actualSet.insert(siter->next());
    }
    EXPECT_EQ(expectedSet, actualSet);
    delete siter;
    delete e5;
}

//Tests for Exp::operator<(Exp)
TEST_F(ExpTest, OperatorLessThan){
    //create some terms so we can add them to Exp's
    Term term(30);
    Term term2(42);
    Term term3(72);
    VarTerm vterm(5, "N");
    VarTerm vterm2(7, "y");
    TupleVarTerm tvterm(3, 4);
    TupleVarTerm tvterm2(7, 4);
    UFCallTerm ufcterm("abc", 1); // abc(w)
    Exp *g0 = new Exp();
    g0->addTerm(new VarTerm("w"));
    ufcterm.setParamExp(0,g0);

    UFCallTerm ufcterm2(5, "def", 1); // def(w)
    Exp *g1 = new Exp();
    g1->addTerm(new VarTerm("w"));
    ufcterm2.setParamExp(0,g1);

    //This test assumes these terms have the following ordering:
    //  TupleVarTerm < VarTerm < UFCallTerm < Term
    EXPECT_TRUE(tvterm<tvterm2); EXPECT_FALSE(tvterm2<tvterm); EXPECT_TRUE(tvterm2<vterm);
    EXPECT_TRUE(vterm<vterm2); EXPECT_FALSE(vterm2<vterm); EXPECT_TRUE(vterm2<ufcterm);
    EXPECT_TRUE(ufcterm<ufcterm2); EXPECT_FALSE(ufcterm2<ufcterm); EXPECT_TRUE(ufcterm2<term);
    EXPECT_TRUE(term<term2); EXPECT_TRUE(term2<term3); EXPECT_FALSE(term2<term); EXPECT_FALSE(term3<term);

    //Exp comparisons:

    // e1: 3 __tv4 + 5 N + abc(w) + 30
    Exp *e1 = new Exp;
    e1->addTerm(new TupleVarTerm(tvterm));
    e1->addTerm(new VarTerm(vterm));
    e1->addTerm(new UFCallTerm(ufcterm));
    e1->addTerm(new Term(term));
    
    Exp *e1_copy = new Exp(*e1); //copy constructor
    
    //self comparison should always be false
    EXPECT_FALSE( *e1 < *e1 );
    EXPECT_FALSE( *e1 < *e1_copy );
    EXPECT_FALSE( *e1_copy < *e1 );
    
    // e2: 3 __tv4 + 5 N + abc(w) + 30 
    Exp *e2 = new Exp;
    e2->addTerm(new Term(term));
    e2->addTerm(new VarTerm(vterm));
    e2->addTerm(new TupleVarTerm(tvterm));
    e2->addTerm(new UFCallTerm(ufcterm));
    
    // e1 == e2, items should be sorted the same way
    EXPECT_FALSE( *e1 < *e2 );
    EXPECT_FALSE( *e2 < *e1 );
    
    // e3: 3 __tv4 + 5 N + abc(w) + 42 
    Exp *e3 = new Exp;
    e3->addTerm(new TupleVarTerm(tvterm));
    e3->addTerm(new VarTerm(vterm));
    e3->addTerm(new UFCallTerm(ufcterm));
    e3->addTerm(new Term(term2)); // term (30) < term2 (42)
    
    EXPECT_TRUE( *e2 < *e3 );
    EXPECT_FALSE(*e3 < *e2 );
   
    // e4: 3 __tv4 + 5 N + 5 def(w) + 42
    Exp *e4 = new Exp;
    e4->addTerm(new TupleVarTerm(tvterm));
    e4->addTerm(new VarTerm(vterm));
    e4->addTerm(new UFCallTerm(ufcterm2)); // ufcterm (abc(w)) < ufcterm2 (5 def(w))
    e4->addTerm(new Term(term2));
    
    EXPECT_TRUE( *e3 < *e4 );
    EXPECT_FALSE(*e4 < *e3 );
    EXPECT_TRUE( *e2 < *e4 );
    EXPECT_FALSE(*e4 < *e2 );
    
    // e5: 3 __tv4 + 7 y + 5 def(w) + 42 
    Exp *e5 = new Exp;
    e5->addTerm(new TupleVarTerm(tvterm));
    e5->addTerm(new VarTerm(vterm2)); // vterm (5 N) < vterm2 (7 y)
    e5->addTerm(new UFCallTerm(ufcterm2));
    e5->addTerm(new Term(term2));
    
    EXPECT_TRUE( *e4 < *e5 );
    EXPECT_FALSE(*e5 < *e4 );
    EXPECT_TRUE( *e2 < *e5 );
    EXPECT_FALSE(*e5 < *e2 );
    EXPECT_TRUE( *e3 < *e5 );
    EXPECT_FALSE(*e5 < *e3 );
   
    // e6: 7 __tv4 + 7 y + 5 def(w) + 42
    Exp *e6 = new Exp;
    e6->addTerm(new TupleVarTerm(tvterm2)); // tvterm (3 TupleVarE) < tvterm2 (7 TupleVarE)
    e6->addTerm(new VarTerm(vterm2));
    e6->addTerm(new UFCallTerm(ufcterm2));
    e6->addTerm(new Term(term2));
    
    EXPECT_TRUE( *e5 < *e6 );
    EXPECT_FALSE(*e6 < *e5 );
    EXPECT_TRUE( *e2 < *e5 );
    EXPECT_FALSE(*e5 < *e2 );
    EXPECT_TRUE( *e3 < *e5 );
    EXPECT_FALSE(*e5 < *e3 );
    EXPECT_TRUE( *e4 < *e5 );
    EXPECT_FALSE(*e5 < *e4 );
     
    // e7: 6 __tv4 + 5 N + 7 y + abc(w) + 30 
    Exp *e7 = new Exp;
    e7->addTerm(new Term(term));
    e7->addTerm(new VarTerm(vterm));
    e7->addTerm(new VarTerm(vterm2));
    e7->addTerm(new TupleVarTerm(tvterm));
    e7->addTerm(new UFCallTerm(ufcterm));  // 4 terms < 5 terms
    
    EXPECT_TRUE( *e6 < *e7 );
    EXPECT_FALSE(*e7 < *e6 );
    
    // e8: 5 N + 30 
    Exp *e8 = new Exp;
    e8->addTerm(new Term(term));
    e8->addTerm(new VarTerm(vterm));
    e7->addTerm(new TupleVarTerm(tvterm));// 3 terms < 4 terms
    
    EXPECT_TRUE( *e8 < *e2 );
    EXPECT_FALSE(*e2 < *e8 );
    
    //how about combining terms?
    // e9: 30 + 42 = 72
    Exp *e9 = new Exp;
    // e10: 72
    Exp *e10 = new Exp;
    e9->addTerm(new Term(term));
    e9->addTerm(new Term(term2));
    e10->addTerm(new Term(term3));
    
    EXPECT_FALSE(*e9 < *e10 );
    EXPECT_FALSE(*e10 < *e9 );

    delete e1;
    delete e1_copy;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
    delete e8;
    delete e9;
    delete e10;
}

//Tests for Exp::operator==(Exp)
TEST_F(ExpTest, OperatorEqualsExp){
    //create some terms so we can add them to Exp's
    Term term(30);
    Term term2(42);
    Term term3(72);
    VarTerm vterm(5, "N");
    VarTerm vterm2(7, "N");
    VarTerm vterm3(12, "N");
    TupleVarTerm tvterm(3, 4);
    TupleVarTerm tvterm2(7, 4);
    TupleVarTerm tvterm3(10, 4);
    UFCallTerm ufcterm("abc", 1); // abc(w)
    Exp *g0 = new Exp();
    g0->addTerm(new VarTerm("w"));
    ufcterm.setParamExp(0,g0);

    UFCallTerm ufcterm2(5, "def", 1); // def(w)
    Exp *g1 = new Exp();
    g1->addTerm(new VarTerm("w"));
    ufcterm2.setParamExp(0,g1);
    
    //Comparing equal things:

    // e1: 3 __tv4 + 5 N + abc(w) + 30
    Exp *e1 = new Exp;
    e1->addTerm(new TupleVarTerm(tvterm));
    e1->addTerm(new VarTerm(vterm));
    e1->addTerm(new UFCallTerm(ufcterm));
    e1->addTerm(new Term(term));
    
    Exp *e1_copy = new Exp(*e1); //copy constructor
    
    //self comparison should always be true
    EXPECT_TRUE( *e1 == *e1 );
    EXPECT_TRUE( *e1 == *e1_copy );
    EXPECT_TRUE( *e1_copy == *e1 );
    
    // e2: 3 __tv4 + 5 N + abc(w) + 30
    Exp *e2 = new Exp;
    e2->addTerm(new Term(term));
    e2->addTerm(new VarTerm(vterm));
    e2->addTerm(new TupleVarTerm(tvterm));
    e2->addTerm(new UFCallTerm(ufcterm));
    
    // e1 == e2, items should be sorted the same way
    EXPECT_TRUE( *e1 == *e2 );
    EXPECT_TRUE( *e2 == *e1 );

    //combining terms
    // e3: 10 __tv4 + 12 N + 72
    Exp *e3 = new Exp;
    // e4: 10 __tv4 + 12 N + 72
    Exp *e4 = new Exp;
    e3->addTerm(new Term(term));
    e3->addTerm(new Term(term2));
    e3->addTerm(new VarTerm(vterm));
    e3->addTerm(new VarTerm(vterm2));
    e3->addTerm(new TupleVarTerm(tvterm));
    e3->addTerm(new TupleVarTerm(tvterm2));
    e4->addTerm(new Term(term3));
    e4->addTerm(new VarTerm(vterm3));
    e4->addTerm(new TupleVarTerm(tvterm3));
    
    EXPECT_TRUE( *e3 == *e4 );
    EXPECT_TRUE( *e4 == *e3 );
    
    //Comparing nonequal things:
    
    // e5: 30 
    Exp *e5 = new Exp;
    // e6: 42
    Exp *e6 = new Exp;
    e5->addTerm(new Term(term));
    e6->addTerm(new Term(term2));
    EXPECT_FALSE( *e5 == *e6 );
    EXPECT_FALSE( *e6 == *e5 );
   
    // e7: 5 N
    Exp *e7 = new Exp;
    // e8: 7 N
    Exp *e8 = new Exp;
    e7->addTerm(new VarTerm(vterm));
    e8->addTerm(new VarTerm(vterm2));
    EXPECT_FALSE( *e7 == *e8 );
    EXPECT_FALSE( *e8 == *e7 );
    
    // e9: 3 __tv4
    Exp *e9 = new Exp;
    // e10: 7 __tv4 
    Exp *e10 = new Exp;
    e9->addTerm(new TupleVarTerm(tvterm));
    e10->addTerm(new TupleVarTerm(tvterm2));
    EXPECT_FALSE( *e9 == *e10 );
    EXPECT_FALSE( *e10 == *e9 );
    
    // e11: abc(w)
    Exp *e11 = new Exp;
    // e12: 5 abc(w)
    Exp *e12 = new Exp;
    e11->addTerm(new UFCallTerm(ufcterm));
    e12->addTerm(new UFCallTerm(ufcterm2));
    EXPECT_FALSE( *e11 == *e12 );
    EXPECT_FALSE( *e12 == *e11 );
    
    // e13: 30 
    Exp *e13 = new Exp;
    // e14: (empty)
    Exp *e14 = new Exp;
    e13->addTerm(new Term(term));
    EXPECT_FALSE( *e13 == *e14 );
    EXPECT_FALSE( *e14 == *e13 );

    delete e1;
    delete e1_copy;
    delete e2;
    delete e3;
    delete e4;
    delete e5;
    delete e6;
    delete e7;
    delete e8;
    delete e9;
    delete e10;
    delete e11;
    delete e12;
    delete e13;
    delete e14;
}

// Test the UFCallTerm methods
TEST_F(ExpTest, UFCallTermWithIndexing) {

    // Test construction and conversion to string
    Exp *e1 = new Exp();  // __tv3
    e1->addTerm(new TupleVarTerm(1, 3));
        
    UFCallTerm u1(7, "foo", 1, 0);
    u1.setParamExp(0,e1);
    EXPECT_EQ("7 foo(__tv3)[0]", u1.toString());
    TupleDecl tupleDecl(4);
    tupleDecl.setTupleElem(3,"x");
    EXPECT_EQ("7 foo(x)[0]", u1.prettyPrintString(tupleDecl));
}

// Test the method that exposes factors that are part of 
// actual parameter expressions to UFCalls.
// IEGRTWO-70, this test case implements the example inputs and 
// outputs given in IEGRTWO-70
TEST_F(ExpTest, InvertFuncToExposeFactorFuncInverse) {

    ///////////////////////
    // __tv0 + x + f( y )
    Exp *e1 = new Exp();  
    e1->addTerm(new TupleVarTerm(0));
    e1->addTerm(new VarTerm("x"));
    UFCallTerm* uf_call = new UFCallTerm("f", 1); // f(y)
    Exp *f_arg = new Exp();
    f_arg->addTerm( new VarTerm("y") );
    uf_call->setParamExp(0,f_arg);
    e1->addTerm(uf_call);
    EXPECT_EQ("__tv0 + x + f(y)", e1->toString());
    
    // Now set up an environment that defines an inverse for f because
    // f is bijective.
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    // Now expose y from above expression
    Exp * y_exposed = e1->invertFuncToExposeFactor(new VarTerm("y"));
    EXPECT_TRUE(y_exposed);
    if (y_exposed) {
        EXPECT_EQ("y - f_inv(-__tv0 - x)", y_exposed->toString());
    }
    
    delete e1;
    delete y_exposed;

    ///////////////////////
    // a + 3*b - g(2*x-7, -__tv2+42), 
    Exp *e2 = new Exp();  
    e2->addTerm(new VarTerm("a"));
    e2->addTerm(new VarTerm(3,"b"));
    uf_call = new UFCallTerm(-1,"g", 2); // g(2 x - 7, -__tv2 + 42)
    Exp *g_arg_1 = new Exp();
    g_arg_1->addTerm(new VarTerm(2,"x"));
    g_arg_1->addTerm(new Term(-7));
    uf_call->setParamExp(0,g_arg_1);
    Exp *g_arg_2 = new Exp();
    g_arg_2->addTerm(new TupleVarTerm(-1,2));
    g_arg_2->addTerm(new Term(42));
    uf_call->setParamExp(1,g_arg_2);
    e2->addTerm(uf_call);
    EXPECT_EQ("a + 3 b - g(2 x - 7, -__tv2 + 42)", e2->toString());
    
    // where g has an inverse g_inv 
    iegenlib::appendCurrEnv("g",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    
    // want to expose __tv2
    Exp * tv2_exposed = e2->invertFuncToExposeFactor(new TupleVarTerm(2));
    EXPECT_TRUE(tv2_exposed);
    if (tv2_exposed) {
        EXPECT_EQ("-__tv2 - g_inv(a + 3 b)[1] + 42", tv2_exposed->toString());
    }

    delete e2;
    delete tv2_exposed;

    ///////////////////////
    // k+tau(i,j)
    Exp *e3 = new Exp();  
    e3->addTerm(new VarTerm("k"));
    uf_call = new UFCallTerm("tau", 2); // tau(i,j)
    Exp *tau_arg_1 = new Exp();
    tau_arg_1->addTerm(new VarTerm("i"));
    uf_call->setParamExp(0,tau_arg_1);
    Exp *tau_arg_2 = new Exp();
    tau_arg_2->addTerm(new VarTerm("j"));
    uf_call->setParamExp(1,tau_arg_2);
    e3->addTerm(uf_call);
    EXPECT_EQ("k + tau(i, j)", e3->toString());
    
    // where tau has an inverse tau_inv
    iegenlib::appendCurrEnv("tau",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    
    // want to expose i
    Exp * i_exposed = e3->invertFuncToExposeFactor(new VarTerm("i"));
    EXPECT_TRUE(i_exposed);
    if (i_exposed) {
        EXPECT_EQ("i - tau_inv(-k)[0]", i_exposed->toString());
    }

    delete e3;
    delete i_exposed;

}

#pragma mark TupleExpTerm
// Test multi-dimensional expression class.  See IEGRTWO-85.
TEST_F(ExpTest, TupleExpTerm) {

    // 7 + x
    Exp* aExp = new Exp();
    aExp->addTerm( new VarTerm("x") );
    aExp->addTerm( new Term(7) );
    // y
    Exp* bExp = new Exp();
    bExp->addTerm( new VarTerm("y") );
    // __tv0
    Exp* cExp = new Exp();
    cExp->addTerm( new TupleVarTerm(3,0) );
    // g(__tv1)
    UFCallTerm* uf_call = new UFCallTerm("g",1);
    Exp* dExp = new Exp();
    Exp* eExp = new Exp();
    eExp->addTerm( new TupleVarTerm(1) );
    uf_call->setParamExp(0,eExp);
    dExp->addTerm( uf_call );

    // multi-dim expression [7+x, y, 3*__tv0, g(__tv1)]
    TupleExpTerm* outexps = new TupleExpTerm(4);
    EXPECT_EQ((unsigned)4, outexps->size() );
    outexps->setExpElem(0,aExp);
    outexps->setExpElem(1,bExp);
    outexps->setExpElem(2,cExp);
    outexps->setExpElem(3,dExp);

    EXPECT_EQ("( x + 7, y, 3 __tv0, g(__tv1) )", outexps->toString() );
    
    // Declare names and constants for the tuple variables and 
    // check that they are printed correctly.
    TupleDecl tupleDecl(2);
    tupleDecl.setTupleElem(0,"bye");
    tupleDecl.setTupleElem(1,"hello");
    EXPECT_EQ("( x + 7, y, 3 bye, g(hello) )", 
        outexps->prettyPrintString(tupleDecl) );
    
    // Grab some of the expressions
    Exp* e0 = outexps->cloneExp(0);
    EXPECT_EQ("x + 7", e0->toString());
    Exp* e1 = outexps->cloneExp(1);
    EXPECT_EQ("y", e1->toString());
    Exp* e2 = outexps->cloneExp(2);
    EXPECT_EQ("3 __tv0", e2->toString());
    Exp* e3 = outexps->cloneExp(3);
    EXPECT_EQ("g(__tv1)", e3->toString());
    
    delete e0;
    delete e1;
    delete e2;
    delete e3;

    delete outexps;
}

#pragma mark HasIndexedUFCall
// Test detection of UFCallTerm that is indexed.  See step 0 in IEGRTWO-87.
TEST_F(ExpTest, HasIndexedUFCall) {
    Exp* have = new Exp();
    // f has one input arg and returns a tuple and we are indexing it with 2
    // f(x)[2]
    UFCallTerm* ufcall = new UFCallTerm("f",1,2);
    Exp* paramExp = new Exp();  paramExp->addTerm( new VarTerm("x") );
    ufcall->setParamExp(0,paramExp);
    have->addTerm( ufcall );
    EXPECT_EQ("f(x)[2]", have->toString());
    EXPECT_TRUE(have->hasIndexedUFCall());
    UFCallTerm* ufcallclone = have->cloneIndexedUFCallTerm();
    EXPECT_EQ( *ufcallclone, *ufcall );
    delete have;
    delete ufcallclone;
    
    // x + 2
    Exp* have_not = new Exp();
    have_not->addTerm( new VarTerm("x") );
    have_not->addTerm( new Term(2) );
    EXPECT_EQ("x + 2", have_not->toString());
    EXPECT_FALSE(have_not->hasIndexedUFCall());
    delete have_not;    

}

/******************************************/
