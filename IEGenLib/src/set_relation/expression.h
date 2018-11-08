/*!
 * \file expression.h
 *
 * \brief Declarations for expression and related classes.
 *
 * The expression classes represent affine expressions with
 * uninterpreted function calls as terms allowed.
 *
 * \date Started: 3/18/2012
 *
 * \authors Michelle Strout and Joe Strout
 *
 * Copyright (c) 2012, Colorado State University <br>
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */
 
#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include "environment.h"
#include "TupleDecl.h"
#include "SubMap.h"
class Visitor;

#include <util/util.h>

namespace iegenlib{

class Exp;

/*!
 *  \class Term
 *
 *  \brief A coefficient multiplied by one.  Subclasses are multiplied by
 *         other entities.
 *
 */
class Term {
public:
    //! Default constructor
    inline Term(int coeff) : mCoeff(coeff), mTermType(ConstVal) {}

    //! Copy constructor
    Term(const Term& other);

    //! Copy assignment
    virtual Term& operator=(const Term& other);

    //! Destructor
    virtual ~Term() {}

    //! Comparison operator -- lexicographic order
    virtual bool operator<(const Term& other) const;

    //! equality operator
    bool operator==(const Term& other) const {
        return ( (((*this)<other) == false)  && ((other<(*this)) == false) );
    }

    //! Create a copy of this Term (and of the same subclass)
    virtual Term* clone() const;

    //! Creates a compact string to help with debugging.
    //! \param absValue Will use absolute value of coeff if true.
    virtual std::string toString(bool absValue = false) const;

    //! Creates a compact string, pretty printed.
    //! \param aTupleDecl name or constant for each tuple var
    //! \param absValue Will use absolute value of coeff if true.
    virtual std::string prettyPrintString(const TupleDecl & aTupleDecl, 
        bool absValue = false) const;
    
    //! Creates a brief compact string to help with DOT output.
    //! \param absValue Will use absolute value of coeff if true.
    virtual std::string toDotString(bool absValue = false) const;

    //! Get the coefficient of this term.
    int coefficient() const { return mCoeff; }
    
    //! Set the coefficient of this term.
    void setCoefficient(int coeff) { mCoeff = coeff; }
    
    //! Returns true if the Term is really a UFCallTerm.
    virtual bool isUFCall() const { return false; }

    //! Returns true if the Term is really a TupleExpTerm.
    virtual bool isTupleExp() const { return false; }

    //! Returns true if the Term is a const
    virtual bool isConst() const { return true; }

// MMS, FIXME: don't want the below.  Can I get rid of above?
//             maybe not, using to do recursion, hmmm
    //! Returns true if the Term is a VarTerm
//    virtual bool isVar() const { return false; }

    //! Returns true if the Term is a TupleVarTerm
//    virtual bool isTupleVar() const { return false; }

    //! Multiply the coefficient by a constant.
    void multiplyBy(int constant);
    
    //! Divide the coefficient by a constant.
    void divideBy(int divisor);
    
    //--------------------- methods for the use in expression

    //! Returns true if this term can be combined with the given term.
    virtual bool factorMatches(const Term& other) const;
    
    /*! Combine another term into this one, if possible, by
    **  adding coefficients of corresponding factors.
    **
    ** \param other -- term to attempt to combine with this one.(adopt)
    ** \return true if other was combined with this one; false otherwise
    */
    virtual bool combine(Term* other);
    
    //! Returns string of subclass type.
    virtual std::string type() const;
    
    //! Return a new Exp with all nested functions such as
    //! f ( f_inv ( i ) ) changed to i.
    virtual Exp* collapseNestedInvertibleFunctions() const;
    
    //! Visitor design pattern, see Visitor.h for usage
    virtual void acceptVisitor(Visitor *v);

protected:
    typedef enum {TupleVar, SymConst, UFCall, ConstVal, TupleExp} termtype;
    inline termtype getTermType() const { return mTermType; }
    inline void setTermType(termtype tt) { mTermType = tt; }
    int compareTermTypes(const Term& other) const;

    //! \param absValue Will use absolute value of coeff if true.
    void coeffToStream(std::stringstream& ss, bool absValue) const;
    
private:
    int mCoeff;
    termtype mTermType;

};

/*!
 *  \class UFCallTerm
 *
 *  \brief Represents a coefficient multiplied by an uninterpreted function call.
 */
class UFCallTerm : public Term {
public:
    /*! Constructor
    **
    **  Memory management: this object assumes ownership of the
    **  passed-in Exp objects.  The caller must not destroy them,
    **  and should not assume they survive destruction of this
    **  UFCallTerm.
    **
    ** \param coeff -- coefficient for the call
    ** \param funcName -- function to call
    ** \param num_args -- number of arguments the function call will have
    ** \param args -- arguments for the function call 
    **                (adopted)
    ** \param tuple_loc -- index value if function returns a tuple
    **                     and is being indexed,
    **                     so term is still of type integer.  If value
    **                     is -1 assuming not in use.
    **
    ** If a tuple_loc is not specified and the function returns a tuple,
    ** then the UFCallTerm type is a tuple.  size() will indicate size
    ** of tuple being returned.
    */
    UFCallTerm(int coeff, std::string funcName, unsigned int num_args, 
               int tuple_loc=-1);

    //! Convenience constructor, assumes coeff = 1
    UFCallTerm(std::string funcName, unsigned int num_args, int tuple_loc=-1);

    //! Copy constructor
    UFCallTerm(const UFCallTerm& other);

    //! Copy assignment
    UFCallTerm& operator=(const UFCallTerm& other);

    //! Destructor
    void reset();
    ~UFCallTerm();

    //! Comparison operator -- lexicographic order
    bool operator<(const Term& other) const;

    //! Create a copy of this Term (and of the same subclass)
    Term* clone() const;

    //! Creates a compact string to help with debugging.
    //! \param absValue Will use absolute value of coeff if true.
    std::string toString(bool absValue = false) const;

    //! Replaces any tuple var instances with given tuple var decl.
    //! \param aTupleDecl name or constant for each tuple var
    //! \param absValue Will use absolute value of coeff if true.
    std::string prettyPrintString(const TupleDecl & aTupleDecl, 
        bool absValue = false) const;

    //! Creates a brief compact string to help with DOT output.
    //! \param absValue Will use absolute value of coeff if true.
    std::string toDotString(bool absValue = false) const;

    //! Returns string of subclass type.
    std::string type() const;

    //! Returns true if the Term is really a UFCallTerm.
    bool isUFCall() const { return true; }

    //! Returns true if the Term is a const
    bool isConst() const { return false; }

    //--------------------- UFCallTerm specific methods

    //! Returns the number of arguments for the call.
    unsigned int numArgs() const;
    
    //! Set the ith parameter expression to the given pointer.
    //! This UFCallTerm becomes owner of the expression.
    void setParamExp(unsigned int i, Exp* param_exp);
    
    //! Returns a pointer to the ith parameter expression.
    //! This UFCallTerm still owns the pointer.
    Exp* getParamExp(unsigned int i) const;

    //! Indicate if the function return value is being
    //! indexed.
    bool isIndexed() const;
    
    //! Returns a UFCallTerm that is identical except it is
    //! not indexed.
    UFCallTerm* nonIndexedClone() const;
    
    //! Returns the index in a tuple that the return
    //! value is.  For functions that return tuples
    //! of size 1, this is always 0.
    //! If the function return is not being indexed this
    //! will also be zero.  Use in coordination with isIndexed().
    int tupleIndex() const;
    
    //! Enables the tuple index to be set.
    void setTupleIndex(unsigned int idx);

    // Number of entries in the returned tuple.
    // The size will be one if the function returns
    // a tuple but is being indexed.
    unsigned int size() const;

    //! Returns the function name as a string.
    std::string name() const { return mFuncName; }
    
    //! Enables the function name to be set.
    void setName(std::string n) { mFuncName = n; }

    //! Return a new Exp with all nested functions such as
    //! f ( f_inv ( i ) ) changed to i.
    Exp* collapseNestedInvertibleFunctions() const;

    //--------------------- methods for the use in expression

    //! Returns true if this term can be combined with the given term.
    bool factorMatches(const Term& other) const;
    
    //! Visitor design pattern, see Visitor.h for usage
    void acceptVisitor(Visitor *v);


private:
    void argsToStream(std::stringstream& ss) const;
    void argsToStreamPrettyPrint(const TupleDecl & aTupleDecl,
        std::stringstream& ss) const;

    std::string mFuncName;
    unsigned int mNumArgs;
    std::vector<Exp*> mArgs;
    int mTupleIndex;
};

/*!
 *  \class TupleVarTerm
 *
 *  \brief Represents a coefficient multiplied by a tuple variable.
 */
class TupleVarTerm : public Term {
public:
    //! Constructor
    inline TupleVarTerm(int coeff, int location)
    : Term(coeff), mLocation(location) { setTermType(TupleVar); }

    //! Convenience constructor, assumes coeff = 1
    inline TupleVarTerm(int location)
    : Term(1), mLocation(location) { setTermType(TupleVar); }
    
    //! Copy constructor
    TupleVarTerm(const TupleVarTerm& other);

    //! Copy assignment
    TupleVarTerm& operator=(const TupleVarTerm& other);

    //! Comparison operator -- lexicographic order
    bool operator<(const Term& other) const;

    //! Create a copy of this Term (and of the same subclass)
    Term* clone() const;

    //! Creates a compact string to help with debugging.
    //! \param absValue Will use absolute value of coeff if true.
    std::string toString(bool absValue = false) const;

    //! Creates a compact string, pretty printed.
    //! \param aTupleDecl name or constant for each tuple var
    //! \param absValue Will use absolute value of coeff if true.
    std::string prettyPrintString(const TupleDecl & aTupleDecl,
        bool absValue = false) const;

    //! Returns string of subclass type.
    std::string type() const;

    //! Returns true if the Term is a const
    bool isConst() const { return false; }

    // Returns location of TV 
    int tvloc(){return mLocation;}

    //--------------------- methods for the use in expression

    //! Returns true if this term has the same factor (i.e. everything
    //! except the coefficient) as the given other term.
    bool factorMatches(const Term& other) const;

    //! Remap our location according to the given map vector.
    //! See Exp::remapTupleVars for more detail.
    void remapLocation(const std::vector<int>& oldToNewLocs);

    //! Visitor design pattern, see Visitor.h for usage
    void acceptVisitor(Visitor *v);

private:
    int mLocation;
};

/*!
 *  \class VarTerm
 *
 *  \brief Represents a coefficient multiplied by a variable or symbolic constant.
 */
class VarTerm : public Term {
public:
    //! Constructor
    inline VarTerm(int coeff, std::string symbol) 
    : Term(coeff), mSymbol(symbol) { setTermType(SymConst); }

    //! Convenience constructor, assumes coeff = 1
    inline VarTerm(std::string symbol) 
    : Term(1), mSymbol(symbol) { setTermType(SymConst); }
    
    //! Copy constructor
    VarTerm(const VarTerm& other);

    //! Copy assignment
    VarTerm& operator=(const VarTerm& other);

    //! Comparison operator -- lexicographic order
    bool operator<(const Term& other) const;

    //! Create a copy of this Term (and of the same subclass)
    Term* clone() const;

    //! Creates a compact string to help with debugging.
    //! \param absValue Will use absolute value of coeff if true.
    std::string toString(bool absValue = false) const;

    //! Creates a compact string, pretty printed.
    //! \param aTupleDecl name or constant for each tuple var
    //! \param absValue Will use absolute value of coeff if true.
    std::string prettyPrintString(const TupleDecl & aTupleDecl,
        bool absValue = false) const;

    //! Return the variable string
    std::string symbol() const { return mSymbol; }

    //! Returns string of subclass type.
    //! Used by toDotString() only.  Might want to refactor.
    std::string type() const;

    //! Returns true if the Term is a const
    bool isConst() const { return false; }

    //--------------------- methods for the use in expression

    //! Returns true if this term can be combined with the given term.
    bool factorMatches(const Term& other) const;

    //! Visitor design pattern, see Visitor.h for usage
    void acceptVisitor(Visitor *v);

private:
    std::string mSymbol;
};

/*!
 *  \class TupleExpTerm
 *
 *  \brief A tuple of expressions.
 *
 *  Modeled somewhat off of UFCallTerm but also off TupleDecl in
 *  terms of not exposing how expressions are stored.
 *
 *  Memory management: the TupleExpTerm assumes ownership of any 
 *  expressions it contains,
 *  copies those when the TupleExpTerm itself is copied, 
 *  and deletes them when the TupleExpTerm is destroyed.
 */
class TupleExpTerm : public Term {
public:

    /*! Constructor
    **
    **  Memory management: this object assumes ownership of the
    **  passed-in Exp objects.  The caller must not destroy them,
    **  and should not assume they survive destruction of this
    **  UFCallTerm.
    **
    ** \param coeff -- coefficient for the call
    */
    TupleExpTerm(int coeff, unsigned int size);
    void reset();
    ~TupleExpTerm();

    //! Convenience constructor, assumes coeff = 1
    TupleExpTerm(unsigned int size);
    
    //! Copy constructor.
    TupleExpTerm( const TupleExpTerm& other );
    
    //! Copy assignment operator.
    TupleExpTerm& operator=( const TupleExpTerm& other);
    
    //! Less than operator.
    bool operator<( const Term& other) const;
    
    //! Equality operator.
    //bool operator==( const TupleExpTerm& other) const;

    /*! Combine another tuple expression term 
    **  into this one, if possible, by
    **  adding sub expressions pointwise.
    **
    ** \param other -- term to attempt to combine with this one.(adopt)
    ** \return true if other was combined with this one; false otherwise
    */
    bool combine(Term* other);

    //! Create a copy of this Term (and of the same subclass)
    Term* clone() const;

    //! Creates a comma separated list of expressions.
    //! \param absValue Will use absolute value of coeff if true.
    std::string toString(bool absValue = false) const;

    //! In comma-separated list of expressions,
    //! replaces any tuple var instances with given tuple var decl.
    std::string prettyPrintString(const TupleDecl & aTupleDecl, 
        bool absValue = false) const;

    //! Creates a brief compact string to help with DOT output.
    std::string toDotString(bool absValue = false) const;

    //! Returns string of subclass type.
    std::string type() const;

    //! Returns true if the Term is really a UFCallTerm.
    bool isTupleExp() const { return true; }

    //! Returns true if the Term is a const
    bool isConst() const { return false; }

    //--------------------- UFCallTerm specific methods

    //! Setting individual expressions.  Indexing starts at 0.
    //! Becomes owner of passed in expression.
    void setExpElem(unsigned int exp_index, Exp* exp);

    //! Returns pointer to individual expression.  Indexing starts at 0.
    Exp* getExpElem(unsigned int exp_index);

    //! Returns a clone of the specified expression.
    Exp* cloneExp(unsigned int exp_index) const;        
  
    // Number of entries in the tuple.
    unsigned int size() const;

    //--------------------- methods for the use in expression

    //! Returns true if this term can be combined with the given term.
    bool factorMatches(const Term& other) const;

    //! Visitor design pattern, see Visitor.h for usage
    void acceptVisitor(Visitor *v);

private:
    unsigned int mSize;
    std::vector<Exp*> mExps;
};


/*!
 *  \class Exp
 *
 *  \brief An affine expression that allows uninterpreted function call terms.
 *
 *  Memory management: the Exp manages its own copies of any terms it contains,
 *  copies those when the Exp itself is copied, and deletes them when the Exp
 *  is destroyed.
 */
class Exp {
public:
    //! Default constructor
    inline Exp() {setExpression();}

    //! Copy constructor.  Performs a deep copy.
    Exp(const Exp& other);

    //! Copy assignment
    Exp& operator=(const Exp& other);

    //! Destructor
    void reset();
    virtual ~Exp();

    //! Create a copy of this Exp (and of the same subclass)
    virtual Exp* clone() const;

    //! Creates a compact string to help with debugging.
    virtual std::string toString() const;

    //! Convert to a human-readable string (substitute in tuple vars).
    virtual std::string prettyPrintString(const TupleDecl & aTupleDecl) const;

    //! Add a term to this expression.
    //! /param term (adopted)
    void addTerm(Term* term);
    
    //! Add another expression to this one.  
    //! /param term (adopted)
    void addExp(Exp* exp);

    //! Multiply all terms in this expression by a constant.
    void multiplyBy(int constant);

    //! Return whether all coefficients in this expression are
    //! evenly divisible by the given integer.
    bool isDivisible(int divisor) const;

    //! Divide all coefficients and the constant term by the given divisor.
    void divideBy(int divisor);
   
    //! Assumes the equality Exp=0 and solves for the given factor.
    //! FIXME: should only work on equality expressions.  Really?
    //! /param factor (adopted) 
    Exp* solveForFactor(Term* factor) const;

    /*! Search this expression for the given factor and invert a function
        to expose the factor.  Return a new expression (which is an equality
        constraint, exp=0, that exposes the term with the factor.
        Returns NULL if can't do this.
    */
    //! FIXME: should only work on equality expressions.  Really?
    Exp* invertFuncToExposeFactor(Term * factor) const;

    //! Substitute each expression for the factor (i.e. the non-coefficient
    //! part of a term), which is its key.
    //void substitute(const std::map<Term*,Exp*>& searchTermToSubExp);
    void substitute(SubMap& searchTermToSubExp);
    
    /*! Normalize this expression for use in an equality expression.
    **  This is called when we know this expression is equal to zero;
    **  in that case, it's valid to multiply the whole expression by
    **  -1.  So we do so, in order to ensure the first term has a
    **  positive coefficient, so that equivalent expressions can be
    **  reliably compared.
    */
    //! FIXME: should only work on equality expressions.
    void normalizeForEquality();
    
    //! Return a new Exp with all nested functions such as
    //! f ( f_inv ( i ) ) changed to i.
    Exp* collapseNestedInvertibleFunctions() const;

    /*! Search for the given factor anywhere in this expression
    **  (including within UFCallTerm arguments, recursively).
    */
    bool dependsOn(const Term& factor) const;
    
    /*! Returns true if this expression contains a UFCallTerm
    **  that is being indexed.
    */
    bool hasIndexedUFCall() const;

    /*! Returns a clone of the single indexed UFCall in expression.
    **  If there are none or more than one then an exception is thrown.
    */
    UFCallTerm* cloneIndexedUFCallTerm() const;

    /*! Return true iff this expression has no terms, or has
    **  only a constant term equal to 0.
    */
    bool equalsZero() const;
    
    //! Returns true if this expression equals the given term.
    bool operator==(const Term& other) const;
    
    //! Less than operator.
    bool operator<( const Exp& other) const;
    
    //! Equality operator.
    bool operator==( const Exp& other) const;
    
    /*! Find any TupleVarTerms in this expression (and subexpressions)
    **  and remap the locations according to the oldToNewLocs vector,
    **  where oldToNewLocs[i] = j means that old location i becomes new
    **  location j (i.e. __tvi -> __tvj).  Throws an exception if an
    **  old location is out of range for the given oldToNewLocs.
    **  The new location will be -1 for old locations that are not being
    **  remapped.
    */
    void remapTupleVars(const std::vector<int>& oldToNewLocs);
    
    //! Calls the ExpCase for the visitor design pattern
    //void apply(SRVisitor* visitor) const { }

    //! Sets mExpType to Expression, for a simple expression
    inline void setExpression() { mExpType = Expression; }

    //! Sets mExpType to Inequality, to indicate Exp >= 0
    inline void setInequality() { mExpType = Inequality; }

    //! Sets mExpType to Equality, to indicate Exp == 0
    inline void setEquality() { mExpType = Equality; }

    //! Returns true if the Exp is a simple expression, not a constraint
    //! Does not mean it is a UFCall param.
    inline bool isExpression() { return (getExpType() == Expression); }

    //! Returns true if the Exp is an inequality, ie expression >= 0
    inline bool isInequality() { return (getExpType() == Inequality); }

    //! Returns true if the Exp is an equality, ie expression == 0
    inline bool isEquality() { return (getExpType() == Equality); }

    //! Returns true if only have a constant term
    bool isConst() const;

    //! Returns true if we have something like: 2 = 0
    bool isContradiction() const;

    //! Return Term* if the expression has only one Term.
    //! Otherwise returns NULL.
    //! this still owns Term.
    Term* getTerm() const;
    
    //! Return Term* for constant term if there is one.
    //! Otherwise return NULL.
    //! This expression still owns the Term.
    Term* getConstTerm() const;

    //! Output the Exp in dot format.
    //! Note here, we still need to provide "digraph name {" and "}"
    //! Pass in the parent node id and the next node id.
    //! The next node id will be set upon exit from this routine.
    //! If no parent id is given then will not draw edge from parent to self.
    std::string toDotString(int & next_id) const;
    std::string toDotString(int parent_id, int & next_id) const;

    //! Returns an iterator over symbolic constant variables.
    //! Assumes all VarTerm's are symbolic constants.
    StringIterator* getSymbolIterator() const;

    //! Visitor design pattern, see Visitor.h for usage
    void acceptVisitor(Visitor *v);
    
    //! Get a list of pointers to the terms in this expression.
    //! All pointers in this list are still owned by the expression.
    //! Caller should NOT modify expressions or delete them.
    std::list<Term*> getTermList() const;

protected:

    typedef enum {Expression, Inequality, Equality} exptype;
    inline exptype getExpType() const { return mExpType; }

private:
    /*! Search this Exp for the given factor.  The cloned Term that is returned
       can have a coefficient other than 1.  The factor param should have 
       a coefficient of 1.  Returns NULL if a matching Term is not found.
    */
    Term* findMatchingFactor(const Term & factor) const;        

    std::list<Term*> mTerms;
    exptype mExpType; 

};



}//end namespace iegenlib

#endif /* EXPRESSION_H_ */
