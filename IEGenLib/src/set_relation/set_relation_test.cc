/*!
 * \file set_relation_test.cc
 *
 * \brief Set and Relation tests.
 *
 * This file is to test all of the Set and Relation classes, along with the
 * related Conjunction class.
 *
 * \date Started: 3/28/12
 *
 * \authors Michelle Strout, Joseph Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2012, 2013 Colorado State University <br>
 * Copyright (c) 2015-2016, University of Arizona <br>
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

// This is just a test setup class that is used in the other tests in this
// file.  Sets up some expressions for use in many of the tests.
class SetRelationTest : public::testing::Test {
    public:
    Exp e1;
    Exp e2;
    Exp x_minus_y;
    Exp x;
    Exp negx_plus_y;
    Conjunction *conj1;
    Conjunction *conj2;
    Conjunction *conj1r;
    Conjunction *conj2r;
    std::string root_dir;
    std::string dot_data;

    protected:
    // copied most of these expressions from expression_test.cc
    virtual void SetUp();

    virtual void TearDown();

};

void SetRelationTest::SetUp() {

    char* temp = getenv ("IEGEN_HOME");
    if (temp == NULL) {
        throw iegenlib::assert_exception("Please set the IEGEN_HOME "
            "environment variable to the distribution root directory "
            "(see README for more details).\n");
    } else {
        root_dir = temp;
    }
    dot_data = root_dir + "/test_data/dotTest/";

    // 5 N + 12 x + 7 y + 42
    e1.addTerm(new Term(42));
    e1.addTerm(new VarTerm(5, "N"));
    e1.addTerm(new VarTerm(7, "y"));
    e1.addTerm(new VarTerm(12, "x"));

    // 5 N + 12 x + 7 y + 5 N + g(z) + 42
    e2.addTerm(new Term(42));
    e2.addTerm(new VarTerm(5, "N"));
    e2.addTerm(new VarTerm(7, "y"));
    e2.addTerm(new VarTerm(12, "x"));
    UFCallTerm* uf_call = new UFCallTerm("g", 1);
    Exp *g0 = new Exp();
    g0->addTerm(new VarTerm("z"));
    uf_call->setParamExp(0,g0);
    e2.addTerm(uf_call);

    // x - y
    x_minus_y.addTerm(new VarTerm("x"));
    x_minus_y.addTerm(new VarTerm(-1,"y"));

    // x
    x.addTerm(new VarTerm("x"));

    // -x + y
    negx_plus_y.addTerm(new VarTerm(-1,"x"));
    negx_plus_y.addTerm(new VarTerm("y"));

    // conjunction: {[x,3,1,y] : x = 0 && -x + y = 0 }
    conj1 = new Conjunction(4);
    TupleDecl tdecl1(4);
    tdecl1.setTupleElem(0,"x");
    tdecl1.setTupleElem(1,3);
    tdecl1.setTupleElem(2,1);
    tdecl1.setTupleElem(3,"y");
    conj1->setTupleDecl(tdecl1);
    conj1->addEquality(x.clone());
    conj1->addEquality(negx_plus_y.clone());

    // conjunction: {[x,3,1] -> [y] : x = 0 && -x + y = 0 }
    conj1r = new Conjunction(4,3);
    conj1r->setTupleDecl(tdecl1);
    conj1r->addEquality(x.clone());
    conj1r->addEquality(negx_plus_y.clone());

    // conjunction: {[x,y,2,0] : x = 0 && x - y = 0 }
    conj2 = new Conjunction(4);
    TupleDecl tdecl2(4);
    tdecl2.setTupleElem(0,"x");
    tdecl2.setTupleElem(1,"y");
    tdecl2.setTupleElem(2,2);
    tdecl2.setTupleElem(3,0);
    conj2->setTupleDecl(tdecl2);
    conj2->addInequality(x.clone());
    conj2->addEquality(x_minus_y.clone());

    // conjunction: {[x,y,2] -> [0] : x >= 0 && x - y = 0 }
    conj2r = new Conjunction(4,3);
    conj2r->setTupleDecl(tdecl2);
    conj2r->addInequality(x.clone());
    conj2r->addEquality(x_minus_y.clone());

}

void SetRelationTest::TearDown() {
  delete conj1;
  delete conj2;
  delete conj1r;
  delete conj2r;
}

// Ensure that all of the expressions created in
// the SetUp method are what is expected.
TEST_F(SetRelationTest, SetUpTest) {
    EXPECT_EQ("5 N + 12 x + 7 y + 42", e1.toString());
    EXPECT_EQ("5 N + 12 x + 7 y + g(z) + 42", e2.toString());
    EXPECT_EQ("x - y", x_minus_y.toString());
    EXPECT_EQ("x", x.toString());
    EXPECT_EQ("-x + y", negx_plus_y.toString());
    EXPECT_EQ("{ [x, 3, 1, y] : x = 0 && x - y = 0 }", conj1->toString());
    EXPECT_EQ("{ [x, y, 2, 0] : x - y = 0 && x >= 0 }", conj2->toString());
}

// Test the construction of a Conjunction.
#pragma mark SetNoTupleVarDecl
TEST_F(SetRelationTest, SetNoTupleVarDecl) {
    Set* s = new Set(3);
    
    EXPECT_EQ("{ [tv0, tv1, tv2] }", s->toString());
    delete s;
}

// Test the construction of a Conjunction.
#pragma mark ConjunctionConstruction
TEST_F(SetRelationTest, ConjunctionConstruction) {

    // Test construction and conversion to string
    Conjunction c1(4);
    TupleDecl tdecl1(4);
    tdecl1.setTupleElem(0,"x");
    tdecl1.setTupleElem(1,3);
    tdecl1.setTupleElem(2,1);
    tdecl1.setTupleElem(3,"y");
    c1.setTupleDecl(tdecl1);
    EXPECT_EQ("{ [x, 3, 1, y] }", c1.toString());

    // Test construction with equalities.
    c1.addEquality(x.clone());
    c1.addEquality(negx_plus_y.clone());
    EXPECT_EQ("{ [x, 3, 1, y] : x = 0 && x - y = 0 }", c1.toString());

    // Test construction with inequalities.
    c1.addInequality(e1.clone());
    EXPECT_EQ("{ [x, 3, 1, y] : x = 0 && x - y = 0 "
              "&& 5 N + 12 x + 7 y + 42 >= 0 }", c1.toString());

    // Test substituting the tuple elements in for the corresponding
    // variables in the expressions.
    c1.substituteTupleDecl();

    EXPECT_EQ("{ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 "
              "&& 12 __tv0 + 7 __tv3 + 5 N + 42 >= 0 }", c1.toString());

    // Now a similar test, but with the same variable occurring
    // twice -- in this case we substitute the first one, and
    // add a new equality constraint relating the second to the first.
    Conjunction c2(3);
    TupleDecl tdecl2(3);
    tdecl2.setTupleElem(0,"x");
    tdecl2.setTupleElem(1,"y");
    tdecl2.setTupleElem(2,"x");
    c2.setTupleDecl(tdecl2);
    EXPECT_EQ("{ [x, y, x] }", c2.toString());

    c2.addEquality(x.clone());
    c2.addEquality(negx_plus_y.clone());
    EXPECT_EQ("{ [x, y, x] : x = 0 && x - y = 0 }", c2.toString());

    // This needs to happen after all constraints based on the string
    // representation of sets/relations have been added
    // to the conjunction.  It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    c2.substituteTupleDecl();
    EXPECT_EQ("{ [x, y, x] : __tv0 = 0 && __tv0 - __tv1 = 0 "
              "&& __tv0 - __tv2 = 0 }",
            c2.toString());

}

TEST_F(SetRelationTest, ConjunctionToDot) {
    // (Same setup from test of Exp class)
    // Test construction and conversion to string
    Exp *e2 = new Exp();  // 42 __tv0 + 7
    e2->addTerm(new TupleVarTerm(42, 0));
    e2->addTerm(new Term(7));

    Exp *e3 = new Exp();  // N + 3 __tv3
    e3->addTerm(new VarTerm("N"));
    e3->addTerm(new TupleVarTerm(3, 3));

    UFCallTerm* uf_call = new UFCallTerm(5, "foo", 2);
    uf_call->setParamExp(0,e2->clone());
    uf_call->setParamExp(1,e3->clone());
    Exp *e4 = new Exp();
    e4->addTerm(uf_call);

    Conjunction *conj = new Conjunction(2);
    conj->addInequality(e4);
    conj->addEquality(e2);
    conj->addEquality(e3);

    // Check that the generated dot file for this conjunction is
    // the same as the gold standard input file.
    string testFile = dot_data + "testConjToDotStringTest1.in.dot";

    int node_id = 0;
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              conj->toDotString(node_id)));
    delete conj;

}


// Test making a SparseConstraints object.
#pragma mark SparseConstraints
TEST_F(SetRelationTest, SparseConstraints) {
    //SparseConstraints sc();
    SparseConstraints sc;
    EXPECT_EQ("{  : FALSE }", sc.toString());

    // Add a conjunction to the sparse constraints object.
    Conjunction* conj1_copy = new Conjunction(*conj1);
    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    sc.addConjunction(conj1_copy);
    EXPECT_EQ("{ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
            sc.toString());

    // Add another conjunction
    Conjunction* conj2_copy = new Conjunction(*conj2);
    conj2_copy->substituteTupleDecl();
    sc.addConjunction(conj2_copy);
    EXPECT_EQ("{ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }"
              " union { [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
              sc.toString());

}

// Test making a Set object.
#pragma mark Set
TEST_F(SetRelationTest, Set) {
    Set s(4);
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3] }", s.toString());

    // Add a conjunction to the set object.
    Conjunction* conj1_copy = new Conjunction(*conj1);
    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    s.addConjunction(conj1_copy);
    EXPECT_EQ("{ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
            s.toString());


    // Add another conjunction
    Conjunction* conj2_copy = new Conjunction(*conj2);
    conj2_copy->substituteTupleDecl();
    s.addConjunction(conj2_copy);
    EXPECT_EQ("{ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }"
              " union { [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
              s.toString());

}

// Test pretty printing a Set object
TEST_F(SetRelationTest, SetPrettyPrintString) {
    Set s(4);
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3] }", s.toString());

    // Add a conjunction to the set object.
    Conjunction* conj1_copy = new Conjunction(*conj1);
    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    s.addConjunction(conj1_copy);
    EXPECT_EQ("{ [x, 3, 1, y] : x = 0 && x - y = 0 }",
            s.prettyPrintString());


    // Add another conjunction
    Conjunction* conj2_copy = new Conjunction(*conj2);
    conj2_copy->substituteTupleDecl();
    s.addConjunction(conj2_copy);
    EXPECT_EQ("{ [x, y, 2, 0] : x - y = 0 && x >= 0 }"
              " union { [x, 3, 1, y] : x = 0 && x - y = 0 }",
              s.prettyPrintString());

}

// Test making a Relation object.
#pragma mark Relation
TEST_F(SetRelationTest, Relation) {
    Relation r( 3, 1);
    EXPECT_EQ("{ [tv0, tv1, tv2] -> [tv3] : FALSE }", r.toString());


    // Add a conjunction to the set object.
    // conjunction: {[x,3,1,y] : x = 0 && -x + y = 0 }
    Conjunction* conj1 = new Conjunction(4,3);   // inArity of 3
    TupleDecl tdecl1(4);
    tdecl1.setTupleElem(0,"x");
    tdecl1.setTupleElem(1,3);
    tdecl1.setTupleElem(2,1);
    tdecl1.setTupleElem(3,"y");
    conj1->setTupleDecl(tdecl1);
    conj1->addEquality(x.clone());
    conj1->addEquality(negx_plus_y.clone());
    Conjunction* conj1_copy = new Conjunction(*conj1);
    delete conj1;

    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    r.addConjunction(conj1_copy);
    EXPECT_EQ("{ [x, 3, 1] -> [y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
            r.toString());

    // Add another conjunction
    // conjunction: {[x,y,2,0] : x = 0 && x - y = 0 }
    Conjunction* conj2 = new Conjunction(4,3);  // inArity of 3
    TupleDecl tdecl2(4);
    tdecl2.setTupleElem(0,"x");
    tdecl2.setTupleElem(1,"y");
    tdecl2.setTupleElem(2,2);
    tdecl2.setTupleElem(3,0);
    conj2->setTupleDecl(tdecl2);
    conj2->addInequality(x.clone());
    conj2->addEquality(x_minus_y.clone());
    Conjunction* conj2_copy = new Conjunction(*conj2);
    delete conj2;
    conj2_copy->substituteTupleDecl();
    r.addConjunction(conj2_copy);
    EXPECT_EQ("{ [x, y, 2] -> [0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }"
              " union { [x, 3, 1] -> [y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",
              r.toString());

    // Test the copy ctor
    Relation r2(r);
    EXPECT_EQ(r.toString(), r2.toString());

}

// Test pretty printing a Relation object
TEST_F(SetRelationTest, RelationPrettyPrintString) {
    Relation r( 3, 1);
    EXPECT_EQ("{ [tv0, tv1, tv2] -> [tv3] : FALSE }", r.toString());

    // Add a conjunction to the set object.
    Conjunction* conj1_copy = new Conjunction(*conj1r);
    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    r.addConjunction(conj1_copy);
    EXPECT_EQ("{ [x, 3, 1] -> [y] : x = 0 && x - y = 0 }",
            r.prettyPrintString());


    // Add another conjunction
    Conjunction* conj2_copy = new Conjunction(*conj2r);
    conj2_copy->substituteTupleDecl();
    r.addConjunction(conj2_copy);
    EXPECT_EQ("{ [x, y, 2] -> [0] : x - y = 0 && x >= 0 }"
              " union { [x, 3, 1] -> [y] : x = 0 && x - y = 0 }",
              r.prettyPrintString());

    // Test the copy ctor
    Relation r2(r);
    EXPECT_EQ(r.prettyPrintString(), r2.prettyPrintString());
}

TEST_F(SetRelationTest, RelationWithUFCall) {
    Relation R_X_to_Xprime(2,2);
    Conjunction *c = new Conjunction(2+2,2);
    TupleDecl * tdecl = new TupleDecl(4);
    tdecl->setTupleElem(0,"k");
    tdecl->setTupleElem(1,"i");
    tdecl->setTupleElem(2,"k");
    tdecl->setTupleElem(3,"i'");
    c->setTupleDecl(*tdecl);
    delete tdecl;
    
    // Create the expression
    Exp* exp = new Exp();
    exp->addTerm(new VarTerm("i'"));
    UFCallTerm* uf_call = new UFCallTerm(-1, "sigma", 1);
    Exp *arg0 = new Exp();
    arg0->addTerm(new VarTerm("i"));
    uf_call->setParamExp(0,arg0);
    exp->addTerm(uf_call);

    // add the equality to the conjunction
    c->addEquality(exp);

    // add the conjunction to the relation
    R_X_to_Xprime.addConjunction(c);

    EXPECT_EQ("{ [k, i] -> [k, i'] : i' - sigma(i) = 0 }",
             R_X_to_Xprime.prettyPrintString());

}

#pragma mark SolveForFactor
TEST_F(SetRelationTest, SolveForFactor) {
    iegenlib::setCurrEnv();
    // Now set up an environment that defines an inverse for f.
    // Upon creation all expressions, conjunctions, sets, etc.
    // should use this environment until another one is constructed.
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    // Create the expression
    Exp* exp = new Exp();			// t - f(y) = 0
    exp->addTerm(new VarTerm("t"));
    UFCallTerm* uf_call = new UFCallTerm(-1, "f", 1);
    Exp *arg0 = new Exp();
    arg0->addTerm(new VarTerm("y"));
    uf_call->setParamExp(0,arg0);
    exp->addTerm(uf_call);

    Exp* result = exp->solveForFactor(new VarTerm("y"));
    EXPECT_TRUE(result);
    EXPECT_EQ("f_inv(t)", result->toString());
    delete exp;
    delete result;
}

#pragma mark SolveForFactorMultipleTerms
TEST_F(SetRelationTest, SolveForFactorMultipleTerms) {
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), false,
        iegenlib::Monotonic_NONE);

    // Create the expression t - 2 f(y) + 3 x - 7 __tv1 + 42 = 0
    Exp* exp = new Exp();
    exp->addTerm(new VarTerm("t"));         // t
    UFCallTerm* uf_call = new UFCallTerm(1, "f", 1);
    Exp *arg0 = new Exp();
    arg0->addTerm(new VarTerm("y"));
    uf_call->setParamExp(0,arg0);
    exp->addTerm(uf_call);                  // f(y)
    exp->addTerm(new VarTerm(3,"x"));       // 3 x
    exp->addTerm(new TupleVarTerm(-7,1));   // -7 __tv1
    exp->addTerm(new Term(42));             // 42

    Exp* result = exp->solveForFactor(new UFCallTerm(*uf_call));
    EXPECT_TRUE(result);
    EXPECT_EQ("7 __tv1 - t - 3 x - 42", result->toString());
    delete exp;
    delete result;
}


#pragma mark FindFunction
TEST_F(SetRelationTest, FindFunction) {
    // { [x,y] -> [s,t] : x = s && t = f(y) }
    iegenlib::setCurrEnv();
    Conjunction* conj = new Conjunction(4);
    TupleDecl * tdecl = new TupleDecl(4);
    tdecl->setTupleElem(0, "x");
    tdecl->setTupleElem(1, "y");
    tdecl->setTupleElem(2, "s");
    tdecl->setTupleElem(3, "t");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    Exp *exp = new Exp();		// x - s = 0
    exp->addTerm(new VarTerm("x"));
    exp->addTerm(new VarTerm(-1, "s"));
    conj->addEquality(exp);

    exp = new Exp();			// t - f(y) = 0
    exp->addTerm(new VarTerm("t"));
    UFCallTerm* uf_call = new UFCallTerm(-1, "f", 1);
    Exp *arg0 = new Exp();
    arg0->addTerm(new VarTerm("y"));
    uf_call->setParamExp(0,arg0);
    exp->addTerm(uf_call);
    conj->addEquality(exp);

    EXPECT_EQ("{ [x, y, s, t] : s - x = 0 && t - f(y) = 0 }", conj->toString());
    conj->substituteTupleDecl();

    // find x as a function of s and t
    exp = conj->findFunction(0, 2,3);
    EXPECT_TRUE(exp);
    EXPECT_EQ("__tv2", exp->toString());
    delete exp;

    // find s as a function of x and y
    exp = conj->findFunction(2, 0,1);
    EXPECT_TRUE(exp);
    EXPECT_EQ("__tv0", exp->toString());
    delete exp;

    // find t as a function of x and y
    exp = conj->findFunction(3, 0,1);
    EXPECT_TRUE(exp);
    EXPECT_EQ("f(__tv1)", exp->toString());
    delete exp;

    // find y as a function of s and t -- requires inverting f;
    // we haven't provided an inverse of f, so this should return null
    exp = conj->findFunction(1, 2,3);
    EXPECT_FALSE(exp);

    // Now set up an environment that defines an inverse for f.
    // Upon creation all expressions, conjunctions, sets, etc.
    // should use this environment until another one is constructed.
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    exp = conj->findFunction(1, 2,3);
    EXPECT_TRUE(exp);
    EXPECT_EQ("f_inv(__tv3)", exp->toString());

    delete exp;
    delete conj;
}


// Test that fails due to a bug in how expression is printed.
TEST_F(SetRelationTest, ExpressionSignBug){
    Conjunction* conj;
    Exp* exp;
    Relation* r1;

    // input: r1 = { [v] -> [w] : w=v+1 }
    conj = new Conjunction(2,1);
    TupleDecl * tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // w - v - 1 = 0
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    exp->addTerm(new Term(-1));
    EXPECT_EQ("-v + w - 1", exp->toString());
    conj->addEquality(exp);
    EXPECT_EQ("{ [v] -> [w] : v - w + 1 = 0 }", conj->toString());
    conj->substituteTupleDecl();
    EXPECT_EQ("{ [v] -> [w] : __tv0 - __tv1 + 1 = 0 }", conj->toString());
    r1 = new Relation( 1, 1);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w] : __tv0 - __tv1 + 1 = 0 }", r1->toString());
    delete r1;
}

// Test that we don't display 0 = 0 equality with toString method
// Example of some normalization we are already doing.
TEST_F(SetRelationTest, EmptyExpressionBug){    
    std::string expStr = "{ [] : 0 = 0 && 0 = 0 }";
    Set *s = new Set(expStr);
    EXPECT_EQ("{  }", s->toString());
    delete s;

    {
        Set *s = new Set("{[a] : 0=0}");
        EXPECT_EQ("{ [a] }", s->toString());
        delete s;
    }
}


TEST_F(SetRelationTest, SRToDot){

    SparseConstraints sc;
    EXPECT_EQ("", sc.toDotString());

    Set ss(4);
    Conjunction* conj1_copy = new Conjunction(*conj1);
    conj1_copy->substituteTupleDecl();
    ss.addConjunction(conj1_copy);

    // Check that the generated dot file for this conjunction is
    // the same as the gold standard input file.
    string testFile = dot_data + "testSetToDotStringTest1.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              ss.toDotString()));

    // Test Relation
    Relation rr( 3, 1);
    Conjunction* conj1_copy2 = new Conjunction(*conj1r);
    conj1_copy2->substituteTupleDecl();
    rr.addConjunction(conj1_copy2);


    // Check that the generated dot file for this conjunction is
    // the same as the gold standard input file.
    testFile = dot_data + "testRelToDotStringTest1.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              rr.toDotString()));

    // Test Set
    Exp e1;
    Exp e2;

    UFCallTerm* uf_call_g = new UFCallTerm(7, "g", 1);
    Exp *g0 = new Exp();
    g0->addTerm(new TupleVarTerm(3, 0));
    uf_call_g->setParamExp(0,g0);

    e1.addTerm(new TupleVarTerm(16, 3));
    e1.addTerm(new VarTerm(10, "M"));
    e1.addTerm(new VarTerm(-2, "N"));
    UFCallTerm* uf_call_foo = new UFCallTerm(3, "foo", 2);
    Exp *arg1 = new Exp();
    arg1->addTerm(new VarTerm(5, "N"));
    uf_call_foo->setParamExp(0,arg1);
    Exp *arg2 = new Exp();
    arg2->addTerm(uf_call_g);
    uf_call_foo->setParamExp(1,arg2);
    e1.addTerm(uf_call_foo);
    e1.addTerm(new Term(8));

    uf_call_g = new UFCallTerm(7, "g", 1);
    Exp *g1 = new Exp();
    g1->addTerm(new TupleVarTerm(3, 0));
    uf_call_g->setParamExp(0,g1);

    e2.addTerm(new TupleVarTerm(16, 3));
    e2.addTerm(new VarTerm(10, "Q"));
    e2.addTerm(new VarTerm(-2, "Q"));
    uf_call_foo = new UFCallTerm(3, "foo", 2);
    Exp *arg3 = new Exp();
    arg3->addTerm(new VarTerm(5, "Q"));
    uf_call_foo->setParamExp(0,arg3);
    Exp *arg4 = new Exp();
    arg4->addTerm(uf_call_g);
    uf_call_foo->setParamExp(1,arg4);
    e2.addTerm(uf_call_foo);
    e2.addTerm(new Term(8));

    Set s(2);
    Conjunction *c1 = new Conjunction(2);
    Conjunction *c2 = new Conjunction(2);
    c1->addInequality(e1.clone());
    TupleDecl * tdecl1 = new TupleDecl(2);
    tdecl1->setTupleElem(0,"M");
    tdecl1->setTupleElem(1,"N");
    c1->setTupleDecl(*tdecl1);
    delete tdecl1;
    c2->addEquality(e2.clone());
    TupleDecl * tdecl2 = new TupleDecl(2);
    tdecl2->setTupleElem(0,1);
    tdecl2->setTupleElem(1,"Q");
    c2->setTupleDecl(*tdecl2);
    delete tdecl2;
    s.addConjunction(c2);
    s.addConjunction(c1);

    // Check that the generated dot file for this conjunction is
    // the same as the gold standard input file.
    testFile = dot_data + "testSetToDotStringTest2.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              s.toDotString()));

    // Test Relation
    Relation r( 1, 1);
    c1 = new Conjunction(2,1);
    c2 = new Conjunction(2,1);
    c1->addInequality(e1.clone());
    TupleDecl * tdecl1r = new TupleDecl(2);
    tdecl1r->setTupleElem(0,"M");
    tdecl1r->setTupleElem(1,"N");
    c1->setTupleDecl(*tdecl1r);
    delete tdecl1r;
    c2->addEquality(e2.clone());
    TupleDecl * tdecl2r = new TupleDecl(2);
    tdecl2r->setTupleElem(0,1);
    tdecl2r->setTupleElem(1,"Q");
    c2->setTupleDecl(*tdecl2r);
    delete tdecl2r;
    r.addConjunction(new Conjunction(*c2));
    r.addConjunction(new Conjunction(*c1));
    delete c1;
    delete c2;

    // Check that the generated dot file for this conjunction is
    // the same as the gold standard input file.
    testFile = dot_data + "testRelToDotStringTest2.in.dot";
    EXPECT_EQ(true, iegenlib::compareFileContentsToString(testFile,
              r.toDotString()));
}


// Test composition of relations.
// Both relations are functions.
TEST_F(SetRelationTest, CompositionIdentity) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;

    // input: r1 = { [v] -> [w] : w=v }
    conj = new Conjunction(2,1);
    TupleDecl * tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // w = v
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r1 = new Relation(1, 1);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w] : __tv0 - __tv1 = 0 }", r1->toString());

    // input: r2 = { [x] -> [y] : y=x }
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // y = x
    exp->addTerm(new VarTerm("y"));
    exp->addTerm(new VarTerm(-1,"x"));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation(1, 1);
    r2->addConjunction(conj);
    EXPECT_EQ("{ [x] -> [y] : __tv0 - __tv1 = 0 }", r2->toString());

    // output: r1 compose r2 = { [x] -> [w] : __tv0 - __tv1 = 0 }
    Relation *composition = r1->Compose(r2);
    EXPECT_TRUE(composition != NULL);
    EXPECT_EQ("{ [x] -> [w] : __tv0 - __tv1 = 0 }", composition->toString());

    // cleanup
    delete r1;
    delete r2;
    delete composition;
}

// Test composition of relations.
// Both relations are functions.
// Want to break initial version that only works for identities.
// Did, but then we extended Composition.
TEST_F(SetRelationTest, CompositionBreakIdentityOnly) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;
    TupleDecl* tdecl;

    // input: r1 = { [v] -> [w] : w=v+2 }
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // w - v - 2 = 0
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    exp->addTerm(new Term(-2));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r1 = new Relation( 1, 1);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w] : __tv0 - __tv1 + 2 = 0 }", r1->toString());

    // input: r2 = { [x] -> [y] : y = x + 7 }
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // y - x - 7 = 0
    exp->addTerm(new VarTerm("y"));
    exp->addTerm(new VarTerm(-1,"x"));
    exp->addTerm(new Term(-7));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation( 1, 1);
    r2->addConjunction(conj);
    EXPECT_EQ("{ [x] -> [y] : __tv0 - __tv1 + 7 = 0 }", r2->toString());

    // output: r1 compose r2 = { [x] -> [w] : w = x + 9 }
    Relation *composition = r1->Compose(r2);
    EXPECT_TRUE(composition != NULL);
    EXPECT_EQ("{ [x] -> [w] : __tv0 - __tv1 + 9 = 0 }",
        composition->toString());

    // cleanup
    delete r1;
    delete r2;
    delete composition;
}

// Test composition of relations.
// Both relations are function inverses.
TEST_F(SetRelationTest, CompositionInverses) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;
    TupleDecl* tdecl;

    // input: r1 = { [v] -> [w] : v=3w+2 }
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // -3w + v - 2 = 0
    exp->addTerm(new VarTerm(-3,"w"));
    exp->addTerm(new VarTerm("v"));
    exp->addTerm(new Term(-2));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r1 = new Relation( 1, 1);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w] : __tv0 - 3 __tv1 - 2 = 0 }", r1->toString());

    // input: r2 = { [x] -> [y] : x = 2y + 7 }
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // x - 2y - 7 = 0
    exp->addTerm(new VarTerm(-2,"y"));
    exp->addTerm(new VarTerm("x"));
    exp->addTerm(new Term(-7));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation( 1, 1);
    r2->addConjunction(conj);
    EXPECT_EQ("{ [x] -> [y] : __tv0 - 2 __tv1 - 7 = 0 }", r2->toString());

    // y=v, v=3w+2, x=2y+7 ==> x=6w+11
    // output: r1 compose r2 = { [x] -> [w] : x = 6 w + 11 }
    Relation *composition = r1->Compose(r2);
    EXPECT_TRUE(composition != NULL);
    EXPECT_EQ("{ [x] -> [w] : __tv0 - 6 __tv1 - 11 = 0 }",
        composition->toString());

    // cleanup
    delete r1;
    delete r2;
    delete composition;
}

// Test composition of relations.
// Both relations are functions.
#pragma mark CompositionVariousArities
TEST_F(SetRelationTest, CompositionVariousArities) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;
    TupleDecl* tdecl;

    // input: r1 = { [v] -> [w, t] : w=v+2 && t=v }
    conj = new Conjunction(3,1);
    tdecl = new TupleDecl(3);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    tdecl->setTupleElem(2,"t");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // w - v - 2 = 0
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    exp->addTerm(new Term(-2));
    conj->addEquality(exp);
    exp = new Exp();    // t - v = 0
    exp->addTerm(new VarTerm("t"));
    exp->addTerm(new VarTerm(-1,"v"));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r1 = new Relation( 1, 2);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w, t] : __tv0 - __tv2 = 0 "
              "&& __tv0 - __tv1 + 2 = 0 }", r1->toString());

    // input: r2 = { [x,z,r] -> [y] : y = x + z - r - 7 }
    conj = new Conjunction(4,3);
    tdecl = new TupleDecl(4);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"z");
    tdecl->setTupleElem(2,"r");
    tdecl->setTupleElem(3,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // y - x - z + r + 7 = 0
    exp->addTerm(new VarTerm("y"));
    exp->addTerm(new VarTerm(-1,"x"));
    exp->addTerm(new VarTerm(-1,"z"));
    exp->addTerm(new VarTerm("r"));
    exp->addTerm(new Term(7));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation( 3, 1);
    r2->addConjunction(conj);
    EXPECT_EQ("{ [x, z, r] -> [y] : __tv0 + __tv1 - __tv2 - __tv3 - 7 = 0 }",
        r2->toString());

    // output: r1 compose r2 = { [x,z,r] -> [w,t] : w = x+z-r-5 && t = x+z-r-7  }
    Relation *composition = r1->Compose(r2);
    EXPECT_TRUE(composition != NULL);
    EXPECT_EQ("{ [x, z, r] -> [w, t] : __tv0 + __tv1 - __tv2 - __tv3 - 5 = 0 "
              "&& __tv0 + __tv1 - __tv2 - __tv4 - 7 = 0 }",
              composition->toString());

    // cleanup
    delete r1;
    delete r2;
    delete composition;
}

// Test composition of relations.
// Both relations are functions.
TEST_F(SetRelationTest, ComposeNeedsRenaming) {

    Relation* r1 = new Relation("{ [v,s] -> [w, t] : w=v+2 && t=s }");
    Relation* r2
        = new Relation("{ [x,z,r] -> [y,i] : y = x + z - r - 7 && i = r+2 }");

    Relation* result = r1->Compose(r2);
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=x+z-r-5 && t=r+2 }");
    EXPECT_EQ(expected->toString(), result->toString());

    delete r1;
    delete r2;
    delete result;
    delete expected;
}

// Test composition of relations with uninterpreted function calls.
// Both relations are functions.
#pragma mark ComposeWithUFC
TEST_F(SetRelationTest, ComposeWithUFC) {
    Relation* r1 = new Relation("{ [v,s] -> [w, t] : w=v+2 && t=f(s) }");
    Relation* r2
        = new Relation("{ [x,z,r] -> [y,i] : y = x + z - r - 7 && i = r+2 }");

    Relation* result = r1->Compose(r2);
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=x+z-r-5 && t=f(r+2) }");
    EXPECT_EQ(expected->toString(), result->toString());

    delete r1;
    delete r2;
    delete result;
    delete expected;
}

// Test composition of relations with uninterpreted function calls.
// Both relations are functions.
#pragma mark ComposeWithUFCNeedsInverse
TEST_F(SetRelationTest, ComposeWithUFCNeedsInverse) {
    iegenlib::setCurrEnv();
    // Now set up an environment that defines an inverse for f.
    // Upon creation all expressions, conjunctions, sets, etc.
    // should use this environment until another one is constructed.
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    Relation* r1 = new Relation("{ [v,s] -> [w, t] : w=g(v+42) && s = f(t) }");
    Relation* r2
        = new Relation("{ [x,z,r] -> [y,i] : y = x + z - r - 7 && i = r+2 }");

    // Do the compose
    Relation* result = r1->Compose(r2);
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=g(x+z-r+35) && t=f_inv(r+2) }");
    // FIXME Barbara: the below comparison should match but doesn't.  This is
    // because the results are equivalent but have not been normalized.
    // We should talk about the normalization needed here.  Comes after you do
    // your substitution of UF calls back in.  Want to solve for w and t
    // if possible and if not solve for x, z, and r.  Tricky, what to remove.
    //EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    expected = new Relation("{ [x,z,r] -> [w,t]: w=g(x+z-r+35) && r+2=f(t) }");
    EXPECT_EQ(expected->toString(), result->toString());
    
    delete r1;
    delete r2;
    delete result;
    delete expected;

}

// Test composition of relations.
// One relation is a function and the other relation is a function inverse.
TEST_F(SetRelationTest, CompositionCombo) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;
    TupleDecl* tdecl;

    // input: r1 = { [v] -> [w] : v=3w+2 } is a function inverse
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // -3w + v - 2 = 0
    exp->addTerm(new VarTerm(-3,"w"));
    exp->addTerm(new VarTerm("v"));
    exp->addTerm(new Term(-2));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r1 = new Relation( 1, 1);
    r1->addConjunction(conj);
    EXPECT_EQ("{ [v] -> [w] : __tv0 - 3 __tv1 - 2 = 0 }", r1->toString());

    // input: r2 = { [x] -> [y] : y = 2 x + 7 } is a function
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // y - 2 x - 7 = 0
    exp->addTerm(new VarTerm("y"));
    exp->addTerm(new VarTerm(-2,"x"));
    exp->addTerm(new Term(-7));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation(1, 1);
    r2->addConjunction(conj);
    EXPECT_EQ("{ [x] -> [y] : 2 __tv0 - __tv1 + 7 = 0 }", r2->toString());

    // input: r1 = { [v] -> [w] : v=3 w+2 } is a function inverse
    // input: r2 = { [x] -> [y] : y = 2 x + 7 } is a function
    // v=y, 2x+7=3w+2, FIXME: we don't have closure.  What does this mean?
    // output: r1 compose r2 = { [x] -> [w] : 2x = 3w - 5 }
    //Relation *composition = r1->Compose(r2);
    //EXPECT_TRUE(composition != NULL);
    // FIXME: should actually implement this
    //EXPECT_EQ("{ [x] -> [w] : 2 __tv0 - 3 __tv1 + 5 = 0 }",
    //    composition->toString());

    // input: r2 = { [x] -> [y] : y = 2 x + 7 } is a function
    // input: r1 = { [v] -> [w] : v=3 w+2 } is a function inverse
    // FIXME: we can't do this one because neither function has an inverse
    // if they did, then they wouldn't fall to this case.  this case is only
    // relevant when we have UFcalls.  This is because our solve can solve
    // either way.  Could this simplify our algorithm?
    // output: r2 compose r1 = { [x] -> [w] : 2x = 3w - 5 }
    //Relation *composition = r2->Compose(r1);
    //EXPECT_TRUE(composition != NULL);
    // FIXME: this should throw an exception
    //EXPECT_EQ("{ [x] -> [w] : 2 __tv0 - 3 __tv1 + 5 = 0 }",
    //    composition->toString());

    // cleanup
    delete r1;
    delete r2;
}

TEST_F(SetRelationTest, CompositionOneFunctionWithConstraints) {
    Relation* r1 = new Relation("{ [v,s] -> [w, s] : w=v+2  }");
    Relation* r2
        = new Relation("{[x,z] -> [x,z]}");

    Relation* result = r1->Compose(r2);
    Relation* expected
        = new Relation("{ [x,z] -> [w,s]: w=x+2 && z = s }");
    EXPECT_EQ(expected->toString(), result->toString());

    delete r1;
    delete r2;
    delete result;
    delete expected;
   
    r1 = new Relation("{ [v,s] -> [v,s]}");
    r2 = new Relation("{[x,z] -> [y,i] : i=z+2 && y=x-2}");

    result = r1->Compose(r2);
    expected = new Relation("{ [x,z] -> [v,s]: v=x-2 && s=z+2 }");
    EXPECT_EQ(expected->toString(), result->toString());

    delete r1;
    delete r2;
    delete result;
    delete expected;   
    
}

// Test composition of relations.
// Both relations are functions and both have some other constraints.
TEST_F(SetRelationTest, CompositionBothFunctionsWithConstraints) {

    Conjunction* conj;
    Exp* exp;
    Relation* r1;
    Relation* r2;
    TupleDecl* tdecl;

    // input: r1 = { [v] -> [w] : w=v && w>=0 && w-v>3}
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"v");
    tdecl->setTupleElem(1,"w");
    conj->setTupleDecl(*tdecl);
    delete tdecl;

    exp = new Exp();    // w = v
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    conj->addEquality(exp);

    exp = new Exp();    // w >= 0
    exp->addTerm(new VarTerm("w"));
    conj->addInequality(exp);

    exp = new Exp();    // w-v>3
    exp->addTerm(new VarTerm("w"));
    exp->addTerm(new VarTerm(-1,"v"));
    exp->addTerm(new Term(-4));
    conj->addInequality(exp);

    conj->substituteTupleDecl();
    r1 = new Relation(1, 1);
    r1->addConjunction(conj);

    // input: r2 = { [x] -> [y] : y=x && x>=7 && y<N}
    conj = new Conjunction(2,1);
    tdecl = new TupleDecl(2);
    tdecl->setTupleElem(0,"x");
    tdecl->setTupleElem(1,"y");
    conj->setTupleDecl(*tdecl);
    delete tdecl;
    exp = new Exp();    // y = x
    exp->addTerm(new VarTerm("y"));
    exp->addTerm(new VarTerm(-1,"x"));
    conj->addEquality(exp);
    conj->substituteTupleDecl();
    r2 = new Relation( 1, 1);
    r2->addConjunction(conj);

    // output: r1 compose r2 = { [x] -> [w] : __tv0 - __tv1 = 0 }

    // cleanup
    delete r1;
    delete r2;
}


TEST_F(SetRelationTest, ParseTest1) {
    Conjunction* conj = new Conjunction(0);
    std::list<Conjunction*>* conlist = new std::list<Conjunction*>();
    conlist->push_back(conj);
    std::list<Conjunction*>::iterator it=conlist->begin();
    std::list<Conjunction*>::iterator end=conlist->end();
    Set* set = new Set((*it)->arity());
    for (; it!=end; ++it) {
       set->addConjunction(*it);
    }

    EXPECT_EQ("{  }", set->toString());
    delete set;
    delete conlist;
}

#pragma mark ComposeWithConstants
TEST_F(SetRelationTest, ComposeWithConstants) {
    Relation *r1 = new Relation("{[tstep,i]->[accessRelation1]: "
                                "accessRelation1=i}");
    Relation *r2 = new Relation("{[0,tstep0,1,i0,0]->[tstep0,i0]}");
    Relation *result = r1->Compose(r2);
    EXPECT_TRUE(result);
    // FIXME Barbara: if we put the below two in as expected sets,
    // then they should end up equivalent.  Normalization needs
    // to push constants into tuple declaration.
    EXPECT_EQ("{ [0, tstep0, 1, i0, 0] -> [accessRelation1] : "
            "__tv3 - __tv5 = 0 }",
           result->toString());
    //EXPECT_EQ("{ [c_0, tstep0, c_1, i0, c_0] -> [accessRelation1] : "
    //    "__tv0 = 0 && __tv4 = 0 && __tv2 - 1 = 0 && __tv3 - __tv5 = 0 }",
    //        result->toString());
    delete result;
    delete r2;
    delete r1;

    r1 = new Relation("{[sigma_in]->[sigma_out]: sigma_out=sigma(sigma_in)}");
    r2 = new Relation("{[0,tstep0,2,ii0,0]->[accessRelation32]: "
                      "accessRelation32=inter2(ii0)}");
    result = r1->Compose(r2);
    EXPECT_TRUE(result);
    // FIXME Barbara: if we put the below two in as expected sets,
    // then they should end up equivalent.  Normalization needs
    // to push constants into tuple declaration.
    EXPECT_EQ("{ [0, tstep0, 2, ii0, 0] -> [sigma_out] : "
            "__tv5 - sigma(inter2(__tv3)) = 0 }",
            result->toString());
//    EXPECT_EQ("{ [c_0, tstep0, c_2, ii0, c_0] -> [sigma_out] : __tv0 = 0 "
//        "&& __tv4 = 0 && __tv2 - 2 = 0 && __tv5 - sigma(inter2(__tv3)) = 0 }",
//            result->toString());
    delete result;
    delete r2;
    delete r1;
}

#pragma mark CheckDupTupleVars
TEST_F(SetRelationTest, CheckDupTupleVars) {
    Relation *r = new Relation("{[0,i,0]->[0,b,0,r,2,i,0] : "
                            "r-row(i)=0 and b-cb(i)=0}");
    EXPECT_EQ("{ [0, i, 0] -> [0, b, 0, r, 2, i, 0] : "
        "__tv1 - __tv8 = 0 "
        "&& __tv4 - cb(__tv1) = 0 "
        "&& __tv6 - row(__tv1) = 0 "
        "}", r->toString());

    const Conjunction *conj = *(r->conjunctionBegin());
    Exp *func = conj->findFunction(1, 3, 9);
    EXPECT_TRUE(func);
    EXPECT_EQ("__tv8", func->toString());
    delete func;
    delete r;

//    Relation *s = new Relation("{ [time, tri]->[0, time, 1, tri, 0] }");
//    EXPECT_EQ(s->prettyPrintString(), "{ [time, tri]->[0, time, 1, tri, 0] }");
//    delete s;
}

#pragma mark FindConstantFunctions
TEST_F(SetRelationTest, FindConstantFunctions) {
    Relation *r = new Relation("{[0,i,0]->[0,b,0,r,2,i,0] : "
                            "r-row(i)=0 and b-cb(i)=0}");

    // pointer to first conjunction
    const Conjunction *conj = *(r->conjunctionBegin());

    // check that we can find constant functions
    Exp *func = conj->findFunction(0, 3, 9);
    EXPECT_TRUE(func);
    EXPECT_EQ("0", func->toString());
    delete func;

    func = conj->findFunction(7, 0, 2);
    EXPECT_TRUE(func);
    EXPECT_EQ("2", func->toString());
    delete func;

    delete r;
}

#pragma mark ApplySpMV
// Copied some of the test cases from the old set_relation_test_spmv.cc file.
TEST_F(SetRelationTest, ApplySpMV) {

    Set *s = new Set("{[i] : nnz-i>0 and i>=0}");
    Relation *r = new Relation("{[i]->[0,i,0] }");
    Set *expected = new Set("{[0,i,0] : nnz-i>0 and i>=0}");
    Set *result = r->Apply(s);
    EXPECT_TRUE(result);
    //EXPECT_EQ("{ [c_0, i, c_0] : __tv0 = 0 && __tv2 = 0 && __tv1 >= 0 "
    //          "&& -__tv1 + nnz - 1 >= 0 }", result->toString() );
    // FIXME Barbara: below should work once normalize is going.  
    //       Above should break. Should expect 0,i,0 in result, so just modify
    //       above string and uncomment the below.
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r;
    delete s;

    s = new Set("{[0,i0,0] : nnz-i0>0 and i0>=0}");
    r = new Relation("{[0,i,0]->[0,b,0,r,2,i,0] : r-row(i)=0 and b-cb(i)=0}");
    expected = new Set("{[0,b,0,r,2,i,0] : r-row(i)=0 and b-cb(i)=0 "
                       "and nnz-i>0 and i>=0}");
    result = r->Apply(s);
    EXPECT_TRUE(result);
    //EXPECT_EQ("{ [c_0, b, c_0, r, c_2, i, c_0] : c_0 = 0 && c_0 = 0 "
    //            "&& c_0 = 0 && b - cb(i) = 0 && r - row(i) = 0 "
    //            "&& c_2 - 2 = 0 && i >= 0 && -i + nnz - 1 >= 0 }",
    //            result->prettyPrintString() );
    // FIXME Barbara: below should work once normalize is going.  
    //       Above should break. Should expect 0,i,0 in result, so just modify
    //       above string and uncomment the below.
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r;
    delete s;


    s = new Set("{[0,b,0,r,0,i,0] : r-row(i)=0 and b-cb(i)=0 and nnz-i>0 "
                "and i>=0}");
    r = new Relation("{[0,b,0,r,0,i,0]->[b,r,i] }");
    expected = new Set("{[b,r,i] : r-row(i)=0 and b-cb(i)=0 and "
                       "nnz-i>0 and i>=0}");
    result = r->Apply(s);
    EXPECT_TRUE(result);
    EXPECT_EQ("{ [b, r, i] : __tv0 - cb(__tv2) = 0 && __tv1 - row(__tv2) = 0 "
                "&& __tv2 >= 0 && -__tv2 + nnz - 1 >= 0 }",
                result->toString() );
    // FIXME Barbara: below should work once normalize is going.  
    //       Above should break. Should expect 0,i,0 in result, so just modify
    //       above string and uncomment the below.
    //EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r;
    delete s;
}


#pragma mark Union
// Test Union of relations
TEST_F(SetRelationTest, Union) {
    Set *s1 = new Set("{[a,1]: a = a + 2}");
    Set *s2 = new Set("{[b,1]: b = 3}");
    Set *resultSet = s1->Union(s2);
    EXPECT_EQ("{ [b, 1] : __tv0 - 3 = 0 }",
              resultSet->toString());
    delete resultSet;
    delete s1;
    delete s2;

    Set *s3 = new Set("{[a,b,c,d]: b = d}");
    Set *s4 = new Set("{[p,q,r,s]: p > r}");
    Set *resultSet2 = s3->Union(s4);
    EXPECT_EQ("{ [p, q, r, s] : __tv0 - __tv2 - 1 >= 0 }"
              " union { [a, b, c, d] : __tv1 - __tv3 = 0 }",
              resultSet2->toString());
    delete resultSet2;
    delete s3;
    delete s4;

    Set *s5 = new Set("{[1,a,3,b,4,c,1]: a = a + 1}");
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3, tv4, tv5, tv6] : FALSE }",
            s5->toString());
    Set *s6 = new Set("{[x,t,6,1,3,5,1]: x = x + 1}");
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3, tv4, tv5, tv6] : FALSE }",
            s6->toString());
    Set *resultSet3 = s5->Union(s6);
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3, tv4, tv5, tv6] : FALSE }",
              resultSet3->toString());
    EXPECT_EQ("{ [tv0, tv1, tv2, tv3, tv4, tv5, tv6] : FALSE }",
              resultSet3->prettyPrintString());
    delete resultSet3;
    delete s5;
    delete s6;

    Relation *r1 = new Relation("{[a,b]->[u,v]: b = v}");
    Relation *r2 = new Relation("{[c,d]->[x,y]: y < c}");
    Relation *resultRelation = r1->Union(r2);
    EXPECT_EQ("{ [c, d] -> [x, y] : __tv0 - __tv3 - 1 >= 0 }"
              " union { [a, b] -> [u, v] : __tv1 - __tv3 = 0 }",
              resultRelation->toString());
    delete resultRelation;
    delete r1;
    delete r2;

    Relation *r3 = new Relation("{[]->[u,v,b]: u = b}");
    Relation *r4 = new Relation("{[]->[u,v,a]: a = 0}");
    Relation *resultRelation2 = r3->Union(r4);
    EXPECT_EQ("{ [u, v, a] : __tv2 = 0 }"
              " union { [u, v, b] : __tv0 - __tv2 = 0 }",
              resultRelation2->toString());
    delete resultRelation2;
    delete r3;
    delete r4;

    Relation *r5 = new Relation("{[99,1,2]->[1,2,1]: 0 = 0}");
    Relation *r6 = new Relation("{[1,2,3]->[1,5,3]: 1 = 0}");
    Relation *resultRelation3 = r5->Union(r6);
    EXPECT_EQ("{ [99, 1, 2] -> [1, 2, 1] }",
              resultRelation3->toString());
    delete resultRelation3;
    delete r5;
    delete r6;
}

#pragma mark Inverse
// Test Inverse of relations
TEST_F(SetRelationTest, Inverse) {
    Relation *r1 = new Relation("{[a,x,y]->[u,v,b]: u = b}");
    Relation *resultRelation1 = r1->Inverse();
    EXPECT_EQ("{ [u, v, b] -> [a, x, y] : __tv0 - __tv2 = 0 }",
              resultRelation1->toString());
    delete resultRelation1;
    delete r1;


    Relation *r2 = new Relation("{[p,q,r,1]->[s,t]: s = t && p = 7}");
    Relation *resultRelation2 = r2->Inverse();
    EXPECT_EQ("{ [s, t] -> [p, q, r, 1] : __tv0 - __tv1 = 0 && __tv2 - 7 = 0 }",
              resultRelation2->toString());

    delete resultRelation2;
    delete r2;

    Relation *r3 = new Relation("{[1,2,3,4]->[x,y]: x = y}");
    Relation *resultRelation3 = r3->Inverse();
    EXPECT_EQ("{ [x, y] -> [1, 2, 3, 4] : __tv0 - __tv1 = 0 }",
              resultRelation3->toString());

    delete resultRelation3;
    delete r3;

    Relation *r4 = new Relation("{[3,a,1,b,4,c]->[z]: a = b}");
    Relation *resultRelation4 = r4->Inverse();
    EXPECT_EQ("{ [z] -> [3, a, 1, b, 4, c] : __tv2 - __tv4 = 0 }",
              resultRelation4->toString());

    delete resultRelation4;
    delete r4;

    Relation *r5 = new Relation("{[0,1,0]->[1,0,1]: 0 = 0}");
    Relation *resultRelation5 = r5->Inverse();
    EXPECT_EQ("{ [1, 0, 1] -> [0, 1, 0] }",
              resultRelation5->toString());

    delete resultRelation5;
    delete r5;
}

#pragma mark ComposeWithInnerConstants
TEST_F(SetRelationTest, ComposeWithInnerConstants) {
    // Constants align.
    Relation *r1 = new Relation("{[1,k]->[1,k]}");
    Relation *r2 = new Relation("{[m]->[1,w] : w = m + 7}");
    Relation *result = r1->Compose(r2);
    Relation *expected = new Relation("{ [m] -> [1, k] : k = m + 7}");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());

    delete expected;
    delete result;
    delete r2;
    delete r1;

    // Empty relation as result.
    r1 = new Relation("{[3,k]->[3,k]}");
    r2 = new Relation("{[m]->[1,w] : w = m + 7}");
    result = r1->Compose(r2);
    EXPECT_TRUE(result);
    EXPECT_EQ("{ [tv0] -> [tv1, tv2] : FALSE }", result->toString());
    delete result;
    delete r2;
    delete r1;

    // FIXME Barbara: below are some simple examples where
    // the expected could have been put in without constants in
    // tuple declarations and the result should still match

    // both functions, rhs var is constant
    r1 = new Relation("{[i,j] -> [k] : k=i}");
    r2 = new Relation("{[m]->[3,p] : p=m-7} ");
    result = r1->Compose(r2);
    expected = new Relation("{ [m] -> [3] }");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;

    // both functions, lhs var is constant
    r1 = new Relation("{[3,j] -> [k] : k=j}");
    r2 = new Relation("{[m]->[v,p] : v=m && p=m-7} ");
    result = r1->Compose(r2);
    expected = new Relation("{ [m] -> [k] : k=m-7 && m=3}");
    EXPECT_TRUE(result);
    // FIXME Barbara: just plain broken due to normalization issues
    //EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;

    // both inverse functions, rhs var is constant
    r1 = new Relation("{[i,j] -> [k] : f(k)=i && j=g(k+3)}");
    r2 = new Relation("{[m]->[3,p] : m=h(p)} ");
    result = r1->Compose(r2);
    expected = new Relation("{ [m] -> [k] : m=h(g(k+3)) && f(k)=3}");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;

    // both inverse functions, lhs var is constant
    r1 = new Relation("{[3,j] -> [k] : j=g(k+3)}");
    r2 = new Relation("{[m]->[i,p] : m=h(p+i)} ");
    result = r1->Compose(r2);
    expected = new Relation("{ [m] -> [k] : m=h(3+g(k+3))}");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;
}


#pragma mark ComposeWithUnions
TEST_F(SetRelationTest, ComposeWithUnions) {
    Relation *r1 = new Relation("{[1,k]->[1,k]} union {[3,k]->[3,k]}");
    Relation *r2 = new Relation("{[m]->[1,w] : w = m + 7}");
    Relation *result = r1->Compose(r2);
/*
    Relation *expected = new Relation("{ [m] -> [c_1, k] : c_1=1 && k=m+7}");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());
FIXME Barbara: that same constants issue
*/
    Relation *expected = new Relation("{ [m] -> [1, k] : k = m + 7}");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->toString(), result->toString());
    
    delete expected;
    delete result;
    delete r2;
    delete r1;

    r1 = new Relation("{[0,tstep,1,i,0]->[sigma_out]: sigma_out=sigma(i)}");
    r2 = new Relation("{[0,s,1,i,0]->[0,s,1,i',0]: i'=sigma_inv(i)} "
                    "union {[0,s,0,i,0]->[0,s,0,i,0]: 0=i} "
                    "union {[0,s,2,i,0]->[0,s,2,i,0]} "
                    "union {[0,s,3,i,0]->[0,s,3,i',0]: i'=sigma_inv(i)}");
    result = r1->Compose(r2);
    EXPECT_TRUE(result);
/*
    EXPECT_EQ("{ [c_0, s, c_1, i, c_0] -> [sigma_out] : __tv0 = 0 && __tv4 = 0 "
        "&& __tv2 - 1 = 0 && __tv5 - sigma(sigma_inv(__tv3)) = 0 }",
        result->toString());
 FIXME Barbara: that same constants issue
*/
    EXPECT_EQ("{ [0, s, 1, i, 0] -> [sigma_out] : "
              "__tv5 - sigma(sigma_inv(__tv3)) = 0 }",
              result->toString());

    delete result;
    delete r2;
    delete r1;
}

#pragma mark ComposeWithOtherConstraints
TEST_F(SetRelationTest, ComposeWithOtherConstraints) {

    // lhs = { v -> z : z = F1(v) && C1 }
    // ex: F1() is identity
    Relation *r1 = new Relation("{[j,k]->[i,k] : i=j && k >= N}");

    // rhs = { x -> y : y = F2(x) && C2 }
    // ex: [t,w] = F2( [m] ) = [m,m+7]
    Relation *r2 = new Relation("{[m]->[t,w] : t=m && w = m + 7 "
                                "&& w>=4 && m+4-N>=1}");

    // lhs compose rhs = { x -> z : z=F1(F2(x)) && C1[v/F2(x)] && C2[y/F2(x)] }
    // ex: C1[[j,k]/F2([m])] = m+7>=N, C2[[t,w]/F2([m])] = m+7>=4
    Relation *result = r1->Compose(r2);
    Relation *expected = new Relation("{ [m] -> [i, k] : m=i && k = m + 7 "
                                      "&& m+7>=N && m+7>=4 && m+4-N>=1 }");
    EXPECT_TRUE(result);
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;
}

TEST_F(SetRelationTest, CompositionWithNestedFunctions){
    Relation *r1 = new Relation("{[j,k,l,m]->[z]: f(z + g(z + j + m))"
                                " = f(z + g(z + k + l)) and z=f(j,k,l,m)}");
    Relation *r2 = new Relation("{[x] -> [p,q,r,s]: p=2x and q=3x"
                                " and r=4x and s=5x}");
    Relation *result = r1->Compose(r2);
    Relation *expected = new Relation("{ [x] -> [z] : z = f(2 x, 3 x, 4 x, 5 x)"
                                      "}");
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;
}

#pragma mark ComposeWithUFCallConstraints
// Mark Heim came up with this one.  It exposes some strange
// handling of expressions equal to zero.  See IEGRONE-41.
TEST_F(SetRelationTest, ComposeWithUFCallConstraints) {
    // lhs = { v -> z : z = F1(v) && C1 }
    // ex: [d] = F1([a,b,c]) = [f(a)], C1 = f(b) + c = f(c) + b
    Relation
        *r1 = new Relation("{[a,b,c]->[d]:d = f(a) and f(b) + c = f(c) + b}");

    // rhs = { x -> y : y = F2(x) && C2 }
    // ex: [x,x,x] = F2( [x] ) = [x,x,x]
    Relation *r2 = new Relation("{[x] -> [x,x,x]}");

    // lhs compose rhs = { x -> z : z=F1(F2(x)) && C1[v/F2(x)] && C2[y/F2(x)] }
    // ex: C1[[a,b,c]/F2([x])] = f(x)+x=f(x)+x
    Relation *result = r1->Compose(r2);
    Relation *expected = new Relation("{[x]->[d]:d=f(x) && f(x)+x=f(x)+x}");

    EXPECT_TRUE(result);
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;
    delete result;
    delete r2;
    delete r1;
}

TEST_F(SetRelationTest, RemoveDuplicateConstraints) {
 /* FIXME: this causes inverFuncToExposeFActor to throw an exception
    // Verify that duplicate constraints are removed
    Relation
        *r1 = new Relation("{[q,b,c,d]->[b]:f(q,b)=f(b,q) and r(d,c)=r(3c-2d,d)"
             " and f(q,q) = f(q,b) and q=b and b=q and x(q)=y(b) and x(c)=y(d)"
             " and g(k(h(c)+2b-2q+c),q+h(b)) = g(k(h(b-c+q)+c),b+h(c))}");
    Relation *r2 = new Relation("{[x,y]->[y,x,y,x]}");
    Relation *r3 = new Relation("{[a]->[a,a]}");
    Relation *result = r1->Compose(r2);
    Relation *result2 = result->Compose(r3);
    Relation *expected = new Relation("{[a]->[b]: x(a)=y(a) && a=b}");
    expected->cleanUp();
    result2->cleanUp();
    EXPECT_EQ(expected->prettyPrintString(), result2->prettyPrintString());
    EXPECT_EQ(expected->toString(), result2->toString());
    delete expected;
    delete result2;
    delete result;
    delete r3;
    delete r2;
    delete r1;

    Relation *k1 = new Relation("{[i,j,k]->[k,j,i]: i+j+k=2i+j"
                   " and 3i+1j+4k+z(1i+5j+9k+g(k-k+4j)-g(4j-h(i)+h(i)))=0"
                   " and j+4k=-3i-z(i+9k+5j) and i=2k+j and -j=2k-i"
                   " and k=k and q(j+j)=q(k+j-j+j-k+j) and k=i"
                   " and f(k+n(p(j-j))+i,j,i)=f(n(p(f(j)-f(j)))+i+k,j,i)}");
    Relation *k2 = new Relation("{ [i, j, k] -> [k, j, i] : i - k = 0 "
                                "&& 3 i + j + 4 k + z(i + 5 j + 9 k) = 0 "
                                "&& i - j - 2 k = 0}");
    EXPECT_EQ(k2->prettyPrintString(), k1->prettyPrintString());
    EXPECT_EQ(k2->toString(), k1->toString());
    delete k2;
    delete k1;
    */
}

TEST_F(SetRelationTest, RemoveManyDups) {
    // Verify that duplicate constraints are removed
    Set *s = new Set("{[a,b,c,d] : a=b && b=c && a=c && b=a && d=d && c=b}");
    Set *expected = new Set("{[a,b,c,d] : a=b && b=c && a=c}");
    s->cleanUp();
    expected->cleanUp();
    EXPECT_EQ(expected->prettyPrintString(), s->prettyPrintString());
    EXPECT_EQ(expected->toString(), s->toString());
    delete expected;
    delete s;
}


TEST_F(SetRelationTest, RemoveColon) {
    // Verify that colon is removed
    Set *q = new Set("{[x,y] : x + x = 2x}");
    Set *r = new Set("{[x,y]}");
    q->cleanUp();
    EXPECT_EQ(q->prettyPrintString(), r->prettyPrintString());
    EXPECT_EQ(q->toString(), r->toString());
    delete r;
    delete q;
}

#pragma mark UsingEnvironment
TEST_F(SetRelationTest, UsingEnvironment) {
    iegenlib::setCurrEnv();

    // Now set up an environment that defines an inverse for f.
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    // Create the relations
    Relation* r1 = new Relation("{ [v,s] -> [w, t] : w=v+2 && s = f(t) }");
    Relation* r2 = new Relation("{ [x,z,r] -> [y,i] : y = x + z - r - 7 "
                                "&& i = r+2 }");

    // Do the compose
    Relation* result = r1->Compose(r2);
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=x+z-r-5 && r+2=f(t) }");
    EXPECT_EQ(expected->toString(), result->toString());
    /*
    // FIXME Barbara: the below comparison should match but doesn't.  This is
    // because the results are equivalent but have not been normalized.
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=x+z-r-5 && t=f_inv(r+2) }");
    EXPECT_EQ(expected->toString(), result->toString());
    */

    delete expected;
    delete result;
    delete r2;
    delete r1;
};

#pragma mark LCPC12SubmissionComposeNotFunc
// Getting an exception that not both of these relations are functions.
TEST_F(SetRelationTest, LCPC12SubmissionComposeNotFunc) {

    Relation *R_X_to_Xprime
        = new Relation("{[k,i] -> [k,i'] : i' = sigma(i) }");

    EXPECT_EQ("{ [k, i] -> [k, i'] : "
        "__tv0 - __tv2 = 0 && __tv3 - sigma(__tv1) = 0 }",
        R_X_to_Xprime->toString());

    // grab the first conjunction to determine if we can find the output tuple
    // vars as a function of the input tuple vars
    const Conjunction *conj = *(R_X_to_Xprime->conjunctionBegin());
    Exp *func = conj->findFunction(2, 0,1);
    EXPECT_TRUE(func);
    EXPECT_EQ("__tv0", func->toString());
    delete func;
    func = conj->findFunction(3, 0,1);
    EXPECT_TRUE(func);
    EXPECT_EQ("sigma(__tv1)", func->toString());
    delete func;

    Relation *A1_I_to_X = new Relation("{[k,p] -> [v,i] : v=k-1 && i=col(p) }");

    Relation *result = R_X_to_Xprime->Compose(A1_I_to_X);
    EXPECT_TRUE(result);
    Relation *expected
        = new Relation("{[k,p] -> [k1,i'] : k1=k-1 && i' = sigma(col(p))}");

    // IEGRONE-65
    // EXPECT_TRUE( (*result)==(*expected) );

    // This illustrates a bug in Compose, IEGRONE-64.  Fixed.
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());

    delete expected;
    delete result;
    delete R_X_to_Xprime;
    delete A1_I_to_X;
}

#pragma mark OrderingConstraints
// Verify that the ordering of constraints is happening as expected
TEST_F(SetRelationTest, OrderingConstraints) {
    Set *s1 = new Set("{[k,i'] :  26<=sigma(i') && k<=N }");
    Set *s2 = new Set(*s1);
    EXPECT_EQ(s1->toString(), s2->toString());
    EXPECT_EQ(s1->toString(), s2->toString());
    delete s1;
    delete s2;


    s1 = new Set("{[k,i'] :  26<=sigma(i') && k<=N }");
    s2 = new Set("{[k,i'] :  k<=N && 26<=sigma(i')}");
    EXPECT_EQ(s1->toString(), s2->toString());
    delete s1;
    delete s2;
}

#pragma mark IEGRONE64Apply
// Getting an exception that not both of these relations are functions.
TEST_F(SetRelationTest, IEGRONE64Apply) {

    Relation *r = new Relation("{[k,i] -> [k,i'] : i = sigma(i') }");

    EXPECT_EQ("{ [k, i] -> [k, i'] : "
        "__tv0 - __tv2 = 0 && __tv1 - sigma(__tv3) = 0 }",
        r->toString());


    Set *s = new Set("{[k,i'] : k<=N && i'>=26 }");

    Set *result = r->Apply(s);
    EXPECT_TRUE(result);
    // set s = {[v,w] : v<=N && w>=26 }
    // r(s) = {[k,i'] : 26<=sigma(i') && k<=N }
    Set *expected = new Set("{[k,i'] :  26<=sigma(i') && k<=N }");

    // This illustrates a bug in Compose, IEGRONE-64?  Actually, the Apply
    // method does not have the fresh variable problem because the result
    // variable names only come from the output tuple variables of the relation.
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    EXPECT_EQ(expected->toString(), result->toString());

    delete expected;
    delete result;
    delete s;
    delete r;
}

#pragma mark IEGRONE41Zeros
// Example from Python bindings example in the README.
TEST_F(SetRelationTest, IEGRONE41Zeros) {

    Set *s1 = new Set("{[s,i]: 0<=s && s<T && 0<=i && i<N}");

    Relation *r1 = new Relation("{[s,i]->[0,s,1,i,0]}");

    Set *s2 = r1->Apply(s1);
    EXPECT_TRUE(s2);

    Set *expected = new Set("{ [0,s,1,i,0]: s>=0 && T-s-1 >= 0 && i>=0 && "
                            "N-i-1>=0 }");
    EXPECT_EQ(expected->prettyPrintString(), s2->prettyPrintString());
    EXPECT_EQ(expected->toString(), s2->toString());
    delete expected;

/* FIXME Barbara: same constants issue, neither of the below work and
   they both should be equivalent
    Set *expected = new Set("{ [c_0,s,c_1,i,c_0]: c_0=0 && c_1=1 && "
        "s>=0 && T-s-1 >= 0 && i>=0 && N-i-1>=0 }");
*/
//    EXPECT_EQ("{ [c_0, s, c_1, i, c_0] : c_0 = 0 && c_0 = 0 && c_1 - 1 = 0 "
//        "&& s >= 0 && i >= 0 && -s + T - 1 >= 0 && -i + N - 1 >= 0 }",
//        s2->prettyPrintString());
    // FIXME: we also don't want the above to be the pretty print of s2
    
    delete s2;
    delete s1;
    delete r1;

}

#pragma mark IEGRONE72CollapsingInverseFuncs
TEST_F(SetRelationTest, IEGRONE72CollapsingInverseFuncs) {
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("g",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("h",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("k",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("t",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    EXPECT_EQ(iegenlib::queryInverseCurrEnv("f"),"f_inv");

    // The easiest one
    Set *s1 = new Set("{[i,j]:i=f(f_inv(j))}");
    Set *expected1 = new Set("{[i,j]:i=j}");
    EXPECT_EQ(expected1->toString(), s1->toString() );
    delete s1;
    delete expected1;

    // Some tricky ones that Mark came up with
    Set *s2 = new Set("{[i,j]:f_inv(f(f(f_inv(-h(g(g_inv(g(m))))))+h(g(g(g_inv"
            "(m+g(x+2f(f_inv(y))-k(k_inv(y)))-g(x+h_inv(h(y)))))))))=0}");
    Set *expected2 = new Set("{[i,j]}");
    EXPECT_EQ(expected2->toString(), s2->toString() );
    delete s2;
    delete expected2;

    Set *s3 = new Set("{[i,j]:g(g(g_inv(f_inv(x)-f_inv(f(f_inv(x))))))=g(0)}");
    Set *expected3 = new Set("{[i,j]}");
    EXPECT_EQ(expected3->toString(), s3->toString() );
    delete s3;
    delete expected3;

    Relation *s4 = new Relation("{[i,j,k]->[k,j,i]: g(g_inv(j))="
            "g_inv(g(i+f(k)-f(j)))}");
    Relation *s4prime = new Relation("{[i]->[i,i,i]}");
    Relation *s4primeprime = s4->Compose(s4prime);
    Relation *expected4 = new Relation("{[i]->[k,j,i1]:i-k=0&&i-j=0&&i-i1=0}");
    EXPECT_EQ(expected4->prettyPrintString(),s4primeprime->prettyPrintString());
    delete s4;
    delete s4prime;
    delete s4primeprime;
    delete expected4;

/* FIXME: what do we do when what to have a function be its own inverse?
    Set *s5 = new Set("{[i]:f(f_inv(i)+f(i-i)-f(0))"
        "=g_inv(f_inv(f(g(f(f_inv(i)))-0f(0))))}");
    Relation *s5prime = new Relation("{[i]->[i]: t(t(t(i))) = i}");
    Set *s5primeprime = s5prime->Apply(s5);
    Set *expected5 = new Set("{[i]: i = t(i)}");
    EXPECT_EQ(expected5->prettyPrintString(),s5primeprime->prettyPrintString());
    delete s5;
    delete s5prime;
    delete s5primeprime;
    delete expected5;
*/

/*  FIXME (IEGRTWO-3):
    Small issue with associativity of equals for tuple variables
    Excellent example for Mark and normal form.
    Normalization example.

    Relation *s6 = new Relation("{[i]->[i,i,i]:0=t(t(i))+t(t(-t(t(i))))}");
    Relation *s6prime = s6->Inverse();
    Relation *expected6 = new Relation("{[i,i,i]->[i]}");
    EXPECT_EQ(expected6->toString(),s6prime->toString());
    delete s6;
    delete s6prime;
    delete expected6;
*/
}

#pragma mark MoldynFSTExample
TEST_F(SetRelationTest, MoldynFSTExample) {

    Relation* r = new Relation("{[ii] -> [p] : p=inter1(ii) } "
                              "union {[ii] -> [p] : p=inter2(ii) }");
    EXPECT_EQ("{ [ii] -> [p] : p - inter1(ii) = 0 } "
              "union { [ii] -> [p] : p - inter2(ii) = 0 }", r->prettyPrintString());
    delete r;
}

#pragma mark MoldynFSTExampleInverse
TEST_F(SetRelationTest, MoldynFSTExampleInverse) {

    Relation* r = new Relation("{ [0, s, 1, i, 1] -> [k] : k - inter2(i) = 0 }"
        " union { [0, s, 0, i, 0] -> [i1] : i - i1 = 0 }"
        " union { [0, s, 1, i, 0] -> [k] : k - inter1(i) = 0 }");

    Relation* expected
        = new Relation("{ [k] ->[0, s, 1, i, 1]: k - inter2(i) = 0 }"
            " union { [i1] -> [0, s, 0, i, 0] : i - i1 = 0 }"
            " union { [k] -> [0, s, 1, i, 0] : k - inter1(i) = 0 }");
    Relation* result = r->Inverse();

    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());

    delete result;
    delete r;
    delete expected;

    // Looks like to duplicate the bug I might have to do the inverse after
    // a different inverse and a compose have been performed.
    // Yes I managed to duplicate the bug!  IEGRONE-83
    Relation* a1 = new Relation("{ [s, i] -> [i] }");
    Relation* S1_sched = new Relation("{ [s, i] -> [0, s, 0, i, 0] }");

    Relation* S1_sched_inv = S1_sched->Inverse();
    EXPECT_EQ("{ [0, s, 0, i, 0] -> [s, i] : s - s = 0 && i - i = 0 }",
        S1_sched_inv->prettyPrintString() );
    // FIXME: don't like above for pretty print output
/* FIXME Barbara: same constants issue, neither of the below work but both equiv
    expected = new Relation("{ [0, s, 0, i, 0] -> [s, i] }");
    EXPECT_EQ(expected->prettyPrintString(), S1_sched_inv->prettyPrintString());
    delete expected;
    expected = new Relation("{ [c_0, s, c_0, i, c_0] -> [s, i] : c_0=0}");
    EXPECT_EQ(expected->prettyPrintString(), S1_sched_inv->prettyPrintString());
    delete expected;
*/

    Relation* a1_0 = a1->Compose(S1_sched_inv);
/*    EXPECT_EQ("{ [c_0, s, c_0, i, c_0] -> [i1] : __tv0 = 0 && __tv2 = 0 "
        "&& __tv4 = 0 && __tv3 - __tv5 = 0 }",
        a1_0->toString() );
 FIXME Barbara: same constants issue
*/
    expected = new Relation("{ [0, s, 0, i, 0] -> [i1] : i = i1 }");
    EXPECT_EQ(expected->toString(), a1_0->toString());
    delete expected;

    Relation* a1_0_inv = a1_0->Inverse();  // this did not work, now FIXED!
    expected = new Relation("{ [i1] -> [0, s, 0, i, 0] : i - i1 = 0 }");
    EXPECT_EQ(expected->prettyPrintString(), a1_0_inv->prettyPrintString());
    delete expected;
/* FIXME Barbara: same constants issue
    expected = new Relation("{ [i1] -> [c_0,s,c_0,i,c_0]: c_0=0 && i-i1=0}");
    EXPECT_EQ(expected->prettyPrintString(), a1_0_inv->prettyPrintString());
    delete expected;
    // FIXME: Yuck
    EXPECT_EQ("{ [i1] -> [c_0, s, c_0, i, c_0] : c_0 = 0 && c_0 = 0 && "
        "c_0 = 0 && i1 - i = 0 }",
        a1_0_inv->prettyPrintString());
*/
    delete a1;
    delete S1_sched;
    delete S1_sched_inv;
    delete a1_0;
    delete a1_0_inv;

}
/* FIXME
#pragma mark MoldynFSTExampleCompose
// Does not pass in revision 484.  Breaks assumption that
// both relations should be functions or function inverses.
// See August 1-6 wiki entry in Michelle Journal.
TEST_F(SetRelationTest, MoldynFSTExampleCompose) {

    Relation* r1 = new Relation("{ [0, s, 1, i, q] -> [i] }");
    Relation* r2 = new Relation("{ [k] -> [0, s, 1, i, 1] : k - inter2(i) = 0 }"
        " union { [i1] -> [0, s, 0, i, 0] : i - i1 = 0 }"
        " union { [k] -> [0, s, 1, i, 0] : k - inter1(i) = 0 }");
    std::cout << "r2 = " << r2->toDotString();

    Relation* expected
        = new Relation("{ [k] ->[i]: k - inter2(i) = 0 }"
            " union { [i1] -> [i] : i - i1 = 0 }"
            " union { [k] -> [i] : k - inter1(i) = 0 }");

    EXPECT_EQ(expected->prettyPrintString(),
              r1->Compose(r2)->prettyPrintString());

    delete r1;
    delete r2;
    delete expected;
}
*/

#pragma mark AnandComposeExample
TEST_F(SetRelationTest, AnandComposeExample) {

    Relation* r1;
    Relation* r2;
    Relation* result;
    Relation* expected;

    r1 = new Relation("{ [0, s, 0, i, 0]->[0, s, 1, e, 0] : i-left(e)=0 }");
    r2 = new Relation("{ [0, s, 0, i1, 0]->[0, s, 0, i, 0] : i1-sigma(i)=0 }");
    result = r1->Compose(r2);
    EXPECT_EQ("{ [0, s, 0, i1, 0] -> [0, s1, 1, e, 0] : __tv1 - __tv6 = 0 "
              "&& __tv3 - sigma(left(__tv8)) = 0 }",
              result->toString());
    delete result;
    delete r1;
    delete r2;

    r1 = new Relation("{ [0, s, 0, i, 0]->[0, s, 1, e, 0] : i-left(e)=0 }");
    r2 = new Relation("{ [0, s, 0, i1, 0]->[0, s, 0, i, 0] : i1-sigma(i)=0 }");
    expected = new Relation("{ [0, s, 0, i1, 0]->[0, s1, 1, e, 0] : s=s1 and "
                            "i1-sigma(left(e))=0 }");
    result =  r1->Compose(r2);
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    delete r1;
    delete r2;
    delete result;
    delete expected;

    r1 = new Relation("{ [0, s, 0, i, 0]->[0, s, 1, e, 0] : i-left(e)=0 }");
    r2 = new Relation("{ [0, s, 0, i1, 0]->[0, s, 0, i, 0] : i1-sigma(i)=0 }");
    expected = new Relation("{ [0, s, 0, i1, 0]->[0, s1, 1, e, 0] : "
                            " i1-sigma(left(e))=0 and s=s1 }");
    result =  r1->Compose(r2);
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    delete r1;
    delete r2;
    delete result;
    delete expected;

    r1 = new Relation("{ [0, s, 0, i, 0]->[0, s, 1, e, 0] : i-left(e)=0 }");
    r2 = new Relation("{ [0, s, 0, i1, 0]->[0, s, 0, i, 0] : i1-sigma(i)=0 }");
    expected = new Relation("{ [0, s, 0, i1, 0]->[0, s1, 1, e, 0] : "
                            "i1-sigma(left(e))=0 && s1=s }");
    result =  r1->Compose(r2);
    EXPECT_EQ(expected->toString(), result->toString());
    delete r1;
    delete r2;
    delete result;
    delete expected;
}


#pragma mark MoldynManyTests
// Attempting to identify if the compose issue only occurs
// when trying to create input to the reordering functions.
// Started from bottom of file
// TrunkIEGenR192PreRewrite/src/iegenlib/set_relation/set_relation_test_moldyn.cc
TEST_F(SetRelationTest, MoldynManyTests) {

    Relation* r1;
    Relation* r2;
    Relation* expected;
    Relation* result;

    // FIXME: Why projecting out variable i?
/* 8/6/12, MMS, fails
    r1 = new Relation("{[s,t,i]->[s,t]}");
    r2 = new Relation("{[s,1,t,2]->[s,t,i]}");
    expected = new Relation("{[s,1,t,2]->[s,t]}");
    EXPECT_EQ(expected->toString(),
              r1->Compose(r2)->toString());
    delete r1;
    delete r2;
    delete expected;
*/

    r1 = new Relation("{[c00,s00,c10,t0,c01,i00,x0]->[c00,s00,c10,t0,c01]}");
    r2 = new Relation("{[s0,t,i0]->[0,s0,1,t,2,i0,x0]: 0=x0}");
    expected = new Relation("{[s0,t,i0]->[0,s00,1,t0,2] : t-t0=0 && s0-s00=0}");
    result = r1->Compose(r2);
    EXPECT_EQ(expected->toString(), result->toString());
    delete r1;
    delete r2;
    delete result;
    delete expected;


/* FIXME: 8/6/12, MMS, fails because projecting out variables and constants.
    r1 = new Relation("{[c0,s,c1,t,c2,i,x]->[s,t,i]}");
    r2 = new Relation("{[sigma_out]->[0,s,1,t,2,i,x]: t=theta(2,i) and 0=x and sigma_out=i}");
    expected = new Relation("{[sigma_out]->[s,t,i]: t=theta(2,sigma_out) and sigma_out=i}");
    EXPECT_EQ(expected->toString(),
              r1->Compose(r2)->toString());
    delete r1;
    delete r2;
    delete expected;
*/

    r1 = new Relation("{[0,s0,3,c04,0]->[sigma_out]: sigma_out=c04}");
    r2 = new Relation("{[0,s0,1,t,2,i,x]->[0,s0,3,i,x]: t=theta(2,i)} union "
        "{[0,s0,1,t,1,i,x]->[0,s0,2,i,x]: t=theta(1,i)} union "
        "{[0,s0,1,t,0,i,x]->[0,s0,1,i,x]: t=theta(0,i)} union "
        "{[0,s0,0,t,0,i,x]->[0,s0,0,i,x]: 0=t}");
    expected = new Relation("{[0,s0,1,t,2,i,0]->[sigma_out]: sigma_out=i and "
                            "t=theta(2,i)}");
    result = r1->Compose(r2);
    EXPECT_EQ(expected->toString(), result->toString());
    delete r1;
    delete r2;
    delete result;
    delete expected;


    // Fixed 8/9/12, IEGRONE-85, used to cause segfault
    r1 = new Relation("{[0,s0,1,i,x]->[0,s0,1,t,0,i,x]: t=theta(0,i)} "
        "union {[0,s0,2,i,x]->[0,s0,1,t,1,i,x]: t=theta(1,i)} "
        "union {[0,s0,3,i,x]->[0,s0,1,t,2,i,x]: t=theta(2,i)} "
        "union {[0,s0,0,i,x]->[0,s0,0,t,0,i,x]: 0=t}");
    r2 = new Relation("{[tstep,i]->[0,tstep,3,i,0]}");

    // FIXME: that constraint ordering issue
    expected = new Relation("{[tstep,i]->[0,s0,1,t,2,i1,0]: "
        "tstep=s0 and i=i1 and t=theta(2,i)}");

    result = r1->Compose(r2);

    EXPECT_EQ(expected->toString(), result->toString());
    EXPECT_EQ("{ [tstep, i] -> [0, s0, 1, t, 2, i1, 0] : "
        "__tv0 - __tv3 = 0 && __tv1 - __tv7 = 0 "
        "&& __tv5 - theta(2, __tv1) = 0 }",
        result->toString());

    delete r1;
    delete r2;
    delete expected;
    delete result;

    // Fixed 8/9/12, IEGRONE-85, used to cause segfault
    r1 = new Relation("[nt]->{[0,s0,1,i,x]->[0,s0,1,t,0,i,x]: t=theta(0,i)}"
        " union {[0,s0,2,i,x]->[0,s0,1,t,1,i,x]: t=theta(1,i)}"
        " union {[0,s0,3,i,x]->[0,s0,1,t,2,i,x]: t=theta(2,i)}"
        " union {[0,s0,0,i,x]->[0,s0,0,t,0,i,x]: 0=x and 0=i and 0=t}");

    r2 = new Relation("[n_inter,n_moles,n_tstep]->"
        "{[tstep,i]->[0,tstep,1,i,0]}");

    result = r1->Compose(r2);
    EXPECT_EQ("{ [tstep, i] -> [0, s0, 1, t, 0, i1, 0] : "
        "__tv0 - __tv3 = 0 && __tv1 - __tv7 = 0 "
        "&& __tv5 - theta(0, __tv1) = 0 }",
        result->toString());

    expected = new Relation("[n_inter,n_moles,n_tstep,nt]->"
        "{[tstep,i]->[0,s0,1,t,0,i1,0] "
        ": i-i1=0 and t-theta(0,i)=0 and tstep=s0 }");
    EXPECT_EQ(expected->toString(), result->toString());
    delete expected;

    delete r1;
    delete r2;
    delete result;

    // Left off at commented out test case on line 644 in
    // TrunkIEGenR192PreRewrite/src/iegenlib/set_relation/set_relation_test_moldyn.cc
    // going backwards.
}

#pragma mark MoldynLoopAlign
// When testing moldyn-FST-M2-example_sparse.py ran
// into an issue updating the access functions with the loop
// alignment transformation.  This ended up not being an issue.
TEST_F(SetRelationTest, MoldynLoopAlign) {

    //iegenlib::appendCurrEnv("sigma() = inverse sigma_inv()");
    iegenlib::appendCurrEnv("sigma",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);

    Relation* a1_1;
    Relation* T_I0_to_I1;
    Relation* expected;
    Relation* result;

    a1_1 = new Relation("{ [0, s, 0, i, 0] -> [j] : j - sigma(i) = 0 }");

    T_I0_to_I1 = new Relation("{ [0, s, 0, i, 0] -> [0, s, 0, j, 0] : "
                              "j - sigma(i) = 0 }");

    Relation* T_I1_to_I0 = T_I0_to_I1->Inverse();
    result = a1_1->Compose( T_I1_to_I0 );

    expected = new Relation("{ [0, s, 0, j, 0] -> [j1] : j - j1 = 0 }");
    EXPECT_EQ( expected->toString(), result->toString() );

    delete a1_1;
    delete T_I0_to_I1;
    delete T_I1_to_I0;
    delete result;
    delete expected;

}

#pragma mark ISLString
// Testing the declaration of symbolics for ISL
TEST_F(SetRelationTest, ISLString) {

    Set* s1 = new Set("{[i] : 0 <= i && i < N }");
    EXPECT_EQ( "[ N ] -> { [i] : i >= 0 && -i + N - 1 >= 0 }",
                s1->toISLString() );
    delete s1;

    Set* s2 = new Set("{[i,j] : 0 <= i && i < N && 0 <= j && j < N }");
    Set* expected
        = new Set("[N] -> {[i,j] : 0 <= i && i < N && 0 <= j && j < N }");
    EXPECT_EQ( "[ N ] -> { [i, j] : i >= 0 && j >= 0 && -i + N - 1 >= 0 "
               "&& -j + N - 1 >= 0 }",
                s2->toISLString() );
    EXPECT_EQ( expected->toISLString(), s2->toISLString() );
    delete expected;
    delete s2;

    Set* s3 = new Set("{[i,j] : 0 <= i && i < N && 0 <= j && j < M }");
    EXPECT_EQ( "[ M, N ] -> { [i, j] : i >= 0 && j >= 0 && -i + N - 1 >= 0 "
               "&& -j + M - 1 >= 0 }",
                s3->toISLString() );
    delete s3;

    Set* s4
        = new Set("{[i,j] : 0 <= i && i < N && 0 <= j && j < M && i=foo(j) }");
    EXPECT_EQ( "[ M, N ] -> { [i, j] : i - foo(j) = 0 && i >= 0 && j >= 0 "
               "&& -i + N - 1 >= 0 && -j + M - 1 >= 0 }",
                s4->toISLString() );
    delete s4;

    Set* s5
        = new Set("{[i,j] : 0 <= i && i < N && 0 <= j && j < M && i=foo(X) }");
    EXPECT_EQ( "[ M, N, X ] -> { [i, j] : i - foo(X) = 0 && i >= 0 && j >= 0 "
               "&& -i + N - 1 >= 0 && -j + M - 1 >= 0 }",
                s5->toISLString() );
    delete s5;

    Relation* s6 = new Relation("{[i]->[j] : 0 <= i && i < N && 0 <= j &&"
                                " j < M && i=foo(X) }");
    EXPECT_EQ( "[ M, N, X ] -> { [i] -> [j] : i - foo(X) = 0 && i >= 0"
               " && j >= 0 && -i + N - 1 >= 0 && -j + M - 1 >= 0 }",
                s6->toISLString() );
    delete s6;

}

#pragma mark SetParserError
// The following should cause a parser exception to be thrown and the
// Set constructor should catch it.  Right now it just seg faults.
// IEGRTWO-18
TEST_F(SetRelationTest, SetParserError) {

/*
    Set* s6 =
        new Set("{[i]->[j] : 0 <= i && i < N && 0 <= j && j < M && i=foo(X) }");
*/
// FIXME: how do we EXPECT an exception?
//    delete s6;

}

#pragma mark SetZeroArity
// Need zero arity sets for parameters / symbolic variables.
TEST_F(SetRelationTest, SetZeroArity) {

    Set* s1 = new Set("{ : 5 <=  N }");
    EXPECT_EQ( "[ N ] -> {  : N - 5 >= 0 }", s1->toISLString() );

    delete s1;
}

#pragma mark SetZeroArityIntersect
// Need zero arity sets for parameters / symbolic variables.
// Need to be able to intersect such sets.
TEST_F(SetRelationTest, SetZeroArityIntersect) {

    Set* s1 = new Set("{ : N > 0}");
    Set* s2 = new Set("{ : M > 0}");
    Set* s3 = s1->Intersect(s2);
    EXPECT_EQ( "[ M, N ] -> {  : M - 1 >= 0 && N - 1 >= 0 }",
               s3->toISLString() );

    delete s1;
    delete s2;
    delete s3;
}

#pragma mark SetIntersect
/* IEGRONE-90
// These are examples where we aren't detecting non-sat
// conjunctions or we have redundant constraints in
// a conjunction.
TEST_F(SetRelationTest, SetIntersect) {

    Set* s1 = new Set("{ [i] : i> 0}");
    Set* s2 = new Set("{ [i] : i<0 }");
    Set* s3 = s1->Intersect(s2);
    EXPECT_EQ( "{ [t0] : FALSE }", s3->toString() );
    delete s1;
    delete s2;
    delete s3;

    s1 = new Set("{ [i] : i > 0}");
    s2 = new Set("{ [i] : i > 1 }");
    s3 = s1->Intersect(s2);
    EXPECT_EQ( "{ [i] : i > 0 }", s3->toString() );
    delete s1;
    delete s2;
    delete s3;
}
*/

#pragma mark SetTupleIterator
// Checking that get all tuple variables in correct order.
TEST_F(SetRelationTest, SetTupleIterator) {

    Set* s1 = new Set("{ [i,j,k,l] }");

    std::list<std::string> expectedList;
    expectedList.push_back("i");
    expectedList.push_back("j");
    expectedList.push_back("k");
    expectedList.push_back("l");
    std::list<std::string> actualList;
    iegenlib::StringIterator* siter = s1->getTupleIterator();
    while (siter->hasNext()) {
        actualList.push_back(siter->next());
    }
    EXPECT_EQ(expectedList, actualList);

    delete s1;
    delete siter;
}

#pragma mark SetTupleIterator
// Checking that get the executed tuple declaration back.
TEST_F(SetRelationTest, SetTupleDecl) {
    Set * s1 = new Set("{[0,i,1,j,k] }");
    TupleDecl tdecl = s1->getTupleDecl();
    EXPECT_EQ("0, i, 1, j, k", tdecl.toString());

    delete s1;
}

#pragma mark ConjunctionLessThan
// IEGRONE-93: Checking that Conjunction::operator<(Conjunction) behaves as
// expected.
TEST_F(SetRelationTest, ConjunctionLessThanCount) {
    /*
     * Comparisons by equalities and inequalities (by count)
     */

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4]} ==
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4]}
    Conjunction conj1(5);
    Conjunction conj2(5);

    //Empty Conjunction comparisons
    EXPECT_FALSE(conj1 < conj2); //should be ==, not <
    EXPECT_FALSE(conj2 < conj1); //should be ==, not <

    // e1 : x
    Exp e1 = Exp();
    e1.addTerm(new VarTerm("x"));

    //Give conj2 an equality, now: conj1 < conj2
    conj2.addEquality(e1.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4]} <
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    //Give conj1 the same equality, now: conj1 == conj2
    conj1.addEquality(e1.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0} ==
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    // e2: y
    Exp e2 = Exp();
    e2.addTerm(new VarTerm("y"));

    //Give conj2 an inequality, now conj1 < conj2
    conj2.addInequality(e2.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0} <
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && y >= 0}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    //Give conj1 the same inequality, now: conj1 == conj2
    conj1.addInequality(e2.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && y >= 0} ==
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && y >= 0}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    // e3: a, e4: b, e5: c
    Exp e3 = Exp();
    Exp e4 = Exp();
    Exp e5 = Exp();
    e3.addTerm(new VarTerm("a"));
    e4.addTerm(new VarTerm("b"));
    e5.addTerm(new VarTerm("c"));

    //Give conj1 1 equality and give conj2 2 inequalities
    //since equalities are compared before ineqaulities conj1 > conj2
    conj1.addEquality(e3.clone());
    conj2.addInequality(e4.clone());
    conj2.addInequality(e5.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && a = 0 && y >= 0} >
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && y >= 0 && b >= 0
    //                                        && c >= 0}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_TRUE(conj2 < conj1);

    //Give conj2 the equality, now conj1 < conj2
    conj2.addEquality(e3.clone());

    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && a = 0 && y >= 0} <
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4] : x = 0 && a = 0 && y >= 0
    //                                        && b >= 0 && c >= 0}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);
}

// IEGRONE-93: Checking that Conjunction::operator<(Conjunction) behaves as
// expected.
TEST_F(SetRelationTest, ConjunctionLessThanValues) {
    /*
     * Comparisons by equalities and inequalities (by values)
     */
    // conj1: {[__tv0,__tv1,__tv2,__tv3,__tv4]} ==
    // conj2: {[__tv0,__tv1,__tv2,__tv3,__tv4]}
    Conjunction conj1(5);
    Conjunction conj2(5);

    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    /*
     * Start by comparing TupleDecl's
     */
    // tdecl1: [a,b,c,d]
    TupleDecl tdecl1(4);
    tdecl1.setTupleElem(0,"a");
    tdecl1.setTupleElem(1,"b");
    tdecl1.setTupleElem(2,"c");
    tdecl1.setTupleElem(3,"d");
    //tdecl2: [a,b,c,d,e]
    TupleDecl tdecl2(5);
    tdecl2.setTupleElem(0,"a");
    tdecl2.setTupleElem(1,"b");
    tdecl2.setTupleElem(2,"c");
    tdecl2.setTupleElem(3,"d");
    tdecl2.setTupleElem(4,"e");
    EXPECT_TRUE(tdecl1 < tdecl2); //We're assuming tdecl1 < tdecl2, check this

    // set conj1 and conj2 to have the same tuple declaration (tdecl1), still
    // conj1 == conj2
    conj1.setTupleDecl(tdecl1);
    conj2.setTupleDecl(tdecl1);

    // conj1: {[a,b,c,d,0]} ==
    // conj2: {[a,b,c,d,0]}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    //change conj2 tuple declaration to tdecl2, tdecl1<tdecl2 => conj1 < conj2
    conj2.setTupleDecl(tdecl2);

    // conj1: {[a,b,c,d,0]} <
    // conj2: {[a,b,c,d,e]}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    //change both conj1 and conj2 to have the same tuple decl
    conj1.setTupleDecl(tdecl2);
    conj2.setTupleDecl(tdecl2);

    // conj1: {[a,b,c,d,e]} ==
    // conj2: {[a,b,c,d,e]}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj2);

    /*
     * Then compare equality values
     */

    // e1: a, e2: b, e1 < e2
    Exp e1 = Exp();
    Exp e2 = Exp();
    e1.addTerm(new VarTerm("a"));
    e2.addTerm(new VarTerm("b"));
    EXPECT_TRUE(e1<e2); //We're assuming e1 < e2, check this

    // Give conj1 equality e1:a = 0 and conj2 equality e2:b = 0,
    //   since e1<e2 => conj1 < conj2
    conj1.addEquality(e1.clone());
    conj2.addEquality(e2.clone());

    // conj1: {[a,b,c,d,e] : __tv0 = 0} <
    // conj2: {[a,b,c,d,e] : __tv1 = 0}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    // Now make conj1 == conj2 by giving conj1 equality e2:b = 0 and conj2
    //   equality e1:a = 0
    // This assumes that equalities are inserted in sorted order
    conj1.addEquality(e2.clone());
    conj2.addEquality(e1.clone());

    // conj1: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0} ==
    // conj2: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    /*
     * Now compare inequality values
     */

    // e3: c, e4: d, e3 < e4
    Exp e3 = Exp();
    Exp e4 = Exp();
    e3.addTerm(new VarTerm("c"));
    e4.addTerm(new VarTerm("d"));
    EXPECT_TRUE(e3<e4); //We're assuming e3 < e4, check this

    // Give conj1 inequality e3:c >= 0 and conj2 inequality e4:d >= 0, since
    //   e3<e4 => conj1 < conj2
    conj1.addInequality(e3.clone());
    conj2.addInequality(e4.clone());

    // conj1: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0 && __tv2 >= 0} <
    // conj2: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0 && __tv3 >= 0}
    EXPECT_TRUE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);

    // Now make conj1 == conj2 by giving conj1 inequality e4:d >= 0 and conj2
    //   inequality e3:c >= 0
    // This assumes that inequalities are inserted in sorted order
    conj1.addInequality(e4.clone());
    conj2.addInequality(e3.clone());

    // conj1: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0 && __tv2 >= 0 &&
    //                       __tv3 >= 0} ==
    // conj2: {[a,b,c,d,e] : __tv0 = 0 && __tv1 = 0 && __tv2 >= 0 &&
    //                       __tv3 >= 0}
    EXPECT_FALSE(conj1 < conj2);
    EXPECT_FALSE(conj2 < conj1);
}

// IEGRONE-94: Checking that SparseConstraints::operator<(SparseConstraints)
//   behaves as expected.
TEST_F(SetRelationTest,SparseConstraintsLessThanCount) {
    /*
     * Comparisons by conjunctions (by count)
     */

    SparseConstraints sc1;
    SparseConstraints sc2;
    // sc1={  : FALSE }
    // sc2={  : FALSE }

    // Empty SparseConstraints comparisons
    EXPECT_FALSE(sc1 < sc2); //should be ==, not <
    EXPECT_FALSE(sc2 < sc1); //should be ==, not <

    // Add a conjunction to the sc1 sparse constraints object.
    Conjunction* conj1_copy = new Conjunction(*conj1);
    // It is necessary to convert the var terms
    // for tuple variables to TupleVarTerms.
    conj1_copy->substituteTupleDecl();
    sc1.addConjunction(conj1_copy->clone());

    // sc1={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } >
    // sc2={  : FALSE }
    EXPECT_FALSE(sc1 < sc2);
    EXPECT_TRUE(sc2 < sc1);

    // Add the same conjunction to the sc2 sparse constraints object.
    sc2.addConjunction(conj1_copy->clone());

    // sc1={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } ==
    // sc2={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }
    EXPECT_FALSE(sc1 < sc2);
    EXPECT_FALSE(sc2 < sc1);

    // Add another conjunction to sc2
    Conjunction* conj2_copy = new Conjunction(*conj2);
    conj2_copy->substituteTupleDecl();
    sc2.addConjunction(conj2_copy->clone());
    EXPECT_EQ("{ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 } union "
              "{ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }" ,
              sc2.toString());

    // sc1={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } <
    // sc2={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } union
    //       { [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }
    EXPECT_TRUE(sc1 < sc2);
    EXPECT_FALSE(sc2 < sc1);

    // Add the same conjunction to the sc1 sparse constraints object.
    // This assumes that conjunctions are inserted in sorted order.
    sc1.addConjunction(conj2_copy->clone());

    // sc1={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } union
    //       { [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }     ==
    // sc2={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } union
    //       { [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }
    EXPECT_FALSE(sc1 < sc2);
    EXPECT_FALSE(sc2 < sc1);

    delete conj1_copy;
    delete conj2_copy;
}

// IEGRONE-94: Checking that SparseConstraints::operator<(SparseConstraints) behaves as expected.
TEST_F(SetRelationTest,SparseConstraintsLessThanValues) {
    /*
     * Comparisons by conjunctions (by values)
     */

    // Create four conjunctions where conj1 < conj2 < conj3 < conj4

    // Copy in conj1 and conj2 from SetUp()
    Conjunction* conj1_copy = new Conjunction(*conj1);
    conj1_copy->substituteTupleDecl();
    Conjunction* conj2_copy = new Conjunction(*conj2);
    conj2_copy->substituteTupleDecl();

    // Create two new Conjunction's of our own
    // tdecl1=[a,b,c,d]
    TupleDecl tdecl1(4);
    tdecl1.setTupleElem(0,"a");
    tdecl1.setTupleElem(1,"b");
    tdecl1.setTupleElem(2,"c");
    tdecl1.setTupleElem(3,"d");
    // e1: a
    Exp e1 = Exp();
    e1.addTerm(new VarTerm("a"));
    // e2: b
    Exp e2 = Exp();
    e2.addTerm(new VarTerm("b"));

    // conj1={ [a, b, c, d] : __tv0 >= 0 }
    Conjunction conj1(4);
    conj1.addInequality(e1.clone());
    conj1.setTupleDecl(tdecl1);

    // conj2={ [a, b, c, d] : __tv0 >= 0 && __tv1 >= 0 }
    Conjunction conj2(4);
    conj2.addInequality(e1.clone());
    conj2.addInequality(e2.clone());
    conj2.setTupleDecl(tdecl1);

    //Use coppied conj1 and conj2 from SetUp as our conj3 and conj4
    // conj3={ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }
    Conjunction conj3 = Conjunction(*conj2_copy);
    // conj4={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }
    Conjunction conj4 = Conjunction(*conj1_copy);

    // conj1={ [a, b, c, d] : __tv0 >= a } <
    // conj2={ [a, b, c, d] : __tv0 >= a && __tv1 >= b } <
    // conj3={ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 } <
    // conj4={ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }
    EXPECT_TRUE(conj1<conj2); //We're assuming conj1 < conj2 < conj3 < conj4,
    EXPECT_FALSE(conj2<conj1);//  check this
    EXPECT_TRUE(conj2<conj3);
    EXPECT_FALSE(conj3<conj2);
    EXPECT_TRUE(conj3<conj4);
    EXPECT_FALSE(conj4<conj3);

    SparseConstraints sc1;
    SparseConstraints sc2;
    // sc1={  : FALSE }
    // sc2={  : FALSE }

    // Empty SparseConstraints comparisons
    EXPECT_FALSE(sc1 < sc2); //should be ==, not <
    EXPECT_FALSE(sc2 < sc1); //should be ==, not <

    // Give sc1 the conjunction conj1 and give sc2 the conjunction conj2, now sc1 < sc2 since conj1 < conj2
    sc1.addConjunction(conj1.clone());
    sc2.addConjunction(conj2.clone());

    // sc1={ [a, b, c, d] : __tv0 >= a } <
    // sc2={ [a, b, c, d] : __tv0 >= a && __tv1 >= b }
    EXPECT_TRUE(sc1 < sc2);
    EXPECT_FALSE(sc2 < sc1);

    // Give sc1 the conjunction conj2 and give sc2 the conjunction conj1, now sc1 == sc2
    // This assumes adding conjunctions is done in sorted order.
    sc1.addConjunction(conj2.clone());
    sc2.addConjunction(conj1.clone());

    // sc1={ [a, b, c, d] : __tv0 >= a } union
    //       { [a, b, c, d] : __tv0 >= a && __tv1 >= b } ==
    // sc2={ [a, b, c, d] : __tv0 >= a } union
    //       { [a, b, c, d] : __tv0 >= a && __tv1 >= b }
    EXPECT_FALSE(sc1 < sc2);
    EXPECT_FALSE(sc2 < sc1);

    // Give sc1 the conjunction conj4 and give sc2 the conjunction conj3, now sc1 > sc2
    sc1.addConjunction(conj4.clone());
    sc2.addConjunction(conj3.clone());

    // sc1={ [a, b, c, d] : __tv0 >= a } union
    //       { [a, b, c, d] : __tv0 >= a && __tv1 >= b } union
    //       { [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 } >
    // sc2={ [a, b, c, d] : __tv0 >= a } union
    //       { [a, b, c, d] : __tv0 >= a && __tv1 >= b } union
    //       { [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }
    EXPECT_FALSE(sc1 < sc2);
    EXPECT_TRUE(sc2 < sc1);

    EXPECT_EQ("{ [a, b, c, d] : a >= 0 } union "
              "{ [a, b, c, d] : a >= 0 && b >= 0 } union "
              "{ [x, 3, 1, y] : __tv0 = 0 && __tv0 - __tv3 = 0 }",sc1.toString());
    EXPECT_EQ("{ [a, b, c, d] : a >= 0 } union "
              "{ [a, b, c, d] : a >= 0 && b >= 0 } union "
              "{ [x, y, 2, 0] : __tv0 - __tv1 = 0 && __tv0 >= 0 }",sc2.toString());

    delete conj1_copy;
    delete conj2_copy;
}

// IEGRONE-95: Checking that Set::operator<(Set) behaves as expected.
TEST_F(SetRelationTest,SetLessThan) {
    Set *s1 = new Set("{ }");
    Set *s2 = new Set("{ }");
    
    // compare empty Sets
    EXPECT_FALSE(*s1 < *s2);
    EXPECT_FALSE(*s2 < *s1);

    Set *s3 = new Set("{ [a] }");
    
    // compare empty to non-empty
    // s2 = { }
    // s3 = { [a] }
    EXPECT_TRUE(*s2 < *s3);
    EXPECT_FALSE(*s3 < *s2);

    Set *s4 = new Set("{ [a] : a = 0 }");

    // compare by conjunctions
    // s3 = { [a] }
    // s4 = { [a] : __tv0 = 0 }
    // Given: ([a]) < ([a] : __tv0 = 0)
    EXPECT_TRUE(*s3 < *s4);
    EXPECT_FALSE(*s4 < *s3);

    Set *s5 = new Set("{ [a,b] : a = 0 && b >= 0 }");
    Set *s6 = new Set("{ [a,b] : a = 0 }");

    // s5 = { [a,b] : __tv0 = 0 && tv1 >= 0 }
    // s6 = { [a,b] : __tv0 = 0 }
    // Given: ([a,b] : __tv0 = 0 && tv1 >= 0) > ([a,b] : __tv0 = 0)
    EXPECT_FALSE(*s5 < *s6);
    EXPECT_TRUE(*s6 < *s5);

    Set *s7 = new Set("{ [a] : a = 0 } union { [b] : b >= 0 }");

    // s4 = { [a] : __tv0 = 0 }
    // s7 = { [a] : __tv0 = 0 } union { [b] : __tv0 >= 0 }
    // Given: ([a] : __tv0 = 0) < ( [a] : __tv0 = 0 ) union ([b] : __tv0 >= 0)
    EXPECT_TRUE(*s4 < *s7);
    EXPECT_FALSE(*s7 < *s4);

    Set *s8 = new Set("{ [a, b, c, d] : a >= 0 && b >= 0 } union "
                      "{ [a, b, c, d] : a >= 0 }");
    Set *s9 = new Set("{ [a, b, c, d] : a >= 0 } union "
                      "{ [a, b, c, d] : a >= 0 && b >= 0 }");
    
    // s8 = s9 = { [a, b, c, d] : __tv0 >= 0 } union
    //           { [a, b, c, d] : __tv0 >= 0 && __tv1 >= 0 }
    // (Order shouldn't matter since SparseConstraints sorts Conjunctions)
    EXPECT_FALSE(*s8 < *s9);
    EXPECT_FALSE(*s9 < *s8);


    // Comparing sets with constants for some of the locations.
    Set *s10 = new Set("{ [0, 1, i, 2] }");
    Set *s11 = new Set("{ [0, 1, j, 2] }");
    
    // s10 == s11
    EXPECT_FALSE(*s10 < *s11);
    EXPECT_FALSE(*s10 < *s11);

    
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    delete s6;
    delete s7;
    delete s8;
    delete s9;
    delete s10;
    delete s11;
}

// IEGRONE-96: Checking that Relation::operator<(Relation) behaves as expected.
TEST_F(SetRelationTest,RelationLessThan) {
    Relation *r1 = new Relation("{ []->[] }");
    Relation *r2 = new Relation("{ []->[] }");

    // compare empty Relation
    EXPECT_FALSE(*r1 < *r2);
    EXPECT_FALSE(*r2 < *r1);

    Relation *r3 = new Relation("{ [a]->[b] }");

    // compare empty to non-empty
    // r2 = { []->[] }
    // r3 = { [a]->[b] }
    EXPECT_TRUE(*r2 < *r3);
    EXPECT_FALSE(*r3 < *r2);

    Relation *r4 = new Relation("{ [a]->[b] : a = 0 }");

    // compare by conjunctions
    // r3 = { [a]->[b] }
    // r4 = { [a]->[b] : __tv0 = 0 }
    // Given: ([a]->[b]) < ([a]->[b] : __tv0 = 0)
    EXPECT_TRUE(*r3 < *r4);
    EXPECT_FALSE(*r4 < *r3);

    Relation *r5 = new Relation("{ [a]->[b] : a = 0 && b >= 0 }");
    Relation *r6 = new Relation("{ [a]->[b] : a = 0 }");

    // r5 = { [a]->[b] : __tv0 = 0 && tv1 >= 0 }
    // r6 = { [a]->[b] : __tv0 = 0 }
    // Given: ([a]->[b] : __tv0 = 0 && tv1 >= 0) > ([a]->[b] : __tv0 = 0)
    EXPECT_FALSE(*r5 < *r6);
    EXPECT_TRUE(*r6 < *r5);

    Relation *r7 = new Relation("{ [a]->[b] : a = 0 }"
                         " union { [c]->[d] : c >= 0 }");

    // r4 = { [a]->[b] : __tv0 = 0 }
    // r7 = { [a]->[b] : __tv0 = 0 } union { [c]->[d] : __tv0 >= 0 }
    // Given: ([a]->[b] : __tv0 = 0) <         ([a]->[b] : __tv0 = 0 )
    //                                   union ([c]->[d] : __tv0 >= 0)
    EXPECT_TRUE(*r4 < *r7);
    EXPECT_FALSE(*r7 < *r4);

    Relation *r8 = new Relation("{ [a, b]->[c, d] : a >= 0 && b >= 0 } union "
                                "{ [a, b]->[c, d] : a >= 0 }");
    Relation *r9 = new Relation("{ [a, b]->[c, d] : a >= 0 } union "
                                "{ [a, b]->[c, d] : a >= 0 && b >= 0 }");
    // r8 = r9 = { [a, b]->[c, d] : __tv0 >= 0 } union
    //           { [a, b]->[c, d] : __tv0 >= 0 && __tv1 >= 0 }
    // (Order shouldn't matter since SparseConstraints sorts Conjunctions)
    EXPECT_FALSE(*r8 < *r9);
    EXPECT_FALSE(*r9 < *r8);

    Relation *r10 = new Relation("{ []->[a] }");
    Relation *r11 = new Relation("{ []->[a,b] }");

    //Equal in arity, r1 (empty relation) < r10 < r11 (by out arity)
    EXPECT_TRUE(*r1 < *r10);
    EXPECT_FALSE(*r10 < *r1);
    EXPECT_TRUE(*r10 < *r11);
    EXPECT_FALSE(*r11 < *r10);

    Relation *r12 = new Relation("{ [a]->[c] }");
    Relation *r13 = new Relation("{ [a,b]->[c] }");

    //Equal out arity, r12 < r13 (by in arity)
    EXPECT_TRUE(*r12 < *r13);
    EXPECT_FALSE(*r13 < *r12);

    // comparing constants in the tuple declarations
    Relation *r14 = new Relation("{ [0,i]->[0,j] }");
    Relation *r15 = new Relation("{ [0,i]->[1,j] }");

    //Equal out arity, r14 < r15 (by value of the constant in out tuple)
    EXPECT_TRUE(*r14 < *r15);
    EXPECT_FALSE(*r15 < *r14);


    delete r1;
    delete r2;
    delete r3;
    delete r4;
    delete r5;
    delete r6;
    delete r7;
    delete r8;
    delete r9;
    delete r10;
    delete r11;
    delete r12;
    delete r13;
    delete r14;
    delete r15;
}

// Test composition of relations.
// Both relations are functions.
TEST_F(SetRelationTest, ComposeForDataReorderingMoldyn) {

    Relation* A = new Relation("{ [ii]->[i]:i=inter2(ii) }");
    Relation* R
        = new Relation("{ [i]->[j]:j=sigma(i) }");

    Relation* result = R->Compose(A);
    Relation* expected = new Relation("{ [ii]->[j]:j=sigma(inter2(ii)) }");
    EXPECT_EQ(expected->toString(), result->toString());

    delete A;
    delete R;
    delete result;
    delete expected;
}

TEST_F(SetRelationTest, ComposeForIterationReorderingMoldyn) {

    iegenlib::appendCurrEnv("delta",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    
    // scattering function
    Relation* S1 = new Relation("{ [ i ] -> [ 0, s, 0, i, 0 ] }");
    Relation* S2 = new Relation("{ [ ii ] -> [ 0, s, 1, ii, 0 ] }");
    Relation* S3 = new Relation("{ [ i ] -> [ 0, s, 2, i, 0 ] }");
    
    // transformation
    Relation* T = new Relation("{ [0,s,0,i,0] -> [0,s,0,i,0] } "
      "union { [ 0, s, 1, ii, 0 ] -> [ 0, s, 1, k, 0] : k = delta(ii)} "
      "union { [ 0, s, 2, i, 0 ] -> [ 0, s, 2, i, 0] }");

    Relation* S1prime = T->Compose(S1);
    Relation* S1primeExp = new Relation("{ [i]->[0, s, 0, i, 0 ] }");
    EXPECT_TRUE((*S1primeExp) == (*S1prime));
    
    Relation* S2prime = T->Compose(S2);
    Relation* S2primeExp = new Relation("{ [ii]->[0, s, 1, k,"
                                        " 0]:ii=delta_inv(k)}");
    EXPECT_TRUE((*S2primeExp) == (*S2prime));
    
    Relation* S3prime = T->Compose(S3);
    Relation* S3primeExp = new Relation("{ [i]->[0, s, 2, i, 0 ] }");
    EXPECT_TRUE((*S3primeExp) == (*S3prime));
    
    delete S1;
    delete S2;
    delete S3;
    delete T;
    delete S1prime;
    delete S2prime;
    delete S3prime;
    delete S1primeExp;
    delete S2primeExp;
    delete S3primeExp;

}

// This test case walks through the first example of RTRTs in the journal paper
TEST_F(SetRelationTest, ComposeForDataReorderingPlusIterationReorderingMoldyn){
    
    //refresh the env and add delta's inverse delta_inv
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("delta",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    
    // This is the original Data Access Function
    // it maps from the local iteration space to the data space
    Relation* A_i_to_a = new Relation("{ [ii]->[i]:i=inter1(ii) } union "
                                      "{ [ii]->[i]:i=inter2(ii) } ");                                  
    // scattering function
    // maps from the local iteration space to the full iteration space
    Relation* S1 = new Relation("{ [ i ] -> [ 0, s, 0, i, 0 ] }");
    Relation* S2 = new Relation("{ [ ii ] -> [ 0, s, 1, ii, 0 ] }");
    Relation* S3 = new Relation("{ [ i ] -> [ 0, s, 2, i, 0 ] }");
    
    // the data reordering transformation                                                                
    Relation* R_x0_to_x1 = new Relation("{ [i]->[j]:j=sigma(i) }");
    
    // iteration reordering transformation transformation
    Relation* T = new Relation("{ [0,s,0,i1,0] -> [0,s,0,k1,0] : k1=delta(i1)} "
      "union { [ 0, s, 1, ii, 0 ] -> [ 0, s, 1, ii, 0] } "
      "union { [ 0, s, 2, i2, 0 ] -> [ 0, s, 2, k2, 0] : k2=delta(i2) }");
    
    // Step 0: calculate inverses
    // need the inverse of each of these for the modified access function
    Relation* S1_inv = S1->Inverse();
    Relation* S2_inv = S2->Inverse();
    Relation* S3_inv = S3->Inverse();
    
    // verify inverses
    Relation* S1_inv_exp = new Relation("{[0,s,0,i,0]->[i]}");
    ASSERT_TRUE((*S1_inv) == (*S1_inv_exp));
    delete S1_inv_exp;
    Relation* S2_inv_exp = new Relation("{[0,s,1,ii,0]->[ii]}");
    ASSERT_TRUE((*S2_inv) == (*S2_inv_exp));
    delete S2_inv_exp;
    Relation* S3_inv_exp = new Relation("{[0,s,2,i,0]->[i]}");
    ASSERT_TRUE((*S3_inv) == (*S3_inv_exp));
    delete S3_inv_exp;
    
    // STEP 1: create modified data access function
    // maps from the full iteration space to the data space
    Relation* A_I1_to_a = A_i_to_a->Compose(S1_inv);
    Relation* A_I2_to_a = A_i_to_a->Compose(S2_inv);
    Relation* A_I3_to_a = A_i_to_a->Compose(S3_inv);
    
    // verify compose 
    Relation* A_I1_to_a_exp = new Relation("{[0,s,0,i1,0]->[i2]:i2=inter1(i1)}"
                                    "union {[0,s,0,i1,0]->[i2]:i2=inter2(i1)}");
    ASSERT_TRUE((*A_I1_to_a) == (*A_I1_to_a_exp));
    delete A_I1_to_a_exp;
    Relation* A_I2_to_a_exp = new Relation("{[0,s,1,i1,0]->[i2]:i2=inter1(i1)}"
                                    "union {[0,s,1,i1,0]->[i2]:i2=inter2(i1)}");
    ASSERT_TRUE((*A_I2_to_a) == (*A_I2_to_a_exp));
    delete A_I2_to_a_exp;
    Relation* A_I3_to_a_exp = new Relation("{[0,s,2,i1,0]->[i2]:i2=inter1(i1)}"
                                    "union {[0,s,2,i1,0]->[i2]:i2=inter2(i1)}");
    ASSERT_TRUE((*A_I3_to_a) == (*A_I3_to_a_exp));
    delete A_I3_to_a_exp;

    // STEP 2: apply data reordering to modified access functions
    Relation* A_I1_to_aprime = R_x0_to_x1->Compose(A_I1_to_a);
    Relation* A_I2_to_aprime = R_x0_to_x1->Compose(A_I2_to_a);
    Relation* A_I3_to_aprime = R_x0_to_x1->Compose(A_I3_to_a);
    delete R_x0_to_x1;
    
    // verify compose
    Relation* A_I1_to_aprime_exp = new Relation(
                                  "{[0,s,0,i,0]->[j]:j=sigma(inter1(i))} union "
                                  "{[0,s,0,i,0]->[j]:j=sigma(inter2(i))}");
    EXPECT_TRUE((*A_I1_to_aprime_exp) == (*A_I1_to_aprime));
    delete A_I1_to_aprime_exp;
    Relation* A_I2_to_aprime_exp = new Relation(
                                  "{[0,s,1,i,0]->[j]:j=sigma(inter1(i))} union "
                                  "{[0,s,1,i,0]->[j]:j=sigma(inter2(i))}");
    EXPECT_TRUE((*A_I2_to_aprime_exp) == (*A_I2_to_aprime));
    delete A_I2_to_aprime_exp;
    Relation* A_I3_to_aprime_exp = new Relation(
                                  "{[0,s,2,i,0]->[j]:j=sigma(inter1(i))} union "
                                  "{[0,s,2,i,0]->[j]:j=sigma(inter2(i))}");
    EXPECT_TRUE((*A_I3_to_aprime_exp) == (*A_I3_to_aprime));
    delete A_I3_to_aprime_exp;
    
    // STEP 3: apply iteration reordering by composing the inverse
    // of the iteration reordering transformation function
    Relation* T_inv = T->Inverse();
    
    // verify the inverse
    Relation* T_inv_exp = new Relation(
      "{[0,s,0,k1,0]->[0,s,0,i1,0]:k1=delta(i1) } union "
      "{[0,s,1,ii,0]->[0,s,1,ii,0] } union "
      "{[0,s,2,k2,0]->[0,s,2,i2,0]:k2=delta(i2) }");
    EXPECT_TRUE((*T_inv_exp) == (*T_inv));
    delete T_inv_exp;
    
    // The final access functions
    Relation* A_I1prime_to_aprime = A_I1_to_aprime->Compose(T_inv);
    Relation* A_I2prime_to_aprime = A_I2_to_aprime->Compose(T_inv);
    Relation* A_I3prime_to_aprime = A_I3_to_aprime->Compose(T_inv);
    
    // verify the final access functions
    Relation* A_I1prime_to_aprime_exp = new Relation(
      "{[0,s,0,k1,0]->[j]: j=sigma(inter1(delta_inv(k1)))} union "
      "{[0,s,0,k2,0]->[j]: j=sigma(inter2(delta_inv(k2)))}");
    EXPECT_TRUE((*A_I1prime_to_aprime_exp) == (*A_I1prime_to_aprime));
    delete A_I1prime_to_aprime_exp;
    
    Relation* A_I2prime_to_aprime_exp = new Relation(
      "{[0,s,1,k1,0]->[j]: j=sigma(inter1(k1))} union "
      "{[0,s,1,k2,0]->[j]: j=sigma(inter2(k2))}");
    EXPECT_TRUE((*A_I2prime_to_aprime_exp) == (*A_I2prime_to_aprime));
    delete A_I2prime_to_aprime_exp;
    
    Relation* A_I3prime_to_aprime_exp = new Relation(
      "{[0,s,2,k1,0]->[j]: j=sigma(inter1(delta_inv(k1)))} union "
      "{[0,s,2,k2,0]->[j]: j=sigma(inter2(delta_inv(k2))) }");
    EXPECT_TRUE((*A_I3prime_to_aprime_exp) == (*A_I3prime_to_aprime));
    delete A_I3prime_to_aprime_exp;
    
    // STEP 4: apply the iteration reordering to the scattering functions
    Relation* S1prime = T->Compose(S1);
    Relation* S2prime = T->Compose(S2);
    Relation* S3prime = T->Compose(S3);
    
    // verify new scheduling functions
    Relation* S1prime_exp = new Relation("{[i]->[0,s,0,k1,0]:i=delta_inv(k1)}");
    EXPECT_TRUE((*S1prime_exp) == (*S1prime));
    delete S1prime_exp; 
    Relation* S2prime_exp = new Relation("{[i]->[0,s,1,i,0]}");
    EXPECT_TRUE((*S2prime_exp) == (*S2prime));
    delete S2prime_exp;
    Relation* S3prime_exp = new Relation("{[i]->[0,s,2,k1,0]:i=delta_inv(k1)}");
    EXPECT_TRUE((*S3prime_exp) == (*S3prime));
    delete S3prime_exp;
    
    // STEP 5: TODO add the dependencies
    
    // clean up all of the relations
    delete T;
    delete T_inv;
    delete A_i_to_a;
    delete S1;
    delete S2;
    delete S3;
    delete S1prime;
    delete S2prime;
    delete S3prime;
    delete S1_inv;
    delete S2_inv;
    delete S3_inv;
    delete A_I1_to_a;
    delete A_I2_to_a;
    delete A_I3_to_a;
    delete A_I1_to_aprime;
    delete A_I2_to_aprime;
    delete A_I3_to_aprime;
    delete A_I1prime_to_aprime;
    delete A_I2prime_to_aprime;
    delete A_I3prime_to_aprime;
        

}

TEST_F(SetRelationTest, LoopFlatteningMoldyn){

    //refresh the env and add alpha's inverse
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("alpha",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    // FIXME: we need to detect that the domain I give above
    // and the way alpha is being used is inconsistent
    
    // Iteration space for each statement
    Set* Phi_S1 = new Set("{[0,s,0,i,0]: 0<=s and s<Ns "
                               " and 0<=i and i<Nv }");
    Set* Phi_S2 = new Set("{[0,s,1,e,q]: 0<=s and s<Ns "
                               " and 0<=e and e<Nv and 0<=q and q<=1 }");
    Set* Phi_S3 = new Set("{[0,s,2,k,0]: 0<=s and s<Ns "
                               " and 0<=k and k<Nv }");
    
    
    // loop flattening transformation
    Relation* T = new Relation("{[0,s,0,i,0] -> [0,e,0] : e=alpha(s,i)} "
                         "union {[0,s,1,i,0] -> [0,e,1] : e=alpha(s,i)} "
                         "union {[0,s,2,i,0] -> [0,e,2] : e=alpha(s,i)}");
                         
    // calculate the new scheduling functions
    Set* Phi_S1prime = T->Apply(Phi_S1);
    Set* expected = new Set( "{[0,e,0]: 0<=alpha_inv(e)[0] "
        "&& alpha_inv(e)[0]<Ns && 0<=alpha_inv(e)[1] && alpha_inv(e)[1]<Nv}");
    EXPECT_EQ( *expected, *Phi_S1prime);
    //EXPECT_EQ( expected->toString(), Phi_S1prime->toString());
    
    delete expected;
    delete Phi_S1;
    delete Phi_S2;
    delete Phi_S3;
    delete T;
    delete Phi_S1prime;

}

TEST_F(SetRelationTest, LoopFlatteningCSRtoCOO){

    //refresh the env and add alpha's inverse
    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("tau",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), true,
        iegenlib::Monotonic_NONE);
    // FIXME: we need to detect that the domain I give above
    // and the way tau is being used is inconsistent
    
    // Iteration space for original statement in
    // for (i=0; i<nr; i++) {
    //    for (k=ptr(i); k<ptr(i+1); k++) {
    //      y[i] += ...
    Set* s1_iter = new Set("{[i,k]: 0<=i and i<nr "
                           " and ptr(i)<=k and k<ptr(i+1) }");    
    
    // loop flattening transformation
    Relation* T = new Relation("{[i,j]->[k]:k=tau(i,j)}");
                         
    // apply transformation to set
    Set* s1_iter_prime = T->Apply(s1_iter);
    Set* expected = new Set( "{[k] : 0<=tau_inv(k)[0] && tau_inv(k)[0]<nr "
        "&& ptr(tau_inv(k)[0])<=tau_inv(k)[1] "
        "&& tau_inv(k)[1]<ptr(tau_inv(k)[0]+1)   }");
    //EXPECT_EQ( *expected, *Phi_S1prime);
    EXPECT_EQ( expected->toString(), s1_iter_prime->toString());
    
    delete expected;
    delete s1_iter;
    delete s1_iter_prime;
    delete T;
}

#pragma mark SubstituteInConstraints
// The en mass substitution.  IEGRTWO-97
TEST_F(SetRelationTest, SubstituteInConstraints){

    Set* initial_set = new Set( "{[k] : 0<=tau_inv(k)[0] && tau_inv(k)[0]<nr "
        "&& ptr(tau_inv(k)[0])<=tau_inv(k)[1] "
        "&& tau_inv(k)[1]<ptr(tau_inv(k)[0]+1)   }");

    // Substitution for k, which is __tv0, and nr
    SubMap subMap;
    TupleVarTerm* tv0 = new TupleVarTerm(0);
    Exp* e0 = new Exp();
    e0->addTerm(new VarTerm(7,"N"));
    subMap.insertPair(tv0, e0);
    VarTerm* nrTerm = new VarTerm("nr");
    Exp* e1 = new Exp();
    e1->addTerm(new VarTerm(9,"bye"));
    subMap.insertPair(nrTerm, e1);    
    
    // perform the substitution
    initial_set->substituteInConstraints(subMap);
    
    // check results
    Set* expected = new Set( "{[k] : 0<=tau_inv(7 N)[0] "
        "&& tau_inv(7 N)[0]<9 bye "
        "&& ptr(tau_inv(7 N)[0])<=tau_inv(7 N)[1] "
        "&& tau_inv(7 N)[1]<ptr(tau_inv(7 N)[0]+1)   }");
    EXPECT_EQ(expected->toString(), initial_set->toString() );

    delete initial_set;
    delete expected;
}

#pragma mark SetTupleDecl
// Changing an existing tuple declaration.  IEGRTWO-98
TEST_F(SetRelationTest, SparseConstraintsSetTupleDecl){

    // Check one that has constants but should work.
    Relation* r = new Relation("{[0,s,1,i,0]->[0,s,1,i',0]: i'=sigma_inv(i)} "
                    "union {[0,s,0,i,0]->[0,s,0,i,0]: 0=i} "
                    "union {[0,s,2,i,0]->[0,s,2,i,0]} "
                    "union {[0,s,3,i,0]->[0,s,3,i',0]: i'=sigma_inv(i)}");
    Relation* r_org = new Relation(*r);
    
    TupleDecl tdecl_good(10);
    tdecl_good.setTupleElem(0,"a");
    tdecl_good.setTupleElem(1,"b");
    tdecl_good.setTupleElem(2,"c");
    tdecl_good.setTupleElem(3,"d");
    tdecl_good.setTupleElem(4,"e");
    tdecl_good.setTupleElem(5,"f");
    tdecl_good.setTupleElem(6,"g");
    tdecl_good.setTupleElem(7,"h");
    tdecl_good.setTupleElem(8,"i");
    tdecl_good.setTupleElem(9,"j");
    
    r->setTupleDecl(tdecl_good);

/*
    EXPECT_EQ( "{ [0, b, 0, d, 0] -> [0, g, 0, i, 0] : d = 0 && b - g = 0 && d - i = 0 } union { [0, b, 1, d, 0] -> [0, g, 1, i, 0] : b - g = 0 && i - sigma_inv(d) = 0 } union { [0, b, 2, d, 0] -> [0, g, 2, i, 0] : b - g = 0 && d - i = 0 } union { [0, b, 3, d, 0] -> [0, g, 3, i, 0] : b - g = 0 && i - sigma_inv(d) = 0 }", r->prettyPrintString() );
*/
    EXPECT_EQ( *r_org, *r );
    
    // Check one that should throw an exception because
    // trying to change the constant.
    TupleDecl tdecl_bad(10);
    tdecl_bad.setTupleElem(0,"a");
    tdecl_bad.setTupleElem(1,"b");
    tdecl_bad.setTupleElem(2,1); // doesn't work for all conjunctions
    tdecl_bad.setTupleElem(3,"d");
    tdecl_bad.setTupleElem(4,"e");
    tdecl_bad.setTupleElem(5,"d");
    tdecl_bad.setTupleElem(6,"f");
    tdecl_bad.setTupleElem(7,"g");
    tdecl_bad.setTupleElem(8,"h");
    tdecl_bad.setTupleElem(9,"i");
    
   EXPECT_THROW(r->setTupleDecl(tdecl_bad), iegenlib::assert_exception );
    
    delete r;
    delete r_org;
}

#pragma mark IsFunction
TEST_F(SetRelationTest, RelationIsFunction){
    // Example from MoldynManyTests.  This should be a function.
    Relation* r1 = new Relation("{[s,t,i]->[s,t]}");
    EXPECT_EQ(r1->isFunction(), true);

    // Example from MoldynManyTests.  And so should this.
    /* FIXME, ok that is why the MoldynManyTests isn't working
    Relation* r2 = new Relation("{[s,1,t,2]->[s,t,i]}");
    EXPECT_EQ(r2->isFunction(), true);
    */
    
    
    delete r1;
    //delete r2;
}

#pragma mark VisitorDebugTest
class VisitorDebugTest : public Visitor {
  private:
    std::stringstream ss;
    
  public:
    std::string returnResult() { return ss.str(); }
    
    void postVisitTerm(iegenlib::Term * t) {
        ss << "visitTerm (" << t->toString() << ")" << std::endl;
    }
    void postVisitUFCallTerm(iegenlib::UFCallTerm * t) {
        ss << "visitUFCallTerm (" << t->toString() << ")" << std::endl;
    }
    void postVisitTupleVarTerm(iegenlib::TupleVarTerm * t) {
        ss << "visitVarTerm (" << t->toString() << ")" << std::endl;
    }
    void postVisitVarTerm(iegenlib::VarTerm * t) {
        ss << "visitTerm (" << t->toString() << ")" << std::endl;
    }
    void postVisitTupleExpTerm(iegenlib::TupleExpTerm * t) {
        ss << "visitTupleExpTerm (" << t->toString() << ")" << std::endl;
    }
    void postVisitExp(iegenlib::Exp * e) {
        ss << "visitExp (" << e->toString() << ")" << std::endl;
    }
   
    // Classes in set_relation.h
    void postVisitConjunction(iegenlib::Conjunction * c) {
        ss << "visitConjunction (" << c->toString() << ")" << std::endl;
    }
    void postVisitSparseConstraints(iegenlib::SparseConstraints * sc) {
        ss << "visitSparseConstraints (" << sc->toString() 
                 << ")" << std::endl;
    }
    void postVisitSet(iegenlib::Set * s) {
        ss << "visitSet (" << s->toString() << ")" << std::endl;
    }
    void postVisitRelation(iegenlib::Relation * r) {
        ss << "visitRelation (" << r->toString() << ")" << std::endl;
    }
};

TEST_F(SetRelationTest, VisitorDebugTest){
    // FIXME: parser is not handling "union"
    //Set *s = new Set("{[i,j,k,0] : i>=N } union {[x,y,z]}");
    Set *s1 = new Set("{[i,0,k] : i>=N }");
    Set *s2 = new Set("{[x,y,z] : y <N}");
    Set *s = s1->Union(s2);
    Relation *r = new Relation("{[k,p] -> [k1,i'] : k1=k-1 && "
        "i' = sigma(col(p)) && v=foo(a,b,bar(p+x))}");
        
    VisitorDebugTest* v = new VisitorDebugTest();
    s->acceptVisitor(v);
    EXPECT_EQ("visitVarTerm (-__tv1)\nvisitTerm (N)\nvisitTerm (-1)\nvisitExp"
              " (-__tv1 + N - 1)\nvisitConjunction ({ [x, y, z] : "
              "-__tv1 + N - 1 >= 0 })\nvisitVarTerm (__tv0)\nvisitTerm "
              "(-N)\nvisitExp (__tv0 - N)\nvisitConjunction ({ [i, 0, k] : "
              "__tv0 - N >= 0 })\nvisitSparseConstraints ({ [x, y, z] : "
              "-__tv1 + N - 1 >= 0 } union { [i, 0, k] : __tv0 - N >= 0 "
              "})\nvisitSet ({ [x, y, z] : -__tv1 + N - 1 >= 0 } union "
              "{ [i, 0, k] : __tv0 - N >= 0 })\n", 
              v->returnResult());
    //std::cout << v->returnResult();

    delete r;
    delete s;
}
#if 0
#pragma mark addUFConstraintsTest
TEST_F(SetRelationTest, addUFConstraintsTest){
  {
    Set* s = new Set("{[i,j] : index(i) <= j && j < index(i+1)}");
    Set* result = s->addUFConstraints("index","<=", "diagptr");
    
    EXPECT_EQ("{ [i, j] : j - index(i) >= 0 && diagptr(i) - index(i) >= 0 "
              "&& diagptr(i + 1) - index(i + 1) >= 0 && -j + index(i + 1) - "
              "1 >= 0 }",
              result->prettyPrintString());

    delete s;
    delete result;
  }

  {
    Relation* r = new Relation("{[i,j]->[k] : index(i) <= j && "
        "j < index(i+1) && diagptr(v+1)<k && k<indexptr(i)}");
    
    Relation* result1 = r->addUFConstraints("index",">", "diagptr");
    
    EXPECT_EQ("{ [i, j] -> [k] : j - index(i) >= 0 && -j + index(i + 1) "
              "- 1 >= 0 && -k + indexptr(i) - 1 >= 0 && k - diagptr(v + "
              "1) - 1 >= 0 && -diagptr(i) + index(i) + 1 >= 0 && -diagpt"
              "r(i + 1) + index(i + 1) + 1 >= 0 && -diagptr(v + 1) + ind"
              "ex(v + 1) + 1 >= 0 }",
              result1->prettyPrintString());

    Relation* result2 = r->addUFConstraints("index","=", "diagptr");

    EXPECT_EQ("{ [i, j] -> [k] : diagptr(i) - index(i) = 0 && diagptr(i + "
              "1) - index(i + 1) = 0 && diagptr(v + 1) - index(v + 1) = 0 "
              "&& j - index(i) >= 0 && -j + index(i + 1) - 1 >= 0 && -k + "
              "indexptr(i) - 1 >= 0 && k - diagptr(v + 1) - 1 >= 0 }",
              result2->prettyPrintString());
    
    delete r;
    delete result1;
    delete result2;
  }
 
              
}

#pragma mark addConstraintsDueToMonotonicity
TEST_F(SetRelationTest, addConstraintsDueToMonotonicity){
  {
    iegenlib::setCurrEnv();
    // Now set up an environment that defines an inverse for f.
    // Upon creation all expressions, conjunctions, sets, etc.
    // should use this environment until another one is constructed.
    iegenlib::appendCurrEnv("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), false,
        iegenlib::Monotonic_Nondecreasing);

    {
    Set* s = new Set("{[i,j] : f(i)<f(j) && 0<=f(i) && 0<=f(j)}");
    Set* copyS = s->boundDomainRange();

    Set* result = copyS->addConstraintsDueToMonotonicity();
    Set* expected = new Set("{[i,j] : f(i) >= 0 && f(j) >= 0 && "
                            "f(i)<G && f(j)<G && "
                            "0<=i && i<G && 0<=j && j<G &&"
                            "f(i)<f(j) && i<j}");
    
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    
    delete s;
    delete copyS;
    delete result;
    delete expected;
    }
/*
    {
    Set* s = new Set("{[i,j] : f(i)<f(j) && 0<=f(i) && 0<=f(j)}");
    Set* copyS = s->boundDomainRange();
    Set* result = copyS->addConstraintsDueToMonotonicity();
    Set* expected = new Set("{[i,j] : f(i) >= 0 && f(j) >= 0 && "
                            "f(i)<G && f(j)<G && "
                            "0<=i && i<G && 0<=j && j<G &&"
                            "f(i)<f(j) && i<j}");
                            
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    
    delete s;
    delete copyS;
    delete result;
    delete expected;
    }

    iegenlib::appendCurrEnv("g",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"), false,
        iegenlib::Monotonic_Increasing);

    {
    Set* s = new Set("{[i,j] : g(i)<g(j) && 0<=g(i) && 0<=g(j)}");
    Set* copyS = s->boundDomainRange();
    Set* result = copyS->addConstraintsDueToMonotonicity();
    Set* expected = new Set("{[i,j] : g(i) >= 0 && g(j) >= 0 && "
                            "g(i)<G && g(j)<G && "
                            "0<=i && i<G && 0<=j && j<G &&"
                            "g(i)<g(j) && i<j}");
                            
    EXPECT_EQ(expected->prettyPrintString(), result->prettyPrintString());
    
    delete s;
    delete copyS;
    delete result;
    delete expected;
    }

*/
  }

}
#endif 

//*********************** Testing isUFCallParam  ***************************

//Testing isUFCallParam: is a tuple variable argument to a UFS? (with its index)

TEST_F(SetRelationTest, isUFCallParam) {

    Relation *r1 = new Relation("[n] -> { [i,j] -> [ip,jp] : i = col(jp) "
       "and i < ip and 0 <= i and i < n and idx(i) <= j and j < idx(i+1) "
         "and 0 <= ip and ip < n and idx(ip) <= jp and jp < idx(ip+1) }");

    Relation *r2 = new Relation("[n] -> { [i,k,j1,j2] -> [ip,kp,jp1,jp2] :"
    " i < ip and j1 = jp2 and 0 <= i and i < n and 0 <= ip and ip < n and "
     "k+1 <= j1 and j1 < row(i+1) and kp+1 <= jp1 and jp1 < row(ip+1) and "
       "diag(col(k))+1 <= j2 and j2 < row(col(k)+1) and diag(col(kp))+1 <="
           " jp2 and jp2 < row(col(kp)+1) and row(i) <= k and k < diag(i) "
           "and row(ip) <= kp and kp < diag(ip) }");

   //  Test case for r2
   string org_tup("11001100");
   string ins_tup("00000000");
 
   int ar = r2->arity();
   for(int i = 0 ; i < ar ; i++)
   {
     if ( r2->isUFCallParam(i) )
     {
       ins_tup[i] = '1';
//       std::cout << std::endl <<i << "   is UFS arg" << std::endl;
     }
     else
     {
       ins_tup[i] = '0';
//       std::cout << std::endl <<i << "   is not UFS arg" << std::endl;
     }
   }

    EXPECT_EQ( org_tup , ins_tup );


   //  Test case for r1
   string org_tup_r1("1011");
   string ins_tup_r1("0000");
 
   int ar_r1 = r1->arity();
   for(int i = 0 ; i < ar_r1 ; i++)
   {
     if ( r1->isUFCallParam(i) )
     {
       ins_tup_r1[i] = '1';
//       std::cout << std::endl <<i << "   is UFS arg" << std::endl;
     }
     else
     {
       ins_tup_r1[i] = '0';
//       std::cout << std::endl <<i << "   is not UFS arg" << std::endl;
     }
   }

   EXPECT_EQ( org_tup_r1 , ins_tup_r1 );

   delete r1;
   delete r2;
}

#pragma mark mapUFCtoSym
//Testing mapUFCtoSym: get a map of all UFCalls to equ. symbolic constant
TEST_F(SetRelationTest, mapUFCtoSym) {

    iegenlib::setCurrEnv(); // Clears out the environment
    iegenlib::appendCurrEnv("col", new Set("{[i]: 0 <= i && i < nnz}"),
        new Set("{[j]: 0<=j && j < n}"), true, iegenlib::Monotonic_NONE );
    iegenlib::appendCurrEnv("idx", new Set("{[i]: 0 <= i && i < n}"),
        new Set("{[j]: 0<=j && j < nnz}"), true, iegenlib::Monotonic_Increasing );
    Relation *r = new Relation("[n] -> { [i,j] -> [ip,jp] : i = col(jp) "
       "and i < ip and 0 <= i and i < n and idx(i) <= j and j < idx(i+1) "
         "and 0 <= ip and ip < n and idx(ip) <= jp and jp < idx(ip+1) }");

    //! Creating expected expression
    std::stringstream ss;
    ss<<"UFCallMap:" << std::endl;
    ss<<"\tUFC = col(__tv3)  ,  sym = col___tv3_"<< std::endl;
    ss<<"\tUFC = idx(__tv0)  ,  sym = idx___tv0_"<< std::endl;
    ss<<"\tUFC = idx(__tv0 + 1)  ,  sym = idx___tv0P1_"<< std::endl;
    ss<<"\tUFC = idx(__tv2)  ,  sym = idx___tv2_"<< std::endl;
    ss<<"\tUFC = idx(__tv2 + 1)  ,  sym = idx___tv2P1_"<< std::endl;
    string exp_str = ss.str();

    iegenlib::UFCallMap *map = new iegenlib::UFCallMap();

    // ---------        Getting a map of UFCalls    ---------------
    r->superAffineRelation(map);


    EXPECT_EQ( exp_str , map->toString() );


//    std::cout<<std::endl<<map->toString()<<std::endl;
    delete r;
    delete map;
}


#pragma mark boundDomainRange
//Testing boundDomainRange: bounding by domain and range of UFCalls
TEST_F(SetRelationTest, boundDomainRange) {

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("col",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("idx",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<m}"), true, iegenlib::Monotonic_NONE);

    Set *s = new Set("[n] -> { [i,j,ip,jp] : i = col(jp) "
       "and i < ip and 0 <= i and i < n and idx(i) <= j and j < idx(i+1) "
         "and 0 <= ip and ip < n and idx(ip) <= jp and jp < idx(ip+1) }");

    Set *ex_s = new Set("{ [i, j, ip, jp] : i - col(jp) = 0 && i >= 0 &&"
                  " ip >= 0 && jp >= 0 && col(jp) >= 0 && idx(i) >= 0 &&"
  " idx(i + 1) >= 0 && idx(ip) >= 0 && idx(ip + 1) >= 0 && i + 1 >= 0 &&"
                " j - idx(i) >= 0 && ip + 1 >= 0 && jp - idx(ip) >= 0 &&"
             " -i + ip - 1 >= 0 && -i + n - 2 >= 0 && -i + n - 1 >= 0 &&"
   " -j + idx(i + 1) - 1 >= 0 && -ip + n - 2 >= 0 && -ip + n - 1 >= 0 &&"
          " -jp + m - 1 >= 0 && -jp + idx(ip + 1) - 1 >= 0 && m - idx(i)"
      " - 1 >= 0 && m - idx(i + 1) - 1 >= 0 && m - idx(ip) - 1 >= 0 && m"
                      " - idx(ip + 1) - 1 >= 0 && n - col(jp) - 1 >= 0 }");

    Relation* r = new Relation("[n] -> { [i,j] -> [ip,jp] : i = col(jp) "
       "and i < ip and 0 <= i and i < n and idx(i) <= j and j < idx(i+1) "
         "and 0 <= ip and ip < n and idx(ip) <= jp and jp < idx(ip+1) }");

    Relation *ex_r = new Relation("{ [i, j] -> [ip, jp] : i - col(jp) = "
    "0 && i >= 0 && ip >= 0 && jp >= 0 && col(jp) >= 0 && idx(i) >= 0 &&"
  " idx(i + 1) >= 0 && idx(ip) >= 0 && idx(ip + 1) >= 0 && i + 1 >= 0 &&"
                " j - idx(i) >= 0 && ip + 1 >= 0 && jp - idx(ip) >= 0 &&"
             " -i + ip - 1 >= 0 && -i + n - 2 >= 0 && -i + n - 1 >= 0 &&"
   " -j + idx(i + 1) - 1 >= 0 && -ip + n - 2 >= 0 && -ip + n - 1 >= 0 &&"
          " -jp + m - 1 >= 0 && -jp + idx(ip + 1) - 1 >= 0 && m - idx(i)"
      " - 1 >= 0 && m - idx(i + 1) - 1 >= 0 && m - idx(ip) - 1 >= 0 && m"
                      " - idx(ip + 1) - 1 >= 0 && n - col(jp) - 1 >= 0 }");


    //!  ----------------   Testing boundDomainRange for Set ------------

    s->boundDomainRange();
    Set* extendedS = s->boundDomainRange();
//    std::cout<<std::endl<<ns->prettyPrintString()<<std::endl;

    EXPECT_EQ( ex_s->prettyPrintString() , extendedS->prettyPrintString() );


    //!  ----------------   Testing boundDomainRange for Relation -------

    r->boundDomainRange();
    Relation* extendedR = r->boundDomainRange();
//    std::cout<<std::endl<<nr->prettyPrintString()<<std::endl;

    EXPECT_EQ( ex_r->prettyPrintString() , extendedR->prettyPrintString() );


    delete s;
    delete extendedS;
    delete ex_s;
    delete r;
    delete extendedR;
    delete ex_r;
}

#pragma mark superAffineSet
//Testing superAffineSet/Relation: creating super affine Sets
TEST_F(SetRelationTest, superAffineSet) {

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("col",
        new Set("{[i,t]:0<=i && i< m && 0<=t && t< m}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("idx",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<m}"), true, iegenlib::Monotonic_NONE);

    iegenlib::UFCallMap *ufcmap = new iegenlib::UFCallMap();

    //!  ----------------   Testing superAffineSet     ------------

    Set *s1 = new Set("[n] -> { [i,j] : idx(col(i,j)) < n}");

    std::string ex_s1 ("{ [i, j] : __tv0 >= 0 && __tv1 >= 0 &&"
       " col___tv0__tv1_ >= 0 && idx_col___tv0__tv1__ >= 0 &&"
            " -__tv0 + m - 1 >= 0 && -__tv1 + m - 1 >= 0 &&"
    " -col___tv0__tv1_ + n - 1 >= 0 && -idx_col___tv0__tv1__"
       " + m - 1 >= 0 && -idx_col___tv0__tv1__ + n - 1 >= 0 }");

    //! Getting the superAffineSet
    Set* su_s1 = s1->superAffineSet(ufcmap);
//    std::cout<<std::endl<<su_s1->toString()<<std::endl;

    EXPECT_EQ( ex_s1 , su_s1->toString() );

    delete ufcmap;

    Set* s2 = new Set( "{[i]: 0<=idx(i)[0] && idx(i)[1]<Nv}");

    std::string ex_s2 ("{ [i] : __tv0 >= 0 && idx___tv0_B0B >= 0"
               " && idx___tv0_B1B >= 0 && -__tv0 + n - 1 >= 0 &&"
                 " Nv - idx___tv0_B1B - 1 >= 0 && -idx___tv0_B0B"
                 " + m - 1 >= 0 && -idx___tv0_B1B + m - 1 >= 0 }");

    ufcmap = new iegenlib::UFCallMap();
    //! Getting the superAffineSet
    Set* su_s2 = s2->superAffineSet(ufcmap);
//    std::cout<<std::endl<<su_s2->toString()<<std::endl;

    EXPECT_EQ( ex_s2 , su_s2->toString() );

    delete ufcmap;

    //!  ----------------   Testing superAffineRelation  ---------

    Relation* r1 = new Relation("[n] -> { [i,j] -> [ip,jp] :"
       " i = col(jp,idx(j)) and i < ip and ip < n }");

    std::string ex_r1 ("{ [i, j] -> [ip, jp] :"
  " __tv0 - col___tv3idx___tv1__ = 0 && __tv1 >= 0 && __tv3 >= 0 &&"
         " col___tv3idx___tv1__ >= 0 && idx___tv1_ >= 0 && -__tv0 +"
       " __tv2 - 1 >= 0 && -__tv1 + n - 1 >= 0 && -__tv2 + n - 1 >="
            " 0 && -__tv3 + m - 1 >= 0 && -col___tv3idx___tv1__ + n"
                       " - 1 >= 0 && -idx___tv1_ + m - 1 >= 0 }");

    ufcmap = new iegenlib::UFCallMap();
    Relation* su_r1 = r1->superAffineRelation(ufcmap);
//    std::cout<<std::endl<<su_r1->toString()<<std::endl;

    EXPECT_EQ( ex_r1 , su_r1->toString() );

    delete ufcmap;

    delete s1;
    delete su_s1;
    delete s2;
    delete su_s2;
    delete r1;
    delete su_r1;

}

#pragma mark reverseAffineSubstitution
//Testing reverseAffineSubstitution: creating sub non-affine Sets
TEST_F(SetRelationTest, reverseAffineSubstitution) {

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("col",
        new Set("{[i,t]:0<=i && i< m && 0<=t && t< m}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("idx",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<m}"), true, iegenlib::Monotonic_NONE);

    iegenlib::UFCallMap *ufcmap = new iegenlib::UFCallMap();

    //! --------   Testing reverseAffineSubstitution for Set ---

    Set *s1 = new Set("[n] -> { [i,j] : idx(col(i,j)) < n}");

    Set *ex_s1 = new Set ("{ [i, j] : i >= 0 && j >= 0 && col(i,j) >= 0 &&"
          " idx(col(i, j)) >= 0 && i < m && j < m && idx(col(i, j)) < m &&"
                                     " col(i, j) < n && idx(col(i,j))< n }");

    //! Getting the superAffineSet
    Set* su_s1 = s1->superAffineSet(ufcmap);

    //! Getting the reverseAffineSubstitution
    Set* sub_s1 = su_s1->reverseAffineSubstitution(ufcmap);
//    std::cout<<std::endl<<sub_s1->toString()<<std::endl;

    EXPECT_EQ( ex_s1->toString() , sub_s1->toString() );

    delete ufcmap;

    Set* s2 = new Set( "{[i]: 0<=idx(i)[0] && idx(i)[1]<Nv}");

    Set *ex_s2 = new Set ("{ [i] : i >= 0 && idx(i)[0] >= 0 && idx(i)[1] >= 0"
            " && i < n && idx(i)[1] < Nv && idx(i)[0] < m && idx(i)[1] < m}");

    ufcmap = new iegenlib::UFCallMap();
    //! Getting the superAffineSet
    Set* su_s2 = s2->superAffineSet(ufcmap);

     //! Getting the reverseAffineSubstitution
    Set* sub_s2 = su_s2->reverseAffineSubstitution(ufcmap);
//    std::cout<<std::endl<<sub_s2->toString()<<std::endl;

    EXPECT_EQ( ex_s2->toString() , sub_s2->toString() );

    delete ufcmap;

    //! --------   Testing reverseAffineSubstitution  for Relation ---

    Relation* r1 = new Relation("[n] -> { [i,j] -> [ip,jp] :"
       " i = col(jp,idx(j)) and i < ip and ip < n }");

    Relation* ex_r1 = new Relation("{ [i, j] -> [ip, jp] : i = col(jp, idx(j))"
      " && j >= 0 && jp >= 0 && col(jp, idx(j)) >= 0 && idx(j) >= 0 && i < ip"
       " && j < n && ip < n && jp < m && idx(j)< m && col(jp, idx(j)) < n }");

    ufcmap = new iegenlib::UFCallMap();
    Relation* su_r1 = r1->superAffineRelation(ufcmap);

    //! Getting the reverseAffineSubstitution
    Relation* sub_r1 = su_r1->reverseAffineSubstitution(ufcmap);
//    std::cout<<std::endl<<sub_r1->toString()<<std::endl;

    EXPECT_EQ( ex_r1->toString() , sub_r1->toString() );

    delete ufcmap;

    delete s1;
    delete su_s1;
    delete s2;
    delete su_s2;
    delete r1;
    delete su_r1;
}


#pragma mark projectOut
// Testing projectOut: project out tuple variable # tvar
TEST_F(SetRelationTest, projectOut) {

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("col",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("idx",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("row",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);
    iegenlib::appendCurrEnv("diag",
        new Set("{[i]:0<=i &&i<n}"), 
        new Set("{[j]:0<=j &&j<n}"), true, iegenlib::Monotonic_NONE);

   // Note that, current project_out function first checks to see
   // whether the tuple variable that is going to be projected out
   // is argument to any UFS, if it is, it terminates, and returns NULL.
   // However, if the tuple variable is not argument to any UFS,
   // it will project it out, and return new Relation/Set.
   // Additionally, the function adds constraints that are due to domain
   //  and range info of all UFCall terms in the Set/Relation, for instance:
   //  if we have: col( x ) + y > 5    then  
   //                             ldb <= x <= udb and lrb <= col(x) <= urb
   //              will be added to constraints.
   //              * ldb = lower domain bound, urb = upper range bound

    Relation *r1 = new Relation("{ [i,k] -> [ip,kp] :  i = kp and col(i) < n"
                                     " and i < ip and diag(col(i))+1 <= k }");

    Relation *ex_r1 = new Relation("{ [i] -> [ip] : i < ip  and "
     "0 <= i and i < n and col(i) >= 0 and diag(col(i)) >= 0 and"
                             " col(i) < n and diag(col(i)) < n }");

    Relation *r2; 
    r2 = r1->projectOut(3);    // 3 == index of 'kp'
    if ( r2 ){                 // Did we project out 'jp': YES!
        delete r1;             // removing old r1
        r1 = r2;
    }

    r2 = r1->projectOut(0);    // 0 == index of 'i'
    if ( r2 ){                 // Did we project out 'k': NO!
        delete r1;             // The reason is that k is argumnet to col().
        r1 = r2;               // We don't project out variables
    }                          // that are argument to an UFCall.

    r2 = r1->projectOut(1);    // 2 == index of 'k'
    if ( r2 ){                 // did we project out 'j': YES!
        delete r1;             // removing old r1
        r1 = r2;
    }

//   std::cout << std::endl << "r1 = " << r1->toISLString() << std::endl;

   EXPECT_EQ( ex_r1->toISLString() , r1->toISLString() );

    Set *s1 = new Set("{ [i,j,ip,jp] : i = col(jp)+1 and 0 <= i and i < n"
                                     " and idx(i) <= j and j < idx(i+1) }");

    Set *ex_s1 = new Set("{ [i, jp] : i = col(jp)+1 and jp >= 0 and "
          "idx(i) >= 0 and idx(i+1) >= 0 and col(jp) >= 0 and jp < n and"
  " col(jp)+2 < n and idx(i) < n and idx(i+1) < n and idx(i) < idx(i + 1) }");

   Set *s2;
   // projectOut has the same behaivor for both Relation and Set

  // Projecting out 'j' from s1
  s2 = s1->projectOut(1);     // 1 == index of 'j'

  if ( s2 ){
     delete s1;               // removing old s1    
     s1 = s2;
  }
  // Projecting out 'ip' from s1
  s2 = s1->projectOut(1);     // 1 == index of 'ip' (in new results)

  if ( s2 ){
     delete s1;               // removing old s1    
     s1 = s2;
  }
//   std::cout << std::endl << "s1 = " << s1->toISLString() << std::endl;

   EXPECT_EQ( ex_s1->toISLString() , s1->toISLString() );

   delete r1;
   delete s1;
   delete ex_r1;
   delete ex_s1;
}
#if 0
#pragma mark debuggingForILU
TEST_F(SetRelationTest, debuggingForILU){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

   Set *islEqSet = new Set("{[i,ip,k,kp]: "
                                  "i < 1+colidx(kp) && colidx(kp) < i+1}");

   // ISL should find that i=colidx(kp).
   // Also normalization adds in the domain and range bounds for UF.
   Set *ex_islEqSet = new Set("{[i,ip,k,kp]:  i = colidx(kp) "
                                "&& 0<=kp && kp<nnz "
                                "&& 0<=colidx(kp) && colidx(kp)<m}"); 

   // Check and see if ISL library adds equality when we pass a set through it
   // with a < b+1 and b < a+1 constraints. Results should have a = b.   
   islEqSet->normalize();
   EXPECT_EQ( ex_islEqSet->toISLString() , islEqSet->toISLString() );

   // Check addConstraintsDueToMonotonicity when partial ordering is
   // given explicitly.  From Figure 5 in paper draft.
   Set *partOrd = new Set("{[i,ip,k,kp,j1,j1p,j2,j2p]: "
                          "rowptr(i) <= k && k < j1 && "
                          "j1 < rowptr(i+1) && j1 = j2p && "
                          "rowptr(colidx(kp)) <= diagptr(colidx(kp)) && "
                          "diagptr(colidx(kp)) < j2p && "
                          "j2p < rowptr(colidx(kp)+1) }");

   // The constraints i<1+colidx(kp) and colidx(kp)<i+1 should be added.
   // Also will have domain and range bounds.
   Set *ex_partOrd = new Set("{[i,ip,k,kp,j1,j1p,j2,j2p]: "
                             "rowptr(i) <= k && k < j1 && "
                             "j1 < rowptr(i+1) && j1 = j2p && "
                             "rowptr(colidx(kp)) <= diagptr(colidx(kp)) && "
                             "diagptr(colidx(kp)) < j2p && "
                             "j2p < rowptr(colidx(kp)+1) && "
                             "i<1+colidx(kp) && colidx(kp)<i+1 && "
                             "0<=kp && kp<nnz && "
                             "0<=colidx(kp) && colidx(kp)<m && "
                             "0<=i && i<m && "
                             "0<=colidx(kp) && colidx(kp)<m && "
                             "0<=colidx(kp)+1 && colidx(kp)+1<m &&"
                             "0<=rowptr(i) && rowptr(i)<nnz && "
                             "0<=rowptr(colidx(kp)) && "
                             "rowptr(colidx(kp))<nnz && "
                             "0<=rowptr(i+1) && rowptr(i+1)<nnz && "
                             "0<=i+1 && i+1<m && "
                             "0<=diagptr(colidx(kp)) && "
                             "diagptr(colidx(kp))<nnz && "
                             "0<=rowptr(colidx(kp)+1) && "
                             "rowptr(colidx(kp)+1)<nnz }");


   // Adding constraints due to monotonicity. This also considers constraints
   // that should be added based on partial ordering.

   Set* extendedPartOrd = partOrd->boundDomainRange();
   Set* partOrdMont = extendedPartOrd->addConstraintsDueToMonotonicity();

   EXPECT_EQ( ex_partOrd->toISLString() , partOrdMont->toISLString() );

   delete islEqSet;
   delete partOrd;
   delete extendedPartOrd;
   delete partOrdMont;
}
#endif

#pragma mark numUFCallConstsMustRemove
TEST_F(SetRelationTest, numUFCallConstsMustRemove){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

    Set *F1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(kp))"
                       " && diagptr(k) = 1+colidx(kp)"
                                     " && k = kp}");


   // Adding constraints due to domain and range of UFCs
   Set* extendedF1 = F1->boundDomainRange();
   std::set<Exp> domainRangeConsts = extendedF1->constraintsDifference(F1);

   int KPcount = F1->numUFCallConstsMustRemove(3, domainRangeConsts);

   EXPECT_EQ( 2 , KPcount );

   int Kcount = F1->numUFCallConstsMustRemove(2, domainRangeConsts);

   EXPECT_EQ( 3 , Kcount );

//std::cout<<"\n\nKcount = "<<Kcount<<"  KPcount = "<<KPcount<<"\n\n";

   delete F1;
   delete extendedF1;
}

#pragma mark removeUFCallConsts
TEST_F(SetRelationTest, removeUFCallConsts){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

    Set *F1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(kp))"
                       " && diagptr(k) = 1+colidx(kp)"
                                     " && k = kp}");

   F1->removeUFCallConsts(3);

   Set* ex1_F1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                                     " && k = kp}");

   EXPECT_EQ( ex1_F1->toISLString() , F1->toISLString() );


   F1->removeUFCallConsts(2);

   Set* ex2_F1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                     " && k = kp}");

   EXPECT_EQ( ex2_F1->toISLString() , F1->toISLString() );


    Set *F2 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(kp))"

                                     " && k = kp}");
   
   // Adding domain and range constraints before removing UFCall constraints
   Set *extendedF2 = F2->boundDomainRange();
   extendedF2->removeUFCallConsts(2);

   Set* ex_F2 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                                    " && k >= 0 && kp >= 0"
                                    " && k < nnz && kp < nnz"
                                    " && rowptr(i) >= 0"
                                    " && diagptr(i) >= 0"
                                    " && colidx(kp) >= 0"
                                  " && colidx(kp) + 1 >= 0"
                                  " && colidx(kp) + 1 >= 0"
                              " && rowptr(1+colidx(kp)) >= 0"
                                    " && rowptr(i) < nnz"
                                    " && diagptr(i) < nnz"
                                    " && colidx(kp) < m"
                                  " && colidx(kp) < m"
                                  " && colidx(kp) + 1 < m"
                              " && rowptr(1+colidx(kp)) < nnz"

                                     " && k = kp}");

   EXPECT_EQ( ex_F2->toISLString() , extendedF2->toISLString() );

//std::cout<<"\n\nF1 = "<<F1->toISLString()<<"\n\n"; 

   delete F1;
   delete ex1_F1;
   delete ex2_F1;
   delete F2;
   delete ex_F2;
   delete extendedF2;
}

/*
#pragma mark domainInfoT
TEST_F(SetRelationTest, domainInfoT){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

    Set *F1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && 0 <= i && i < m"
                                  " && 0 <= ip && ip < m"
                           " && rowptr(i) <= k && k < diagptr(i)"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(kp))"
                       " && diagptr(k) = 1+colidx(kp)"
                                     " && k = kp}");

    json data;

    Set *res = F1->domainInfo(data, 0);
  
}
*/


#pragma mark getString
TEST_F(SetRelationTest, getString){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

    Set *s1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                   " && -5 <= i < m"
                                  " && 0 <= ip && ip+1 <= m"
                           " && rowptr(i) <= k && k+10 < diagptr(i)"
                         " && rowptr(ip) <= kp < 10"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(k) = 1+colidx(kp)"
                                     " && k-3 = kp}");

    Set *s1_ex = new Set((s1->getString()).c_str());

    EXPECT_EQ( string("{ [i, ip, k, kp] : rowptr(colidx(k) + 1) = diagptr(colidx(k)) "
                                      "&& kp + 3 = k && diagptr(k) = colidx(kp) + 1 "
                                      "&& 0 <= ip && -6 < i && rowptr(i) <= k && kp <= 9 "
                                      "&& rowptr(ip) <= kp && i < ip && i < m && ip < m "
                                      "&& k + 10 < diagptr(i) }")
                     , s1->getString());
    
    EXPECT_EQ( *s1_ex , *s1 );

    Set *s2 = new Set("[m] -> {[i,ip,k,kp]: i < ip && 0 < m"
                                   " && -5 <= i < m"
                         " && rowptr(ip) <= kp < 10"
                       " && diagptr(colidx(k)) = 1"
                       " && i = 1+colidx(kp)}");
    Set *s2_ex = new Set((s2->getString()).c_str());
    EXPECT_EQ( *s2_ex , *s2 );



    Relation *rel1 = new Relation("[m] -> {[i,k] -> [ip,kp]: i < ip"
                                   " && 0 <= i < m"
                                  " && 0 <= ip <= 10"
                           " && rowptr(i) <= k && k-10 < diagptr(i)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(kp))"
                                     " && k+3 = kp}");

    Relation *rel1_ex = new Relation((rel1->getString()).c_str());


    EXPECT_EQ( string("{ [i, k] -> [ip, kp] : rowptr(colidx(k) + 1) = diagptr(colidx(k)) "
                                          "&& rowptr(colidx(kp) + 1) = diagptr(colidx(k)) "
                                          "&& kp = k + 3 && 0 <= i && 0 <= ip && rowptr(i) <= k "
                                          "&& ip <= 10 && i < ip && i < m && k <= diagptr(i) + 9 }") 
                      , rel1->getString());
    EXPECT_EQ( *rel1_ex , *rel1 );


   //std::cout << "\n\n--- getString --- \n\nSet = " 
   //          << s->getString()<<"\n\nRelation = "<<rel->getString()<<"\n\n";

   delete s1;
   delete rel1;
   delete s1_ex;
   delete rel1_ex;
}


#pragma mark getZ3form
TEST_F(SetRelationTest, getZ3form){

    iegenlib::setCurrEnv();
    iegenlib::appendCurrEnv("colidx",
            new Set("{[i]:0<=i &&i<nnz}"),         // Domain 
            new Set("{[j]:0<=j &&j<m}"),           // Range
            false,                                 // Not bijective.
            iegenlib::Monotonic_NONE               // no monotonicity
            );
    iegenlib::appendCurrEnv("rowptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);
    iegenlib::appendCurrEnv("diagptr",
        new Set("{[i]:0<=i &&i<m}"), 
        new Set("{[j]:0<=j &&j<nnz}"), false, iegenlib::Monotonic_Increasing);

    Set *s1 = new Set("[m] -> {[i,ip,k,kp]: i < ip"
                                  " && 0 <= ip <= m"
                           " && k+10 < diagptr(i)"
                       " && diagptr(colidx(k)) = rowptr(1+colidx(k))"
                                     " && k-3 = kp}");

    // UFSyms and VarSyms get populated with uninterpreted function symbols (UFSymbols),  
    // and symbolic constants found in the set. Then later when a driver actually  
    // generating a z3 file it can use them to define global variables and UFSymbols.

    std::set<std::string> UFSyms;
    std::set<std::string> VarSyms;
    std::vector<std::string> constrants = s1->getZ3form(UFSyms, VarSyms);
//    std::cout<<"\n>>> z3 Form:";
//    for(int i = 0 ; i < constrants.size(); i++)
//      std::cout<<"\n"<<constrants[i]<<"\n";

    // !! SparseConstraints::getZ3form returns a vector of strings that include 
    //    constraints represented in SMT-LIB format that SMT solvers like z3 
    //    gets as input. This can be used to check the satisfiability of 
    //    an IEGenLib Set/Relation with a SMT solver. The returned list
    //    also includes tuple variable declarations, however they do not include UFSymbol
    //    and global variable declarations. This is because, when checking satisfiability of
    //    a set, we usually also want to define some user defined assertions along side original
    //    constraints. The assertions might have UFSymbols and global variables of their
    //    own that original constraints do not. We can put their UFSymbols and globals into 
    //    UFSyms, and VarSyms std::set that SparseConstraints::getZ3form returns by reference,
    //    and then a driver function can declare all the UFSymbols and globals at the beginning of 
    //    the input file that it is going to generate and pass to a SMT solver.

    EXPECT_EQ( string("(declare-const i Int)") , constrants[0] ); // Just defining i variable
    EXPECT_EQ( string("(declare-const ip Int)") , constrants[1] ); // Just defining ip variable
    EXPECT_EQ( string("(declare-const k Int)") , constrants[2] ); // Just defining k variable
    EXPECT_EQ( string("(declare-const kp Int)" ) , constrants[3] ); // Just defining i variable

    EXPECT_EQ( string("(assert (! (= (rowptr (+ (colidx k) 1))"
                      " (diagptr (colidx k))) :named c11) )") , constrants[4] ); // defining diagptr(colidx(k)) = rowptr(1+colidx(k)) 
    EXPECT_EQ( string("(assert (! (= (+ kp 3) k) :named c12) )") , constrants[5] ); // defining i < ip
    EXPECT_EQ( string("(assert (! (<= 0 ip) :named c13) )") , constrants[6] ); // defining 0 <= ip
    EXPECT_EQ( string("(assert (! (<= ip m) :named c14) )") , constrants[7] ); // defining ip <= m
    EXPECT_EQ( string("(assert (! (< i ip) :named c15) )") , constrants[8] ); // defining i < ip
    EXPECT_EQ( string("(assert (! (< (+ k 10) (diagptr i)) :named c16) )") , constrants[9] ); // defining k+10 < diagptr(i)
    
   delete s1;
}
