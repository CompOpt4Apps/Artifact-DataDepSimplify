/*!
 * \file UFCallMap.cc
 *
 * \brief Interface of UFCallMap class
 *
 * \date Started: 2016-02-02
 *
 * \authors Michelle Strout, Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2015-2016, University of Arizona <br>
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "UFCallMap.h"
#include "set_relation.h"
#include <util/util.h>
#include <iostream>

namespace iegenlib{

//! Assignment operator.
UFCallMap& UFCallMap::operator=( const UFCallMap& other) {

    mUFC2VarParam = other.mUFC2VarParam;
    mVarParam2UFC = other.mVarParam2UFC;

    return *this;
}

//! Returns a string representing ufcterm as a symbolic constant
// The result is in a format that isl and Omega+ libraries can handle
// It is going to look like:
// Original:    row( col(tv2 + 1) - 2 ) ->
// ConsSymbol:  row_col_tv2P1_M2_
string UFCallMap::symUFC( std::string ufcName )
{
    int len = ufcName.length();
    std::stringstream ss;

    for (int i = 0 ; i < len ; i++){
        char c = ufcName[i];
        if( c != ' ' && c != ','){
            if( c == '(' || c == ')'){
                ss <<'_';
            } else if( c == '[' || c == ']'){
                ss <<'B';
            } else if( c == '+' ){
                ss <<'P';
            } else if( c == '-' ){
                ss <<'M';
            } else{
                ss <<c;
            }
        }
    }

    return (ss.str());
}

/*! Use this to insert a UFCallTerm to map.
**  The function creates an VarTerm representing the UFC then,
**  adds (ufc,vt) to mUFC2VarParam & adds (vt,str) to mVarParam2UFC
**  The class does not own the object pointed by ufc,
**  and it is left unchanged.
**
**  NOTE: The function ignores coefficient of the UFCallTerm.
**        So, -2*row(i)  is considered as just row(i)
*/
VarTerm UFCallMap::insert( UFCallTerm *ufc )
{
    UFCallTerm* ufcterm = new UFCallTerm (*ufc);
    ufcterm->setCoefficient(1);
    std::string symCons = ufcterm->toString();

    symCons = UFCallMap::symUFC(symCons);
    VarTerm vt( 1 , symCons );

    if( find(&vt) ){
        return vt;
    }

    mUFC2VarParam.insert ( std::pair<UFCallTerm,VarTerm>(*ufcterm,vt) );
    mVarParam2UFC.insert ( std::pair<VarTerm,UFCallTerm>(vt,*ufcterm) );

    delete ufcterm;

    return vt;
}

/*! Searches for ufcterm in the map. If ufcterm exists, it returns a pointer 
**  to equ. VarTerm, otherwise it adds the ufcterm to the map, then returns
**  the equ. VarTerm. The class does not own the object pointed by ufcterm,
**  and it is left unchanged.
**
**  NOTE: The function ignores coefficient of the UFCallTerm.
**        So, -2*row(i)  is considered as just row(i)
*/
VarTerm* UFCallMap::find( UFCallTerm* ufc )
{
    VarTerm* result;
    UFCallTerm* ufcterm = new UFCallTerm(*ufc);
    ufcterm->setCoefficient(1);
    std::map<UFCallTerm,VarTerm>::iterator it;
    
    it = mUFC2VarParam.find(*ufcterm);
    if (it == mUFC2VarParam.end()){
        result = new VarTerm(  (insert(ufcterm)).symbol() );
    } else {
        result = new VarTerm(it->second);
    }

    delete ufcterm;
    return result;
}

/*! Searches for a VarTerm in the map. If VarTerm exists in the map,
**  it returns the pointer to equ. UFC, otherwise returns NULL. 
**  The class does not own the object pointed by symbol,
**  and it is left unchanged.
**
**  NOTE: The function ignores coefficient of the VarTerm.
**        So, -2*row_i_  is considered as just row_i_
*/
UFCallTerm* UFCallMap::find( VarTerm* vt )
{
    VarTerm* sym = new VarTerm(*vt);
    sym->setCoefficient(1);
    std::map<VarTerm,UFCallTerm>::iterator it;

    it = mVarParam2UFC.find(*sym);
    if (it == mVarParam2UFC.end()){
        return NULL;
    }
    delete sym;

    UFCallTerm* result = new UFCallTerm(it->second) ;
    return result;
}

// prints the content of the map into a string, and returns it
std::string UFCallMap::toString()
{
    std::stringstream ss;
    ss << "UFCallMap:" << std::endl;
    std::map<UFCallTerm,VarTerm>::iterator it;
    for (it=mUFC2VarParam.begin(); it!=mUFC2VarParam.end(); it++) {
        ss << "\tUFC = " << it->first.toString() 
           << "  ,  sym = " << it->second.toString() << std::endl;
    }
    return ss.str();
}


// Outputs list of variable terms created for UFCs, 
// used in rule instantiation functionality.
std::string UFCallMap::varTermStrList()
{
    std::stringstream ss;
    std::map<UFCallTerm,VarTerm>::iterator it;
    for (it=mUFC2VarParam.begin(); it!=mUFC2VarParam.end(); it++) {
        if( it != mUFC2VarParam.begin() ) ss << ", ";
        ss << it->second.toString();
    }
    return ss.str();
}

}  // end of namespace iegenlib
