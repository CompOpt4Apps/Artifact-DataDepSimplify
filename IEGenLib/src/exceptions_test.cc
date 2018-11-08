/*!
 * \file exceptions_test.cc
 *
 * \brief Test exceptions for the parser/, set_relation/ and util/ classes.
 *
 * This file is to test all of the Exception, along with the
 * related classes: EqExp, IneqExp, UFCallTerm, VarTerm, and TupleVarTerm.
 *
 * \date Started: 5/30/12
 * # $Revision:: 391                $: last committed revision
 * # $Date:: 5/30/12                $: date of last committed revision
 * # $Author:: heim                 $: author of last committed revision
 *
 * \authors Mark Heim
 *
 * Copyright (c) 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include <parser/parser.h>
#include <util/util.h>

#include <gtest/gtest.h>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>

using namespace iegenlib;
using namespace iegenlib::parser;


/*!
 * \class ExceptionTestParser
 *
 * \brief Class to test exception handling in parser/ methods.
 *
 * This class holds gtest test cases that test the functionality of the methods
 * inside parser.cc in regards to exception handling.
 */
class ExceptionTestParser : public::testing::Test {
    public:
    protected:
    virtual void SetUp();
    virtual void TearDown();
};

/*!
 * \class ExceptionTestSetRelation
 *
 * \brief Class to test exception handling in set_relation/ methods.
 *
 * This class holds gtest test cases that test the functionality of the methods
 * inside set_relation.cc in regards to exception handling.
 */
class ExceptionTestSetRelation : public::testing::Test {
    public:
    protected:
    virtual void SetUp();
    virtual void TearDown();
};

/*!
 * \class ExceptionTestExp
 *
 * \brief Class to test exception handling in expression/ methods.
 *
 * This class holds gtest test cases that test the functionality of the methods
 * inside expression.cc in regards to exception handling.
 */
class ExceptionTestExp : public::testing::Test {
    public:
    protected:
    virtual void SetUp();
    virtual void TearDown();
};

void ExceptionTestParser::SetUp() {}
void ExceptionTestParser::TearDown() {}
void ExceptionTestSetRelation::SetUp() {}
void ExceptionTestSetRelation::TearDown() {}
void ExceptionTestExp::SetUp() {}
void ExceptionTestExp::TearDown() {}

/******************************** Parser **************************************/


// Tests exception handling in method with following signature in parser.cc:
// Set* parser::parse_set(string set_string)
// The method contains one throw statement.
TEST_F(ExceptionTestParser, ParseSetMethod)
{
    // Reserved variable name (test 1)
    std::string temp = "{[a,__z]}";
    EXPECT_THROW(parse_set(temp), parse_exception);

    // Reserved variable name (test 2)
    std::string temp2 = "{[__x,y,z]}";
    EXPECT_THROW(parse_set(temp2), parse_exception);

    // Reserved variable name (test 3)
    std::string temp3 = "{[__x]}";
    EXPECT_THROW(parse_set(temp3), parse_exception);
}

// Tests exception handling in method with following signature in parser.cc:
// Relation* parser::parse_relation(string relation_string)
// The method contains one throw statement.
TEST_F(ExceptionTestParser, ParseRelationMethod)
{
    // Reserved variable name (test 1)
    std::string temp = "{[a,__z]->[y]}";
    EXPECT_THROW(parse_relation(temp), parse_exception);

    // Reserved variable name (test 2)
    std::string temp2 = "{[a,t]->[__m] : __m = a + t}";
    EXPECT_THROW(parse_relation(temp2), parse_exception);
}


// Tests exception handling in method with following signature in parser.cc:
// Environment* parser::parse_env(std::string env_string)
// The method contains one throw statement.
TEST_F(ExceptionTestParser, ParseEnvMethod)
{
    // Malformed environment (test 1)
    std::string temp = "c = p and q =";
    EXPECT_THROW(parse_env(temp), parse_exception);

    // Malformed environment (test 2)
    std::string temp2 = "and 1 = 0";
    EXPECT_THROW(parse_env(temp2), parse_exception);
}

/****************************** SetRelation ***********************************/

/*! Tests exception handling in methods with following signatures in
**  set_relation.cc:
**  void Conjunction::setTupleElem(int location, int constVal)
**  void Conjunction::setTupleElem(int location, std::string varString)
**  These methods contains four throw statements.
*/
TEST_F(ExceptionTestSetRelation, SetTupleElemMethod)
{
    int inarity = 2;
    int arity = 4;
    std::string tempString = "x";
    Conjunction *testConj = new Conjunction(arity, inarity);

/* FIXME: tuples are declared differently now.
    // void Conjunction::setTupleElem(int location, int constVal)
    // Check lower bound on this method is enforced verified
    EXPECT_THROW(testConj->setTupleElem(-1, 5), assert_exception); // Test 1
    EXPECT_THROW(testConj->setTupleElem(-7, 10), assert_exception); // Test 2

    // void Conjunction::setTupleElem(int location, int constVal)
    // Check upper bound on this method is enforced verified
    EXPECT_THROW(testConj->setTupleElem(4, 5), assert_exception); // Test 1
    EXPECT_THROW(testConj->setTupleElem(7, 5), assert_exception); // Test 2

    // void Conjunction::setTupleElem(int location, std::string varString)
    // Check lower bound on this method is enforced verified
    // Test 1
    EXPECT_THROW(testConj->setTupleElem(-1, tempString), assert_exception);
    // Test 2
    EXPECT_THROW(testConj->setTupleElem(-9, tempString), assert_exception);

    // void Conjunction::setTupleElem(int location, std::string varString)
    // Check upper bound on this method is enforced verified
    // Test 1
    EXPECT_THROW(testConj->setTupleElem(4, tempString), assert_exception);
    // Test 2
    EXPECT_THROW(testConj->setTupleElem(10, tempString), assert_exception);
*/
    delete testConj;
}

/*! Tests exception handling in methods with following signatures in
**  set_relation.cc:
**  Conjunction *Conjunction::Compose(const Conjunction *rhs, 
**      int innerArity) const
**  Relation *Relation::Compose(const Relation *rhs) const
**  These methods contain two throw statements. 
*/
TEST_F(ExceptionTestSetRelation, ComposeMethod)
{
    // Check to see if arity on left of arg 1 equals arity on right of arg 2
    // (test 1)
    Relation *r = new Relation("{[a]->[b,c]}");
    Relation *s = new Relation("{[p,q]->[x,y,z]}");
    EXPECT_THROW(r->Compose(s), assert_exception);
    delete s;
    delete r;

    // Check to see if arity on left of arg 1 equals arity on right of arg 2
    // (test 2)
    Relation *p = new Relation("{[p,q,r,s]->[b,c]}");
    Relation *q = new Relation("{[m]->[n]}");
    EXPECT_THROW(p->Compose(q), assert_exception);
    delete q;
    delete p;

    // Relations must be functions or function inverses (test 1)
    Relation *t = new Relation("{[a]->[b]: f(a)=g(b)}");
    Relation *u = new Relation("{[p]->[q]: y(p)=z(q)}");
    // Need these two lines to extract conjunctions from the Relations
    const Conjunction *v = new Conjunction(**(t->conjunctionBegin()));
    const Conjunction *w = new Conjunction(**(u->conjunctionBegin()));
    EXPECT_THROW(v->Compose(w, 1), assert_exception);
    delete w;
    delete v;
    delete u;
    delete t;

    // Relations must be functions or function inverses (test 2)
    Relation *k = new Relation("{[a]->[b]: g(a)=g(b)}");
    Relation *l = new Relation("{[p]->[q]: y(p)=y(q)}");
    // Need these two lines to extract conjunctions from the Relations
    const Conjunction *m = new Conjunction(**(k->conjunctionBegin()));
    const Conjunction *n = new Conjunction(**(l->conjunctionBegin()));
    EXPECT_THROW(m->Compose(n, 1), assert_exception);
    delete n;
    delete m;
    delete k;
    delete l;
}

/*! Tests exception handling in method with following signatures in
**  set_relation.cc:
**  Set *Relation::Apply(const Set* rhs) const
**  Conjunction *Conjunction::Apply(const Conjunction *rhs) const
**  These methods contain two throw statements.
*/
TEST_F(ExceptionTestSetRelation, ApplyMethod)
{

    // Arity mismatch for Apply method (test 1)
    Relation *a = new Relation("{[l,m,n]->[z]}");
    Set *b = new Set("{[x,y]}");
    EXPECT_THROW(a->Apply(b), assert_exception);
    delete a;
    delete b;

    // Arity mismatch for Apply method (test 2)
    Relation *r = new Relation("{[a,b]->[c]}");
    Set *s = new Set("{[a]}");
    EXPECT_THROW(r->Apply(s), assert_exception);
    delete s;
    delete r;

    // Relation not invertible for Apply method (test 1)
    Relation *t = new Relation("{[a,b,c,d,e]->[f,g]: b = f(f + g(a + b))}");
    Set *u = new Set("{[a,b,c,d,e] : b = f(c + g(a + b))}");
    // Need these two lines to extract conjunctions from the Set and Relation
    const Conjunction *v = new Conjunction(**(t->conjunctionBegin()));
    const Conjunction *w = new Conjunction(**(u->conjunctionBegin()));
    EXPECT_THROW(v->Apply(w), assert_exception);
    delete w;
    delete v;
    delete u;
    delete t;

    // Relation not invertible for Apply method (test 2)
    Relation *k = new Relation("{[a,b,c,d,e]->[f,g]: b = f(f + g(a + b))}");
    Set *l = new Set("{[a,b,c,d,e] : b = f(c + g(a + b))}");
    // Need these two lines to extract conjunctions from the Set and Relation
    const Conjunction *m = new Conjunction(**(k->conjunctionBegin()));
    const Conjunction *n = new Conjunction(**(l->conjunctionBegin()));
    EXPECT_THROW(m->Apply(n), assert_exception);
    delete n;
    delete m;
    delete l;
    delete k;
}

/*! Tests exception handling in method with following signatures in
**  set_relation.cc:
**  Relation* Relation::Union(const Relation* rhs) const
**  Set* Set::Union(const Set* rhs) const
**  These methods contain two throw statements.
*/
TEST_F(ExceptionTestSetRelation, UnionMethod)
{

    // Arity mismatch for Relations (test 1)
    Relation *a = new Relation("{[a]->[b]}");
    Relation *b = new Relation("{[a,c]->[b]}");
    EXPECT_THROW(a->Union(b), assert_exception);
    delete a;
    delete b;

    // Arity mismatch for Relations (test 2)
    Relation *p = new Relation("{[a]->[b]}");
    Relation *q = new Relation("{[a]->[b,c]}");
    EXPECT_THROW(p->Union(q), assert_exception);
    delete q;
    delete p;

    // Arity mismatch for Sets (test 1)
    Relation *c = new Relation("{[a]->[b]}");
    Relation *d = new Relation("{[a,c]->[b]}");
    EXPECT_THROW(c->Union(d), assert_exception);
    delete d;
    delete c;

    // Arity mismatch for Sets (test 2)
    Relation *r = new Relation("{[a]->[b]}");
    Relation *s = new Relation("{[a]->[b,c]}");
    EXPECT_THROW(r->Union(s), assert_exception);
    delete r;
    delete s;

}

/****************************** Expression ************************************/

/*! Tests exception handling in method with following signature in
**  expression.cc:
**  std::string TupleVarTerm::prettyPrintString(
**      const std::vector<TupleElemDecl>& mTupleDecl, bool absValue) const
**  This method contains one throw statement.
*/
TEST_F(ExceptionTestExp, PrettyPrintStringMethod)
{
    // Ensure that prettyPrintString is not called with numeric TupleElemDecls.
    // Test 1
    TupleVarTerm *t = new TupleVarTerm(1);
    TupleDecl tdecl(3);
    tdecl.setTupleElem(0,1);
    tdecl.setTupleElem(1,2);
    tdecl.setTupleElem(2,3);
    /* FIXME, delete
    std::vector<TupleElemDecl> *v = new std::vector<TupleElemDecl>();
    TupleElemDecl *d = new TupleElemDecl(1);
    v->push_back(*d);
    TupleElemDecl *e = new TupleElemDecl(2);
    v->push_back(*e);
    TupleElemDecl *f = new TupleElemDecl(3);
    v->push_back(*f);
    */
    EXPECT_THROW(t->prettyPrintString(tdecl, false), assert_exception);
    // Delete TupleVarTerm object
    delete t;

    // Ensure that prettyPrintString is not called with numeric TupleElemDecls.
    // Test 1
    TupleVarTerm *t2 = new TupleVarTerm(2);
    TupleDecl tdecl2(3);
    tdecl2.setTupleElem(0,"x");
    tdecl2.setTupleElem(1,"y");
    tdecl2.setTupleElem(2,6);
    EXPECT_THROW(t2->prettyPrintString(tdecl2, true), assert_exception);

    // Delete TupleVarTerm object
    delete t2;
}

/*! Tests exception handling in method with following signature in
**  expression.cc:
**  void TupleVarTerm::remapLocation(const std::vector<int>& oldToNewLocs)
**  This method contains two throw statements.
*/
TEST_F(ExceptionTestExp, RemapLocationMethod)
{
    // Remap to location < 0 (test 1)
    std::vector<int> permutation;
    permutation.push_back(-1);
    permutation.push_back(2);
    TupleVarTerm *t = new TupleVarTerm(0);
    EXPECT_THROW(t->remapLocation(permutation), assert_exception);
    delete t;

    // Remap to location < 0 (test 2)
    std::vector<int> permutation2;
    permutation2.push_back(-7);
    permutation2.push_back(-7);
    TupleVarTerm *u = new TupleVarTerm(1);
    EXPECT_THROW(u->remapLocation(permutation2), assert_exception);
    delete u;

    // mLocation out of bounds (test 1)
    std::vector<int> permutation3;
    permutation3.push_back(1);
    permutation3.push_back(0);
    TupleVarTerm *v = new TupleVarTerm(-1);
    EXPECT_THROW(v->remapLocation(permutation3), assert_exception);
    delete v;

    // mLocation out of bounds (test 2)
    std::vector<int> permutation4;
    permutation4.push_back(1);
    permutation4.push_back(0);
    TupleVarTerm *w = new TupleVarTerm(-31);
    EXPECT_THROW(w->remapLocation(permutation4), assert_exception);
    delete w;
}

/*! Tests exception handling in method with following signature in
**  expression.cc:
**  Term* Exp::findMatchingFactor(const Term & factor) const
**  This method contains one throw statement.
**
**  This exception is unreachable in the code unless findMatchingFactor
**  is changed from being a private method into a public or protected method.
*/
TEST_F(ExceptionTestExp, FindMatchingFactorMethod)
{
    EXPECT_TRUE(true);
}

/*! Tests exception handling in method with following signature in
**  expression.cc:
**  Exp* Exp::solveForFactor(Term* factor, const Environment *env) const
**  This method contains one throw statement.
*/
TEST_F(ExceptionTestExp, SolveForFactorMethod)
{
    // Coefficient of input Term is not one.
    // Test 1
    Exp *e = new Exp();
    TupleVarTerm *t = new TupleVarTerm(3, 2);
    Environment *env = new Environment();
    EXPECT_THROW(e->solveForFactor(t), assert_exception);
    delete env;
    delete t;
    delete e;

    // Test 2
    // Coefficient of input Term is not one.
    Exp *e2 = new Exp();
    TupleVarTerm *t2 = new TupleVarTerm(7, 7);
    Environment *env2 = new Environment();
    EXPECT_THROW(e2->solveForFactor(t2), assert_exception);
    delete env2;
    delete t2;
    delete e2;
}

