/*!
 * \file expression.cc
 *
 * \brief Implementations of the expression classes.
 *
 * \authors Michelle Strout and Joe Strout
 *
 * Copyright (c) 2012, Colorado State University <br>
 * Copyright (c) 2015, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "expression.h"
#include "TupleDecl.h"
#include "set_relation.h"
#include "Visitor.h"


namespace iegenlib{

/******************************** Term **************************************/
Term::Term(const Term& other) {
    *this = other;
}

Term& Term::operator=(const Term& other) {
    mCoeff = other.mCoeff;
    mTermType = other.mTermType;
    return *this;
}

/*! Compare two terms in a canonical order, defined as follows:
**      1. by term type: TupleVar, SymConst, UFCall, ConstVal
**      2. within each type, in alphabetical or numeric order.
** @param other, object to be compared
*/
bool Term::operator<(const Term& other) const {
    // Compare first by type -- note that the order of the
    // enumeration should match our desired sort order.
    if (mTermType < other.mTermType) { return true; }
    if (mTermType > other.mTermType) { return false; }

    // If the types match, then compare by coefficient.
    return (mCoeff < other.mCoeff);
}


/*! Return -1 if this mTermType < other.mTermType,
**          1 if this mTermType > other.mTermType,
**      and 0 if the mTermTypes are identical.
** @param other, object to be compared
*/
int Term::compareTermTypes(const Term& other) const {
    if (mTermType < other.mTermType) { return -1; }
    if (mTermType > other.mTermType) { return 1; }
    return 0;
}

//! We need a clone method because Term has subclasses.  We don't
//! always know what copy constructor to use.
Term* Term::clone() const {
    return new Term(*this);
}

/*! coeffToStream: output the coefficient to the stream,
**  if it is not equal to one; this is used mainly by
**  the subclasses, as part of their toString() process.
*/
void Term::coeffToStream(std::stringstream& ss, bool absValue) const {
    if (absValue) {
        // Emit the absolute value of the coefficient, unless
        // that abs value is 1, in which case it is not needed.
        if (1 == abs(mCoeff)) return;
        ss << abs(mCoeff) << ' ';
    } else {
        // Emit the actual coefficient, unless it's 1 (not needed
        // at all) or -1 (emit just the sign).
        if (1 == mCoeff) return;
        if (-1 == mCoeff) {
            ss << '-';
        } else {
            ss << mCoeff << ' ';
        }
    }
}

//! Creates a compact string to help with debugging.
std::string Term::toString(bool absValue) const {
    std::stringstream ss;
    ss << (absValue ? abs(mCoeff) : mCoeff);
    return ss.str();
}

//! Creates a compact string pretty printed.
std::string 
Term::prettyPrintString(const TupleDecl & aTupleDecl, bool absValue) const {
    std::stringstream ss;
    ss << (absValue ? abs(mCoeff) : mCoeff);
    return ss.str();
}

//! Creates a brief compact string to help with DOT output.
std::string Term::toDotString(bool absValue) const {
    return toString(absValue);
}

// Returns Term subclass type.
std::string Term::type() const{
    return "Term";
}

//--------------------- methods for the use in expression

//! Returns true if this term has the same factor (i.e. everything
//! except the coefficient) as the given other term.
bool Term::factorMatches(const Term& other) const {
    // At the Term level, we can always combine, provided the
    // two terms are of the same subtype.
    return (other.mTermType == mTermType);
}

bool Term::combine(Term* other) {
    if (not factorMatches(*other)) { delete other; return false; }
    mCoeff += other->mCoeff;
    delete other;
    return true;    
}

void Term::multiplyBy(int constant) {
    mCoeff *= constant;
}

void Term::divideBy(int divisor) {
    // throw an exception here if mCoeff is not divisible by divisor
    if ( (mCoeff % divisor) != 0 ) {
        throw assert_exception( "Term::divideBy: "
                                "mCoeff is not divisible by divisor");
    }
    mCoeff /= divisor;
}

//! Return a new Exp with all nested functions such as
//! f ( f_inv ( i ) ) changed to i.
Exp* Term::collapseNestedInvertibleFunctions() const {
    Exp* retval = new Exp();
    retval->addTerm( this->clone() );
    return retval;
}


#pragma mark -
/****************************************************************************/
/******************************** UFCallTerm ********************************/
UFCallTerm::UFCallTerm(int coeff, std::string funcName, unsigned int num_args,
                       int tuple_index)
: Term(coeff), mFuncName(funcName), mNumArgs(num_args),
  mTupleIndex(tuple_index) 
{
    // Initialize all ptrs in vector to NULL.
    for (unsigned int i=0; i<num_args; i++) {
        mArgs.push_back(NULL);
    }
    setTermType(UFCall);
    
}

UFCallTerm::UFCallTerm(std::string funcName, unsigned int num_args,
                       int tuple_index)
: Term(1), mFuncName(funcName), mNumArgs(num_args),
  mTupleIndex(tuple_index)
{
    // Initialize all ptrs in vector to NULL.
    for (unsigned int i=0; i<num_args; i++) {
        mArgs.push_back(NULL);
    }
    setTermType(UFCall);
}

UFCallTerm::UFCallTerm(const UFCallTerm& other) : Term(other) {
    *this = other;
}

void UFCallTerm::reset() {
    for (unsigned int i=0; i<mArgs.size(); i++) {
        if (mArgs[i]) { delete mArgs[i]; }
    }
    mArgs.clear();
}

//! Copy assignment
UFCallTerm& UFCallTerm::operator=(const UFCallTerm& other) {
    // clear out own args
    reset();

    mFuncName = other.mFuncName;
    mNumArgs = other.mNumArgs;
    mTupleIndex = other.mTupleIndex;

    for (unsigned int i=0; i<mNumArgs; i++) {
        if (other.mArgs[i]==NULL) {
            throw assert_exception("UFCallTerm::operator=: other missing"
                    " a parameter expression");
        }
        mArgs.push_back( other.mArgs[i]->clone() );
    }

    return *this;
}

//! Destructor
UFCallTerm::~UFCallTerm() {
    reset();
}

/*! Compare two terms in a canonical order, defined as follows:
**      1. by term type: TupleVar, SymConst, UFCall, ConstVal
**      2. within each type, in alphabetical or numeric order.
*/
bool UFCallTerm::operator<(const Term& other) const {
    // Compare first by type -- note that the order of the
    // enumeration should match our desired sort order.
    int termComparison = compareTermTypes(other);
    if (-1 == termComparison) { return true; }
    if (1 == termComparison) { return false; }

    // Next, compare by function name
    if (mFuncName < ((UFCallTerm&)other).mFuncName) { return true; }
    if (mFuncName > ((UFCallTerm&)other).mFuncName) { return false; }

    // Then compare by tuple location (always -1 unless function returns tuple)
    if (mTupleIndex < ((UFCallTerm&)other).mTupleIndex) 
    { return true; }
    if (mTupleIndex > ((UFCallTerm&)other).mTupleIndex) 
    { return false; }

    // Now compare the argument list (as one big string).
    std::stringstream ss1;
    argsToStream(ss1);
    std::string args = ss1.str();

    std::stringstream ss2;
    ((UFCallTerm&)other).argsToStream(ss2);
    std::string otherArgs = ss2.str();
    
    if (args < otherArgs) { return true; }
    if (args > otherArgs) { return false; }
    
    // If everything else matches, let the superclass compare by coefficient.
    return Term::operator<(other);
}

Term* UFCallTerm::clone() const {
    return new UFCallTerm(*this);
}

//! Emits our argument list, as strings, to the given stream.
//! Helper method for toString and operator<.
void UFCallTerm::argsToStream(std::stringstream& ss) const {
    bool firstArg = true;
    for (std::vector<Exp*>::const_iterator i=mArgs.begin(); 
            i != mArgs.end(); ++i) {
        if (not firstArg) { ss << ", "; }
        if (*i) { ss << (*i)->toString(); }
        firstArg = false;
    }
}

//! Emits our argument list, as strings, to the given stream (pretty printed).
//! Helper method for prettyPrintString.
void UFCallTerm::argsToStreamPrettyPrint(
        const TupleDecl & aTupleDecl, std::stringstream& ss) const {
    bool firstArg = true;
    for (std::vector<Exp*>::const_iterator i=mArgs.begin(); 
            i != mArgs.end(); ++i) {
        if (not firstArg) { ss << ", "; }
        if (*i) { ss << (*i)->prettyPrintString(aTupleDecl); }
        firstArg = false;
    }
}

//! Creates a compact string to help with debugging.
std::string UFCallTerm::toString(bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << mFuncName << '(';
    argsToStream(ss);
    ss << ')';
    if (isIndexed()) {
        ss << '[' << tupleIndex() << ']';
    }
    return ss.str();
}

//! Creates a compact string, pretty Printed.
std::string UFCallTerm::prettyPrintString(
        const TupleDecl & aTupleDecl, bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << mFuncName << '(';
    argsToStreamPrettyPrint(aTupleDecl, ss);
    ss << ')';
    if (isIndexed()) {
        ss << '[' << tupleIndex() << ']';
    }
    return ss.str();
}

//! Creates a compact string to help with DOT output.
std::string UFCallTerm::toDotString(bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << mFuncName << "(...)";
    if (isIndexed()) {
        ss << '[' << tupleIndex() << ']';
    }
    return ss.str();
}

// Returns Term subclass type.
std::string UFCallTerm::type() const{
    return "UFCallTerm";
}

//--------------------- UFCalTerm specific methods

unsigned int UFCallTerm::numArgs() const
{ return mNumArgs; }

//! Set the ith parameter expression to the given pointer.
void UFCallTerm::setParamExp(unsigned int i, Exp* param_exp) {
    if (i>=mNumArgs) {
        throw assert_exception("UFCallTerm::setParamExp: i is out of bounds");
    }
    if (mArgs[i]) { 
        throw assert_exception("UFCallTerm::setParamExp: param exp being set "
                                "twice");
    }
    mArgs[i] = param_exp;
}

//! Returns a pointer to the ith parameter expression.
Exp* UFCallTerm::getParamExp(unsigned int i) const {
    if (i>=mNumArgs) {
        throw assert_exception("UFCallTerm::setParamExp: i is out of bounds");
    }
    if (mArgs[i]==NULL) {
        throw assert_exception("UFCallTerm::setParamExp: requested parameter "
                                "hasn't been set");
    }
    return mArgs[i];
}

//! Returns a UFCallTerm that is identical except it is
//! not indexed.
UFCallTerm* UFCallTerm::nonIndexedClone() const {
    UFCallTerm* retval = new UFCallTerm(*this);
    retval->mTupleIndex = -1;
    return retval;
}


//! Indicate if the function return value is being
//! indexed.
bool UFCallTerm::isIndexed() const {
    // If tuple_loc is -1,
    // then this function call result is not being indexed.
    if (mTupleIndex==-1) { return false; }
    else { return true; }   
}
    
//! Returns the location in a tuple that the return
//! value is.  For functions that return tuples
//! of size 1, this is always 0.
int UFCallTerm::tupleIndex() const{
    if (!isIndexed()) { return 0; }
    else { return mTupleIndex; }
}

//! Enables the tuple index to be set.
void UFCallTerm::setTupleIndex(unsigned int idx) {
    mTupleIndex= idx;
}

// Number of entries in the returned tuple.
// The size will be one if the function returns
// a tuple but is being indexed.
unsigned int UFCallTerm::size() const {
    // look up the range arity of the function
    return iegenlib::queryRangeArityCurrEnv(name());
}


//--------------------- methods for the use in expression

//! Returns true if this term can be combined with the given term.
bool UFCallTerm::factorMatches(const Term& other) const {
    // bail out on the basic inherited check (comparing types)
    if (not Term::factorMatches(other)) { return false; }
    
    // check that the function names match
    const UFCallTerm &ufo = (const UFCallTerm&)other;
    if (mFuncName != ufo.mFuncName) { return false; }
    
    // check that the tuple location matches
    if (tupleIndex() != ufo.tupleIndex()) { return false; }

    // check that the argument lists match
    std::stringstream ss1;
    argsToStream(ss1);
    std::stringstream ss2;
    ufo.argsToStream(ss2);
    return (ss1.str() == ss2.str());    
}


//! Return a new Exp with all nested functions such as
//! f ( f_inv ( i ) ) changed to i.
//! g(g_inv(x)[0], g_inv(x)[1]) changed to x
//! Both outside functions
//! need to NOT return a tuple, so mTupleIndex should be -1
//! in the UFCallTerm in the returned expression.
Exp* UFCallTerm::collapseNestedInvertibleFunctions() const {

    Exp* retval;

    // determine the name of any inverse function for this function call
    std::string inverseFuncName;
    inverseFuncName = iegenlib::queryInverseCurrEnv(mFuncName);

    // Check to see if each argument for this function call
    // is a call to our inverse with the same expression passed in.
    Exp* saved_arg = NULL;
    int arg_index = 0;
    bool can_collapse_self = true;
    for (std::vector<Exp*>::const_iterator i=mArgs.begin(); 
            i != mArgs.end(); ++i) {
        Exp* arg = *i;
        
        // Get the first term to the argument expression.  
        // Should only have one term. 
        Term* t = arg->getTerm();
        if ( (t!=NULL) && t->isUFCall() ) {
            UFCallTerm* ufcallterm = dynamic_cast<UFCallTerm*>(t);
            
            // See if the nested ufcallterm is our inverse
            if (ufcallterm->mFuncName != inverseFuncName) {
                can_collapse_self = false;
                break;
            } else {
                // Check that the tuple location for this nested function
                // call maps which are we are in outer function call.
                // IOW, if at arg 0 in outer function call need f_inv(...)[0].
                if (arg_index != ufcallterm->tupleIndex()) {
                    can_collapse_self = false;
                    break;
                }
                
                // check that nested ufcallterm only has one argument
                if (ufcallterm->mArgs.size() !=1 ) {
                    // FIXME: will we need to handle this case at
                    // some point?
                    throw assert_exception(
                        "UFCallTerm::collapseNestedInvertibleFunctions: "
                        "nested invertible function should only take one arg");
                }

                // Save off the nested arg to the zeroth nested function call.
                Exp* nestedParam = ufcallterm->mArgs.front();
                if (arg_index==0) {
                    saved_arg = nestedParam;
                }
                // otherwise make sure the argument to the nested function 
                // is the same as other nested functions
                else {
                    if (not (*nestedParam == *saved_arg)) { 
                        can_collapse_self = false;
                        break;
                    }
                }
            
            }
            
        }
        // one of the parameter arguments is not a UFCallTerm 
        else {
            can_collapse_self = false;
                        break;
        }
        
        arg_index++;
    }
    if (can_collapse_self) {
        // return a collapse of the arguments
        Exp* arg = saved_arg;
        retval = arg->collapseNestedInvertibleFunctions();
        retval->multiplyBy(this->coefficient());
    } else {    
        // did not need to collapse self
        // try to do a recursive collapse on parameters
        retval = new Exp();
        UFCallTerm* uf_call = new UFCallTerm( this->coefficient(), 
                         this->mFuncName, mArgs.size(), 
                         this->mTupleIndex);
        unsigned int count = 0;
        for (std::vector<Exp*>::const_iterator i=mArgs.begin(); 
                i != mArgs.end(); ++i) {
            uf_call->setParamExp(count++,     
                (*i)->collapseNestedInvertibleFunctions());
        } 
        retval->addTerm( uf_call );
    }
    
    return retval;
}


#pragma mark -
/****************************************************************************/
/******************************** TupleVarTerm ******************************/
//! Copy constructor
TupleVarTerm::TupleVarTerm(const TupleVarTerm& other) : Term(other) {
    *this = other;
}

//! Copy assignment
TupleVarTerm& TupleVarTerm::operator=(const TupleVarTerm& other) {
    Term::operator=(other);
    mLocation = other.mLocation;
    return *this;
}

/*! Compare two terms in a canonical order, defined as follows:
**      1. by term type: TupleVar, SymConst, UFCall, ConstVal
**      2. within each type, in alphabetical or numeric order.
*/
bool TupleVarTerm::operator<(const Term& other) const {
    // Compare first by type -- note that the order of the
    // enumeration should match our desired sort order.
    int termComparison = compareTermTypes(other);
    if (-1 == termComparison) { return true; }
    if (1 == termComparison) { return false; }

    // Next, by tuple variable location.
    if (mLocation < ((TupleVarTerm&)other).mLocation) { return true; }
    if (mLocation > ((TupleVarTerm&)other).mLocation) { return false; }

    // If everything else matches, let the superclass compare by coefficient.
    return Term::operator<(other);
}

Term* TupleVarTerm::clone() const {
    return new TupleVarTerm(*this);
}

//! Creates a compact string to help with debugging.
std::string TupleVarTerm::toString(bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << "__tv" << mLocation;
    return ss.str();
}

//! Creates a compact string, pretty printed (replace __tv# with name).
std::string TupleVarTerm::prettyPrintString(
        const TupleDecl & aTupleDecl, bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << aTupleDecl.elemToString(mLocation);
    if(aTupleDecl.elemIsConst(mLocation))
    {
        throw assert_exception("TupleVarTerm::prettyPrintString: "
            "tuple var location declaration for tuple var term should "
            "not be constant");
    }
    return ss.str();
}

//! Returns true if this term can be combined with the given term.
bool TupleVarTerm::factorMatches(const Term& other) const {
    return Term::factorMatches(other) 
            and mLocation == ((TupleVarTerm&)other).mLocation;
}

//! Remap our location according to the given map vector.
//! See Exp::remapTupleVars for more detail.
void TupleVarTerm::remapLocation(const std::vector<int>& oldToNewLocs) {
    if (mLocation >= 0 and (unsigned int)mLocation < oldToNewLocs.size()) {
        if (oldToNewLocs[mLocation] < 0) {
           throw iegenlib::assert_exception(
                "TupleVarTerm::remapLocation: mapping "
                + toString() + " to location < 0");
        }
        mLocation = oldToNewLocs[mLocation];
    } else {
        std::stringstream ss;
        ss << "TupleVarTerm::remapLocation: mLocation = "
           << mLocation << ", out of range [0," << (oldToNewLocs.size()-1) 
           << "]";
        throw iegenlib::assert_exception(ss.str());
    }
}

// Returns Term subclass type.
std::string TupleVarTerm::type() const{
    return "TupleVarTerm";
}

#pragma mark -
/****************************************************************************/
/******************************** VarTerm ******************************/
//! Copy constructor
VarTerm::VarTerm(const VarTerm& other) : Term(other) {
    *this = other;
}

//! Copy assignment
VarTerm& VarTerm::operator=(const VarTerm& other) {
    Term::operator=(other);
    mSymbol = other.mSymbol;
    return *this;
}

/*! Compare two terms in a canonical order, defined as follows:
**      1. by term type: TupleVar, SymConst, UFCall, ConstVal
**      2. within each type, in alphabetical or numeric order.
*/
bool VarTerm::operator<(const Term& other) const {
    // Compare first by type -- note that the order of the
    // enumeration should match our desired sort order.
    int termComparison = compareTermTypes(other);
    if (-1 == termComparison) { return true; }
    if (1 == termComparison) { return false; }

    // Next, by symbol name.
//std::cout << "mSymbol = " << mSymbol << ", other.mSymbol = " << ((VarTerm&)other).mSymbol << std::endl;
    if (mSymbol < ((VarTerm&)other).mSymbol) { return true; }
    if (mSymbol > ((VarTerm&)other).mSymbol) { return false; }

    // If everything else matches, let the superclass compare by coefficient.
    return Term::operator<(other);
}

Term* VarTerm::clone() const {
    return new VarTerm(*this);
}

//! Creates a compact string to help with debugging.
std::string VarTerm::toString(bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << mSymbol;
    return ss.str();
}

//! Creates a compact string, pretty printed.
std::string 
VarTerm::prettyPrintString(const TupleDecl & aTupleDecl, bool absValue) const {
    std::stringstream ss;
    coeffToStream(ss, absValue);
    ss << mSymbol;
    return ss.str();
}

// Returns Term subclass type.
std::string VarTerm::type() const{
    return "VarTerm";
}

//--------------------- methods for the use in expression

//! Returns true if this term can be combined with the given term.
bool VarTerm::factorMatches(const Term& other) const {
    return Term::factorMatches(other) 
            and mSymbol == ((VarTerm&)other).mSymbol;
}

#pragma mark -
/****************************************************************************/
/****************************** TupleExpTerm ********************************/


TupleExpTerm::TupleExpTerm(int coeff, unsigned int size) 
: Term(coeff), mSize(size) {
    setTermType(TupleExp);
    // Initialize all ptrs in vector to NULL.
    for (unsigned int i=0; i<size; i++) {
        mExps.push_back(NULL);
    }
}


TupleExpTerm::TupleExpTerm(unsigned int size) : Term(1), mSize(size) {
    setTermType(TupleExp);
    // Initialize all ptrs in vector to NULL.
    for (unsigned int i=0; i<size; i++) {
        mExps.push_back(NULL);
    }
}

//! Copy constructor.
TupleExpTerm::TupleExpTerm( const TupleExpTerm& other ) : Term(other) {
    *this = other;
}

void TupleExpTerm::reset() {
    for (unsigned int i=0; i<mExps.size(); i++) {
        delete mExps[i];
    }
    mExps.clear();
}

//! Destructor.
TupleExpTerm::~TupleExpTerm() {
    reset();
}

//! Copy assignment operator.
TupleExpTerm& TupleExpTerm::operator=( const TupleExpTerm& other) {
    reset();
    for (std::vector<Exp*>::const_iterator i=other.mExps.begin(); 
                i != other.mExps.end(); ++i) {
        mExps.push_back((*i)->clone());
    }
    mSize = mExps.size();

    return *this;
}

/*! Less than operator.
**      Compare two TupleExpTerm in the following order:
**          1. by size: the number of elements
**          2. by expressions starting with index 0
** @param other, object to be compared
*/
bool TupleExpTerm::operator<( const Term& other) const {
    // Compare first by type -- note that the order of the
    // enumeration should match our desired sort order.
    int termComparison = compareTermTypes(other);
    if (-1 == termComparison) { return true; }
    if (1 == termComparison) { return false; }

    // first compare sizes
    if (size() < ((TupleExpTerm&)other).size()) { return true; }
    if (((TupleExpTerm&)other).size() < size()) { return false; }
    // then compare our elements
    std::vector<Exp*>::const_iterator iter=mExps.begin();
    std::vector<Exp*>::const_iterator 
        other_iter=((TupleExpTerm&)other).mExps.begin();
    for (; iter!=mExps.end() && other_iter!=((TupleExpTerm&)other).mExps.end(); 
           ++iter, ++other_iter) {
        if ( *(iter) < *(other_iter) ) { return true; }
        else if ( *(other_iter) < *(iter) ) { return false; }
    }

    //all our elements are equal, so we're not less than other
    return false;
}

/*! Combine another tuple expression term 
**  into this one, if possible, by
**  adding sub expressions pointwise.
**
** \param other -- term to attempt to combine with this one.(adopt)
** \return true if other was combined with this one; false otherwise
*/
bool TupleExpTerm::combine(Term* other) {
    if (not factorMatches(*other)) { delete other; return false; }
    
    // Must have another TupleExpTerm and it must be the same size
    TupleExpTerm* other_texp = dynamic_cast<TupleExpTerm*>(other);
    if (!other_texp || other_texp->size()!=size() ) {
        throw assert_exception("TupleExpTerm::combine: "
            "something wrong with other tuple expression term");
    }
    
    // Take our individual expressions and multiply them
    // by our coefficient and change our coefficient to 1.
    for (unsigned int i=0; i<mSize; i++) {
        if (mExps[i]==NULL || other_texp->mExps[i]==NULL) {
            throw assert_exception("TupleExpTerm::combine: "
                "NULL expression");
        }
        mExps[i]->multiplyBy(coefficient());
        // Also add in expressions from other tuple expression
        other_texp->mExps[i]->multiplyBy(other_texp->coefficient());
        // add the other expression into this one
        mExps[i]->addExp(new Exp(*(other_texp->mExps[i])));
    }
    delete other;
    return true;
}

//! Create a copy of this Term (and of the same subclass)
Term* TupleExpTerm::clone() const {
    return new TupleExpTerm(*this);
}



std::string TupleExpTerm::toString(bool absValue) const {
    std::stringstream ss;
    ss << "( ";
    for (unsigned int i=0; i<mSize; i++) {
        if (i > 0) {
            ss << ", ";
        }
        if (mExps[i] != NULL) { ss << mExps[i]->toString(); }
    }
    ss << " )";

    return ss.str();
}

//! In comma-separated list of expressions,
//! replaces any tuple var instances with given tuple var decl.
std::string TupleExpTerm::prettyPrintString(const TupleDecl & aTupleDecl, 
        bool absValue) const {
    std::stringstream ss;
    ss << "( ";
    for (unsigned int i=0; i<mSize; i++) {
        if (i > 0) {
            ss << ", ";
        }
        if (mExps[i]!=NULL) {
            ss << mExps[i]->prettyPrintString(aTupleDecl);
        }
    }
    ss << " )";

    return ss.str();
}


//! Creates a brief compact string to help with DOT output.
std::string TupleExpTerm::toDotString(bool absValue) const {
    return toString();
}

//! Returns string of subclass type.
std::string TupleExpTerm::type() const {
    return "TupleExpTerm";
}

void TupleExpTerm::setExpElem(unsigned int exp_index, Exp* exp) {
    if (exp_index>=mSize) {
        throw assert_exception("Attempt to set expression out of tuple bounds");
    } else if (mExps[exp_index]) { 
        throw assert_exception("UFCallTerm::setParamExp: param exp being set "
                                "twice");
    } else {
        mExps[exp_index] = exp;
    }
}

//! Returns pointer to individual expression.  Indexing starts at 0.
Exp* TupleExpTerm::getExpElem(unsigned int exp_index) {
    if (exp_index>=mSize) {
        throw assert_exception("Attempt to get expression out of tuple bounds");
    } else {
        return mExps[exp_index];
    }
}

Exp* TupleExpTerm::cloneExp(unsigned int exp_index) const {
    if (exp_index>=mSize) {
        throw assert_exception("Attempt to get expression out of tuple bounds");
    } else {
        if (mExps[exp_index]==NULL) {
            throw assert_exception("TupleExpTerm::cloneExp: NULL exp");
        }
        return mExps[exp_index]->clone();
    }
}


unsigned int TupleExpTerm::size() const {
    return mSize;
}

//! Returns true if this term can be combined with the given term.
bool TupleExpTerm::factorMatches(const Term& other) const {
    // bail out on the basic inherited check (comparing types)
    if (not Term::factorMatches(other)) { return false; }
    
    // check that number of expressions in the term matches
    const TupleExpTerm &tuple_exp = (const TupleExpTerm&)other;
    if (size() != tuple_exp.size()) { return false; }
    else { return true; }
}


#pragma mark -
/****************************************************************************/
/******************************** Exp ********************************/
//! Copy constructor
Exp::Exp(const Exp& other) {
    *this = other;
}

void Exp::reset() {
    for (std::list<Term*>::iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        delete (*i);
    }
    mTerms.clear();
}

//! Copy assignment
Exp& Exp::operator=(const Exp& other) {
    reset();
    for (std::list<Term*>::const_iterator i=other.mTerms.begin(); 
                i != other.mTerms.end(); ++i) {
        mTerms.push_back((*i)->clone());
    }
    mExpType = other.mExpType;

    return *this;
}

//! Destructor
Exp::~Exp() {
    reset();
}

//! Create a copy of this Exp (of the same subclass)
Exp* Exp::clone() const {
    return new Exp(*this);
}

//! Creates a compact string to help with debugging.
std::string Exp::toString() const {
    std::string result;
    bool firstTerm = true;
    // FIXME does the line just below go in other
    // toString() or prettyPrintString methods?
    if(mTerms.size() == 0) return "0";
    for (std::list<Term*>::const_iterator i=mTerms.begin(); 
            i != mTerms.end(); ++i) {
        bool absValue = false;
        if (not firstTerm) {
            if ((*i)->coefficient() < 0) {
                result += " - ";
            } else {
                result += " + ";
            }
            absValue = true;
        }
        result += (*i)->toString(absValue);
        firstTerm = false;
    }
    return result;
}

//! Creates a compact string, pretty printed.
std::string 
Exp::prettyPrintString(const TupleDecl & aTupleDecl) const 
{
    std::string result;
    bool firstTerm = true;
    for (std::list<Term*>::const_iterator i=mTerms.begin(); 
            i != mTerms.end(); ++i) {
        bool absValue = false;
        if (not firstTerm) {
            if ((*i)->coefficient() < 0) {
                result += " - ";
            } else {
                result += " + ";
            }
            absValue = true;
        }
        result += (*i)->prettyPrintString(aTupleDecl, absValue);
        firstTerm = false;
    }
    return result;
/*
    std::string result_left, result_right, result;
    result_left = result_right = std::string("0");
    bool firstTerm_left = true,firstTerm_right = true;
    for (std::list<Term*>::const_iterator i=mTerms.begin(); 
            i != mTerms.end(); ++i) {
        if ((*i)->coefficient() < 0) {
            if (firstTerm_left) result_left = (*i)->prettyPrintString(aTupleDecl, true);
            else result_left += " + " + (*i)->prettyPrintString(aTupleDecl, true);
            firstTerm_left = false;
        } else {
            if (firstTerm_right) result_right = (*i)->prettyPrintString(aTupleDecl, true);
            result_right += " + " + (*i)->prettyPrintString(aTupleDecl, true); 
            firstTerm_right = false;
        }
    }
    if( (getExpType() == Equality) )        result = result_left + std::string(" = ") + result_right;
    else if( (getExpType() == Inequality) ) result = result_left + std::string(" <= ") + result_right;
    else   throw assert_exception("Exp::prettyPrintString: Exp "
                               "type is not declared");

    return result;
*/
}

//! Add a term to this expression
void Exp::addTerm(Term *term) {

    // If the coefficient is zero, then the term will have no effect.
    if(term->coefficient() == 0){ delete term; return; }

    // Approach:
    // Iterate over our existing items (which are already sorted).
    // If we find one we can combine with the given term, combine them.
    // If we find one that comes after the given term, insert it.
    // Otherwise, append to the end.
    for (std::list<Term*>::iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        Term *t = *i;
        if (t->combine(term->clone())) {
            delete term;
            // We've successfully combined this term with an existing one,
            // but it's possible that the resulting term has a coefficient
            // of zero, in which case it should be removed.
            if (0 == t->coefficient()) {
                mTerms.erase(i);
                delete t;
            }
            return;
        }
        if (*term < *t) {
            // We've found a term that belongs after the new one,
            // so insert the new term here, and we're done.
            mTerms.insert(i, term);
            return;
        }
    }
    
    // We could neither combine this term with an existing one, nor
    // find a term that comes after it in the sort order.  So, just
    // append it to the end of the list.
    mTerms.push_back(term);
}

//! Add another expression to this one
void Exp::addExp(Exp *exp) {
    for (std::list<Term*>::const_iterator i=exp->mTerms.begin(); 
                i != exp->mTerms.end(); ++i) {
        addTerm((*i)->clone());
    }
    delete exp;
}

//! Multiply all terms in this expression by a constant
void Exp::multiplyBy(int constant) {
    for (std::list<Term*>::iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        (*i)->multiplyBy(constant);
    }   
}

//! Return whether all coefficients in this expression are
//! evenly divisible by the given integer.
bool Exp::isDivisible(int divisor) const {
    for (std::list<Term*>::const_iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        Term* t = *i;
        if (t->coefficient() % divisor != 0) { return false; }
    }
    return true;
}

void Exp::divideBy(int divisor) {
    for (std::list<Term*>::iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        Term* t = *i;
        t->divideBy(divisor);
    }
}

/* Search this Exp for the given factor.  The cloned Term that is returned
   can have a coefficient other than 1.  The factor param should have 
   a coefficient of 1.  Returns NULL if a matching Term is not found.
*/
Term* Exp::findMatchingFactor(const Term & factor) const {
                           
    if(factor.coefficient()!=1){
        std::stringstream ss;
        ss << "coefficient to factor is " << factor.coefficient() 
           << " it is required to be 1";
        throw iegenlib::assert_exception(ss.str());
    }
    
    Term* matchingFactor = NULL;
    for (std::list<Term*>::const_iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        const Term* t = *i;
        if (t->factorMatches(factor) and isDivisible(t->coefficient()) ) {
            // found a simple match for this factor
            matchingFactor = t->clone();
            break;
        }
    }
    return matchingFactor;
}

/*  solveForFactor: Treat this expression as equal to zero, and
**  solve for the given factor (which must have a coefficient of
**  1, but in expression can have a different coefficient as long
**  as it evenly divides the other terms), returning a new expression which
**  is equal to the given factor.  (The caller is responsible for
**  deallocating the result; we adopt the given factor.)
**  Calls the iegenlib::queryInverseCurrEnv() function.
*/
Exp* Exp::solveForFactor(Term* factor_ptr) const {

    if(factor_ptr->coefficient()!=1){
        std::stringstream ss;
        ss << "coefficient to factor is " << factor_ptr->coefficient() 
           << " it is required to be 1";
        throw iegenlib::assert_exception(ss.str());
    }

    // Declare pointer for returned expression.
    Exp * factorExposedExp = NULL;

    // First find the matching term, so we can get its coefficient.
    Term* matchingFactor = findMatchingFactor(*factor_ptr);

    // If at first we don't succeed, attempt to invert a function.
    if (matchingFactor==NULL) {
        factorExposedExp = invertFuncToExposeFactor(factor_ptr->clone());
        if (factorExposedExp) {
            matchingFactor = factorExposedExp->findMatchingFactor(*factor_ptr);
        }
    } 
    // If we succeeded, be happy and move on.
    else {
        factorExposedExp = clone(); // clone "this" expression
    }
    
    // If we found a matching factor.
    // Check that the rest of the expression divisible by the
    // coefficient for the matchingFactor.
    //  eg. r - 3*g( s ), if g is invertible, g_inv(r) = 3*s, 
    //      can't solve for x since rest of expression not divisible by 3
    if (matchingFactor!=NULL && isDivisible(matchingFactor->coefficient()) ) {

        // Now, we can solve by removing the matching term,
        // and dividing by its coefficient.
        // running example: will return f_inv(x)
        factor_ptr->multiplyBy(-matchingFactor->coefficient());
        factorExposedExp->addTerm(factor_ptr);    // note: this deletes factor
        factorExposedExp->divideBy(-matchingFactor->coefficient());
        delete matchingFactor;
    }
    // if factor_ptr has not been adopted then delete it
    else {
        delete factor_ptr;
    }
    
    return factorExposedExp;
}


//! Creates an expression by inverting a function and exposing the given
//! factor if the given factor is in an argument expression to a
//! bijective uninterpreted function.  For example,
//!     this Exp: __tv0 + x + f( y )
//!     factor: y
//!     returns: y - f_inv(-_tv0 - x)
//!     
//! If can't expose the given factor, then returns NULL.
//! Owns given factor but caller is responsible for deallocating
//! space for returned expression.
Exp* Exp::invertFuncToExposeFactor(Term * factor_ptr) const {
    Exp* retval = NULL;

    UFCallTerm* foundUFC = NULL;
    
    // Search through terms in this expression.
    std::string inverseFuncName = "";
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); ++i) {
        Term* t = (*i);
    
        // Is the term a UFCallTerm?  If not keep looking.
        if (not t->isUFCall()) {continue;}
        
        // Does the UFCallTerm depend on our factor?
        // IOW is the factor part of one of its parameter expressions?
        // If not keep looking.
        Exp* texp = new Exp();
        texp->addTerm(t->clone());
        bool dependsOnFactor = texp->dependsOn(*factor_ptr);
        delete texp;
        if (not dependsOnFactor) {continue;}
        
        // Is the function invertible?  If not keep looking.
        UFCallTerm* callptr = dynamic_cast<UFCallTerm*>(t);
        inverseFuncName = iegenlib::queryInverseCurrEnv(callptr->name());
        if (inverseFuncName.empty()) { continue; }
        
        // If all of the above questions are answered yes, then
        // we will be able to expose the factor in question.
        // FIXME: what if the factor shows up in another function argument?
        //        that case should probably be handled in the solveForFactor.
        if (foundUFC) {
            throw assert_exception("Exp::invertFuncToExposeFactor: Already "
                    "found a UF call.");
        }
        // FIXME: what if the factor shows up nested in another function call
        //        whose result is being passed to this function?
        foundUFC = (UFCallTerm*)(callptr->clone());
    }
    
    // If found a UFCallTerm that depends on our factor and that function
    // is invertible, then invert the function and create an expresion
    // where the factor is exposed.
    if (foundUFC) {

        // remove the Term with the factor from the clone of our
        // original Exp
        Exp* expClone = this->clone();
        // running example: negFoundUFC = -f(y), retval = x
        Exp* negFoundUFC = new Exp();
        negFoundUFC->addTerm(foundUFC->clone());
        negFoundUFC->multiplyBy(-1);
        expClone->addExp(negFoundUFC);

        // Need to divide the expClone by the negation of the
        // original UFCallTerm coefficient
        // to know what expression equal to UFCallTerm with coefficient
        // of one would be.
        expClone->divideBy( - foundUFC->coefficient() );

        // figure out which argument to the UFCall contains
        // the factor we are looking for
        unsigned int arg_count = 0;
        int arg_index = 0;
        Exp* relevant_arg;
        for (; arg_count<foundUFC->numArgs(); arg_count++) {
            Exp* arg = foundUFC->getParamExp( arg_count );
            if (arg->dependsOn(*factor_ptr)) {
                arg_index = arg_count;
                relevant_arg = arg;
            }
        }
        // create a new UFCall term for the inverse function
        // with the current clone Exp as the only argument
        // running example: invCall = -f_inv(x)
        // If original function only had one argument do not have
        // to index into result of inverse function call.
        UFCallTerm *invCall;
        if (arg_count==1) {
            invCall = new UFCallTerm(-1, inverseFuncName, 1);
            invCall->setParamExp(0, expClone);
        } else {
            // otherwise do need to know what arg index
            invCall = new UFCallTerm(-1, inverseFuncName, 1, arg_index);
            invCall->setParamExp(0, expClone);
        }

        // create an expression that contains the inverted function
        // call and add in the expression argument to the original
        // UFCall to that expression
        // running example: retval = y - f_inv(x)
        retval = new Exp();
        retval->addTerm(invCall);
        retval->addExp(relevant_arg->clone());
        delete foundUFC;
    }

    // No longer need the factor_ptr and what it points at.
    delete factor_ptr;

    return retval;
}

//! Substitute each expression for the factor (i.e. the non-coefficient
//! part of a term), which is its key.
void Exp::substitute(SubMap& searchTermToSubExp) {
    Exp *addedTerms = new Exp();
    std::list<Term*>::iterator i=mTerms.begin();
    while (i != mTerms.end()) {
        Term *t = *i;
        
        // Lookup term in the substitution map
        Exp* sub = searchTermToSubExp.subExp(t);
        // Found a matching term.
        if (sub!=NULL) {
            // We've found a matching term in substition map.  
            // Note the coefficient for this term and then remove 
            // it from the list for the expression.
            int foundCoefficient = t->coefficient();
            // Note that we don't increment i here; it already points
            // to the item after the one just erased.
            i = mTerms.erase(i);
            delete t;
            // Put substituted expression in and multiple by coeff of term.
            Exp *product = sub->clone();
            product->multiplyBy(foundCoefficient);
            addedTerms->addExp(product);
            
        } else { 
            // clone term and remove from original expression
            Term* term_clone = t->clone();
            i = mTerms.erase(i);  // removing term from current exp
            delete t;

            // If we have a UFCallTerm
            // below here change term_clone to callTerm
            if (term_clone->isUFCall()) {
                UFCallTerm *callTerm = dynamic_cast<UFCallTerm*>(term_clone);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<callTerm->numArgs(); count++) {
                    Exp* arg = callTerm->getParamExp(count);
                    arg->substitute(searchTermToSubExp);
                }
            }
            if (term_clone->isTupleExp()) {
                TupleExpTerm *tupTerm = dynamic_cast<TupleExpTerm*>(term_clone);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<tupTerm->size(); count++) {
                    Exp* arg = tupTerm->getExpElem(count);
                    arg->substitute(searchTermToSubExp);
                }
            }
            // We need to add the substituted factor (function call with
            // params substituted or term that doesn't match factor) back in
            addedTerms->addTerm(term_clone);
        }
    }

    // put all the modifications into this Exp
    addExp(addedTerms);
}

// Want first coefficient to always be a positive integer.
void Exp::normalizeForEquality() {
    if (mTerms.empty()) { return; }
    Term *firstTerm = mTerms.front();
    if (firstTerm->coefficient() < 0) {
        multiplyBy(-1);
    }

    // Remove zero term at end
    Term *lastTerm = mTerms.back();
    if(lastTerm->coefficient() == 0){
        delete lastTerm;
        mTerms.remove(lastTerm);
    }
}

//! Return a new Exp with all nested functions such as
//! f ( f_inv ( i ) ) changed to i.
//! g(g_inv(x)[0], g_inv(x)[1]) changed to x
Exp* Exp::collapseNestedInvertibleFunctions() const {
    Exp* retval = new Exp();
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        retval->addExp( (*i)->collapseNestedInvertibleFunctions() );
    }   
    return retval;
}

/*! Search for the given factor anywhere in this expression
**  (including within UFCallTerm arguments, recursively).
*/
bool Exp::dependsOn(const Term& factor) const {
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        if ((*i)->factorMatches(factor)) {
            // We found the matching term.
            return true;            
        } else {
            // This term doesn't match, but maybe it contains other
            // expressions that we need to search recursively.
            if ((*i)->isUFCall()) {
                UFCallTerm *callTerm = dynamic_cast<UFCallTerm*>((*i));
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<callTerm->numArgs(); count++) {
                    Exp* arg = callTerm->getParamExp(count);
                    if (arg->dependsOn(factor)) { return true; }
                }
            }
            if ((*i)->isTupleExp()) {
                TupleExpTerm *tupTerm = dynamic_cast<TupleExpTerm*>(*i);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<tupTerm->size(); count++) {
                    Exp* arg = tupTerm->getExpElem(count);
                    if (arg->dependsOn(factor)) { return true; }
                }
            }

        }
    }
    return false;
}

/*! Returns true if this expression contains a UFCallTerm
**  that is being indexed.
*/
bool Exp::hasIndexedUFCall() const {
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        if ( (*i)->isUFCall() ) {
            UFCallTerm* ufcallptr = dynamic_cast<UFCallTerm*>(*i);
            return ufcallptr->isIndexed();
        }
    }
    return false;
}

/*! Returns a clone of the single indexed UFCall in expression.
**  If there are none or more than one then an exception is thrown.
*/
UFCallTerm* Exp::cloneIndexedUFCallTerm() const {
    if (!hasIndexedUFCall()) {
        throw assert_exception("Exp::getIndexedUFCallTerm: "
            "no indexed UFCallTerm");
    }
    UFCallTerm* retval = NULL;
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        if ( (*i)->isUFCall() ) {
            UFCallTerm* ufcallptr = dynamic_cast<UFCallTerm*>(*i);
            if (ufcallptr->isIndexed()) {
                if (retval!=NULL) {
                    throw assert_exception("Exp::getIndexedUFCallTerm: "
                        "more than one indexed UFCallTerm in expression");
                } else {
                    retval = new UFCallTerm(*ufcallptr);
                }
            }
        }
    }
    return retval;
}



/*! Return true iff this expression has no terms, or has
**  only a constant term equal to 0.
*/
bool Exp::equalsZero() const {
    if (mTerms.size() == 0) return true;
    if (mTerms.size() > 1) return false;
    return mTerms.front()->coefficient() == 0; // FIXME: should never happen?
}

//! Returns true if this expression equals the given term.
bool Exp::operator==(const Term& other) const {
    if (mTerms.size() == 1) {
        return *(mTerms.front()) == other;
    } else if (this->equalsZero()) {
        Term zero(0);
        return zero==other;
    } else {
        return false;
    }
}

/*! Less than operator.
**      Compare two Exp in the following order:
**          1. by size: number of terms in mTerms
**          2. by sorted term values: using Term::operator<(Term)
**      This operator assumes that the list of terms (mTerms) is
**          sorted for both Exp being compared.
** @param other, object to be compared
*/
bool Exp::operator<( const Exp& other) const {
    // first compare mTerms sizes
    if (mTerms.size() < other.mTerms.size()) { return true; }
    if (other.mTerms.size() < mTerms.size()) { return false; }
    // then compare our elements
    std::list<Term*>::const_iterator otherIter = other.mTerms.begin();
    std::list<Term*>::const_iterator myIter = mTerms.begin(); 
    while ( myIter != mTerms.end() ) {
        //compare values
        if (**myIter < **otherIter) { return true; }
        if (**otherIter < **myIter) { return false; }
        //our values are equal, continue
        ++myIter;
        ++otherIter; 
    }
    //all our elements are equal, so we're not less than other
    return false;
}

/*! Equality operator.
**      Compare two Exp for equality.
**      This operator assumes operator<(Exp) is well defined.
** @param other, object to be compared
*/
bool Exp::operator==( const Exp& other) const {
    return !((*this < other) || (other < *this));
}


/*! Find any TupleVarTerms in this expression (and subexpressions)
**  and remap the locations according to the oldToNewLocs vector,
**  where oldToNewLocs[i] = j means that old location i becomes new
**  location j (i.e. __tvi -> __tvj).  Throws an exception if an
**  old location is out of range for the given oldToNewLocs.
**  The new location will be -1 for old locations that are not being
**  remapped.
*/
void Exp::remapTupleVars(const std::vector<int>& oldToNewLocs) {
    // Verifying that oldToNewLocs doesn't map
    // two old locations to the same new location.
    // First see what the range of locations is for new locs.
    int max = 0;
    for (unsigned int i=0; i<oldToNewLocs.size(); i++) {
        if (oldToNewLocs[i]>max) {
            max = oldToNewLocs[i];
        }
    }
    // Create a vector to keep track of whether a location as been seen.
    std::vector<bool> newLocSeen(max+1, false);
    for (unsigned int i=0; i<oldToNewLocs.size(); i++) {
        if ( oldToNewLocs[i]>=0 ) { // ignoring those negative ones
            if ( newLocSeen.at(oldToNewLocs[i]) ) {
                throw assert_exception("Exp::remapTupleVars: two old locations"
                    " are being mapped to the same new location");
            } else {
                newLocSeen[oldToNewLocs[i]] = true;
            }
        }
    }
    
    // We do one pass through all the terms and all TupleVarTerms map
    // themselves to a new location as specified.
    // Since we only do one pass we don't have __tv0 mapping to __tv3 and
    // then __tv3 mapping to something else later.
    for (std::list<Term*>::iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        TupleVarTerm *tupleVarTerm = dynamic_cast<TupleVarTerm*>(*i);
        if (tupleVarTerm) {
            tupleVarTerm->remapLocation(oldToNewLocs);
        } else {
            // This is not a tuple var term, but maybe it contains other
            // expressions that we need to search recursively.
            if ((*i)->isUFCall()) {
                UFCallTerm *callTerm = dynamic_cast<UFCallTerm*>((*i));
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<callTerm->numArgs(); count++) {
                    Exp* arg = callTerm->getParamExp(count);
                    arg->remapTupleVars(oldToNewLocs);
                }
            }
            if ((*i)->isTupleExp()) {
                TupleExpTerm *tupTerm = dynamic_cast<TupleExpTerm*>(*i);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<tupTerm->size(); count++) {
                    Exp* arg = tupTerm->getExpElem(count);
                    arg->remapTupleVars(oldToNewLocs);
                }
            }

        }
    }
}

//! Returns true if only have a constant term
bool Exp::isConst() const {
    if (this->equalsZero() ||
        (this->mTerms.size()==1 && this->mTerms.front()->isConst()) ) {
        return true;
    } else {
        return false;
    }
}

//! Returns true if we have something like: 2 = 0
bool Exp::isContradiction() const {
    if ( this->mTerms.size()==1 && 
         (this->mTerms.front()->isConst() && this->mTerms.front()->coefficient() != 0) && (getExpType() == Equality) ) {
        return true;
    } else {
        return false;
    }
}


//! Return Term* if the expression has only one Term.
//! Otherwise returns NULL.
//! Still owns Term.
Term* Exp::getTerm() const {
    if (mTerms.size() == 1) {
        return mTerms.front();
    } else {
        return NULL;
    }
}

//! Return Term* for constant term if there is one.
//! Otherwise return NULL.
//! This expression still owns the Term.
Term* Exp::getConstTerm() const {
    for (std::list<Term*>::const_iterator i=mTerms.begin(); 
            i != mTerms.end(); i++) {
        Term* t = (*i);
        if (t->isConst()) {
            return t;
        }
    }
    return NULL;
}

//! Output the Exp in dot format.
//! Pass in the next node id.
//! The next node id will be set to next id upon exit from this routine.
std::string Exp::toDotString(int & next_id) const {
    std::stringstream result;
    int self_id = next_id++;

    result << self_id << " [label = \"Exp\\n";
    if(this->mTerms.size() > 1) {
        result << "... + ..."; // Denote multiple addends
    }
    result << "\"];\n";

    // Printing out children here and calling recursively
    // on grandchildren due to a function call.
    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {
        int term_id = next_id++;
        // Connect self to term child
        result << self_id << " -> " << term_id << ";\n";
        // Term node
        result << term_id << " [label = \"" << (*i)->type() << "\\n ";
        result << (*i)->toDotString() << "\"];\n";

        // Create grandchildren if needed and recurse
        if ((*i)->isUFCall()) {
            UFCallTerm *callTerm = dynamic_cast<UFCallTerm*>((*i));
            // This term doesn't match, but maybe it contains other
            // expressions that we need to search recursively.
            for (unsigned int count=0; count<callTerm->numArgs(); count++) {
                Exp* arg = callTerm->getParamExp(count);
                result << arg->toDotString(term_id,next_id);
            }
        }
        if ((*i)->isTupleExp()) {
            TupleExpTerm *tupTerm = dynamic_cast<TupleExpTerm*>(*i);
            // This term doesn't match, but maybe it contains other
            // expressions that we need to search recursively.
            for (unsigned int count=0; count<tupTerm->size(); count++) {
                Exp* arg = tupTerm->getExpElem(count);
                result << arg->toDotString(term_id,next_id);
            }
        }

    }

    return result.str();
}


//! Output the Exp in dot format.
//! Pass in the parent node id and the next node id.
//! The next node id will be set upon exit from this routine.
std::string Exp::toDotString(int parent_id, int & next_id) const {
    std::stringstream result;
    int self_id = next_id;

    // Connect parent to self
    result << parent_id << " -> " << self_id << ";\n";

    // Self
    result << this->toDotString(next_id);
    
    return result.str();
}

//! Creates iterator over all symbolic constants in the expression.
//! Caller is responsible for deleting the StringIterator.
StringIterator* Exp::getSymbolIterator() const {

    std::set<std::string> symbolSet;

    for (std::list<Term*>::const_iterator i=mTerms.begin();
                i != mTerms.end(); i++) {

        VarTerm *varTerm = dynamic_cast<VarTerm*>(*i);
        
        // If have a var term then put in the set of symbols
        if (varTerm) {
            symbolSet.insert( varTerm->symbol() );
            
        } else {
        
            // Recurse if needed.
            if ((*i)->isUFCall()) {
                UFCallTerm *callTerm = dynamic_cast<UFCallTerm*>(*i);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<callTerm->numArgs(); count++) {
                    Exp* arg = callTerm->getParamExp(count);
                    StringIterator* subSymIter = arg->getSymbolIterator();
                    while (subSymIter->hasNext()) {
                        symbolSet.insert( subSymIter->next() );
                    }
                    delete subSymIter;
                }
            }
            if ((*i)->isTupleExp()) {
                TupleExpTerm *tupTerm = dynamic_cast<TupleExpTerm*>(*i);
                // This term doesn't match, but maybe it contains other
                // expressions that we need to search recursively.
                for (unsigned int count=0; count<tupTerm->size(); count++) {
                    Exp* arg = tupTerm->getExpElem(count);
                    StringIterator* subSymIter = arg->getSymbolIterator();
                    while (subSymIter->hasNext()) {
                        symbolSet.insert( subSymIter->next() );
                    }
                    delete subSymIter;
                }
            }
            
        }
    }

    return new StringIterator( symbolSet ); 
}

#pragma mark -
/****************************************************************************/
/*************** Visitor Design Pattern Code ********************************/

/*!
**  The acceptVisitor code does the traversal of the data structure and
**  calls a particular Visitor subclass visit methods for a node after
**  children nodes in the data structure have been visited.
*/

void Term::acceptVisitor(Visitor *v) {
    v->preVisitTerm(this);
    v->postVisitTerm(this);
}

void UFCallTerm::acceptVisitor(Visitor *v) {
    v->preVisitUFCallTerm(this);

    // Iterate over parameters passed to UF.
    for (std::vector<Exp*>::iterator i=mArgs.begin(); i != mArgs.end(); ++i) {
        (*i)->acceptVisitor(v);
    }

    v->postVisitUFCallTerm(this);
}

void TupleVarTerm::acceptVisitor(Visitor *v) {
    v->preVisitTupleVarTerm(this);
    v->postVisitTupleVarTerm(this);
}

void VarTerm::acceptVisitor(Visitor *v) {
    v->preVisitVarTerm(this);
    v->postVisitVarTerm(this);
}

void TupleExpTerm::acceptVisitor(Visitor *v) {
    v->preVisitTupleExpTerm(this);
    v->postVisitTupleExpTerm(this);
}

void Exp::acceptVisitor(Visitor *v) {
    v->preVisitExp(this);
    for (std::list<Term*>::iterator i=mTerms.begin(); i != mTerms.end(); ++i) {
        (*i)->acceptVisitor(v);
    }
    v->postVisitExp(this);
}

#pragma mark -
/****************************************************************************/

//! Get a list of pointers to the terms in this expression.
//! All pointers in this list will be owned by caller.
std::list<Term*> Exp::getTermList() const {
    return mTerms;
}


}//end namespace iegenlib
