/*!
 * \file SubMap.h
 *
 * \brief Interface of SubMap class
 *
 * \date Started: 2013-08-20
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */
 
#ifndef SUBMAP_H_
#define SUBMAP_H_

#include <map>
#include <string>

namespace iegenlib{

class Term;
class Exp;

/*!
 * \class SubMap
 *
 * This class contains a mapping of factors (Terms) to expressions
 * that should replace those factors.  Needed so that the appropriate
 * cleanup of these terms and expressions occur.
 */
class SubMap {
public:
    SubMap();
    ~SubMap();
    
    //! Use this to insert a term, expression pairing into the SubMap.
    //! SubMap takes ownership of term and expression.
    //! Term has to be a pointer because need polymorphism.
    //! Changes term to have a coefficient of 1.
    void insertPair( Term* factor, Exp* e );
    
    //! Starts iteration over the substitution map.
    void startIter();
    
    //! Indicates whether the map has another pairing.
    bool hasNext() const;
    
    //! Returns the next term.
    Term* next();
    
    //! Returns the expression associated with the given Term.
    //! Ignores the Terms coefficient and just checks if factor
    //! matches something in the SubMap.
    //! SubMap still owns expression.  If substitution map doesn't
    //! know about the factor sent in then NULL is returned.
    Exp* subExp(Term* factor) const;
    
     //! Returns a string representation of the class instance for debugging.
    std::string toString() const;        
        
private:
    std::map<Term*,Exp*>                     mTerm2ExpMap;
    std::map<Term*,Exp*>::const_iterator     mIter; 
};

}

#endif
