/*!
 * \file environment_test.cc
 *
 * \brief Test for the Environment class.
 *
 * \date Started: 5/1/12
 * # $Revision:: 760                $: last committed revision
 * # $Date:: 2013-08-15 04:08:06 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "environment.h"
#include "set_relation.h"

#include <gtest/gtest.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>

using iegenlib::Environment;
using iegenlib::UninterpFunc;
using iegenlib::Set;


// This is just a test setup class that is used in the other tests in this
// file.  We could create a data structure that many of the tests use.
class EnvironmentTest:public::testing::Test {
public:
    // data structure declaration
protected:
    // method that sets up the data structure
    void SetUp() { }

    void TearDown() {
    }
};

// FIXME!!! Need more testing of the environment in here.

// Test a successful query.
TEST_F(EnvironmentTest, Success) {

    Environment *env = new Environment();
    env->setInverse("f", "f'");
    EXPECT_TRUE(env->hasInverse("f"));
    EXPECT_TRUE(env->hasInverse("f'"));
    std::string finv = env->funcInverse("f");
    EXPECT_EQ("f'", finv);
    std::string finvinv = env->funcInverse("f'");
    EXPECT_EQ("f", finvinv);
    delete env;
    
}

// Test an unsuccessful query.
TEST_F(EnvironmentTest, NotFound) {
	Environment *env = new Environment();
	env->setInverse("f", "f'");
    std::string ginv = env->funcInverse("g");
    EXPECT_EQ("", ginv);    
    delete env;
}

// Test toString.
TEST_F(EnvironmentTest, ToString) {
	Environment *env = new Environment();
	env->setInverse("f", "f'");
	env->setInverse("g", "p");
	env->setInverse("foo", "bar");

    EXPECT_EQ("Environment:\n    bar ==> foo\n    f ==> f'\n"
              "    f' ==> f\n    foo ==> bar\n    g ==> p\n    p ==> g\n",
              env->toString());    
    delete env;
}

// Testing inserting UninterpFunc declarations into an environment.
TEST_F(EnvironmentTest, EnvUninterpFunc) {
    // Declare a function f.
    UninterpFunc *sf = new UninterpFunc("f",
        new Set("{[i]:0<=i &&i<G}"), new Set("{[i]:0<=i &&i<G}"),
                true, iegenlib::Monotonic_NONE);

    // Now set up an environment with f.
    Environment *env = new Environment(sf);

    EXPECT_EQ("f_inv", env->funcInverse("f"));
    EXPECT_EQ("f", env->funcInverse("f_inv"));
    EXPECT_EQ("", env->funcInverse("g"));
    
    delete env;
}

/*
// Testing environment parsing.
TEST_F(EnvironmentTest, EnvParse) {
    // Now set up an environment that defines an inverse for f.
    Environment *env = new Environment("f() = inverse f'()");

    EXPECT_EQ("f'", env->funcInverse("f"));
    EXPECT_EQ("f", env->funcInverse("f'"));
    EXPECT_EQ("", env->funcInverse("g"));
    
    delete env;
}

// Testing environment parsing.
TEST_F(EnvironmentTest, EnvAppend) {
    // Now set up an environment that defines an inverse for f.
    Environment *env = new Environment("f() = inverse f'()");

    EXPECT_EQ("f'", env->funcInverse("f"));
    EXPECT_EQ("f", env->funcInverse("f'"));
    EXPECT_EQ("", env->funcInverse("g"));
    
    Environment *env2 = new Environment("g() = inverse g'()");
    env->append(env2);
    EXPECT_EQ("g'", env->funcInverse("g"));
    EXPECT_EQ("g", env->funcInverse("g'"));
    
    
    delete env;
}
*/


/******************************************/
