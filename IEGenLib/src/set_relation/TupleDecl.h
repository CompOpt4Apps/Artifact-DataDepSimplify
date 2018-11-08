/*!
 * \file TupleDecl.h
 *
 * \brief Interface of TupleDecl class
 *
 * \date Started: 2013-01-25
 * # $Revision:: 751                $: last committed revision
 * # $Date:: 2013-08-09 00:07:40 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout and Joe Strout
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */
 
#ifndef TUPLE_DECL_H_
#define TUPLE_DECL_H_

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include "environment.h"
#include <util/util.h>

namespace iegenlib{

class Term;

/*!
 * \class TupleDecl
 *
 * This class should ONLY be used internally by Set, Relation, Conjunction.
 * \brief Info about a vector of sequence elements, each of which is 
 *        a constant or a variable.
 */
class TupleDecl {
public:
    TupleDecl() : mSize(0) {}
    TupleDecl(int size);
    ~TupleDecl() {}

    unsigned int size() const;
    
    //! Returns the default name for a tuple variable at a particular
    //! location.
    static std::string sDefaultTupleVarName(unsigned int loc);

    //! If a tuple declaration has not been provided then this
    //! is the default tuple declaration.
    static TupleDecl sDefaultTupleDecl(unsigned int arity);

    //! Copy constructor.
    TupleDecl( const TupleDecl& other );
    
    //! Assignment operator.
    TupleDecl& operator=( const TupleDecl& other);
    
    //! Less than operator.
    bool operator<( const TupleDecl& other) const;
    
    //! Equality operator.
    bool operator==( const TupleDecl& other) const;

    //! Concatenate the other tuple declarations
    //! to this set of tuple declarations and return reference to self.
    TupleDecl& concat(const TupleDecl& other);
    
    //! returns a clone of "this"
    TupleDecl* clone() const;
    
    // Methods that manipulate the whole tuple declaration.
    
    //! Creates comma separated list of tuple declaration.
    //! WithBrackets indicates whether to include brackets or not.
    //! Arity split is to indicate when we have a relation
    //! and what the input arity is for that relation.
    //! Use zero arity for a set. 
    std::string toString(bool withBrackets=false, 
                         unsigned int aritySplit=0) const;
    
    // Setting individual elements
    void setTupleElem(unsigned int elem_loc, int const_val);
    void setTupleElem(unsigned int elem_loc, std::string var_string);
    
    //! Copy the tuple element declaration elem_loc_from from other to
    //! this elem_loc_to location.
    void copyTupleElem(const TupleDecl& other, unsigned int elem_loc_from,
                       unsigned int elem_loc_to);
    
    // Methods that access individual elements in the tuple.
    std::string elemToString(unsigned int elem_loc) const;
    bool elemIsConst(unsigned int elem_loc) const;
    int elemConstVal(unsigned int elem_loc) const;
    std::string elemVarString(unsigned int elem_loc) const;
    unsigned int getSize(){ return mSize; }

    //! Returns an appropriate term for this tuple element.
    //! If the view location is a constant then will use that
    //! constant.  Otherwise makes a tuple variable for the 
    //! use_elem_loc. 
    //! Caller is responsible for deleting the resulting Term.
    Term* elemCreateTerm(unsigned int view_elem_loc, 
                         unsigned int use_elem_loc) const;
        
        
private:
    unsigned int mSize;
    std::vector<bool>   mIsConst;
    std::vector<int>    mConstVal;
    std::vector<std::string>    mVarString;
};

}

#endif
