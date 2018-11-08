/*!
 * \file Visitor.h
 *
 * \brief Visitor base class for expressions.h and set_relation.h.
 *
 * Usage: See VisitorDebugTest in set_relation_test for an example.
 *        You do not have to overload every visit method below.
 *        Just overload those that your feature needs to do something at.
 *        The preVisit methods should be called on a node before the
 *        acceptVisit method is called on children nodes and the postVisit
 *        method could be called after acceptVisit has been called on
 *        children nodes.
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */
 
#ifndef VISITOR_H_
#define VISITOR_H_

#include "set_relation.h"
#include "expression.h"

class Visitor {

  public:
    Visitor() {}
    virtual ~Visitor() {}
  
    // Classes in expression.h
    virtual void preVisitTerm(iegenlib::Term * t) {}
    virtual void preVisitUFCallTerm(iegenlib::UFCallTerm * t) {}
    virtual void preVisitTupleVarTerm(iegenlib::TupleVarTerm * t) {}
    virtual void preVisitVarTerm(iegenlib::VarTerm * t) {}
    virtual void preVisitTupleExpTerm(iegenlib::TupleExpTerm * t) {}
    virtual void preVisitExp(iegenlib::Exp * e) {}
    
    virtual void postVisitTerm(iegenlib::Term * t) {}
    virtual void postVisitUFCallTerm(iegenlib::UFCallTerm * t) {}
    virtual void postVisitTupleVarTerm(iegenlib::TupleVarTerm * t) {}
    virtual void postVisitVarTerm(iegenlib::VarTerm * t) {}
    virtual void postVisitTupleExpTerm(iegenlib::TupleExpTerm * t) {}
    virtual void postVisitExp(iegenlib::Exp * e) {}
   
    // Classes in set_relation.h
    virtual void preVisitConjunction(iegenlib::Conjunction * c) {}
    virtual void preVisitSparseConstraints(iegenlib::SparseConstraints * sc) {}
    virtual void preVisitSet(iegenlib::Set * s) {}
    virtual void preVisitRelation(iegenlib::Relation * r) {}
   
    virtual void postVisitConjunction(iegenlib::Conjunction * c) {}
    virtual void postVisitSparseConstraints(iegenlib::SparseConstraints * sc) {}
    virtual void postVisitSet(iegenlib::Set * s) {}
    virtual void postVisitRelation(iegenlib::Relation * r) {}
   
};


#endif
