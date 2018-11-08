/*!
 * \file SubMap.cc
 *
 * \brief Implementation of SubMap class
 *
 * \date Started: 2013-08-20
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "SubMap.h"
#include "expression.h"
#include <util/util.h>
#include <iostream>

namespace iegenlib{

SubMap::SubMap() {
    mIter = mTerm2ExpMap.begin();
}

//! Delete all of the terms and expressions we are storing.
SubMap::~SubMap() {
    std::map<Term*,Exp*>::iterator iter;
    for (iter=mTerm2ExpMap.begin(); iter!=mTerm2ExpMap.end(); iter++) {
        delete iter->first;
        delete iter->second;
    }
}
    
//! Use this to insert a term, expression pairing into the SubMap.
//! SubMap takes ownership of expression.
void SubMap::insertPair( Term* factor, Exp* e ) {
    mTerm2ExpMap[factor] = e;
    // FIXME: do we want to check for duplicate factors here?
}

//! Starts iteration over the substitution map.
void SubMap::startIter() {
    mIter = mTerm2ExpMap.begin();
}

//! Indicates whether the map has another pairing.
bool SubMap::hasNext() const {
    return (mIter!=mTerm2ExpMap.end());
}

//! Returns the next term.
Term* SubMap::next() {
    Term* retval = mIter->first;
    mIter++;
    return retval;
}

//! Returns the expression associated with the given Term.
//! Ignores the Terms coefficient and just checks if factor
//! matches something in the SubMap.
//! SubMap still owns expression.  If substitution map doesn't
//! know about the factor sent in then NULL is returned.
Exp* SubMap::subExp(Term* factor) const {
    Exp* retval = NULL;

    // This is where we loop to see if our factor matches any of
    // the keys in searchTermToSubExp.
    std::map<Term*,Exp*>::const_iterator miter;
    for (miter = mTerm2ExpMap.begin(); miter != mTerm2ExpMap.end(); miter++) {
        Term* searchptr = miter->first;
        if (factor->factorMatches(*searchptr)) {
            retval = miter->second;
            break;  // going to return first expression for a matching term
        }
    }
    return retval;

}
        
//! Returns a string representation of the class instance for debugging.
std::string SubMap::toString() const {
    std::stringstream ss;
    ss << "SubMap:" << std::endl;
    std::map<Term*,Exp*>::const_iterator iter;
    for (iter=mTerm2ExpMap.begin(); iter!=mTerm2ExpMap.end(); iter++) {
        ss << "\tterm = " << iter->first->toString() 
           << ", exp = " << iter->second->toString() << std::endl;
    }
    return ss.str();
}

}
