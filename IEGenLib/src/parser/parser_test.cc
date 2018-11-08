/*! \file parser_test.cc
 *
 * \brief Test file for parsing sets and relations from an omega-like syntax.
 *
 * \date Started: 5/17/2010
 * # $Revision:: 755                $: last committed revision
 * # $Date:: 2013-08-12 00:29:45 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout, Alan LaMielle, Nicholas Jeanette
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
*/

#include <gtest/gtest.h>
#include <string>
#include <parser/parser.h>
#include <iostream>

using namespace iegenlib;

TEST(Parser, OmegaSetEmpty) {

   std::string temp="{  }";
   Set* s = parser::parse_set(temp);

   EXPECT_EQ(temp,s->toString());
   delete s;
}

TEST(Parser, OmegaSet1Tuple) {
    std::string temp = "{ [a] }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ(temp, s->toString());
   delete s;
}

TEST(Parser, OmegaSet2Tuple) {
   std::string temp = "{ [ab, b] }";
   Set* s = parser::parse_set(temp);
   EXPECT_EQ(temp, s->toString());
   delete s;
}

TEST(Parser, OmegaSetDuplicateTupleVar) {
    std::string temp = "{ [x, x] }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, x] : __tv0 - __tv1 = 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaSetUnion) {
    std::string temp = "{ [x, y] }"; //{ [x, y] } union { [a, b] } == { [x, y] }
    Set* s = parser::parse_set(temp);
    EXPECT_EQ(temp, s->toString());
    delete s;
}

TEST(Parser, OmegaSetInEqualityConstraintGT) {
    std::string temp = "{ [i] : j > 42 }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : j - 43 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaSetInEqualityConstraintGTE) {
    std::string temp = "{ [i] : i >= 42 }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : __tv0 - 42 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaSetInEqualityConstraintLT) {
    std::string temp = "{ [i] : i < 42 }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : -__tv0 + 41 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaSetInEqualityConstraintLTE) {
    std::string temp = "{ [i] : j <= 42 }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : -j + 42 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaSetEqualityConstraintEQ) {
    std::string temp = "{ [i] : j = 10 }";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : j - 10 = 0 }", s->toString());
    delete s;
}

TEST(Parser, OmegaRelation3to3) {
    //try {
    std::string temp = "{ [x, y, z] -> [a, b, c] }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ(temp, r->toString());
    //}
    //catch (std::string exp){
    //    printf("%s\n", exp.c_str());
    //}
    delete r;
}

TEST(Parser, OmegaRelation1to3) {
    std::string temp = "{ [c] -> [d, e, f] }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ(temp, r->toString());
    delete r;
}

TEST(Parser, OmegaRelationUnion) {
    std::string temp = "{ [c] -> [d, e, f] }"; //{ [c] -> [d, e, f] } union { [w] -> [x, y, z] } == { [c] -> [d, e, f] }
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ(temp, r->toString());
    delete r;
}

TEST(Parser, OmegaRelationInEqualityConstraintGT) {
    std::string temp = "{ [i] -> [d, e, f] : j > 42 }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [i] -> [d, e, f] : j - 43 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, OmegaRelationInEqualityConstraintGTE) {
    std::string temp = "{ [i] -> [d, e, f] : i >= 42 }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [i] -> [d, e, f] : __tv0 - 42 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, OmegaRelationInEqualityConstraintLT) {
    std::string temp = "{ [i] -> [d, e, f] : j < 42 }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [i] -> [d, e, f] : -j + 41 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, OmegaRelationInEqualityConstraintLTE) {
    std::string temp = "{ [i] -> [d, e, f] : j <= 42 }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [i] -> [d, e, f] : -j + 42 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, OmegaRelationEqualityConstraintEQ) {
    std::string temp = "{ [i] -> [d, e, f] : j = 10 }";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [i] -> [d, e, f] : j - 10 = 0 }", r->toString());
    delete r;
}

TEST(Parser, SetNoConstraints1) {
    std::string temp = "{[x]}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x] }", s->toString());
    delete s;
}

TEST(Parser, SetNoConstraints2) {
    std::string temp = "{[xy,z]}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [xy, z] }", s->toString());
    delete s;
}

TEST(Parser, RelationNoConstraints1) {
    std::string temp = "{[x,y,z]->[a,b,c]}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, y, z] -> [a, b, c] }", r->toString());
    delete r;
}

TEST(Parser, RelationNoConstraints2) {
    std::string temp = "{[c]->[d,e,f]}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [c] -> [d, e, f] }", r->toString());
    delete r;
}

TEST(Parser, SetConstraintsGT) {
    std::string temp = "{[i]: i > 42}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : __tv0 - 43 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, RelationConstraintsEQ) {
    std::string temp = "{[x,y,z]->[a,b,c]: a=10}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, y, z] -> [a, b, c] : __tv3 - 10 = 0 }", r->toString());
    delete r;
}

TEST(Parser, RelationConstraintsLT) {
    std::string temp = "{[x]->[y]: y < 0}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x] -> [y] : -__tv1 - 1 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, SetConstraintsGTE) {
    std::string temp = "{[x] : -x-1>=0}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x] : -__tv0 - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, RelationConstraintsGTE) {
    std::string temp = "{[x,y]->[i,j] : -x-1>=0}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, y] -> [i, j] : -__tv0 - 1 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, RelationConstraintsEQandEQandLTE) {
    std::string temp = "{[x,y,z]->[a,b,c]: a=5 and z=6 and c<=z}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, y, z] -> [a, b, c] : __tv2 - 6 = 0 && __tv3 - 5 = 0 &&"
       " __tv2 - __tv5 >= 0 }", r->toString());
    delete r;
}

TEST(Parser, RelationConstraintsLTExp) {
    std::string temp = "{[x,y,z]->[a,b,c]:a<y+4}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, y, z] -> [a, b, c] : __tv1 - __tv3 + 3 >= 0 }",
        r->toString());
    delete r;
}

TEST(Parser, RelationConstraintsExpGTEandGTE) {
    std::string temp = "{[x,x,x]->[a,b,c]: a-b+c>=0 && a>=10}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, x, x] -> [a, b, c] : __tv0 - __tv1 = 0 && __tv0 - __tv2 ="
        " 0 && __tv3 - 10 >= 0 && __tv3 - __tv4 + __tv5 >= 0 }",
        r->toString());
    delete r;
}

TEST(Parser, SetConstraintsExpGTEandExpGTE) {
    std::string temp = "{[x,y,z] : -x-y+z-1>=0 and -x-1>=0}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, y, z] : -__tv0 - 1 >= 0 && "
              "-__tv0 - __tv1 + __tv2 - 1 >= 0 }",
               s->toString());
    delete s;
}

TEST(Parser, SetDuplicateTupleVar) {
    std::string temp = "{[x,x,x]}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, x, x] : __tv0 - __tv1 = 0 && __tv0 - __tv2 = 0 }",
        s->toString());
    delete s;
}

TEST(Parser, RelationDuplicateTupleVar) {
    std::string temp = "{[x,x,x]->[a,b,c]}";
    Relation* r = parser::parse_relation(temp);
    EXPECT_EQ("{ [x, x, x] -> [a, b, c] :"
        " __tv0 - __tv1 = 0 && __tv0 - __tv2 = 0 }", r->toString());
    delete r;
}

TEST(Parser, SetDuplicateTupleVarConstraintsEXPEQ) {
    std::string temp = "{[x,x,x1]:x1+3=x}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, x, x1] : __tv0 - __tv1 = 0 && __tv0 - __tv2 - 3 = 0 }",
        s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolic) {
    std::string temp = "{[i]: j > k}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : j - k - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolicConstraintLTandLT) {
    std::string temp = "{[x,y,z]:a<b and a<c}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, y, z] : -a + b - 1 >= 0 && -a + c - 1 >= 0 }",
        s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolicDuplicateTupleVarConstraintLTandLT) {
    std::string temp = "{[x,x,x]:a<b and a<c}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x, x, x] : __tv0 - __tv1 = 0 && __tv0 - __tv2 = 0 &&"
        " -a + b - 1 >= 0 && -a + c - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolicConstraintExpLT) {
    std::string temp = "{[x]: x - a < 0}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x] : -__tv0 + a - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolicConstraintGT) {
    std::string temp = "{[i]: j > i}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : -__tv0 + j - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUndeclaredSymbolicConstraintGT2) {
    std::string temp = "{[i]: j > 12}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [i] : j - 13 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUFCallConstraintLT) {
    std::string temp = "{[x]: f(x) < 0}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x] : -f(__tv0) - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, SetUFCallConstraintExpGTE) {
    std::string temp = "{[x] : -f(x)-1>=0}";
    Set* s = parser::parse_set(temp);
    EXPECT_EQ("{ [x] : -f(__tv0) - 1 >= 0 }", s->toString());
    delete s;
}

TEST(Parser, RelationComposeR1) {
    // input: r1 = { [v,s] -> [w, t] : w=v+2 && t=s }
    Relation* r1 = new Relation("{ [v,s] -> [w, t] : w=v+2 && t=s }");
    EXPECT_EQ("{ [v, s] -> [w, t] : __tv1 - __tv3 = 0 &&"
        " __tv0 - __tv2 + 2 = 0 }", r1->toString());
    delete r1;
}

TEST(Parser, RelationComposeR2) {
    // input: r2 = { [x,z,r] -> [y,i] : y = x + z - r - 7 && i = 3r+2 }
    Relation* r2 =
        new Relation("{ [x,z,r] -> [y,i] : y = x + z - r - 7 && i = r+2 }");

    EXPECT_EQ("{ [x, z, r] -> [y, i] : __tv2 - __tv4 + 2 = 0 &&"
        " __tv0 + __tv1 - __tv2 - __tv3 - 7 = 0 }", r2->toString());
    delete r2;
}

TEST(Parser, RelationComposeExpected) {
    // output: r1 compose r2 = { [x,z,r] -> [w,t]: w=x+z-r-5 && t=r+2 }
    Relation* expected
        = new Relation("{ [x,z,r] -> [w,t]: w=x+z-r-5 && t=r+2 }");
    EXPECT_EQ("{ [x, z, r] -> [w, t] : __tv2 - __tv4 + 2 = 0 "
              "&& __tv0 + __tv1 - __tv2 - __tv3 - 5 = 0 }",
              expected->toString());
    delete expected;
}

//reserved identifier passed to parser
TEST(Parser, RestrictedIdent) {
   std::string temp = "{[__x]}";

   //try { parser::parse_set(temp); }
   //catch (std::string except)
   //{printf ("%s\n", except.c_str());}

   EXPECT_THROW(parser::parse_set(temp), parse_exception);
}

TEST(Parser, OmegaSetExpMult) {

   Set* s1 = new Set("{[x]:x*8=0}");
   EXPECT_EQ("{ [x] : 8 __tv0 = 0 }" ,s1->toString());
   delete s1;
}

TEST(Parser, ISLSetUnion) {

   Set* s1 = new Set("{[a,b]: a>=3 && b>=12; [a,b]: a>5 && b<=9}");
   EXPECT_EQ("{ [a, b] : __tv0 - 6 >= 0 && -__tv1 + 9 >= 0 } "
      "union { [a, b] : __tv0 - 3 >= 0 && __tv1 - 12 >= 0 }", s1->toString());
   delete s1;
}

TEST(Parser, ISLSetUnion2) {
   Set* s1 = new Set("{[a]: a>=3; [b]: b>=5}");
   EXPECT_EQ("{ [b] : __tv0 - 5 >= 0 } union { [a] : __tv0 - 3 >= 0 }",
      s1->toString());
   delete s1;
}

TEST(Parser, OmegaSetInSymbolic) {
   Set* s1 = new Set("[a,b] -> {[a,b]: a>=5 && b>=10}");
   EXPECT_EQ("{ [a, b] : __tv0 - 5 >= 0 && __tv1 - 10 >= 0 }",
      s1->toString());
   delete s1;
}

TEST(Parser, OmegaSetInSymbolicUnion) {
   Set* s1 =
      new Set("[w,x] -> {[a,b]: a>=5 && b>=10} union {[a,d]: a>4 && d<=15}");
   EXPECT_EQ("{ [a, d] : __tv0 - 5 >= 0 && -__tv1 + 15 >= 0 } union "
      "{ [a, b] : __tv0 - 5 >= 0 && __tv1 - 10 >= 0 }", s1->toString());
   delete s1;
}

TEST(Parser, ISLSetInSymbolicUnion) {
   Set* s1 =
      new Set ("[w,x] -> {[a,b]: a>=5 && b>=10; [a,d]: a>4 && d<=15}");
   EXPECT_EQ("{ [a, d] : __tv0 - 5 >= 0 && -__tv1 + 15 >= 0 } union "
      "{ [a, b] : __tv0 - 5 >= 0 && __tv1 - 10 >= 0 }", s1->toString());
   delete s1;
}


TEST(Parser, ISLRelationUnion1) {
   Relation* r1 = new Relation("{[a]->[b]: a>=3; [b]->[c]: b>=5}");
   EXPECT_EQ("{ [b] -> [c] : __tv0 - 5 >= 0 }"
             " union { [a] -> [b] : __tv0 - 3 >= 0 }", r1->toString());
   delete r1;
}

/* FIXME: this one should throw an exception because parameter being
used as a tuple variable.  Is this a parsing issue?
TEST(Parser, ISLRelationInSymbolicUnion1) {
   Relation* r1 = new Relation("[a] -> {[a]->[b] ; [b]->[c]}");
   EXPECT_EQ("{ [a] -> [b] }", r1->toString()); //{ [a] -> [b] } union { [b] -> [c] } == { [a] -> [b] }
   delete r1;
}
*/

TEST(Parser, ISLRelationInSymbolicUnion1) {
   Relation* r1 = new Relation("[N] -> {[a]->[b] ; [b]->[c]}");
   EXPECT_EQ("{ [b] -> [c] }", r1->toString()); //{ [a] -> [b] } union { [b] -> [c] } == { [b] -> [c] }, tuple names don't matter
   delete r1;
}


TEST(Parser, ErrorHandlingBug) {
   std::string temp = "{[v]->[1,w] : w = v + 7";
   EXPECT_THROW(parser::parse_relation(temp), parse_exception);
}

TEST(Parser, UFsThatReturnTuplesIndexed) {
   Relation* r1 
      = new Relation("{[k] -> [i,j] : i = tau_inv(k)[0] and j=tau_inv(k)[1] }");
   EXPECT_EQ("{ [k] -> [i, j] : __tv1 - tau_inv(__tv0)[0] = 0 "
             "&& __tv2 - tau_inv(__tv0)[1] = 0 }",
             r1->toString());
   delete r1;
}

TEST(Parser, UFsThatReturnTuples) {
   Relation* r1 
      = new Relation("{[k] -> [i,j] : (i,j) = tau_inv(k) }");
   EXPECT_EQ("{ [k] -> [i, j] : tau_inv(__tv0) - ( __tv1, __tv2 ) = 0 }",
             r1->toString());
   delete r1;
}

#pragma mark ExpressionsFromISL
// Can we parse expressions from ISL.
TEST(Parser, ExpressionsFromISL) {

//FIXME: Fix the parser!
//    Set* s = new Set(" [M, P] -> { [i, 2 + i, x, x] : x >= 0 and i <= -2 + P "
//                     "and i >= -1 and x <= -1 + M }");
//    EXPECT_EQ("", s->toString());
}


// Parsing constraints like:  0 <= x < n and 0 < x <= n and 
//                            0 < x < n and 0 <= x <= n
TEST(Parser, ConstraintRange) {

   Relation* r1 = new Relation("{[i] -> [j] : 0 < i < n and 0 <= j < m }");
   EXPECT_EQ("[ m, n ] -> { [i] -> [j] : j >= 0 && i - 1 >= 0 && "
             "-i + n - 1 >= 0 && -j + m - 1 >= 0 }", r1->toISLString());
   delete r1;

   Set* s1 = new Set("{[i,j] : 0 < i <= n and 0 <= j <= m }");
   EXPECT_EQ("[ m, n ] -> { [i, j] : j >= 0 && -i + n >= 0 && i - 1 >= 0 "
             "&& -j + m >= 0 }", s1->toISLString());
   delete s1;
}

