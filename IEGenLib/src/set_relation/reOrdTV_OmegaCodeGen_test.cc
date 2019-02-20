/*!
 * \file reOrdTV_OmegaCodeGen_test.cc
 *
 * \brief complexity For Partial Parallel tests.
 *
 * \date Started: 2/19/19
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

// set and relation classes
using iegenlib::Conjunction;
using iegenlib::SparseConstraints;
using iegenlib::Set;
using iegenlib::Relation;

#pragma mark reOrdTV_OmegaCodeGenTEST
TEST(reOrdTV_OmegaCodeGenTest, reOrdTV_OmegaCodeGenTEST){

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

  s->reOrdTV_OmegaCodeGen(parallelTvs);

  // {i, ip, k, kp} === (n)*(n)*(nnz/n)*(nnz/n) = nnz^2
//  EXPECT_EQ( std::string("O(nnz^2)") , complexity );

  delete s;


  // 2: No iterator can be projected, and there is no useful equalities
  s = new Set("[n] -> {[i,ip,kp]: i < ip"
                                 " && 0 <= i && i < n"
                                " && 0 <= ip && ip < n"
                         " && rowptr(ip) <= kp && kp < diagptr(ip)"
                               " &&  i = colidx(kp) }");
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 1 ); // ip

  s->reOrdTV_OmegaCodeGen(parallelTvs);

  std::cout<<"\n\nNew set = "<<s->getString()<<"\n\n";

  // {i, ip, k, kp} === (n)*(n)*(nnz/n)*(nnz/n) = nnz^2
//  EXPECT_EQ( std::string("O(nnz^2)") , complexity );

  delete s;

  // 2: No iterator can be projected, but we have an useful equality
  s = new Set("[n] -> {[ip,i,kp]: i < ip"
                               " && 0 <= i && i < n"
                              " && 0 <= ip && ip < n"
                     " && rowptr(ip) <= kp && kp < diagptr(ip)"
                             " && colidx(k) = colidx(kp)"
                                      " && i = kp}");
  parallelTvs.clear();
  parallelTvs.insert( 0 ); // i
  parallelTvs.insert( 1 ); // ip

  s->reOrdTV_OmegaCodeGen(parallelTvs);

  std::cout<<"\n\nNew set = "<<s->getString()<<"\n\n";

  // {i, ip, k} === (n)*(n)*(nnz/n) = n*nnz;   kp can be produced using kp = k
//  EXPECT_EQ( std::string("O(n^1*nnz^1)") , complexity );

  delete s;

}


#pragma mark reOrdTV_OmegaCodeGenTEST
TEST(reOrdTV_OmegaCodeGenTest, rmPsTEST){

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
  Set *s = new Set("[n] -> {[i,ip,k,kp]: i < ip && sw =0"
                                 " && 0 <= i && i < n"
                                " && 0 <= ip && ip < n"
                         " && rowptr(i) <= k && k < diagptr(i)"
                       " && rowptr(ip) <= kp && kp < diagptr(ip)"
                               " && colidx(k) = colidx(kp) }");


  s->removeUPs();
  std::cout<<"\n\nNew set = "<<s->getString()<<"\n\n";

  delete s;


  // 2: No iterator can be projected, and there is no useful equalities
  s = new Set("[n] -> {[i,ip,kp]: i < ip"
                                 " && 0 <= i && i < n"
                                " && 0 <= ip && ip < n"
                         "&& sw = 1 && rowptr(ip) <= kp && kp < diagptr(ip)"
                               " &&  i = colidx(kp) }");
  s->removeUPs();
  std::cout<<"\n\nNew set = "<<s->getString()<<"\n\n";

  delete s;

}

