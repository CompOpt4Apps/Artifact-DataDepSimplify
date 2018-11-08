/*!
 * \file TupleDecl_test.cc
 *
 * \brief Test for the TupleDecl class.
 *
 * \date Started: 1/25/2013
 * # $Revision:: 628                $: last committed revision
 * # $Date:: 2013-01-25             $: date of last committed revision
 * # $Author:: heim                 $: author of last committed revision
 *
 * \authors Michelle Strout, Joe Strout, Mark Heim
 *
 * Copyright (c) 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "expression.h"
#include "set_relation.h"
#include "../util/util.h"
#include "TupleDecl.h"

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

// Test the Term methods
TEST(TupleDeclTest, TupleDeclMethods) {

    // Test constructor from size specification
    // Test setTupleElem
    // Test toString
    TupleDecl t(3);
    t.setTupleElem(0, 1);
    t.setTupleElem(1, "a");
    t.setTupleElem(2, 2);
    EXPECT_EQ("[1, a, 2]", t.toString(true,0) );

    TupleDecl u(4);
    u.setTupleElem(0, "b");
    u.setTupleElem(1, 3);
    u.setTupleElem(2, "c");
    u.setTupleElem(3, 4);
    EXPECT_EQ( "[b] -> [3, c, 4]", u.toString(true,1));

    TupleDecl v(5);
    v.setTupleElem(0, "d");
    v.setTupleElem(1, 5);
    v.setTupleElem(2, "e");
    v.setTupleElem(3, 6);
    v.setTupleElem(4, "f");
    EXPECT_EQ("[d, 5, e] -> [6, f]", v.toString(true,3) );

    // Test size method
    EXPECT_EQ(t.size(),(unsigned int) 3);
    EXPECT_EQ(u.size(),(unsigned int) 4);
    EXPECT_EQ(v.size(),(unsigned int) 5);

    // Test copy constructor
    TupleDecl w(v);
    EXPECT_EQ(w.size(), (unsigned int) 5);
    EXPECT_EQ("[d, 5] -> [e, 6, f]", w.toString(true,2) );

    // Test assignment operator
    w = u;
    EXPECT_EQ(w.size(), (unsigned int) 4);
    EXPECT_EQ("[b, 3, c, 4]", w.toString(true,0) );

    // Test concatenate
    w.concat(u);
    EXPECT_EQ(w.size(), (unsigned int)  8);
    EXPECT_EQ("[b, 3, c, 4] -> [b, 3, c, 4]", w.toString(true, 4) );

    // Test clone
    TupleDecl *x;
    x = w.clone();
    EXPECT_EQ(x->size(), (unsigned int)  8);
    EXPECT_EQ("[b, 3, c, 4, b, 3] -> [c, 4]", x->toString(true,6) );

    // Test copyTupleElem
    x->copyTupleElem(*x, 0, 1);
    x->copyTupleElem(*x, 5, 6);
    EXPECT_EQ("b, b, c, 4, b, 3, 3, 4", x->toString() );

    // Test elemIsConst
    EXPECT_TRUE(t.elemIsConst(0));
    EXPECT_TRUE(!t.elemIsConst(1));
    EXPECT_TRUE(t.elemIsConst(2));

    // Test elemToString
    EXPECT_EQ(t.elemToString(0), "1");
    EXPECT_EQ(t.elemToString(1), "a");
    EXPECT_EQ(t.elemToString(2), "2");

    // Test elemConstVal
    EXPECT_EQ(t.elemConstVal(0), 1);
    EXPECT_EQ(t.elemConstVal(2), 2);

    // Test elemVarString
    EXPECT_EQ(t.elemVarString(1), "a");

    // Test elemCreateTerm
    Term *y = v.elemCreateTerm(0, 1);
    Term *z = v.elemCreateTerm(1, 1);
    EXPECT_EQ("__tv1", y->toString() );
    EXPECT_EQ("5", z->toString() );

    delete x;
    delete y;
    delete z;
}

// Test the operator<(): Variables
TEST(TupleDeclTest, TupleDeclLessThanByVariables) {
    //Variables in TupleDecl's are all equal/ignored in < comparison
    TupleDecl a(3); // a == b == c
    a.setTupleElem(0, "a");
    a.setTupleElem(1, "b");
    a.setTupleElem(2, "c");
    // a=[a,b,c]

    TupleDecl b(3); // a == b == c
    b.setTupleElem(0, "x");
    b.setTupleElem(1, "y");
    b.setTupleElem(2, "z");
    // b=[x,y,z]

    TupleDecl c(3); // a == b == c
    c.setTupleElem(0, "n");
    c.setTupleElem(1, "n");
    c.setTupleElem(2, "n");
    // c=[n,n,n]

    // a:[a,b,c] == b:[x,y,z]
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(b < a);

    // a:[a,b,c] == c:[n,n,n]
    EXPECT_FALSE(a < c);
    EXPECT_FALSE(c < a);

    // b:[x,y,z] == c:[n,n,n]
    EXPECT_FALSE(b < c);
    EXPECT_FALSE(c < b);
}

// Test the operator<(): Sizes
TEST(TupleDeclTest, TupleDeclLessThanBySizes) {
    //compare TupleDecl of different sizes
    TupleDecl s(3);
    s.setTupleElem(0, 1);
    s.setTupleElem(1, "a");
    s.setTupleElem(2, 2);
    // s=[1,a,2]

    TupleDecl u(4);
    u.setTupleElem(0, "b");
    u.setTupleElem(1, 3);
    u.setTupleElem(2, "c");
    u.setTupleElem(3, 4);
    // u=[b,3,c,4]

    TupleDecl x(5);
    x.setTupleElem(0, "d");
    x.setTupleElem(1, 5);
    x.setTupleElem(2, "e");
    x.setTupleElem(3, 6);
    x.setTupleElem(4, "f");
    // x=[d,5,e,6,f]

    // s:[1,a,2] < u:[b,3,c,4]: s.size= 3 < u.size=4
    EXPECT_TRUE( s < u);
    EXPECT_FALSE(u < s);

    // s:[1,a,2] < x:[d,5,e,6,f]: s.size= 3 < u.size=5
    EXPECT_TRUE( s < x);
    EXPECT_FALSE(x < s);

    // u:[b,3,c,4] < x:[d,5,e,6,f]: u.size=4 < x.size=5
    EXPECT_TRUE( u < x);
    EXPECT_FALSE(x < u);
}

// Test the operator<(): Constants
TEST(TupleDeclTest, TupleDeclLessThanByConstants) {
    //Comparing all constants, w/ equal size
    TupleDecl f(3); // f < g < h
    f.setTupleElem(0, 3);
    f.setTupleElem(1, 5);
    f.setTupleElem(2, 7);
    // f=[3,5,7]

    TupleDecl g(3); // f < g < h
    g.setTupleElem(0, 4);
    g.setTupleElem(1, 5);
    g.setTupleElem(2, 7);
    // g=[4,5,7]

    TupleDecl h(3); // f < g < h
    h.setTupleElem(0, 4);
    h.setTupleElem(1, 5);
    h.setTupleElem(2, 8);
    // h=[4,5,8]

    // f:[3,5,7] < g:[4,5,7]: elem 0's
    EXPECT_TRUE( f < g);
    EXPECT_FALSE(g < f);

    // g:[4,5,7] < h:[4,5,8]: elem 3's
    EXPECT_TRUE( g < h);
    EXPECT_FALSE(h < g);

    // f:[3,5,7] < h:[4,5,8]: elem 0's
    EXPECT_TRUE( f < h);
    EXPECT_FALSE(h < f);
}

// Test the operator<(): Mixed Variables and Constants
TEST(TupleDeclTest, TupleDeclLessThanByMixedVarsConsts) {
    //Comparing all constants, w/ equal size
    TupleDecl f(3); // f < g < h
    f.setTupleElem(0, "a");
    f.setTupleElem(1, "b");
    f.setTupleElem(2, "c");
    // f=[a,b,c]

    TupleDecl g(3); // f < g < h
    g.setTupleElem(0, "a");
    g.setTupleElem(1, 1);
    g.setTupleElem(2, "c");
    // g=[a,1,c]

    TupleDecl h(3); // f < g < h
    h.setTupleElem(0, "a");
    h.setTupleElem(1, 1);
    h.setTupleElem(2, 2);
    // h=[a,1,2]

    TupleDecl i(3); // f < g < h
    i.setTupleElem(0, "b");
    i.setTupleElem(1, 1);
    i.setTupleElem(2, 2);
    // i=[b,1,2]

    // f:[a,b,c] < g:[a,1,c]: elem 1's: var < const
    EXPECT_TRUE( f < g);
    EXPECT_FALSE(g < f);

    // f:[a,b,c] < h:[a,1,2]: elem 2's: var < const
    EXPECT_TRUE( f < h);
    EXPECT_FALSE(h < f);

    // f:[a,b,c] < i:[b,1,2]: elem 2's: var < const
    EXPECT_TRUE( f < i);
    EXPECT_FALSE(i < f);

    // g:[a,1,c] < h:[a,1,2]: elem 3's: var < const
    EXPECT_TRUE( g < h);
    EXPECT_FALSE(h < g);

    // g:[a,1,c] < i:[b,1,2]: elem 3's: var < const
    EXPECT_TRUE( g < i);
    EXPECT_FALSE(i < g);

    // h:[a,1,2] == i:[b,1,2]: any var == any var
    EXPECT_FALSE(h < i);
    EXPECT_FALSE(i < h);
}

// Test the operator<(): Equal Items
TEST(TupleDeclTest, TupleDeclLessThanByEqualItems) {
//create some TupleDelc's to work with
    TupleDecl s(3);
    s.setTupleElem(0, 5);
    s.setTupleElem(1, "d");
    s.setTupleElem(2, 4);
    // s=[5,d,4]

    TupleDecl t(s); // t == s
    // t=[5,d,4]

    TupleDecl u(4);
    u.setTupleElem(0, 6);
    u.setTupleElem(1, "d");
    u.setTupleElem(2, "n");
    u.setTupleElem(3, "c");
    // u=[6,d,n,c]

    TupleDecl v(u); // u == v
    // v=[6,d,n,c]

    TupleDecl x(5);
    x.setTupleElem(0, 7);
    x.setTupleElem(1, 2);
    x.setTupleElem(2, 3);
    x.setTupleElem(3, "g");
    x.setTupleElem(4, "f");
    // x=[7,2,3,g,f]

    TupleDecl y(x); // x == y
    // y=[7,2,3,g,f]

    // s:[5,d,4] == t:[5,d,4]
    EXPECT_FALSE(s < t);
    EXPECT_FALSE(t < s);

    // u:[6,d,n,c] == v:[6,d,n,c]
    EXPECT_FALSE(u < v);
    EXPECT_FALSE(v < u);

    // x:[7,2,3,g,f] == y:[7,2,3,g,f]
    EXPECT_FALSE(x < y);
    EXPECT_FALSE(y < x);
}

// Test the operator==(): Equal Items
TEST(TupleDeclTest, TupleDeclEqualsWithEqualItems) {
//create some TupleDelc's to work with
    TupleDecl s(3);
    s.setTupleElem(0, 5);
    s.setTupleElem(1, "d");
    s.setTupleElem(2, 4);
    // s=[5,d,4]

    TupleDecl t(s); // t == s
    // t=[5,d,4]

    TupleDecl u(4);
    u.setTupleElem(0, 6);
    u.setTupleElem(1, "d");
    u.setTupleElem(2, "n");
    u.setTupleElem(3, "c");
    // u=[6,d,n,c]

    TupleDecl v(u); // u == v
    // v=[6,d,n,c]

    TupleDecl x(5);
    x.setTupleElem(0, 7);
    x.setTupleElem(1, 2);
    x.setTupleElem(2, 3);
    x.setTupleElem(3, "g");
    x.setTupleElem(4, "f");
    // x=[7,2,3,g,f]

    TupleDecl y(x); // x == y
    // y=[7,2,3,g,f]

    // s:[5,d,4] == t:[5,d,4]
    EXPECT_TRUE(s == t);
    EXPECT_TRUE(t == s);

    // u:[6,d,n,c] == v:[6,d,n,c]
    EXPECT_TRUE(u == v);
    EXPECT_TRUE(v == u);

    // x:[7,2,3,g,f] == y:[7,2,3,g,f]
    EXPECT_TRUE(x == y);
    EXPECT_TRUE(y == x);

    //a couple self comparisons also
    EXPECT_TRUE(s == s);
    EXPECT_TRUE(u == u);
}

// Test the operator==(): NonEqual Items
TEST(TupleDeclTest, TupleDeclEqualsWithNonEqualItems) {
//create some TupleDelc's to work with
    TupleDecl s(3);
    s.setTupleElem(0, 5);
    s.setTupleElem(1, "d");
    s.setTupleElem(2, 4);
    // s=[5,d,4]

    TupleDecl t(3);
    t.setTupleElem(0, 2);
    t.setTupleElem(1, "d");
    t.setTupleElem(2, 4);
    // t=[2,d,4]

    TupleDecl u(4);
    u.setTupleElem(0, "a");
    u.setTupleElem(1, "b");
    u.setTupleElem(2, "c");
    u.setTupleElem(3, "d");
    // u=[a,b,c,d]

    TupleDecl v(4);
    v.setTupleElem(0, "a");
    v.setTupleElem(1, "b");
    v.setTupleElem(2, "c");
    v.setTupleElem(3, 1);
    // v=[a,b,c,1]

    TupleDecl x(5);
    x.setTupleElem(0, 1);
    x.setTupleElem(1, 2);
    x.setTupleElem(2, 3);
    x.setTupleElem(3, "g");
    x.setTupleElem(4, "f");
    // x=[1,2,3,g,f]

    TupleDecl y(5);
    y.setTupleElem(0, 1);
    y.setTupleElem(1, 2);
    y.setTupleElem(2, 3);
    y.setTupleElem(3, 4);
    y.setTupleElem(4, "f");
    // y=[1,2,3,4,f]

    // s:[5,d,4] != t:[2,d,4]
    EXPECT_FALSE(s == t);
    EXPECT_FALSE(t == s);

    // u:[a,b,c,d] != v:[a,b,c,1]
    EXPECT_FALSE(u == v);
    EXPECT_FALSE(v == u);

    // x:[1,2,3,g,f] != y:[1,2,3,4,f]
    EXPECT_FALSE(x == y);
    EXPECT_FALSE(y == x);
}

// Test the operator<(): Mixed Variables and Constants
TEST(TupleDeclTest, TupleDeclLessThanBugFixing) {
    //Check that this comparison works:
    // a:[x, y, 2, 0] < (since y < 3)
    // b:[x, 3, 1, y]
    TupleDecl a(4);
    a.setTupleElem(0, "x");
    a.setTupleElem(1, "y");
    a.setTupleElem(2, 2);
    a.setTupleElem(3, 0);

    TupleDecl b(4);
    b.setTupleElem(0, "x");
    b.setTupleElem(1, 3);
    b.setTupleElem(2, 1);
    b.setTupleElem(3, "y");

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}
