/*!
 * \file UFCallMap_test.cc
 *
 * \brief Test for the UFCallMap class.
 *
 * \date Started: 2016-02-02
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2015-2016, University of Arizona <br>
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "expression.h"
#include "UFCallMap.h"
#include "set_relation.h"

#include <gtest/gtest.h>
#include <utility>
#include <iostream>
#include <fstream>

using iegenlib::Exp;
using iegenlib::TupleVarTerm;
using iegenlib::Term;
using iegenlib::VarTerm;
using iegenlib::UFCallTerm;
using iegenlib::Set;
using iegenlib::TupleExpTerm;

#pragma mark UFCallMap
// Test the UFCallMap functionalities
TEST(UFCallMapTest, UFCallMap) {

    // Constructing sample UFC
    Exp *e1 = new Exp();  // __tv0 + 1
    e1->addTerm(new TupleVarTerm( 0 ));
    e1->addTerm(new Term(1));
    
    Exp *e2 = new Exp();  // __tv2 + n
    e2->addTerm(new TupleVarTerm(2));
    e2->addTerm(new VarTerm(-1,"n"));
    
    UFCallTerm *ufcall = new UFCallTerm("row", 2);
    ufcall->setParamExp(0,e1);
    ufcall->setParamExp(1,e2);

//////// Defining the UFCallMap
    iegenlib::UFCallMap map;

//////// Inserting a UFC into the map
    map.insert(ufcall);

//////// Checking: UFC -> Symbolic Cnstant    ---LOOK UP
   ///   Finding a UFC in the map, and getting its "symbolic constant" form.
    VarTerm* symCons = map.find(ufcall);

    EXPECT_EQ( "row___tv0P1__tv2Mn_" , symCons->toString() );

//////// Checking: Symbolic Constant -> UFC    ---LOOK UP
   ///   Finding a "symbolic constant" in the map, and getting its equ. UFC.
    UFCallTerm* r_ufcall = map.find(symCons);

    EXPECT_EQ( ufcall->toString() , r_ufcall->toString() );

// Checking: The UFC is not in the map! So, we get NULL.
// ufcMap has 2 find function 1 searches based on UFCallTerm
// the other one based on VarTerm. The one based on UFCcallTerm adds the 
// ufc passed to it if it is not already in the map, so we always
// get back equivalant VarTerm. The VarTerm based one returns NULL
// if UFCallTerm is not in the map.
    UFCallTerm *ufcallFoo = new UFCallTerm("Foo", 0);
    string symConsStr = iegenlib::UFCallMap::symUFC(ufcallFoo->toString());
    VarTerm vt( 1 , symConsStr );
    UFCallTerm* ufcF = map.find(&vt);

    EXPECT_EQ( NULL , ufcF ); 

    delete ufcall;
    delete ufcallFoo;
    delete r_ufcall;
    delete symCons;
}
