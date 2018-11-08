/*!
 * \file util_test.cc
 *
 * \brief Tests for the IEGen utilities
 *
 * \date Started: 9/13/10
 * # $Revision:: 622                $: last committed revision
 * # $Date:: 2013-01-18 13:11:32 -0#$: date of last committed revision
 * # $Author:: cathie               $: author of last committed revision
 *
 * \authors Alan LaMielle
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "util.h"
#include <iegenlib.h>
#include <gtest/gtest.h>

using iegenlib::parse_exception;
using iegenlib::assert_exception;;

// TODO: Test the StringException and ParseException classes and any other util classes/routines
TEST(UtilTests, UtilTestParse){

  try{
    parse_exception excep = parse_exception("test");
    throw excep;
  }catch(parse_exception e){
     string in = e.what();
     EXPECT_EQ(in,"test");
     return; //Success!
  }
  ADD_FAILURE()<<"ParseException exception not raised!";
}

TEST(UtilTests, UtilTestAssert){

  try{
    assert_exception excep = assert_exception("test");
    throw excep;
  }catch(assert_exception e){
     string in = e.what();
     EXPECT_EQ(in,"test");
     return; //Success!
  }
  ADD_FAILURE()<<"assert_exception exception not raised!";
}

