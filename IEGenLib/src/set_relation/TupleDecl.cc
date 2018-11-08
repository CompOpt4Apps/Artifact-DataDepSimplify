/*!
 * \file TupleDecl.cc
 *
 * \brief Implementation of TupleDecl class
 *
 * \date Started: 2013-01-25
 * # $Revision:: 753                $: last committed revision
 * # $Date:: 2013-08-11 22:30:45 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout and Joe Strout
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "expression.h"
#include "set_relation.h" // Necessary for TupleElemDecl::toString declaration

namespace iegenlib{

TupleDecl::TupleDecl(int size) : mSize(size) {
    // default values for the tuple elements
    for (int i=0; i<size; i++) {
        mIsConst.push_back(false);
        mConstVal.push_back(0);
        mVarString.push_back(sDefaultTupleVarName(i));
    }
}

unsigned int TupleDecl::size() const {
    return mSize;
}

//! Returns the default name for a tuple variable at a particular
//! location.
std::string TupleDecl::sDefaultTupleVarName(unsigned int loc) {
    std::stringstream var;
    var << "tv" << loc;
    return var.str();
}

//! If a tuple declaration has not been provided then this
//! is the default tuple declaration.
TupleDecl TupleDecl::sDefaultTupleDecl(unsigned int arity) {
    TupleDecl genericTupleDecl(arity);
    for (unsigned int i=0; i<arity; i++) {
        genericTupleDecl.setTupleElem(i,sDefaultTupleVarName(i));
    }
    return genericTupleDecl; 
}

//! Copy constructor.
TupleDecl::TupleDecl( const TupleDecl& other ) {
    *this = other;
}

//! Assignment operator.
TupleDecl& TupleDecl::operator=( const TupleDecl& other) {
    mSize = other.mSize;
    mIsConst.clear();
    mConstVal.clear();
    mVarString.clear();

    // copy the other vector entries to "this"
    for (unsigned int i=0; i<mSize; i++) {
        mIsConst.push_back( other.mIsConst[i] );
        mConstVal.push_back( other.mConstVal[i] );
        mVarString.push_back( other.mVarString[i] );
    }

    return *this;
}

/*! Less than operator.
**      Compare two TupleDecl in the following order:
**          1. by size: the number of elements
**          2. by constant values
**              - When comparing constants to variables, let variables
**                have a value of -inf.
**      This operator assumes that the vector of constants/variables
**          (mConstVal) is sorted.
** @param other, object to be compared
*/
bool TupleDecl::operator<( const TupleDecl& other) const {
    // first compare sizes
    if (mSize < other.mSize) { return true; }
    if (other.mSize < mSize) { return false; }
    // then compare our elements
    for (unsigned int i=0; i < other.mIsConst.size(); i++) {
        //if both are not const, then we have nothing to compare
        if (!mIsConst[i] && !other.mIsConst[i]) { continue; }

        //if we are const and other is not, we're greater
        if (mIsConst[i] && !other.mIsConst[i]) { return false; }
        //if other is const and we are not, other's greater
        if (other.mIsConst[i] && !mIsConst[i]) { return true; }
        
        //compare values
        if (mConstVal[i] < other.mConstVal[i]) { return true; }
        if (other.mConstVal[i] < mConstVal[i]) { return false; }
    }
    //all our elements are equal, so we're not less than other
    return false;
}

/*! Equality operator.
**      Compare two TupleDecl for equality.
**      This operator assumes operator<(TupleDecl) is well defined.
** @param other, object to be compared
*/
bool TupleDecl::operator==( const TupleDecl& other) const {
    return !((*this < other) || (other < *this));
}

//! Concatenate the input tuple declarations and return the result.
//! Unless the second tuple has the default tuple variable names
//! the tuple variable names will be copied verbatim so conflicts
//! are possible.  If append [a,b] with [a] then get [a,b,a].
//! __tv0=__tv2.  Make sure you know what you are doing.
TupleDecl& TupleDecl::concat(const TupleDecl& other) {
    int new_size = this->mSize + other.mSize;

    // copy the other vector entries after the copy from "this"
    for (unsigned int j=0; j<other.mSize; j++) {
        this->mIsConst.push_back(other.mIsConst[j]);
        this->mConstVal.push_back(other.mConstVal[j]);
        if (other.mVarString[j]==sDefaultTupleVarName(j)) {
            // stick with default name for new location
            this->mVarString.push_back(sDefaultTupleVarName(j+mSize));
        } else {
            // copy over the tuple variable name
            this->mVarString.push_back(other.mVarString[j]);
        }
    }

    // set our new size
    this->mSize = new_size;
    return *this;
}

//! returns a clone of "this"
TupleDecl* TupleDecl::clone() const {
    return new TupleDecl(*this);
}


std::string TupleDecl::toString(bool withBrackets, 
                                unsigned int aritySplit) const {
    std::stringstream ss;
    // Do not print empty brackets if have zero arity.
    if (mSize != 0 ) {
        if (withBrackets) { ss << "["; }
        for (unsigned int i=0; i<mSize; i++) {
            if (i > 0) {
                if (aritySplit == i && withBrackets) {
                    ss << "] -> [";
                } else {
                    ss << ", ";
                }
            }
            ss << elemToString(i);
        }
        if (withBrackets) { ss << "]"; }
    }

    return ss.str();
}

void TupleDecl::setTupleElem(unsigned int elem_loc, int const_val) {
    if (elem_loc>=mSize) {
        throw 
            assert_exception("TupleDecl::setTupleElem: elem_loc out of bounds");
    }
    mIsConst[elem_loc] = true;
    mConstVal[elem_loc] = const_val;
}

void TupleDecl::setTupleElem(unsigned int elem_loc, std::string var_string) {
    if (elem_loc>=mSize) {
        throw 
            assert_exception("TupleDecl::setTupleElem: elem_loc out of bounds");
    }
    mIsConst[elem_loc] = false;
    mVarString[elem_loc] = var_string;
}

//! Copy the tuple element declaration elem_loc_from from other to
//! this elem_loc_to location.
void TupleDecl::copyTupleElem(const TupleDecl& other,
        unsigned int elem_loc_from, unsigned int elem_loc_to) {
    mIsConst[elem_loc_to] = other.mIsConst[elem_loc_from];
    mConstVal[elem_loc_to] = other.mConstVal[elem_loc_from];
    mVarString[elem_loc_to] = other.mVarString[elem_loc_from];
}


std::string TupleDecl::elemToString(unsigned int elem_loc) const {
    std::stringstream ss;
    if (mIsConst[elem_loc]) {
        ss << mConstVal[elem_loc];
    } else {
        ss << mVarString[elem_loc];
    }
    return ss.str();
}

bool TupleDecl::elemIsConst(unsigned int elem_loc) const {
    return mIsConst[elem_loc];
}

int TupleDecl::elemConstVal(unsigned int elem_loc) const {
    return mConstVal[elem_loc];
}

std::string TupleDecl::elemVarString(unsigned int elem_loc) const {
    return mVarString[elem_loc];
}

//! Returns an appropriate term for this tuple element.
//! If the view location is a constant then will use that
//! constant.  Otherwise makes a tuple variable for the 
//! use_elem_loc. 
//! Caller is responsible for deleting the resulting Term.
Term* TupleDecl::elemCreateTerm(unsigned int view_elem_loc,
                                unsigned int use_elem_loc) const {
                                
    // Checking the indices to see if they fall within size.
    if (view_elem_loc>=size()) {
        throw assert_exception("TupleDecl::elemCreateTerm: view_elem_loc "
                               "parameter is out of bounds");
        
    }
    
    // Determining whether to make a constant or a tuple variable term.
    if (mIsConst[view_elem_loc]) {
        return new Term(mConstVal[view_elem_loc]);
    } else {
        return new TupleVarTerm(use_elem_loc);
    }
}

}
