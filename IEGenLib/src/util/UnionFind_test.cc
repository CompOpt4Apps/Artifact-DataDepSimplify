/*!
 * \file union_find_test.cc
 *
 * \brief Tests the class in UnionFindu1.cc
 *
 * \date Started: 2013-01-20
 * # $Revision:: 648                $: last committed revision
 * # $Date:: 2013-03-05 15:02:19 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Mark Heim
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

// Copyright 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <iegenlib.h>
#include <gtest/gtest.h>
#include "UnionFindUniverse.h"

// Test the UnionFind methods
// -Constructor: make sure we can create UnionFindUniverse objects.
// -Find: make sure we can extract equivalence classes.
// -Union: make sure we can merge two equivalence classes.

TEST(UnionFind, UnionFindUniverse){
  
  // Test the constructor

  OA::UnionFindUniverse u1(4); // Universe 1
  OA::UnionFindUniverse u2(80); // Universe 2

  // Test UnionFindUniverse.Find method (insure all not equivalent)

  EXPECT_NE(u1.Find(1), u1.Find(2));
  EXPECT_NE(u1.Find(2), u1.Find(3));
  EXPECT_NE(u1.Find(3), u1.Find(1));

  // Test UnionFindUniverse.Union method (merge equivalence class 1 with 2)

  u1.Union(u1.Find(1), u1.Find(2), u1.Find(1));

  // Test UnionFindUniverse.Find method (equivalence classes are [1,2], [3])

  EXPECT_EQ(u1.Find(1), u1.Find(2));
  EXPECT_NE(u1.Find(2), u1.Find(3));
  EXPECT_NE(u1.Find(3), u1.Find(1));

  // Test UnionFindUniverse.Union method (merge equivalence class 1 with 3)

  u1.Union(u1.Find(1), u1.Find(3), u1.Find(3));

  // Test UnionFindUniverse.Find method (equivalence classe is [1,2,3])

  EXPECT_EQ(u1.Find(1), u1.Find(2));
  EXPECT_EQ(u1.Find(2), u1.Find(3));
  EXPECT_EQ(u1.Find(3), u1.Find(1));

  // Test UnionFindUniverse.Find method (insure all in separate classes)

  EXPECT_NE(u2.Find(11), u2.Find(27));
  EXPECT_NE(u2.Find(27), u2.Find(77));
  EXPECT_NE(u2.Find(77), u2.Find(11));

  // Test UnionFindUniverse.Union method (merge equivalence class 11 with 27)

  u2.Union(u2.Find(11), u2.Find(27), u2.Find(27));

  // Test UnionFindUniverse.Find method (equivalence classes are [11,27], [77])

  EXPECT_EQ(u2.Find(11), u2.Find(27));
  EXPECT_NE(u2.Find(27), u2.Find(77));
  EXPECT_NE(u2.Find(77), u2.Find(11));

  // Test UnionFindUniverse.Union method (merge equivalence classes 77 and 77)

  u2.Union(u2.Find(77), u2.Find(77), u2.Find(77));

  // Test UnionFindUniverse.Find method (equivalence classes are [11,27], [77])

  EXPECT_EQ(u2.Find(11), u2.Find(27));
  EXPECT_NE(u2.Find(27), u2.Find(77));
  EXPECT_NE(u2.Find(77), u2.Find(11));

  // Implicitly test the destructor
}

