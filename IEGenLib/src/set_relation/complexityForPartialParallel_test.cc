/*!
 * \file complexityForPartialParallel_test.cc
 *
 * \brief complexity For Partial Parallel tests.
 *
 * \date Started: 2/5/18
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

// set and relation classes
using iegenlib::Conjunction;
using iegenlib::SparseConstraints;
using iegenlib::Set;
using iegenlib::Relation;

#pragma mark complexityForPartialParallelTEST
TEST(complexityTest, complexityForPartialParallelTEST){

  string complexity = "";  

  // Introducing UFSymbols
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

  
  // We need to define which iterators are meant for parallelization
  // therefore those iterators must not considered projectable. In other
  // words the assumption is that those iterators must be in final inspector
  // and hence we must count their complexity regardless of whatever else.
  std::set<int> parallelTvs;


  // DIFFERENT examples:


  // 1: No iterator can be projected, and there is no useful equalities
  Set *s = new Set("[n] -> {[i,ip,k,kp]: i < ip"
                                 " && 0 <= i && i < n"
                                " && 0 <= ip && ip < n"
                         " && rowptr(i) <= k && k < diagptr(i)"
                       " && rowptr(ip) <= kp && kp < diagptr(ip)"
                               " && colidx(k) = colidx(kp) }");
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 1 ); // ip

  complexity = s->complexityForPartialParallel(parallelTvs);

  // {i, ip, k, kp} === (n)*(n)*(nnz/n)*(nnz/n) = nnz^2
  EXPECT_EQ( std::string("O(nnz^2)") , complexity );

  delete s;


  // 2: No iterator can be projected, but we have an useful equality
  s = new Set("[n] -> {[i,ip,k,kp]: i < ip"
                               " && 0 <= i && i < n"
                              " && 0 <= ip && ip < n"
                       " && rowptr(i) <= k && k < diagptr(i)"
                     " && rowptr(ip) <= kp && kp < diagptr(ip)"
                             " && colidx(k) = colidx(kp)"
                                      " && k = kp}");
  parallelTvs.clear();
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 1 ); // ip

  complexity = s->complexityForPartialParallel(parallelTvs);

  // {i, ip, k} === (n)*(n)*(nnz/n) = n*nnz;   kp can be produced using kp = k
  EXPECT_EQ( std::string("O(n^1*nnz^1)") , complexity );

  delete s;


  // 3: k and kp can be projected, equality becomes irrelevant
  Relation *r = new Relation("[n] -> {[i,k] -> [ip,kp]: i < ip"
                               " && 0 <= i && i < n"
                              " && 0 <= ip && ip < n"
                       " && rowptr(i) <= k && k < diagptr(i)"
                     " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                      " && k = kp}");
  parallelTvs.clear();
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 2 ); // ip

  complexity = r->complexityForPartialParallel(parallelTvs);

  // {i, ip} === (n)*(n) = n^2;
  EXPECT_EQ( std::string("O(n^2)") , complexity );

  delete r;

  // 4: k can be projected, and i = colidx(kp) can be used to get i from kp
  r = new Relation("[n] -> {[i,k] -> [ip,kp]: i < ip"
                               " && 0 <= i && i < n"
                              " && 0 <= ip && ip < n"
                       " && rowptr(i) <= k && k < diagptr(i)"
                     " && rowptr(ip) <= kp && kp < diagptr(ip)"
                                    " && i = colidx(kp) }");
  parallelTvs.clear();
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 2 ); // ip

  complexity = r->complexityForPartialParallel(parallelTvs);

  // {ip, kp} === (n)*(nnz/n) = nnz;
  EXPECT_EQ( std::string("O(nnz^1)") , complexity );

  delete r;



  // 5: Out_6's upper bound is not provided. Nonetheless, we do not need it
  // since it is in an equality constraint with In_6. 
  // Note: This can happen when we pass a relations string through ISL.
  //       ISL would replace all the instances of an iterators with its equal.
  r = new Relation("{ [In_2, In_4, In_6, In_8] -> [Out_2, Out_4, Out_6, Out_8] : "
"Out_6 = In_6 && colidx(In_8) = colidx(In_6) && colidx(Out_8) = colidx(Out_6) && "
"0 <= In_2 && 0 <= In_4 && 0 <= In_6 && 0 <= In_8 && 0 <= Out_2 && 0 <= Out_4 && "
"0 <= Out_8 && 0 <= rowptr(In_2) && 0 <= rowptr(In_2 + 1) && 0 <= rowptr(Out_2) "
"&& 0 <= rowptr(Out_2 + 1) && 0 <= rowptr(colidx(In_4)) && 0 <= rowptr(colidx(In_4) + 1) "
"&& 0 <= rowptr(colidx(Out_4)) && 0 <= rowptr(colidx(Out_4) + 1) && 0 <= colidx(In_4) "
"&& 0 <= colidx(In_6) && 0 <= colidx(In_8 + 1) && 0 <= colidx(Out_4) && 0 <= colidx(Out_6) "
"&& 0 <= colidx(Out_8 + 1) && In_4 <= In_8 && rowptr(colidx(In_4)) <= In_6 && "
"rowptr(colidx(Out_4)) <= In_6 && Out_4 <= Out_8 && colidx(In_8 + 1) <= colidx(In_6) "
"&& colidx(Out_8 + 1) <= colidx(Out_6) && In_2 < Out_2 && In_2 + 1 < m && In_2 + 1 < n "
"&& In_4 < nnz && In_4 < rowptr(In_2 + 1) && rowptr(In_2) < In_4 && In_6 < nnz "
"&& In_6 < rowptr(colidx(In_4) + 1) && In_6 < rowptr(colidx(Out_4) + 1) && In_8 + 1 < nnz "
"&& In_8 < rowptr(In_2 + 1) && Out_2 + 1 < m && Out_2 + 1 < n && Out_4 < nnz "
"&& Out_4 < rowptr(Out_2 + 1) && rowptr(Out_2) < Out_4 && Out_8 + 1 < nnz "
"&& Out_8 < rowptr(Out_2 + 1) && colidx(In_4) + 1 < m && colidx(In_6) < m "
"&& colidx(In_8 + 1) < m && colidx(Out_4) + 1 < m && colidx(Out_6) < m "
"&& colidx(Out_8 + 1) < m && rowptr(In_2) < nnz && rowptr(In_2 + 1) < nnz "
"&& rowptr(Out_2) < nnz && rowptr(Out_2 + 1) < nnz && rowptr(colidx(In_4)) < nnz "
"&& rowptr(colidx(In_4) + 1) < nnz && rowptr(colidx(Out_4)) < nnz "
"&& rowptr(colidx(Out_4) + 1) < nnz }");
  parallelTvs.clear();
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 4 ); // ip

  complexity = r->complexityForPartialParallel(parallelTvs);

//std::cout<<"\n\n Comp = "<<complexity<<"\n\n"

  // {ip, kp} === (n)*(nnz/n) = nnz;
  EXPECT_EQ( std::string("O(nnz^5/n^3)") , complexity );

  delete r;

}

