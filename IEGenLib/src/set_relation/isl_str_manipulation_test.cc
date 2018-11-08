/*!
 * \file UFCallMap_test.cc
 *
 * \brief Test for the UFCallMap class.
 *
 * \date Started: 2016-03-14
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2015-2016, University of Arizona <br>
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "isl_str_manipulation.h"

#include <gtest/gtest.h>
#include <utility>
#include <iostream>
#include <fstream>

using namespace std;
using namespace iegenlib;

#pragma mark revertISLTupDeclToOrig
// Test the revertISLTupDeclToOrig functionality
TEST(islStrManipulationTest, revertISLTupDeclToOrig) {

    std::string orig(" [tstep, theta_0__tv1_, col_tv2_]->{[ i1, i2]->"
                     "[ 0, i4, i5 , i6] : i1 = col_i_ and i4 = tstep"
                                      " and i6 = theta_0__tv1_ + 1}");
    std::string islStr(" [tstep, theta_0__tv1_, col_tv2_] -> "
           "{[col_i_ , i2] -> [0 , tstep, i5, theta_0__tv1_ + 1] }");
    string exp(" [tstep, theta_0__tv1_, col_tv2_] ->"
               " {[ i1, i2]->[ 0, i4, i5 , i6] : i1 = col_i_ and"
                        " i4 = tstep and i6 = theta_0__tv1_ + 1}");
    int inArity = 2, outArity = 4;
    string corrected;

    corrected = revertISLTupDeclToOrig(orig, islStr, inArity, outArity);

    //cout<<endl<<corrected<<endl<<endl; 
    EXPECT_EQ( exp , corrected );

    std::string s_orig(" {[ i1, i2 , i3, i4] : i1 = col_i_ and"
       " i4 = tstep and i4 = theta_0__tv1_ + 1 and i1 >= 0 and i2 <= 10}");
    std::string s_islStr(" {[col_i_ , i2, tstep, theta_0__tv1_ + 1] : "
                                                          " i2 <= 10 }");
    string s_exp(" {[ i1, i2 , i3, i4] :  i2 <= 10  and "
                 " i1 = col_i_ and i3 = tstep and i4 = theta_0__tv1_ + 1}");

    inArity = 4;outArity = 0;
    corrected = revertISLTupDeclToOrig(s_orig, s_islStr, inArity, outArity);

    //cout<<endl<<corrected<<endl<<endl; 
    EXPECT_EQ( s_exp , corrected );
}

#pragma mark projectOutStrCorrection
// Test the projectOutStrCorrection functionality
TEST(islStrManipulationTest, projectOutStrCorrection) {

    string islStr("{[col_i_ , i2] -> [0 , tstep, i5, theta_0__tv1_ + 1] }");
    string exp(   "{[col_i_, i2] -> [tstep, i5, theta_0__tv1_ + 1] }");
    int inArity = 2, outArity = 4;
    string corrected;

    corrected = projectOutStrCorrection(islStr, 2, inArity, outArity);

    EXPECT_EQ( exp , corrected );

    exp = "{[col_i_, i2] -> [0, tstep] }";
    inArity = 2; outArity = 3;

    corrected = projectOutStrCorrection(islStr, 4, inArity, outArity);

    EXPECT_EQ( exp , corrected );


    std::string s_islStr(" {[col_i_ , i2, tstep, theta_0__tv1_ + 1] : "
                                                "col_i_ >= 0 and i2 <= 10}");
    string s_exp(        " {[col_i_, i2, tstep] : "
                                                "col_i_ >= 0 and i2 <= 10}");

    inArity = 4;outArity = 0;
    corrected = projectOutStrCorrection(s_islStr, 3, inArity, outArity);

    //cout<<endl<<corrected<<endl<<endl; 
    EXPECT_EQ( s_exp , corrected );
}


#pragma mark setStr2RelationStrTEST
// Test the projectOutStrCorrection functionality
TEST(islStrManipulationTest, setStr2RelationStrTEST) {

    string set("{[i1,i2,i3,i4]: 0 < i1 and 0 < i2 and 0 < i3 and 0 < i4}");
    string relation;
    string expectedRelation = 
           "{[i1,i2] -> [i3,i4]: 0 < i1 and 0 < i2 and 0 < i3 and 0 < i4}";
    int arity = 4;
    int inArity = arity/2;
    int  outArity = arity-inArity;

    relation = setStr2RelationStr(set, inArity, outArity);
    EXPECT_EQ(expectedRelation , relation );

    set = "{[i1,i2,i3,i4,i5]: 0 < i1 and 0 < i2"
             " and 0 < i3 and 0 < i4 and 0 < i5}";
    expectedRelation = 
               "{[i1,i2] -> [i3,i4,i5]: 0 < i1 and "
               "0 < i2 and 0 < i3 and 0 < i4 and 0 < i5}";
    arity = 5;
    inArity = arity/2;
    outArity = arity-inArity;

    relation = setStr2RelationStr(set, inArity, outArity);
    EXPECT_EQ(expectedRelation , relation );
}


#pragma mark relationStr2SetStrTEST
// Test the projectOutStrCorrection functionality
TEST(islStrManipulationTest, relationStr2SetStrStrTEST) {

    string relation("{[i1,i2]->[i3,i4]: 0 < i1 and 0 < i2 "
                    "and 0 < i3 and 0 < i4}");
    string set;
    string expectedSet = 
           "{[i1,i2,i3,i4]: 0 < i1 and 0 < i2 and 0 < i3 and 0 < i4}";
    int arity = 4;
    int inArity = arity/2;
    int  outArity = arity-inArity;

    set = relationStr2SetStr(relation, inArity, outArity);
    EXPECT_EQ(expectedSet , set );

    relation = "{[i1,i2] -> [i3,i4,i5]: 0 < i1 and 0 < i2"
             " and 0 < i3 and 0 < i4 and 0 < i5}";
    expectedSet = "{[i1,i2,i3,i4,i5]: 0 < i1 and "
               "0 < i2 and 0 < i3 and 0 < i4 and 0 < i5}";
    arity = 5;
    inArity = arity/2;
    outArity = arity-inArity;

    set = relationStr2SetStr(relation, inArity, outArity);
    EXPECT_EQ(expectedSet , set );
}
