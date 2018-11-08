/*!
 * \file set_relation.cc
 *
 * \brief Implementations of the Set and Relation classes.
 *
 * \date Started: 3/28/12
 *
 * \authors Michelle Strout, Joe Strout, and Mahdi Soltan Mohammadi 
 *
 * Copyright (c) 2012, Colorado State University <br>
 * Copyright (c) 2015-2016, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "set_relation.h"
#include "UFCallMap.h"
#include "Visitor.h"
#include <stack>
#include <map>
#include <assert.h>

namespace iegenlib{

/************************ ISL helper routines ****************************/

//! Runs an Affine Set (string) through ISL and returns the resulting set
//  as a string
string passSetStrThruISL(string sstr){

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islSetToString ( islStringToSet(sstr,ctx), ctx );
  isl_ctx_free(ctx);

  return islStr;
}

//! Runs an Affine Union Set* (string) through ISL 
//  and returns the resulting set as a string
//  * Union Set is a set of polyhedrals that may live in different space 
//    (may have different tuple declaration)
string passUnionSetStrThruISL(string sstr){

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islUnionSetToString ( islStringToUnionSet(sstr,ctx), ctx );
  isl_ctx_free(ctx);

  return islStr;
}

//! Runs an Affine Relation through ISL and returns the normalized result
string passRelationStrThruISL(string rstr){

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islMapToString ( islStringToMap(rstr,ctx), ctx );
  isl_ctx_free(ctx);

  return islStr;
}

//! Runs an Affine Union Relation through ISL and returns the normalized result
string passUnionRelationStrThruISL(string rstr){

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islUnionMapToString ( islStringToUnionMap(rstr,ctx), ctx );
  isl_ctx_free(ctx);

  return islStr;
}

//! Runs an Affine Set through ISL and returns the resulting normalized set
Set* passSetThruISL(Set* s){

  string sstr = s->toISLString();

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islSetToString ( islStringToSet(sstr,ctx), ctx );
  isl_ctx_free(ctx);

  // We need to revert changes that isl applies to Tuple Declaration because of 
  // equality constrains. We do this purely using string manipulation.
  // Ex:
  //     sstr      = { [i1, i2, i3] : i1 = col_i_ and ...}  ** dots (...) can
  //     islStr    = { [col_i_, i2, i3] : ...}              ** be different 
  //     corrected = { [i1, i2, i3] : i1 = col_i_ and ...}  ** constraints
  // For more detail refer to revertISLTupDeclToOrig function's comments. 
  int inArity = s->arity(), outArity = 0;
  string corrected = revertISLTupDeclToOrig( sstr, islStr, inArity, outArity);
  Set* result = new Set(corrected);

  return result;
}

//! Runs an Affine Relation through ISL and returns the normalized result
Relation* passRelationThruISL(Relation* r){

  string rstr = r->toISLString();

  isl_ctx *ctx = isl_ctx_alloc();
  string islStr =  islMapToString ( islStringToMap(rstr,ctx), ctx );
  isl_ctx_free(ctx);

  // Same as passSetThruISL
  int inArity = r->inArity(), outArity = r->outArity();
  string corrected = revertISLTupDeclToOrig( rstr, islStr, inArity, outArity);
  Relation* result = new Relation( corrected);

  return result;
}

// This function can be used for Projecting out a tuple variable
// from an affine set string using isl library
Set* islSetProjectOut(Set* s, unsigned pos) {

    string sstr = s->toISLString();

    // Using isl to project out tuple variable #pos
    isl_ctx *ctx = isl_ctx_alloc();
    string islStr = islSetToString ( 
                 isl_set_project_out(islStringToSet(sstr,ctx), 
                                     isl_dim_out, pos, 1), ctx 
                 );
    isl_ctx_free(ctx);

    // We need to revert changes that isl applies to Tuple Declaration similar
    // to passSetThruISL. However, this is different from passSetThruISL.
    // Before using revertISLTupDeclToOrig, we need to change the tuple
    // declaration in the original Set's string.
    // So, it would match projected out declaration.
    // Ex:
    // sstr      = { [i1,i2,i3] : ...}  ,  pos = 1 (we project out i2)
    // projected = { [i1,i3] : ...}
    // For more detail refer to projectOutStrCorrection function's comments.
    // After getting projected string the process becomes like passSetThruISL.
    string projected = projectOutStrCorrection(sstr, pos, s->arity(), 0);
    string corrected = revertISLTupDeclToOrig( projected, islStr, s->arity(), 0);

    Set* result = new Set( corrected);

    return result;
}

#pragma mark -
/****************************** Conjunction *********************************/

Conjunction::Conjunction(int arity) : mTupleDecl(arity), mInArity(0),
                                      unsat(false){
}

Conjunction::Conjunction(TupleDecl tdecl) : mTupleDecl(tdecl), mInArity(0),
                                      unsat(false){
}


Conjunction::Conjunction(int arity, int inarity)
    : mTupleDecl(arity), mInArity(inarity), unsat(false){
}

Conjunction::Conjunction(const Conjunction& other) {
    *this = other;
}

Conjunction& Conjunction::operator=(const Conjunction& other) {
    reset();

    for (std::list<Exp*>::const_iterator i=other.mEqualities.begin();
                i != other.mEqualities.end(); i++) {
        mEqualities.push_back((*i)->clone());
    }

    for (std::list<Exp*>::const_iterator i=other.mInequalities.begin();
                i != other.mInequalities.end(); i++) {
        mInequalities.push_back((*i)->clone());
    }

    mTupleDecl = other.mTupleDecl;
    mInArity = other.mInArity;
    unsat = other.unsat;
    return *this;
}

void Conjunction::reset() {
    for (std::list<Exp*>::iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        delete (*i);
    }
    mEqualities.clear();

    for (std::list<Exp*>::iterator i=mInequalities.begin();
                i != mInequalities.end(); i++) {
        delete (*i);
    }
    mInequalities.clear();
}

Conjunction::~Conjunction() {
    reset();
}

//! Create a copy of this Conjuntion (of the same subclass)
Conjunction* Conjunction::clone() const {
    return new Conjunction(*this);
}

/*! Less than operator.
**      Compare two Conjunction in the following order:
**          1. by tuple declaration: using TupleDecl::operator<(TupleDecl)
**          2. by number of equalities: number of terms in mEqualities
**          3. by number of inequalities: number of terms in mInequalities
**          4. by sorted equalities list values: using Exp::operator<(Exp)
**          5. by sorted inequalities list values: using Exp::operator<(Exp)
**      This operator assumes that the list of equalities (mEqualities) and
**          inequalities (mInequalities) is sorted for both Conjunction being
**          compared.
** @param other, object to be compared
*/
bool Conjunction::operator<( const Conjunction& other) const {
    // 1. compare tuple declarations
    if (mTupleDecl < other.mTupleDecl) { return true; }
    if (other.mTupleDecl < mTupleDecl) { return false; }

    // 2. compare equality sizes
    if (mEqualities.size() < other.mEqualities.size()) { return true; }
    if (other.mEqualities.size() < mEqualities.size()) { return false; }

    // 3. compare inequality sizes
    if (mInequalities.size() < other.mInequalities.size()) { return true; }
    if (other.mInequalities.size() < mInequalities.size()) { return false; }

    // 4. compare sorted equalities lists
    std::list<Exp*>::const_iterator thisIter;
    std::list<Exp*>::const_iterator otherIter;
    otherIter = other.mEqualities.begin();
    thisIter = mEqualities.begin();
    while (thisIter != mEqualities.end()) {
        //compare values
        if (**thisIter < **otherIter) { return true; }
        if (**otherIter < **thisIter) { return false; }
        //our mEqualities[i]'s are equal, continue
        ++thisIter;
        ++otherIter;
    }

    // 5. compare sorted inequalities lists
    otherIter = other.mInequalities.begin();
    for (thisIter = mInequalities.begin();
            thisIter != mInequalities.end(); ++thisIter) {
        //compare values
        if (**thisIter < **otherIter) { return true; }
        if (**otherIter < **thisIter) { return false; }
        //our mInequalities[i]'s are equal, continue
        ++otherIter;
    }

    // All our elements are equal, so we are not less than other
    return false;

}

//! Given inarity parameter is adopted.
//! If inarity parameter is outside of feasible range for the existing
//! existing TupleDecl then throws exception.
void Conjunction::setInArity(int inarity) {
 	if (inarity < 0 || inarity > int(mTupleDecl.size()-1) ) {
        throw assert_exception("Conjunction::setInArity: impossible arity match");
	}
	mInArity = inarity;
}

//! If there are some constants that don't agree then throws exception.
//! If replacing a constant with a variable ignores the substitution
//! for that tuple variable in that conjunction to keep constant.
void Conjunction::setTupleDecl( TupleDecl tuple_decl ) {
    // Check that the given tuple_decl is at least as large as the
    // current mTupleDecl.
    /* FIXME: apparently do use this somewhere.
    if (tuple_decl->size() < mTupleDecl.size()) {
        throw assert_exception("Conjunction::setTupleDecl: given tuple_decl "
            "has fewer entries than current tuple declaration");
    }
    */

    // Iterate through tuple declaration we already have to
    // see if the changes are compatible.  Change entries in given
    // tuple_decl if we already have a constant.
    for (unsigned int i=0; (i<mTupleDecl.size()) && (i<tuple_decl.size()); i++) 
    {
        if (mTupleDecl.elemIsConst(i) && tuple_decl.elemIsConst(i)
                && mTupleDecl.elemToString(i)!=tuple_decl.elemToString(i)) {
            throw assert_exception("Conjunction::setTupleDecl: constants "
                "don't match.");
        }
        if (mTupleDecl.elemIsConst(i) && !tuple_decl.elemIsConst(i)) {
            tuple_decl.copyTupleElem(mTupleDecl, i, i);
        }
    }
    
    // copy possibly modified tuple declaration over our current tuple decl
    mTupleDecl = tuple_decl;
}

//! Creates a copy of its tuple declaration and returns it.
TupleDecl Conjunction::getTupleDecl(  ) const {
    return mTupleDecl;
}

//! Concatenates the two tuple declarations into one.
//! Consumes the passed in tuple_decl.
void Conjunction::setTupleDecl( TupleDecl tuple_decl_in, 
        TupleDecl tuple_decl_out ) {

    TupleDecl concatTuple = tuple_decl_in.concat(tuple_decl_out );
    setTupleDecl(concatTuple);
}


/*! addEquality -- add the given expression, interpreted as an
** equality (Exp = 0), to our list of equalities.
** Maintains a sorted order on the constraints.
**
** FIXME: the check for duplicate equalities is done in O(n) time here,
** but with a different strategy could be reduced to O(log n) time.
**
*/
void Conjunction::addEquality(Exp* equality) {
    equality->normalizeForEquality();
    if(equality->equalsZero()){
        delete equality;
        return;
    }

    // Setting the type of expression
    equality->setEquality();

    for (std::list<Exp*>::iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        Exp* e = *i;

        if(*equality == *e){
            delete equality;
            return;
        }
        if (*equality < *e) {
            // We've found a term that belongs after the new one,
            // so insert the new term here, and we're done.
            mEqualities.insert(i, equality);
            return;
        }
    }

    mEqualities.push_back(equality);
}

/*! addInequality -- add the given expression, interpreted as an
** inequality (Exp >= 0), to our list of inequalities.
**
** FIXME: the check for duplicate inequalities is done in O(n) time here,
** but with a different strategy could be reduced to O(log n) time.
*/
void Conjunction::addInequality(Exp* inequality) {
    if(inequality->equalsZero()){
        delete inequality;
        return;
    }

    // Setting the type of expression
    inequality->setInequality();

    for (std::list<Exp*>::iterator i=mInequalities.begin();
                i != mInequalities.end(); i++) {
        Exp* e = *i;

        if(*inequality == *e){
            delete inequality;
            return;
        }
        if (*inequality < *e) {
            // We've found a term that belongs after the new one,
            // so insert the new exp here, and we're done.
            mInequalities.insert(i, inequality);
            return;
        }
    }
    mInequalities.push_back(inequality);
}

void Conjunction::substituteTupleDecl() {
    std::map<std::string, int> nameToLocationMap;
    SubMap var2TupleVar;

    for (unsigned int i=0; i<mTupleDecl.size(); i++) {
        // if it's a constant, no substitution is needed
        if (mTupleDecl.elemIsConst(i)) continue;

        // prepare the variable to search for
        std::string var = mTupleDecl.elemVarString(i);
        std::map<std::string, int>::iterator it = nameToLocationMap.find(var);
        if (it != nameToLocationMap.end()) {
            // This variable is one we've seen before; we don't
            // need to substitute, but we do need to add a new
            // equality constraint.
            Exp *equality = new Exp;
            equality->addTerm(new TupleVarTerm(it->second));
            equality->addTerm(new TupleVarTerm(-1, i));
            addEquality(equality);
        } else {
            // This is not a variable name we've seen before;
            // store it in our map for future reference, and
            // substitute into our constraints.
            nameToLocationMap[var] = i;
            Exp* tupleVarExp = new Exp();
            tupleVarExp->addTerm(new TupleVarTerm(i));
            var2TupleVar.insertPair(new VarTerm(var), tupleVarExp);
        }
    }

    // perform the substitution on all our constraints
    for (std::list<Exp*>::iterator expIter=mEqualities.begin();
                expIter != mEqualities.end(); expIter++) {
        (*expIter)->substitute(var2TupleVar);
    }
    for (std::list<Exp*>::iterator expIter=mInequalities.begin();
                expIter != mInequalities.end(); expIter++) {
        (*expIter)->substitute(var2TupleVar);
    }

    // The order of tuple locations may not match the previous alphabetical
    // order of the symbols we've replaced.  So, we need to renormalize now.
    this->cleanUp();

}

/*! copyConstraintsFrom -- copy all the equalities and inequalities
**  from source, and add them to our own constraints.
*/
void Conjunction::copyConstraintsFrom(const Conjunction *source) {
    for (std::list<Exp*>::const_iterator expIter=source->mEqualities.begin();
                expIter != source->mEqualities.end(); expIter++) {
        mEqualities.push_back((*expIter)->clone());
    }
    for (std::list<Exp*>::const_iterator expIter=source->mInequalities.begin();
                expIter != source->mInequalities.end(); expIter++) {
        mInequalities.push_back((*expIter)->clone());
    }
}

/*! Substitute each factor (i.e. the non-coefficient
** part of a term) with the expression mapped to that factor 
** in all our equalities and inequalities.
** Calls cleanup at end to resort constraints.
** \param searchTermToSubExp (none of the Term* or Exp* are adopted)
*/
void Conjunction::substituteInConstraints(SubMap& searchTermToSubExp) {

    // straight-forward substitution into equalities
    std::list<Exp*>::iterator expIter=mEqualities.begin();
    while (expIter != mEqualities.end()) {
        (*expIter)->substitute(searchTermToSubExp);
        expIter++;
    }
    expIter=mInequalities.begin();
    while (expIter != mInequalities.end()) {
        (*expIter)->substitute(searchTermToSubExp);
        expIter++;
    }
    cleanUp();
}


/*! Convert to a human-readable string.
**
*/
std::string Conjunction::toString() const {
    std::stringstream ss;
    ss << "{ " << mTupleDecl.toString(true,mInArity);

    bool first = true;
    for (std::list<Exp*>::const_iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        if (not first) { ss << " && "; }
        else { ss << " : ";  first = false; }
        ss << (*i)->toString() << " = 0";
    }

    for (std::list<Exp*>::const_iterator i=mInequalities.begin();
                i != mInequalities.end(); i++) {
        if (not first) { ss << " && "; }
        else { ss << " : ";  first = false; }
        ss << (*i)->toString() << " >= 0";
    }

    ss << " }";
    return ss.str();
}

/*! Convert to a human-readable string (sub in for tuple vars).
*/
std::string Conjunction::prettyPrintString() const {
    std::stringstream ss;
    ss << "{ " << mTupleDecl.toString(true,mInArity);
    bool first = true;
    Conjunction *dup = this->clone();

/* MMS, not sure why we are doing this code
    for (unsigned int i=0; i<dup->mTupleDecl.size(); i++) {
        Exp *varExp = new Exp();
        varExp->addTerm(mTupleDecl.elemCreateTerm(i,i));
        TupleVarTerm *tv = new TupleVarTerm(i);

        // perform the substitution on all our constraints
        for (std::list<Exp*>::const_iterator expIter=dup->mEqualities.begin();
                expIter != dup->mEqualities.end(); expIter++) {
            (*expIter)->substitute(varExp->clone(), *tv);
        }
        for (std::list<Exp*>::const_iterator expIter=dup->mInequalities.begin();
                expIter != dup->mInequalities.end(); expIter++) {
            (*expIter)->substitute(varExp->clone(), *tv);
        }
        delete tv;
        delete varExp;
    }
*/

    dup->cleanUp();
    for (std::list<Exp*>::const_iterator i = dup->mEqualities.begin();
                i != dup->mEqualities.end(); i++) {
        if (not first) { ss << " && "; }
        else { ss << " : ";  first = false; }
        ss << (*i)->prettyPrintString(mTupleDecl)<< " = 0";
    }

    for (std::list<Exp*>::const_iterator i = dup->mInequalities.begin();
                i != dup->mInequalities.end(); i++) {
        if (not first) { ss << " && "; }
        else { ss << " : ";  first = false; }
        ss << (*i)->prettyPrintString(mTupleDecl)<< " >= 0";
    }

    ss << " }";
    delete dup;
    return ss.str();
}

/* Convert to a string for DOT output
**
** \param next_id       id for this Conjunction node
**
** Reuses short segments of code from toString()
**
*/
std::string Conjunction::toDotString(int & next_id) const
{
    std::stringstream result;
    int self_id = next_id++;

    // Print out self node
    result << self_id << " [label = \"Conjunction\\n";
    result << mTupleDecl.toString(true,mInArity);
    result << "\"];\n";

    // Print out the Equalities
    if(mEqualities.begin() != mEqualities.end()){
        int eq_node = next_id++;
        result << self_id << "->" << eq_node << ";\n";
        result << eq_node << " [label = \"Equalities\\n... = 0\"];\n";
        for (std::list<Exp*>::const_iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
            // recursively call on expressions
            result << (*i)->toDotString(eq_node,next_id);
        }
    }

    // Print out the Inequalities
    if(mInequalities.begin() != mInequalities.end()){
        int ineq_node = next_id++;
        result << self_id << "->" << ineq_node << ";\n";
        result << ineq_node << " [label = \"Inequalities\\n... >= 0\"];\n";
        for (std::list<Exp*>::const_iterator i=mInequalities.begin();
                i != mInequalities.end(); i++) {
            // recursively call on expressions
            result << (*i)->toDotString(ineq_node,next_id);
        }
    }

    return result.str();
}

/* Convert to a string for DOT output
**
** \param parent_id     id for parent node
** \param next_id       id for this Conjunction node
**
** Reuses short segments of code from toString()
**
*/
std::string Conjunction::toDotString(int parent_id, int & next_id) const
{
    std::stringstream result;
    int self_id = next_id;

    // Connect parent to self
    result << parent_id << " -> " << self_id << ";\n";

    // Print out self node
    result << this->toDotString(next_id);

    return result.str();
}

/*!
    The generated SymboIterator escapes this method and needs to be
    deleted by the caller.

    FIXME: this is an example where having a visitor design pattern
    would be helpful.  Having to create this getSymbolIterator
    at Conjunction and Exp level.
*/
StringIterator* Conjunction::getSymbolIterator() const {
    std::set<std::string> symbolSet;

    // Collect symbols from equalities
    for (std::list<Exp*>::const_iterator i=mEqualities.begin();
            i != mEqualities.end(); i++) {
        // recursively call on expressions
        StringIterator* subSymIter = (*i)->getSymbolIterator();
        while (subSymIter->hasNext()) {
            symbolSet.insert( subSymIter->next() );
        }
        delete subSymIter;
    }

    // Collect symbols from inequalities
    for (std::list<Exp*>::const_iterator i=mInequalities.begin();
            i != mInequalities.end(); i++) {
        // recursively call on expressions
        StringIterator* subSymIter = (*i)->getSymbolIterator();
        while (subSymIter->hasNext()) {
            symbolSet.insert( subSymIter->next() );
        }
        delete subSymIter;
    }

    return new StringIterator( symbolSet );
}

//! Get an iterator over the tuple variables, in order.
StringIterator* Conjunction::getTupleIterator() const {
    std::list<std::string> tupleList;
    for (int i=0; i<arity(); i++) {
        if (mTupleDecl.elemIsConst(i)) {
            throw assert_exception("Conjunction::getTupleIterator: expecting "
                               "all tuple vars to be non const");
        }
        tupleList.push_back(mTupleDecl.elemVarString(i));
    }
    return new StringIterator( tupleList );
}


bool Conjunction::isFunction(int inArity) const {
    for (int i=inArity; i<arity(); i++) {
        // otherwise, we must be able to find a function for
        // it in terms of the input tuple decls
        Exp *funcExp = findFunction(i, 0, inArity-1);
        if (not funcExp) return false;
        delete funcExp;
    }
    return true;
}

bool Conjunction::isFunctionInverse(int inArity) const {
    for (int i=0; i<inArity; i++) {
        // otherwise, we must be able to find a function for
        // it in terms of the output tuple decls
        Exp *funcExp = findFunction(i, inArity, arity()-1);
        if (not funcExp) return false;
        delete funcExp;
    }
    return true;
}
//! Compose with another conjunction, given innerArity (which
//! is this's in arity and rhs's out arity).
//
// Both the point where we are setting up equality constraints and
// when we do substitute into constraints can cause constraints such
// as 1=0 to occur.  Instead of dealing with them at each point they
// can be introduced, we decided to have the SparseConstraints level
// cleanup remove a conjunction when such constraints exist within
// the function.
Conjunction*
Conjunction::Compose(const Conjunction *rhs, int innerArity) const {
    // Compute the input and output arities for LHS and RHS conjunctions.
    int lhsInArity = innerArity;
    int lhsOutArity = arity() - lhsInArity;
    int rhsOutArity = innerArity;
    int rhsInArity = rhs->arity() - rhsOutArity;
    int finalInArity = rhsInArity;
    int finalOutArity = lhsOutArity;

    // figure out whether the relations are functions or inverses of functions
    bool lhsIsFunction = isFunction(lhsInArity);
    bool rhsIsFunction = rhs->isFunction(rhsInArity);
    bool lhsIsFunctionInverse = isFunctionInverse(lhsInArity);
    bool rhsIsFunctionInverse = rhs->isFunctionInverse(rhsInArity);
    
    // Check that we can do compose.
    if (!( (lhsIsFunction && rhsIsFunction) || 
           (lhsIsFunctionInverse && rhsIsFunctionInverse) ) ) {
        // If none of the above apply, we can't compose.
        // Should never happen, since we require all relations to
        // be either a function or a function inverse.
        std::stringstream ss;
        ss << "Conjunction::Compose: both relations are "
                            "required to be a function or a function "
                            "inverse: lhsIsFunction:" << lhsIsFunction <<
                            " lhsIsFunctionInverse:" << lhsIsFunctionInverse <<
                            " rhsIsFunction:" << rhsIsFunction <<
                            " rhsIsFunctionInverse:" <<rhsIsFunctionInverse;
        throw assert_exception(ss.str());
    }           

    // Set up the resulting conjunction, using input tuple variables
    // from the RHS, and output tuple variables from the LHS, and putting
    // the inner arity variables off the end (we truncate those at the end).
    // e.g. {[a,b,c] -> [e,f]} compose {[i]->[a,b,c]}
    // result will be {[i] -> [e,f, a,b,c]}
    Conjunction *result = new Conjunction( 
        finalInArity +  finalOutArity + innerArity);
    result->mInArity = finalInArity;
    
    // get copies of lhs and rhs to line up their tuple vars with result
    // e.g. if lhs = {[a,b,c]->[e,f]} then lhs_copy = {[ , e, f, a, b, c]}
    Conjunction* lhs_copy = new Conjunction(*this);
    lhs_copy->pushConstToConstraints();
    std::vector<int> shiftLHSVars(lhs_copy->arity());
    for (int i=0; i<lhsInArity; i++) {
        shiftLHSVars[i] = i + finalInArity + finalOutArity;
    }
    for (int i=lhsInArity, j=0; i<lhs_copy->arity(); i++, j++) {
        shiftLHSVars[i] = j + finalInArity;
    }
    lhs_copy->remapTupleVars(shiftLHSVars);
    // e.g. if rhs = {[i]->[a,b,c]} then rhs_copy = {[ i, , , a, b, c]}
    Conjunction* rhs_copy = new Conjunction(*rhs);
    rhs_copy->pushConstToConstraints();
    std::vector<int> shiftRHSVars(rhs->arity());
    for (int i=0; i<rhsInArity; i++) {
        shiftRHSVars[i] = i;
    }
    for (int i=rhsInArity, j=0; i<rhs->arity(); i++, j++) {
        shiftRHSVars[i] = j + finalInArity + finalOutArity;
    }
    rhs_copy->remapTupleVars(shiftRHSVars);

    // copy the constraints from both into result
    result->copyConstraintsFrom(rhs_copy);
    result->copyConstraintsFrom(lhs_copy);    

    // Keep track of variable names so we don't have collisions
    // in the result tuple declarations.  
    // First get used var names from result input tuple.
    std::set<std::string> takenNames;
    for (int i=0; i<finalInArity; i++) {
        result->mTupleDecl.copyTupleElem(rhs_copy->mTupleDecl, i, i);
        takenNames.insert(result->mTupleDecl.elemVarString(i));
    }
    for (int i=0; i<finalOutArity; i++) {
        // Ensure the output tuple vars in result have different
        // names that the input tuple vars in result.
        result->mTupleDecl.setTupleElem(finalInArity+i,
            createUniqueName(lhs_copy->mTupleDecl.elemVarString(finalInArity+i),
                             takenNames) );
    }
    // Not going to name the inner arity tuple variables off the end
    // so they will have default names.  They are going away anyway
    // do don't need names.    
    // done with lhs_copy and rhs_copy
    delete lhs_copy;
    delete rhs_copy;

    // Compose Case 1: both relations are functions
    // lhs = { v -> z : z = F1(v) && C1 }
    // rhs = { x -> y : y = F2(x) && C2 }
    // lhs compose rhs = { x -> z : z=F1(F2(x)) && C1[v/F2(x)] && C2[y/F2(x)] }
    // Recall in result everything has been shifted so have x z v=y
    // F1 substitution is already there so just do F2 substitution 
    // to replace all v and y variables.
    if (lhsIsFunction and rhsIsFunction) {
        // Keep track of required substitutions.
       SubMap submapF2;

        // For each output tuple variable on the RHS, y_j, we'll
        // find it as an expression of the RHS input tuple variables, x.
        for (int j=0; j<innerArity; j++) {
            int rhsIdx = finalInArity + finalOutArity + j;
            Term* t = result->mTupleDecl.elemCreateTerm(rhsIdx, rhsIdx);
            // y_j = F2_j(x)
            Exp* F2_j = result->findFunction(rhsIdx, 0, rhsInArity-1);
            submapF2.insertPair(t, F2_j);
        }
        
        result->substituteInConstraints(submapF2);

    // Compose Case 2: both relations are function inverses
    // lhs = { v -> z : v = G1(z) && C1 }
    // rhs = { x -> y : x = G2(y) && C2 }
    // lhs compose rhs = { x -> z : x=G2(G1(z)) && C1[v/G1(z)] && C2[y/G1(z)] }
    // Recall in result everything has been shifted so have x z v=y
    // G2 substitution is already there so just do G1 substitution 
    // to replace all v and y variables.
    } else if (lhsIsFunctionInverse and rhsIsFunctionInverse) {
        // Keep track of required substitutions.
        SubMap submapG1;

        // For each input tuple variable on the LHS, v_j, we'll
        // find it as an expression of the LHS output tuple variables, z.
        for (int j=0; j<innerArity; j++) {
            int lhsIdx = finalInArity + finalOutArity + j;
            Term* t = result->mTupleDecl.elemCreateTerm(lhsIdx, lhsIdx);
            // v_j = G1_j(z)
            Exp* G1_j = result->findFunction(lhsIdx, 
                finalInArity, finalInArity+finalOutArity-1);
                
            submapG1.insertPair(t, G1_j);
        }
        
        result->substituteInConstraints(submapG1);
    }

    // Remove the inner tuple vars for relation from end of result set.
    // e.g. x z v=y should become x -> z
    // Create a new truncated tuple declaration
    TupleDecl final_tdecl(finalInArity+finalOutArity);
    for (int i=0; i<finalInArity+finalOutArity; i++) {
        final_tdecl.copyTupleElem(result->mTupleDecl, i, i);
    }
    result->setTupleDecl(final_tdecl);
    // All done!
    result->pushConstConstraintsToTupleDecl();
    return result;
}

/*! Pushes the constants in the tuple declaration into equality
**  constraints instead.
*/
void Conjunction::pushConstToConstraints() {
    // Loop through own tuple decl in search of constants.
    for (int i=0; i<arity(); i++) {
        if (mTupleDecl.elemIsConst(i)) {
            int const_val = mTupleDecl.elemConstVal(i);
            std::stringstream ss;
            ss << "c_" << const_val;
            mTupleDecl.setTupleElem(i, ss.str());
            Exp* e = new Exp();
            e->addTerm( mTupleDecl.elemCreateTerm(i,i) );
            e->addTerm( new Term(-1*const_val) );
            addEquality( e );
        }
    }
}

/*! Pushes constants in constraints into the tuple declaration.
*/
void Conjunction::pushConstConstraintsToTupleDecl() {
    std::map<unsigned int,int> tupleIndexToConst;
    std::set<Exp> equalitiesToRemove;

    // loop through own equality constraints to find
    // when tuple vars are equal to constants.
    for (std::list<Exp*>::const_iterator iter=mEqualities.begin();
                iter != mEqualities.end(); iter++ ) {
        Exp* e = *iter;
        
        // For each tuple variable solve for that tuple var in expression.
        // If result is a constant then keep that mapping of tuple var
        // index to constant and keep track of Exp as one we want
        // to erase from constraints.
        for (int i=0; i<arity(); i++) {
                    
            if (! mTupleDecl.elemIsConst(i)) {
                // solve for the tuple variable in expression
                Term* t = mTupleDecl.elemCreateTerm(i,i);
                Exp* solution = e->solveForFactor(t);
                // if the solution is a constant
                if (solution && solution->equalsZero()) {
                    tupleIndexToConst[i] = 0;
                    equalitiesToRemove.insert(*e);
                } 
                else if (solution && (t=solution->getTerm()) && t->isConst() ) {
                    tupleIndexToConst[i] = t->coefficient();
                    equalitiesToRemove.insert(*e);
                }
                if (solution) { delete solution; }
            }
        }
    }
        
    // Now go set the tuple variables to constants where necessary
    std::map<unsigned int,int>::iterator mapIter;
    for (mapIter = tupleIndexToConst.begin();
            mapIter != tupleIndexToConst.end(); mapIter++) {
        mTupleDecl.setTupleElem(mapIter->first, mapIter->second );
    }
    
    // Finally remove all those constant equality expressions.
    for (std::list<Exp*>::iterator iter=mEqualities.begin();
                iter != mEqualities.end(); ) {
        Exp* e = *iter;
        if (equalitiesToRemove.find(*e)!=equalitiesToRemove.end()) {
            delete (*iter);
            iter = mEqualities.erase(iter);
        } else {
            iter++;
        }
    }
}

//! Apply this (interpreted as a Relation) to rhs, which is interpreted
//! as a set.
// Implementation of Apply that uses the en masse substitution.
// FIXME: what if we have name collisions in the tuple declarations?
Conjunction *Conjunction::Apply(const Conjunction *rhs) const {

    // Check that "this" relation can solve for the input tuple elements
    // as constants or functions of the output tuple elements.
    // e.g. for {[i,j,k]->[t,v]} finding i,j,k as functions of t,v
    bool lhsIsFunctionInverse = isFunctionInverse(rhs->arity());
    if (!lhsIsFunctionInverse) {
        // If none of the above apply, we can't apply the relation to the set.
        // This happens if the relation is not a function inverse.
        throw assert_exception("Conjunction::Apply: Relation not invertible");
    }

    // Compute the input and output arities for LHS conjunction and result set.
    int lhsInArity = rhs->arity();
    int lhsOutArity = arity() - lhsInArity;
    int finalResultArity = lhsOutArity;

    // Set up the resulting set.  At first it will start out
    // as a copy of the lhs relation with the input and output
    // variables shifted.
    // e.g. if lhs={[i,j,k]->[t,v]} then result={[t,v]->[i,j,k]}
    Conjunction* result = new Conjunction(*this);
    result->pushConstToConstraints();
    result->mInArity = lhsOutArity;

    // Create map for remapping tuple variables.
    std::vector<int> shiftResultVars(result->arity());
    for (int i=0; i<lhsInArity; i++) {
        // moving input tuple vars over to the right
        // e.g. [i,j,k, , ] to [ , , i, j, k]
        shiftResultVars[i] = i+finalResultArity;
    }
    for (int i=lhsInArity; i<result->arity(); i++) {
        // moving output tuple vars over to the left
        // e.g. [ , , , t, v] to [t, v, , , ]
        shiftResultVars[i] = i-lhsInArity;
    }
    result->remapTupleVars(shiftResultVars);
    
    // Need to do shift the tuple variables in set
    // and in relation so that they all with tuple variables 
    // in the result.
    // e.g. shift [a,b,c] to [ , , a, b, c] so they line up with
    // i,j,k in result tuple declaration [t,v,i,j,k]
    // Shift tuple variables in the 
    Conjunction *rhs_copy = new Conjunction(*rhs);
    rhs_copy->pushConstToConstraints();
    std::vector<int> shiftRHSVars(rhs->arity());
    for (int i=0; i<rhs->arity(); i++) {
        shiftRHSVars[i] = i + finalResultArity;
    }
    rhs_copy->remapTupleVars(shiftRHSVars);

    // Copy all constraints from rhs set into
    // our result, where we will then do the needed substitutions.
    result->copyConstraintsFrom(rhs_copy);
    delete rhs_copy;

    // Apply
    // r = { x -> y : x = G(y) && C }
    // s = { z : D }
    // r(s) = { y : D[z/G(y)] && C[x/G(y)] }
    // For each input tuple variable on the LHS, we'll find it as an
    // expression of the LHS output tuple variables and then substitute this
    // in for the other constraints in the relation and the set.
    SubMap submap;
    for (int i=finalResultArity; i<result->arity(); i++) {
        // x_i = G_i(y)
        // e.g. recall input tuple are now to right of output
        // {[i,j,k] -> [t,v]} is [t,v, i,j,k] in result set
        Exp* G_i = result->findFunction(i, 0, finalResultArity-1);
        // Now create term that this function will be replacing.
        // D[z_i/G_i(y)] and C[x_i/G_i(y)]
        // substituting function in constraints for set and result
        Term* t = result->mTupleDecl.elemCreateTerm(i,i);
        submap.insertPair(t,G_i); 
    }

    // perform the substitution on all z_i and x_i vars at once
    result->substituteInConstraints(submap); 
    
    // Remove the input tuple vars for relation from end of result set.
    // e.g. [t,v,i,j,k] should become [t,v]
    // Create a new truncated tuple declaration
    TupleDecl final_tdecl(finalResultArity);
    for (int i=0; i<finalResultArity; i++) {
        final_tdecl.copyTupleElem(result->mTupleDecl, i, i);
    }
    Conjunction* final_result = new Conjunction(final_tdecl);
    final_result->copyConstraintsFrom(result);
    delete result;

    // All done!
    final_result->pushConstConstraintsToTupleDecl();
    return final_result;
}

//! Returns true if the conjunction has at least one equality or inequality
//! constraints.  If it contains none then this Conjunction is just
//! representing TRUE.
bool Conjunction::hasConstraints() const {
    if (mEqualities.size() + mInequalities.size() > 0 ) { return true; }
    else { return false; }
}



/*! Search among our equality constraints for one that defines tupleLocToFind
**  as a function of only tuple variables in the location range
**  [startTupleRange, endTupleRange].  Return a new copy of that expression.
*/
Exp *Conjunction::findFunction(int tupleLocToFind, int startTupleRange,
                               int endTupleRange) const {

    // Handle the trivial case of a constant tuple element.
    if (mTupleDecl.elemIsConst(tupleLocToFind)) {
        Exp *result = new Exp();
        int constVal = mTupleDecl.elemConstVal(tupleLocToFind);
        result->addTerm(new Term(constVal));
        return result;
    }

    // Otherwise, search all our equalities for one that can
    // be solved for this tuple element.
    for (std::list<Exp*>::const_iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        TupleVarTerm *factor = new TupleVarTerm(tupleLocToFind);
        Exp *solution = (*i)->solveForFactor(factor);
        if (not solution) {
          continue;
        }

        // OK, we found a solution, but we must verify that it
        // does not depend on any tuple variables outside the given range.
        bool acceptable = true;
        for (int j=0; j < arity(); j++) {
            if (j >= startTupleRange and j <= endTupleRange) continue;
            TupleVarTerm term(j);
            if (solution->dependsOn(term)) {
                // This depends on a forbidden tuple variable, so it's no good.
                acceptable = false;
                break;
            }
        } // check next tuple variable dependency
        if (acceptable) {
            return solution;    // Hooray!
        } else {
            delete solution;
        }
    } // try next equality

    // No function could be found.
    return NULL;
}

/*! Search among our equality constraints for one that defines tupleLocToFind
**  as a function of only tuple variables in the location range
**  [startTupleRange, endTupleRange].  Return a new copy of that expression.
**  Additionally remove the equality constraint that the function was
**  derived from.
*/
Exp *Conjunction::findAndRemoveFunction(int tupleLocToFind,
                                int startTupleRange, int endTupleRange) {

    // FIXME: this in large part is a copy of findFunction
    // can we do some refactoring?

    // Handle the trivial case of a constant tuple element.
    if (mTupleDecl.elemIsConst(tupleLocToFind)) {
        Exp *result = new Exp();
        int constVal = mTupleDecl.elemConstVal(tupleLocToFind);
        result->addTerm(new Term(constVal));
        return result;
    }

    // Otherwise, search all our equalities for one that can
    // be solved for this tuple element.
    for (std::list<Exp*>::iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        TupleVarTerm *factor = new TupleVarTerm(tupleLocToFind);
        Exp *solution = (*i)->solveForFactor(factor);
        if (not solution) {
          continue;
        }

        // OK, we found a solution, but we must verify that it
        // does not depend on any tuple variables outside the given range.
        bool acceptable = true;
        for (int j=0; j < arity(); j++) {
            if (j >= startTupleRange and j <= endTupleRange) continue;
            TupleVarTerm term(j);
            if (solution->dependsOn(term)) {
                // This depends on a forbidden tuple variable, so it's no good.
                acceptable = false;
                break;
            }
        } // check next tuple variable dependency
        if (acceptable) {
            //remove the equality constraint and then return the solution
            delete (*i);
            mEqualities.erase(i);
            return solution;    // Hooray!
        } else {
            delete solution;
        }
    } // try next equality

    // No function could be found.
    return NULL;
}


/*! Find any TupleVarTerms in this expression (and subexpressions)
**  and remap the locations according to the oldToNewLocs vector,
**  where oldToNewLocs[i] = j means that old location i becomes new
**  location j (i.e. __tvi -> __tvj).  As a special case, a value
**  of -1 means that old location goes away entirely.
*/
void Conjunction::remapTupleVars(const std::vector<int>& oldToNewLocs) {
    // Remap tuple variables in our equalities.
    for (std::list<Exp*>::iterator i=mEqualities.begin();
                i != mEqualities.end(); i++) {
        (*i)->remapTupleVars(oldToNewLocs);
        (*i)->normalizeForEquality();
    }

    // Remap tuple variables in our inequalities.
    for (std::list<Exp*>::iterator i=mInequalities.begin();
                i != mInequalities.end(); i++) {
        (*i)->remapTupleVars(oldToNewLocs);
    }

    // Remap our tuple declarations to match.
    // (There may be gaps -- we'll declare those as constant zeros.)
    int maxLoc = -1;
    for (unsigned int i=0; i<oldToNewLocs.size(); i++) {
        if (oldToNewLocs[i] > maxLoc) { maxLoc = oldToNewLocs[i]; }
    }

    TupleDecl newDecl(maxLoc+1);    // create a new tuple declaration
    for (unsigned int i=0; i<oldToNewLocs.size(); i++) {
        if (oldToNewLocs[i] >= 0) {
            newDecl.copyTupleElem(mTupleDecl,i,oldToNewLocs[i]);
        }
    }
    mTupleDecl = newDecl;
}

/*! Create the inverse of this conjunction. Returns a new Conjunction,
**    which the caller is responsible for deallocating.
*/
Conjunction* Conjunction::Inverse() const{

    std::vector<int> oldToNewLocs;

    unsigned int outArity = mTupleDecl.size() - mInArity;

    // Result conjunction should have its inArity be our outArity
    Conjunction *result = new Conjunction(*this);
    result->mInArity = outArity;

    // Create permutation for input and output tuples
    // so that the output tuple variables become the input tuple
    // variables and the input tuple variables become the outputs.
    for(unsigned int j = outArity; j < mTupleDecl.size(); j++){
        oldToNewLocs.push_back(j);
    }
    for(unsigned int j = 0; j < outArity; j++){
        oldToNewLocs.push_back(j);
    }
    result->remapTupleVars(oldToNewLocs);

    return result;
}


/*! Intersect this conjunction with the given other one
**    (i.e., this conjunction rhs).  Returns a new Conjunction,
**    which the caller is responsible for deallocating.
** \param rhs (not adopted)
*/
Conjunction* Conjunction::Intersect(const Conjunction* rhs) const {

    // copy in all of the constraints from ourselves
    Conjunction* retval = new Conjunction(*this);

    // then get equalities and equalities from rhs
    for (std::list<Exp*>::const_iterator i=rhs->mEqualities.begin();
            i != rhs->mEqualities.end(); i++) {
        retval->addEquality((*i)->clone());
    }
    for (std::list<Exp*>::const_iterator i=rhs->mInequalities.begin();
            i != rhs->mInequalities.end(); i++ ) {
        retval->addInequality((*i)->clone());
    }

    return retval;
}

/*! Treating this Conjunction like a domain or range.  Creates
** a new set where passed in tuple expression is
** bound assuming this domain, or range.
** User must deallocate returned Conjunction.
** 
** \param tuple_exp Expression tuple to bound.  Could just have one elem.
** 
** \return Conjunction will contain all bounds on expressions 
**         in tuple expression.  Will have no tuple variables.
*/
Conjunction* Conjunction::boundTupleExp(const TupleExpTerm& tuple_exp) const {
    // Check that arities match.
    if (tuple_exp.size()!=(unsigned)arity()) {
        throw assert_exception("Conjunction::boundTupleExp tuple_exp arity "
                               "does not match that of Conjunction");
    }

    // Create a zero arity conjunction.
    Conjunction* retval = new Conjunction(0);
    
    // copy all constraints from a clone of ourselves that has had
    // constant values pushed to the constraints
    Conjunction* dup = new Conjunction(*this);
    dup->pushConstToConstraints();
    retval->copyConstraintsFrom(dup);
    
    // then create bounds by substituting expressions in tuple_exp
    // into the expressions in copy of self in retval.
    // Has to be done in order because 
    SubMap submap;
    for (unsigned int i=0; i<tuple_exp.size(); i++) {
        Term * t = dup->mTupleDecl.elemCreateTerm(i, i);
        Exp * elemExp = tuple_exp.cloneExp(i);
        submap.insertPair( t, elemExp );
    }
    retval->substituteInConstraints( submap );

    delete dup;
    return retval;
}


//! Return true if the constraints in the conjunction are satisfiable.
// FIXME: need to check for affine inequalities and equalities that are
// not satisfiable.  For now just checking for constant equalities.
bool Conjunction::satisfiable() const {

    for (std::list<Exp*>::const_iterator i=mEqualities.begin();
            i != mEqualities.end(); i++) {
        if ( (*i)->isContradiction() ) {
            return false;
        }
    }

    return true;
}


/*! Removes equality and inequality Exps that are equal to zero
**  from list of Expressions.
*/

void Conjunction::cleanUp() {

    // Remove zero equalities, normalize, and remove nested inverse funcs
    for (std::list<Exp*>::iterator i=mEqualities.begin();
            i != mEqualities.end(); ) {

        // make first term positive
        (*i)->normalizeForEquality();

        // get rid of nested inverse funcs like f( f_inv( i ) )
        Exp* temp = (*i);
        (*i) = temp->collapseNestedInvertibleFunctions();
        delete temp;

        // remove zero expressions
        if((*i)->equalsZero()){
            std::list<Exp*>::iterator old = i;
            i++;
            delete *old;
            mEqualities.erase(old);
        } else { i++; }
    }

    // copy the current list of equalities and then add then
    // so they are added in order and duplicates are eliminated
    std::list<Exp*> equalityListCopy = mEqualities;
    mEqualities.clear();
    for (std::list<Exp*>::iterator i=equalityListCopy.begin();
            i != equalityListCopy.end(); i++) {
        this->addEquality(*i);
    }

    // Remove zero inequalities
    for (std::list<Exp*>::iterator i=mInequalities.begin();
            i != mInequalities.end(); ) {

         // get rid of nested inverse funcs like f( f_inv( i ) )
        Exp* temp = (*i);
        (*i) = temp->collapseNestedInvertibleFunctions();
        delete temp;

        if((*i)->equalsZero()){
            std::list<Exp*>::iterator old = i;
            i++;
            delete *old;
            mInequalities.erase(old);
        } else { i++; }
    }

    // copy the current list of inequalities and then add then
    // so they are added in order and duplicates are eliminated
    std::list<Exp*> inequalityListCopy = mInequalities;
    mInequalities.clear();
    for (std::list<Exp*>::iterator i=inequalityListCopy.begin();
            i != inequalityListCopy.end(); i++) {
        this->addInequality(*i);
    }

}

/*!
** Group together all equality expressions that 
** are parts of the same UFCallTerm, IOW i=f(k)[0] and 
** j=f(k)[1] should become (i,j) = f(k).
*/
void Conjunction::groupIndexedUFCalls() {
    // loop over all equalities in conjunction and gather up
    // all indexed UFCall return expressions (i.e. what they are equal to)
    std::map<UFCallTerm,std::map<int,Exp*> > ufcallAndIndex2Exp;
    for (std::list<Exp*>::iterator i=mEqualities.begin();
                i != mEqualities.end(); ) {
        Exp* e = (*i);

        // FIXME: assuming only one indexed UFCall, have
        // hasIndexedUFCall throw an assertion if that is
        // not the case.
        if (e->hasIndexedUFCall()) {
            UFCallTerm* uf_call_ptr = e->cloneIndexedUFCallTerm();
            // ensure that clone has a coefficient of 1
            // FIXME: what if it is something else?
            if (uf_call_ptr->coefficient()==-1) {
                uf_call_ptr->multiplyBy(-1);
            }
            Exp* ret_exp = e->solveForFactor(uf_call_ptr->clone());
            // FIXME: put in code that checks we haven't seen this
            // tuple index before
            // Store off the equality expression.
            UFCallTerm* nonindexed = uf_call_ptr->nonIndexedClone();
            ufcallAndIndex2Exp[*nonindexed][uf_call_ptr->tupleIndex()] 
                = ret_exp;
            delete nonindexed;
            delete uf_call_ptr;
            // remove original equality for now
            delete (*i);
            i=mEqualities.erase(i);
        } else {
            i++;  // must increment iterator if not remove expression
        }
    }
    
    // Loop through return expressions we found and see if have all
    // for a particular UFCall.  If do then replace them all with
    // a single equality.  If don't, then have to put them all back in.
    std::map<UFCallTerm,std::map<int,Exp*> >::const_iterator ufcallIter;
    for (ufcallIter=ufcallAndIndex2Exp.begin();
            ufcallIter!=ufcallAndIndex2Exp.end(); ufcallIter++) {
        bool foundAllIndices = true;
        // Get each ufcall
        UFCallTerm ufcall = ufcallIter->first;
        std::map<int,Exp*> index2retexp = ufcallIter->second;
        // Check that have a return expression for each index
        // and build TupleExpTerm.
        TupleExpTerm ret_exp_tuple(ufcall.size());
        for (unsigned int i=0; i<ufcall.size(); i++) {
            if (index2retexp.find(i)==index2retexp.end()) {
                foundAllIndices = false;
                break;
            } else {
                // Give TupleExpTerm ownership of saved expression.
                ret_exp_tuple.setExpElem(i,index2retexp[i]);
            }
        }
        // If we found all indices create a new equality and add it.
        if (foundAllIndices) {
            Exp* new_equality = new Exp();
            TupleExpTerm* tuple_term = new TupleExpTerm(ret_exp_tuple);
            tuple_term->multiplyBy(-1);
            new_equality->addTerm( tuple_term );
            new_equality->addTerm( new UFCallTerm( ufcall ) );
            addEquality( new_equality );
        }
        // otherwise we have to put all the equalities back in
        else {
            std::map<int,Exp*>::iterator indexIter;
            for (indexIter=index2retexp.begin(); 
                    indexIter!=index2retexp.end(); indexIter++) {
                Exp* ret_exp = indexIter->second;
                ret_exp->multiplyBy(-1);
                // Set ufcall term back to having the associated index.
                ufcall.setTupleIndex(indexIter->first);
                ret_exp->addTerm( new UFCallTerm(ufcall) );
                addEquality( ret_exp );
            }
        }
    }
    
}


/******************************************************************************/
#pragma mark -

/**************************** SparseConstraints *******************************/

SparseConstraints::SparseConstraints(){
}

SparseConstraints::SparseConstraints(const SparseConstraints& other) {
    *this = other;
}

SparseConstraints&
SparseConstraints::operator=(const SparseConstraints& other) {
    reset();
    for (std::list<Conjunction*>::const_iterator i=other.mConjunctions.begin();
                i != other.mConjunctions.end(); i++) {
        this->addConjunction(new Conjunction(**i));
    }
    this->cleanUp();
    return *this;
}

/*! Less than operator.
**      Compare two SparseConstraints in the following order:
**          1. by number of conjunctions: number of Conjunctions in mConjunctions
**          2. by sorted conjunctions list values: using Conjunction::operator<(Exp)
**      This operator assumes that the list of conjunctions (mConjunctions) and
**          is sorted for both SparseConstraints being compared.
** @param other, object to be compared
*/
bool SparseConstraints::operator<(const SparseConstraints& other) const {
    // 1. compare equality sizes
    if (mConjunctions.size() < other.mConjunctions.size()) { return true; }
    if (other.mConjunctions.size() < mConjunctions.size()) { return false; }

    // 2. compare sorted conjunctions list values
    std::list<Conjunction*>::const_iterator thisIter;
    std::list<Conjunction*>::const_iterator otherIter;
    otherIter = other.mConjunctions.begin();
    thisIter = mConjunctions.begin();
    
    while ( thisIter != mConjunctions.end() ) {
        //compare values
        if (**thisIter < **otherIter) { return true; }
        if (**otherIter < **thisIter) { return false; }
        //our mEqualities[i]'s are equal, continue
        ++thisIter;
        ++otherIter;
    }

    // All our elements are equal, so we are not less than other
    return false;
}

void SparseConstraints::reset() {
    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        delete (*i);
    }
    mConjunctions.clear();
}

SparseConstraints::~SparseConstraints() {
    reset();
}

//! For all conjunctions, sets them to the given tuple declaration.
//! If there are some constants that don't agree then throws exception.
//! If replacing a constant with a variable ignores the substitution
//! in that conjunction.
void SparseConstraints::setTupleDecl( TupleDecl tuple_decl ) {
    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        Conjunction* c = *i;
        c->setTupleDecl( tuple_decl );
    }
}

//! Returns a copy of the first conjunction's tuple declaration.
TupleDecl SparseConstraints::getTupleDecl(  ) const {
    return mConjunctions.front()->getTupleDecl();
}

//! For all conjunctions, sets them to the given tuple declarations.
//! If there are some constants that don't agree then throws exception.
//! If replacing a constant with a variable ignores the substitution
//! in that conjunction.
void SparseConstraints::setTupleDecl( TupleDecl tuple_decl_in, 
                                      TupleDecl tuple_decl_out ) {

    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        Conjunction* c = *i;
        c->setTupleDecl( tuple_decl_in, tuple_decl_out);
    }
}


//! addConjunction
//! \param adoptedconjuction (adopted)
void SparseConstraints::addConjunction(Conjunction *adoptedConjunction) {
    // If the Set/Relation was created with only an arity or TupleDecl
    // then there will be a single empty conjunction to indicate TRUE.
    // No Conjunction indicates FALSE.  Remove the empty Conjunction.
    if (mConjunctions.size()==1 && ! mConjunctions.front()->hasConstraints()) {
        delete mConjunctions.front();
        mConjunctions.clear();
    }

    // Look through all conjunctions to determine where this one should go.
    // Maintain a sorted order.
    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        Conjunction* c = *i;

        if (!(*adoptedConjunction < *c) && !(*c < *adoptedConjunction)) {
            //This conjunction already exists, no need to add it
            delete adoptedConjunction;
            return;
        }

        if (*adoptedConjunction < *c) {
            // We've found a term that belongs after the new one,
            // so insert the new term here, and we're done.
            mConjunctions.insert(i, adoptedConjunction);
            return;
        }
    }
    mConjunctions.push_back(adoptedConjunction);
}



std::string SparseConstraints::toString(int aritySplit) const {
    std::stringstream ss;

    // Print out all the conjunctions
    bool first = true;
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        if (not first) { ss << " union "; }
        ss << (*i)->toString();
        first = false;
    }

    // If there are no conjunctions then indicate we have an empty set
    // by printing out generic arity tuple declarations and FALSE as 
    // a constraint.
    if (mConjunctions.size()==0) {
        ss << "{ " 
           << TupleDecl::sDefaultTupleDecl(arity()).toString(true,aritySplit)
           << " : FALSE }";
    }

    return ss.str();
}

std::string SparseConstraints::prettyPrintString(int aritySplit) const {
    std::stringstream ss;

    bool first = true;
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        if (not first) { ss << " union "; }
        ss << (*i)->prettyPrintString();
        first = false;
    }

    // If there are no conjunctions then indicate we have an empty set
    // by printing out generic arity tuple declarations and
    // FALSE as a constraint.
    // FIXME: refactor with identical code in SparseConstraints::toString
    if (mConjunctions.size()==0) {
        TupleDecl genericTupleDecl(arity());
        for (int i=0; i<arity(); i++) {
            std::stringstream varname;
            varname << "tv" << i;
            genericTupleDecl.setTupleElem(i,varname.str());
        }
        ss << "{ " << genericTupleDecl.toString(true,aritySplit)
           << " : FALSE }";
    }

    return ss.str();
}

std::string SparseConstraints::toISLString(int aritySplit) const {

    // collect all symbolic/parameter variable names
    // and print the declaration for the symbolics
    std::stringstream ss;
    StringIterator * symIter;
    bool foundSymbols = false;
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        symIter = (*i)->getSymbolIterator();
        while (symIter->hasNext()) {
            // print out start of symbol declaration at first symbol
            if (foundSymbols == false) {
                ss << "[ ";
                foundSymbols = true;
                // print the symbol declaration itself
                ss << symIter->next();

            // later symbols will have a comma and then var name
            } else {
                ss << ", " << symIter->next();
            }
        }
        delete symIter;
    }
    // finish off declaration if there were symbols
    if (foundSymbols) {
        ss << " ] -> ";
    }

    // do a typical prettyPrint
    ss << prettyPrintString(aritySplit);

    return ss.str();
}


std::string SparseConstraints::toDotString() const{
    std::stringstream result;
    int self_id = 0;
    int next_id = 1;
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        result << (*i)->toDotString(self_id, next_id);
        next_id++;
    }
    return result.str();
}

//! Get an iterator over the tuple variables, in order.
// Just grabs the tuple variable names in the first conjunction.
StringIterator* SparseConstraints::getTupleIterator() const {
    if (mConjunctions.size() < 1) {
        throw assert_exception("SparseConstraints::getTupleIterator: "
                               "no conjunctions");
    }
    return mConjunctions.front()->getTupleIterator();
}

/*! Substitute each factor (i.e. the non-coefficient
** part of a term) with the expression mapped to that factor 
** in all our equalities and inequalities.
** None of the Term's in the map can be constant or an exception will
** be thrown.
** After this substitution the Set or Relation may not be normalized,
** but it is cleaned up.  What does that mean?
** \param searchTermToSubExp (none of the Term* or Exp* are adopted)
*/
void 
SparseConstraints::substituteInConstraints(SubMap& searchTermToSubExp) {
        
    searchTermToSubExp.startIter();
    while ( searchTermToSubExp.hasNext() ) {
        Term* search = searchTermToSubExp.next();
        if (search->isConst()) {
            throw assert_exception("SparseConstraints::substituteInConstraints:"
                " at least one Term in map is a constant, not allowed");
        }
    }

    // Each conjunction should do it's own substitution.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        (*i)->substituteInConstraints(searchTermToSubExp);
    }
}



bool _compareConjunctions( Conjunction* first, Conjunction* second ) {
    return *first < *second;
}

//! Remove duplicate constraints and trivial constraints
// FIXME: need a case where affine constraints within the conjunction
// make the conjunction unsatisfiable.  Should deal with this in release 2
// and use ISL.
void SparseConstraints::cleanUp(){
    // removes duplicate constraints, FIXME: does it also resort constraints?
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        (*i)->cleanUp();
    }
    // remove conjunctions that are not satisfiable
    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
                i != mConjunctions.end(); ) {
        if (not (*i)->satisfiable()) {
            delete (*i);
            mConjunctions.erase(i++);
        } else {
            i++;
        }
    }

    //sort the list of conjunctions
    mConjunctions.sort(_compareConjunctions);
}

/*! Find any TupleVarTerms in this expression (and subexpressions)
**  and remap the locations according to the oldToNewLocs vector,
**  where oldToNewLocs[i] = j means that old location i becomes new
**  location j (i.e. __tvi -> __tvj).  Throws an exception if an
**  old location is out of range for the given oldToNewLocs.
**  The new location will be -1 for old locations that are not being
**  remapped.  For example some might be constants in the TupleDecl.
*/
void SparseConstraints::remapTupleVars(const std::vector<int>& oldToNewLocs) {
    for (std::list<Conjunction*>::iterator iter=mConjunctions.begin();
            iter != mConjunctions.end(); iter++) {
        (*iter)->remapTupleVars(oldToNewLocs);
    }
}


//! This function returns a set of constraints that are in caller but not in A
std::set<Exp> SparseConstraints::constraintsDifference(SparseConstraints* A){

  // Right now, this should only be called if there is a single Conjunction.
  if(mConjunctions.size() != 1 ){
    throw assert_exception("constraintsDifference:: "
                           "more than one Conjunction in Relation");
  }

  Conjunction * conjB = this->mConjunctions.front();
  Conjunction * conjA = A->mConjunctions.front();

  std::set<Exp> diffSet;

  const std::list<Exp*> eqB = conjB->equalities();
  const std::list<Exp*> ineqB = conjB->inequalities();
  const std::list<Exp*> eqA = conjA->equalities();
  const std::list<Exp*> ineqA = conjA->inequalities();

  int found = 0;
  for (std::list<Exp*>::const_iterator it=eqB.begin(); it != eqB.end(); it++){
    found = 0;
    for (std::list<Exp*>::const_iterator jt=eqA.begin(); jt != eqA.end(); jt++){
      if( (*(*it)) == (*(*jt)) ){ 
        found = 1;
        break;
      }
    }
    if(!found){
      diffSet.insert( *(*it) );
    }  
  }

  for (std::list<Exp*>::const_iterator it=ineqB.begin(); it != ineqB.end(); it++){
    found = 0;
    for (std::list<Exp*>::const_iterator jt=ineqA.begin(); jt != ineqA.end(); jt++){
      if( (*(*it)) == (*(*jt)) ){ 
        found = 1;
        break;
      }
    }
    if(!found){
      diffSet.insert( *(*it) );
    }  
  }

  return diffSet;
}



/******************************************************************************/
#pragma mark -

/********************************** Set ***************************************/
Set::Set(std::string str) {
    Set* s = parser::parse_set(str); // parse string
    *this = *s; // copy created Set
    delete s;
}

//! Creates a set with the specified arity.  It starts with no constraints
//! so all tuples of that arity belong in it.
Set::Set(int arity) : SparseConstraints(), mArity(arity) {
    addConjunction(new Conjunction(TupleDecl::sDefaultTupleDecl(arity)));
}

//! Creates a set with the specified tuple declaration.  
//! It starts with no constraints so all tuples of that arity belong in it.
Set::Set(TupleDecl tdecl) : SparseConstraints(), mArity(tdecl.size()) {
    addConjunction(new Conjunction(tdecl));
}

Set::Set(const Set& other) {
    *this = other;
}
Set& Set::operator=(const Set& other) {
    SparseConstraints::operator=(other);
    mArity = other.mArity;
    return *this;
}
/*! Equal operator.
** 
** @param other, object to be compared
*/
bool Set::operator==( const Set& other) const {
    if ((*this) < other){ return false; }
    if ( other < (*this)){ return false; }
    return true;
}

/*! Less than operator.
**      Compare two Set in the following order:
**          1. by arity: mArity
**          2. by Conjunctions: using SparseConstraints::operator<(Exp)
** @param other, object to be compared
*/
bool Set::operator<( const Set& other) const {
    if ((*this).mArity < other.mArity){ return true; }
    if ((*this).mArity > other.mArity){ return false; }
    return (static_cast<SparseConstraints>(*this) < static_cast<SparseConstraints>(other));
}

Set::~Set() {
}

std::string Set::toDotString() const{
    std::stringstream result;
    result << "digraph SparseConstraints {\n";
    result << 0 << " [label = \"Set\\nmArity=" << mArity << "\"];\n";
    result << SparseConstraints::toDotString();
    result << "}\n";
    return result.str();
}

//! For all conjunctions, sets them to the given tuple declaration.
//! If there are some constants that don't agree then throws exception.
//! If replacing a constant with a variable ignores the substitution
//! in that conjunction.
//! Also modifies arity to match.
void Set::setTupleDecl( TupleDecl tuple_decl ) {
    mArity = tuple_decl.size();
    SparseConstraints::setTupleDecl(tuple_decl);
}

/*! Union this set with the given other one
**    (i.e., this Union rhs).  Returns a new Set,
**    which the caller is responsible for deallocating.
** \param rhs (not adopted)
*/
Set* Set::Union(const Set* rhs) const{
    // Check that the arities are compatible.
    if (rhs->mArity != mArity) {
        throw assert_exception("Set::Union: mismatched arities");
    }

    Set *result = new Set(mArity);
    
    // If both sets have no conjunctions then they are both empty sets
    // and so this set needs to be empty as well.  Removing default
    // conjunction before adding in conjunctions of other sets.
    if (result->mConjunctions.size()==1 
            && ! result->mConjunctions.front()->hasConstraints()) {
        delete result->mConjunctions.front();
        result->mConjunctions.clear();
    }
    
    // Add in conjunctions from lhs/this set.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
        i != mConjunctions.end(); i++) {
        result->addConjunction(new Conjunction(**i));
    }

    // Add in conjunctions from other set.
    for (std::list<Conjunction*>::const_iterator i=rhs->mConjunctions.begin();
        i != rhs->mConjunctions.end(); i++) {
        result->addConjunction(new Conjunction(**i));
    }

    return result;
}

/*! Intersect this set with the given other one
**    (i.e., this Intersect rhs).  Returns a new Set,
**    which the caller is responsible for deallocating.
** \param rhs (not adopted)
*/
Set* Set::Intersect(const Set* rhs) const{
    // Check that the arities are compatible.
    if (rhs->mArity != mArity) {
        throw assert_exception("Set::Intersect: mismatched arities");
    }

    Set *result = new Set(mArity);

    // Have to do cross product intersection between conjunctions in both sets.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {

        for (std::list<Conjunction*>::const_iterator
                j=rhs->mConjunctions.begin();
                j != rhs->mConjunctions.end(); j++) {

            result->addConjunction((*i)->Intersect(*j));
        }
    }

    return result;
}

/*! Treating this Set like a domain or range.  Creates
** a new set where passed in tuple expression is
** bound assuming this domain, or range.
** User must deallocate returned Set.
** 
** \param tuple_exp Expression tuple to bound.  Could just have one elem.
**
** \return Set will contain all bounds on expressions in tuple expression.
*/
Set* Set::boundTupleExp(const TupleExpTerm& tuple_exp) const {
    Set *result = new Set(0);

    // Have to pull bounds from all conjunctions.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {
        result->addConjunction((*i)->boundTupleExp(tuple_exp));
    }

    return result;
}

// Replace UFs with vars, pass to ISL, and then reverse substitution.
void Set::normalize(bool bdr) {

    // Sometimes to provide arguments of an UFC like sigma(a1, a2, ...)
    // we use another UFC that is not indexed like left(f). Here, the
    // expanded form would look like this: 
    //                         sigma(left(f)[0], left(f)[1], ...) 
    // indexUFCs() would create the expanded format for normalization purposes.
    indexUFCs();

    // Create variable names for UF calls.
    UFCallMap* uf_call_map = new UFCallMap();
    
    // Replace uf calls with the variables to create an affine superset.
    Set* superset_copy = superAffineSet(uf_call_map,bdr);

    // Send affine super set to ISL and let it normalize it.
    Set* superset_normalized = passSetThruISL(superset_copy);
 
     // Reverse the substitution of vars for uf calls.
    Set* normalized_copy 
        = superset_normalized->reverseAffineSubstitution(uf_call_map);
   
    // Replace self with the normalized copy.
    *this = *normalized_copy;
        
    // Cleanup
    delete normalized_copy;
    delete uf_call_map;
    delete superset_copy;
    delete superset_normalized;
}

/******************************************************************************/
#pragma mark -

/******************************** Relation ************************************/
Relation::Relation(std::string str) {
    Relation* r = parser::parse_relation(str); // parse relation
    *this = *r; // copy created Relation
    delete r;
}

Relation::Relation(int inArity, int outArity)
  : SparseConstraints(), mInArity(inArity), mOutArity(outArity) {
}

Relation::Relation(const Relation& other) {
    *this = other;
}

Relation& Relation::operator=(const Relation& other) {
    mInArity = other.mInArity;
    mOutArity = other.mOutArity;
    SparseConstraints::operator=(other);
    return *this;
}

// copies conjunctions from the Set, but keeps *this's mInArity
// Checks for feasibility of mInArity in relation to other's mTupleDecl.size()
Relation& Relation::operator=(const Set& other) {

	if (mInArity >= other.arity()) {
        throw assert_exception("Relation::operator=(Set): "
                               "impossible arity match");
	}
	mOutArity = other.arity() - mInArity;
	
    reset();
    for (std::list<Conjunction*>::const_iterator i=other.mConjunctions.begin();
                i != other.mConjunctions.end(); i++) {
        // copy Set's conjunction with Relation's inArity     
        Conjunction* c = new Conjunction((*i)->arity(), mInArity);
        c->setTupleDecl((*i)->getTupleDecl());
        c->copyConstraintsFrom(*i);
        // add it to Relation
        this->addConjunction(c);
    }

    this->cleanUp();
    return *this;
}

/*! Equals operator.
**      if self < other return false
**      if other < self return false
**      return true
**   
** @param other, object to be compared
*/   
bool Relation::operator==( const Relation& other) const {

    if( (*this) < other){
        return false;
    }
    if( other < (*this)){
        return false;
    }
    return true;
}

/*! Less than operator.
**      Compare two Relation in the following order:
**          1. by in arity: mInArity
**          2. by out arity: mOutArity
**          2. by Conjunctions: using SparseConstraints::operator<(Exp)
** @param other, object to be compared
*/
bool Relation::operator<( const Relation& other) const {
    //In arity
    if ((*this).mInArity < other.mInArity){ return true; }
    if ((*this).mInArity > other.mInArity){ return false; }
    //Out arity
    if ((*this).mOutArity < other.mOutArity){ return true; }
    if ((*this).mOutArity > other.mOutArity){ return false; }
    //Conjunctions
    return (static_cast<SparseConstraints>(*this) < 
            static_cast<SparseConstraints>(other));
}

Relation::~Relation() {
}

std::string Relation::toString() const {
    return SparseConstraints::toString(mInArity);
}

std::string Relation::prettyPrintString() const {
    return SparseConstraints::prettyPrintString(mInArity);
}

std::string Relation::toDotString() const{
    std::stringstream result;
    result << "digraph SparseConstraints {\n";
    result << 0 << " [label = \"Relation\\nmInArity=" << mInArity;
    result << "\\nmOutArity=" << mOutArity << "\"];\n";
    result << SparseConstraints::toDotString();
    result << "}\n";
    return result.str();
}

//! For all conjunctions, sets them to the given tuple declaration.
//! If there are some constants that don't agree then throws exception.
//! If replacing a constant with a variable ignores the substitution
//! in that conjunction.
//! Also modifies arity to match.
void Relation::setTupleDecl( TupleDecl tuple_decl ) {
    // Only modify the output arity.
    mOutArity = tuple_decl.size() - mInArity;
    SparseConstraints::setTupleDecl(tuple_decl);
}

/*! Compose this relation with the given other one
**    (i.e., this Compose rhs).  Returns a new Relation,
**    which the caller is responsible for deallocating.
*/
Relation *Relation::Compose(const Relation *rhs) const {
    // Check that the arities are compatible.
    if (rhs->mOutArity != mInArity) {
        throw assert_exception("Relation::Compose: mismatched arities");
    }
    // For convenience, remember this inner arity that matches
    // the RHS out arity and the LHS in arity.  This is used
    // below while composing the conjunctions.
    int innerArity = rhs->mOutArity;

    Relation *result = new Relation(rhs->mInArity, mOutArity);

    // Find all combinations of LHS and RHS conjunctions.
    for (std::list<Conjunction*>::const_iterator lhsIter=mConjunctions.begin();
                lhsIter != mConjunctions.end(); lhsIter++) {
        for (std::list<Conjunction*>::const_iterator
                rhsIter=rhs->mConjunctions.begin();
                rhsIter != rhs->mConjunctions.end(); rhsIter++) {
            Conjunction *combo = (*lhsIter)->Compose(*rhsIter, innerArity);
            if (combo) { result->addConjunction(combo); }
        } // next RHS conjunction
    } // next LHS conjunction

    result->cleanUp();
    return result;
}

/*  Apply this relation to the given set. Returns a new Set,
    which the caller is responsible for deallocating.

    Apply is almost exactly like Compose except that now the
    rhs is a set.

*/
Set *Relation::Apply(const Set* rhs) const {
    // Check that the arities are compatible.
    if (rhs->arity() != mInArity) {
       throw assert_exception("Relation::Apply: mismatched arities");
    }

    Set *result = new Set(mOutArity);

    // Find all combinations of LHS relation and RHS set conjunctions.
    for (std::list<Conjunction*>::const_iterator lhsIter=mConjunctions.begin();
                lhsIter != mConjunctions.end(); lhsIter++) {
        for (std::list<Conjunction*>::const_iterator
                rhsIter=rhs->conjunctionBegin();
                rhsIter != rhs->conjunctionEnd(); rhsIter++) {
            Conjunction *combo = (*lhsIter)->Apply(*rhsIter);
            if (combo) { result->addConjunction(combo); }
        } // next RHS conjunction
    } // next LHS conjunction

    result->cleanUp();
    return result;
}


/*! Union this relation with the given other one
**    (i.e., this Union rhs).  Returns a new Relation,
**    which the caller is responsible for deallocating.
** \param rhs (not adopted)
*/
Relation* Relation::Union(const Relation* rhs) const{
    // Check that the arities are compatible.
    if (rhs->mInArity != mInArity || rhs->mOutArity != mOutArity) {
        throw assert_exception("Relation::Union: mismatched arities");
    }

    Relation *result = new Relation(mInArity, mOutArity);

    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
        i != mConjunctions.end(); i++) {
        result->addConjunction(new Conjunction(**i));
    }

    for (std::list<Conjunction*>::const_iterator i=rhs->mConjunctions.begin();
        i != rhs->mConjunctions.end(); i++) {
        result->addConjunction(new Conjunction(**i));
    }
    //result->cleanUp();  FIXME: might want later when cleanup can merge
    //constraints that have adjacent constraints
    return result;
}

/*! Intersect this relation with the given other one
**    (i.e., this Intersect rhs).  Returns a new Relation,
**    which the caller is responsible for deallocating.
** \param rhs (not adopted)
*/
Relation* Relation::Intersect(const Relation* rhs) const{
    // Check that the arities are compatible.
    if (rhs->mInArity != mInArity || rhs->mOutArity != mOutArity) {
        throw assert_exception("Relation::Union: mismatched arities");
    }

    Relation *result = new Relation(mInArity, mOutArity);

    // Have to do cross product intersection between conjunctions in both sets.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
            i != mConjunctions.end(); i++) {

        for (std::list<Conjunction*>::const_iterator
                j=rhs->mConjunctions.begin();
                j != rhs->mConjunctions.end(); j++) {

            result->addConjunction((*i)->Intersect(*j));
        }
    }

    //result->cleanUp();  FIXME: might want later when cleanup can merge
    //constraints that have adjacent constraints
    return result;
}


/*! Inverse this relation. Returns a new Relation,
**    which the caller is responsible for deallocating.
*/
Relation* Relation::Inverse() const{
    Relation *result = new Relation(mOutArity, mInArity);

    // Compute the inverse of each Conjunction.
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
        i != mConjunctions.end(); i++) {
        result->addConjunction((*i)->Inverse());
    }

    result->cleanUp();

    return result;
}

/*! Determine whether all of the outputs can be determined as
**  functions of the inputs.  Need to check for each conjunction.
*/
bool Relation::isFunction() const {
    bool result = true;
    
    // Each conjunction must be a function
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
        i != mConjunctions.end(); i++) {
        result = result && (*i)->isFunction(inArity());
    }
    
    return result;
}

/*! Determine whether all of the inputs can be determined as
**  functions of the outputs.  Need to check for each conjunction.
*/
bool Relation::isFunctionInverse() const {
    bool result = true;
    
    // Each conjunction must be a function
    for (std::list<Conjunction*>::const_iterator i=mConjunctions.begin();
        i != mConjunctions.end(); i++) {
        result = result && (*i)->isFunctionInverse(inArity());
    }
    
    return result;
}


/*! Return the expression that describes the value of the tupleLoc
*   specified as a function of the tuple locations in the start
*   through the stop locations.
*/
Exp* Relation::findFunction(int tupleLocToFind,
                            int startTupleRange, int endTupleRange)
{
    // This should only be called if there is a single Conjunction.
    if(mConjunctions.size() != 1 ){
        throw assert_exception("more than one Conjunction in Relation");
    }

    return mConjunctions.front()->findFunction(tupleLocToFind,
            startTupleRange, endTupleRange);
}

void Relation::addConjunction(Conjunction *adoptedConjunction) {
    if (adoptedConjunction->inarity() != mInArity) {
        throw assert_exception("Relation::addConjunction: mismatched arities");
    }
    SparseConstraints::addConjunction(adoptedConjunction);
}

// Replace UFs with vars, pass to ISL, and then reverse substitution.
void Relation::normalize(bool bdr) {

    // Sometimes to provide arguments of an UFC like sigma(a1, a2, ...)
    // we use another UFC that is not indexed like left(f). Here, the
    // expanded form would look like this: 
    //                         sigma(left(f)[0], left(f)[1], ...) 
    // indexUFCs() would create the expanded format for normalization purposes.
    indexUFCs();

    // Create variable names for UF calls.
    UFCallMap* uf_call_map = new UFCallMap();
    
    // Replace uf calls with the variables to create an affine superset.
    Relation* superset_copy = superAffineRelation(uf_call_map,bdr);

    // Send affine super set to ISL and let it normalize it.
    Relation* superset_normalized = passRelationThruISL(superset_copy);

     // Reverse the substitution of vars for uf calls.
    Relation* normalized_copy 
        = superset_normalized->reverseAffineSubstitution(uf_call_map);

    // Replace self with the normalized copy.
    *this = *normalized_copy;
        
    // Cleanup
    delete normalized_copy;
    delete uf_call_map;
    delete superset_copy;
    delete superset_normalized;
}

            

/******************************************************************************/

/******************************************************************************/
#pragma mark -
/****************** Visitor Design Pattern Code *******************************/


//! Visitor design pattern, see Visitor.h for usage
void Conjunction::acceptVisitor(Visitor *v) {
    v->preVisitConjunction(this);
    
    std::list<Exp*>::iterator expIter = mEqualities.begin();
    while (expIter != mEqualities.end()) {
        (*expIter)->acceptVisitor(v);
        expIter++;
    }
    expIter=mInequalities.begin();
    while (expIter != mInequalities.end()) {
        (*expIter)->acceptVisitor(v);
        expIter++;
    }
    v->postVisitConjunction(this);
}

//! Visitor design pattern, see Visitor.h for usage
void SparseConstraints::acceptVisitor(Visitor *v) {
    v->preVisitSparseConstraints(this);
    for (std::list<Conjunction*>::iterator i=mConjunctions.begin();
                i != mConjunctions.end(); i++) {
        (*i)->acceptVisitor(v);
    }
    v->postVisitSparseConstraints(this);
}

//! Visitor design pattern, see Visitor.h for usage
void Set::acceptVisitor(Visitor *v) {
    v->preVisitSet(this);
    this->SparseConstraints::acceptVisitor(v);
    v->postVisitSet(this);
}

//! Visitor design pattern, see Visitor.h for usage
void Relation::acceptVisitor(Visitor *v) {
    v->preVisitRelation(this);
    SparseConstraints::acceptVisitor(v);
    v->postVisitRelation(this);
}



/******************************************************************************/
#pragma mark -
/*************** VisitorIndexUFC *****************************/
/*! Vistor Class used for indexing, non-indexing UFCs
**  Sometimes to provide arguments of an UFC like sigma(a1, a2, ...)
**  we use another UFC that is not indexed like left(f). Here, the
**  expanded form would look like this: 
**                                sigma(left(f)[0], left(f)[1], ...)
*/
class VisitorIndexUFC : public Visitor {
  public:
    VisitorIndexUFC(){}
    //! For each UFC adds Domain & Range constraints to addedConstSet
    void postVisitUFCallTerm(UFCallTerm * t){
           
        Set * s = queryDomainCurrEnv( t->name() );
        int act_nArg = s->arity();
        int cur_nArg = t->numArgs();
 
        if( cur_nArg == 1 && act_nArg > 1){
            
            Exp* par = t->getParamExp(0);
            Term *ta = par->getTerm();
            if( !(ta) || !(ta->type()=="UFCallTerm") ){
                throw assert_exception("VisitorIndexUFC: Null or non UFC tuple argument");
            }
            UFCallTerm* uft = dynamic_cast<UFCallTerm*>(ta);
 
            UFCallTerm new_ufc(t->coefficient(), t->name() , act_nArg);
            for(int i = 0 ; i < act_nArg ; i++){
                UFCallTerm* temp = dynamic_cast<UFCallTerm*>( uft->clone() );
                temp->setTupleIndex(i);
                Exp *ae = new Exp(); 
                ae->addTerm( temp );
                new_ufc.setParamExp(i,ae);
            }
            *t = new_ufc;               
        }
    }
};

void SparseConstraints::indexUFCs()
{
    VisitorIndexUFC* v = new VisitorIndexUFC();
    this->acceptVisitor(v);
}


/*****************************************************************************/
#pragma mark -
/*************** isUFCallParam *****************************/

class VisitorIsUFCallParam : public Visitor {
  private:
    bool mResult;
    int mTupleID;
    bool mSeenTupleVar;
    bool prevSeen;

  public:
    VisitorIsUFCallParam(int tupleID) : mResult(false),
                 mTupleID(tupleID), mSeenTupleVar(false), prevSeen(false) {}

    virtual ~VisitorIsUFCallParam(){ }

    bool returnResult() { return mResult; }

    void preVisitTupleVarTerm(iegenlib::TupleVarTerm *t) {
        if (t->tvloc()==mTupleID) {
            mSeenTupleVar = true;
        }
    }
    void preVisitExp(iegenlib::Exp * e) {
        // Recording value of mSeenTupleVar, so we can change it back in our
        // post visit.
        prevSeen = mSeenTupleVar;
        mSeenTupleVar = false;
    }
    void postVisitExp(iegenlib::Exp * e) {
        // An expression that is not an inequality or 
        // an equality is a parameter to a UFCall.

        if (e->isExpression() and mSeenTupleVar) {
            mResult = true;
        }
        // Revive flag for any higher level expression in traversal
        mSeenTupleVar = prevSeen;
    }

};

bool SparseConstraints::isUFCallParam(int tupleID) {

    VisitorIsUFCallParam * v = new VisitorIsUFCallParam(tupleID);
    this->acceptVisitor(v);
    bool result = v->returnResult();
    delete v;

    return result;
}

/*****************************************************************************/
#pragma mark -
/*************** VisitorBoundDomainRange *****************************/
/*! Vistor Class used in BoundDomainRange
** Traverses UFCalls for adding constarints due to their Domain and Range
*/
class VisitorBoundDomainRange : public Visitor {
  private:
         Set* addedConstSet;  // In one Conjunction, for internal use only
         int in_ar;
  public:
         //! For each UFC adds Domain & Range constraints to addedConstSet
         void preVisitUFCallTerm(UFCallTerm * t);
         //! Initials addedConstSet and in_ar for each c
         void preVisitConjunction(iegenlib::Conjunction * c){
             in_ar = c->inarity();
             addedConstSet = new Set(c->getTupleDecl());
         }
         //! Adds in all of the gathered constraints in addedConstSet for c
         void postVisitConjunction(iegenlib::Conjunction * c){
             Conjunction *ct = c->Intersect(addedConstSet->mConjunctions.front());
             *c = *ct;
             c->setInArity( in_ar );
             delete addedConstSet;
             delete ct;
         }
};

void VisitorBoundDomainRange::preVisitUFCallTerm(UFCallTerm * t){

    UFCallTerm *uf_call = new UFCallTerm(*t);
    uf_call->setCoefficient(1);

   // Bounding argument expressions of UFCalls by their domain, 
   // and adding them as inequalities to constraints set
   {
    // Create a TupleExpTerm from the parameter expressions.
    TupleExpTerm tuple_exp(uf_call->numArgs());
    for (unsigned int count=0; count<uf_call->numArgs(); count++) {
        tuple_exp.setExpElem(count, uf_call->getParamExp(count)->clone());
    }

    // look up bound for uninterpreted function
    Set* domain = iegenlib::queryDomainCurrEnv(uf_call->name());

    // have the domain create the constraints and store those constraints
    Set* constraintSet = domain->boundTupleExp(tuple_exp);

    // The constraintSet returned by boundTupleExp will not have any
    // tuple variables. We want the tuple variable declaration to line up.
    constraintSet->setTupleDecl( addedConstSet->getTupleDecl() );
    
    // Intersect the new set of constraints with the existing constraints.
    Set* mCC = addedConstSet;     // pointer to current set
    addedConstSet = mCC->Intersect(constraintSet);

    delete mCC; // cleanup old guy
    delete constraintSet;
    delete domain;
   }

   // Bounding UFCalls by their range, 
   // and adding them as inequalities to constraints set
   {
    // look up range for uninterpreted function
    Set* range = iegenlib::queryRangeCurrEnv(uf_call->name());
    
    // Assuming that uf call and its range align.
    if (! uf_call->isIndexed() 
        && ((unsigned)range->arity() != uf_call->size()) ) {
        throw assert_exception("Set::boundDomainRange: "
        "ufcall returning fewer dimensions than declared range");
    }

    // For each dimension of the return value create a instance
    TupleExpTerm tuple_exp(uf_call->size());
    
    // Determine what the output arity of this particular UF call is
    // taking into consideration that it could be indexed.
    unsigned int out_arity = range->arity();

    for (unsigned int i=0; i<out_arity; i++) {    
        // Create a temporary variable and maintain correspondence 
        // with UFCallTerm.  Add one more tuple var to constraints.
        UFCallTerm* indexed_uf_call;
        if (out_arity==1) {
            indexed_uf_call = new UFCallTerm(*uf_call);
        } else {
            indexed_uf_call = new UFCallTerm(*uf_call);
            indexed_uf_call->setTupleIndex( i );
        }
        
        // Create a TupleExpTerm from the new temporary var.
        Exp* tuple_var_exp = new Exp();
        tuple_var_exp->addTerm(indexed_uf_call);
        tuple_exp.setExpElem(i,tuple_var_exp);   
    }

    // have the range create the constraints and store those constraints
    Set* constraintSet = range->boundTupleExp(tuple_exp);

    // Same as Domain
    constraintSet->setTupleDecl( addedConstSet->getTupleDecl() );
    
    // Intersect the new set of constraints with the existing constraints.
    Set* mCC = addedConstSet;     // pointer to current set
    addedConstSet = mCC->Intersect(constraintSet);

    delete mCC;
    delete constraintSet;
    delete range;
   }

   delete uf_call;
}

/*! Adds constraints due to domain and range of all UFCalls in the Set.
**  Function returns the new Set with added constraints, leaving caller
**  unchanged. User owns the returned Set object.
*/
Set* Set::boundDomainRange()
{
    Set* s = new Set(*this);
    VisitorBoundDomainRange *v = new VisitorBoundDomainRange();
    
    s->acceptVisitor(v);

    delete v;

    return s;
}

/*! Adds constraints due to domain and range of all UFCalls in the Relation.
**  Function returns the new Relation with added constraints, leaving caller
**  unchanged. User owns the returned Relation object.
*/
Relation* Relation::boundDomainRange()
{
    Relation* r = new Relation(*this);
    VisitorBoundDomainRange *v = new VisitorBoundDomainRange();
    
    r->acceptVisitor(v);

    delete v;

    return r;
}


/*****************************************************************************/
#pragma mark -
/*************** VisitorSuperAffineSet *****************************/
/*! Vistor Class used in SuperAffineSet
**  Used in traversing a Set/Relation to replace UFCs with symbolic constants
**  We will build up a affineSet (or affineRelation), term by term.
**  And if a term is UFCall, we will convert it to symbolic constant,
**  which is pre-computed and stored in the ufcmap
*/
class VisitorSuperAffineSet : public Visitor {
  private:
         UFCallMap* ufcmap;
         Set* affineSet;
         Relation* affineRelation;
         Conjunction* affineConj;
         std::list<Conjunction*> maffineConj;
         Exp* affineExp;
         int ufcDepth;     // Helps us to recognize nested UFCs,
                           // since we do not want to iterate inside UFC arguments
  public:
         VisitorSuperAffineSet(UFCallMap* imap){ufcmap = imap;}
         // We do not change any type of Term except for UFCallTerm
         void preVisitTerm(Term * t) {
             if( ufcDepth == 0 ){ affineExp->addTerm( t->clone() ); }
         }
         /*! We iterate over terms in Exp, if the term is not a UFCall
         **  then we just add it to our affine set.
         **  On the other hand, We need to turn UFCalls into
         **  symbolic constants to make an affine set.
         */
         void preVisitUFCallTerm(UFCallTerm * t){
             if( ufcDepth == 0 ){
                 VarTerm* vt = ufcmap->find( t );
                 vt->setCoefficient(t->coefficient());
                 affineExp->addTerm( vt );
             }
         }
         void preVisitTupleVarTerm(TupleVarTerm * t){
             if( ufcDepth == 0 ){ affineExp->addTerm( t->clone() ); }
         }
         void preVisitVarTerm(VarTerm * t){
             if( ufcDepth == 0 ){ affineExp->addTerm( t->clone() ); }
         }
         void preVisitTupleExpTerm(TupleExpTerm * t){ 
             if( ufcDepth == 0 ){ affineExp->addTerm( t->clone() ); }
         }
         /*! Intialize an affineExp if Exp is not a UFCall argument
         ** If this is a argument to a UFCall, we don't want to modify it.
         ** This is because A(B(i+1)) gets replaced with A_B_iP1__ without 
         ** doing anything about B(i+1). And keep in mind that we have already
         ** added constraints related to function B's domain and range.
         */
         void preVisitExp(iegenlib::Exp * e){
             if( e->isExpression() ){
                 ufcDepth++; // We are iterating over an UFC's arguments
             }else{
                 ufcDepth = 0;
                 affineExp = new Exp();  
             }
         }
         /*! We don't care about e's that are argument to an UFCall
         **  Nonetheless, we should set visit = true, because of the nested
         **  expressions: we are care about the whole expression i = row(j+1)
         **  but not the sub-expression (j+1) that an is argument to row.
         */ 
         void postVisitExp(iegenlib::Exp * e){
             // we are finished iterating arguments of an UFC
             if( e->isExpression() ){ 
                 ufcDepth--;   // Consider the fact that UFCs can be nested
                 return;
             }
             Exp* CaffineExp = affineExp->clone();
             if( e->isInequality() ){
                 affineConj->addInequality( CaffineExp );
             }else{
                 affineConj->addEquality( CaffineExp );
             }
             delete affineExp;
         }
         //! Initializes an affineConj
         void preVisitConjunction(iegenlib::Conjunction * c){
             affineConj = new Conjunction( c->getTupleDecl() );
             affineConj->setInArity( c->inarity() );
         }
         //! adds the current affineConj to maffineConj
         void postVisitConjunction(iegenlib::Conjunction * c){
             maffineConj.push_back(affineConj->clone());
             delete affineConj;
         }
         //! Add Conjunctions in maffineConj to affineSet
         void postVisitSet(iegenlib::Set * s){
             affineSet = new Set( s->arity() );

             for(std::list<Conjunction*>::const_iterator i=maffineConj.begin();
                             i != maffineConj.end(); i++) {
                 affineSet->addConjunction((*i));
             }
         }
         //! Add Conjunctions in maffineConj to affineRelation
         void postVisitRelation(iegenlib::Relation * r){
             affineRelation = new Relation( r->inArity() , r->outArity() );

             for(std::list<Conjunction*>::const_iterator i=maffineConj.begin();
                              i != maffineConj.end(); i++) {
                 affineRelation->addConjunction((*i));
             }
         }

         Set* getSet(){ return affineSet; }
         Relation* getRelation(){ return affineRelation; }
};

/*! Creates a super affine set from a non-affine set.
**  To do this:
**    (1) We add constraints due to all UFCalls' domain and range
**    (2) We replace all UFCalls with symbolic constants found in the ufc map.
**  The function does not own the ufcmap.
*/
Set* Set::superAffineSet(UFCallMap* ufcmap, bool boundUFCs)
{
    Set *copySet, *result;

    result = new Set (*this);
    // There is no conjunction, so nothing to do
    if( result->getNumConjuncts() == 0 ){  
       return result;   // Just return a copy of the Set
    }
    delete result;

    if( boundUFCs ) copySet = this->boundDomainRange();
    else          copySet = new Set (*this);

    VisitorSuperAffineSet* v = new VisitorSuperAffineSet(ufcmap);
    copySet->acceptVisitor( v );
    
    result = (v->getSet());
    
    delete copySet;
    delete v;

    return result;
}

//! Same as Set
Relation* Relation::superAffineRelation(UFCallMap* ufcmap, bool boundUFCs)
{
    Relation *copyRelation, *result;

    if( !getNumConjuncts() ){  // There is no conjunction, so nothing to do
       result = new Relation (*this);
       return result;   // Just return a copy of the Set
    }

    if( boundUFCs ) copyRelation = this->boundDomainRange();
    else          copyRelation = new Relation (*this);

    VisitorSuperAffineSet* v = new VisitorSuperAffineSet(ufcmap);
    copyRelation->acceptVisitor( v );

    result = (v->getRelation());

    delete copyRelation;
    delete v;

    return result;
}

Relation* Relation::superAffineRelation()
{
      // Create variable names for UF calls.
      iegenlib::UFCallMap* uf_call_map = new UFCallMap();
    return superAffineRelation(uf_call_map);
}

/*****************************************************************************/
#pragma mark -
/*************** VisitorReverseAffineSubstitution *****************************/
/*! Vistor Class used in SuperAffineSet
**  Used in traversing a Set/Relation to replace UFCs with symbolic constants
**  We will build up a nonAffineSet (or nonAffineRelation), term by term.
**  And if a term is UFCall, we will convert it to symbolic constant,
**  which is pre-computed and stored in the ufcmap
*/
class VisitorReverseAffineSubstitution : public Visitor {
  private:
         UFCallMap* ufcmap;
         Set* nonAffineSet;
         Relation* nonAffineRelation;
         Conjunction* nonAffineConj;
         std::list<Conjunction*> nonMAffineConj;
         Exp* nonAffineExp;
         bool visit;       // helps to know which Exp is UFC argument
  public:
         VisitorReverseAffineSubstitution(UFCallMap* imap){ufcmap = imap;}

         /*! We iterate over terms in Exp, if the term is not a VarTerm
         **  then we just add it to our non-affine set. On the other hand,
         **  if a symbolic constants is in our ufcmap, we need to turn it
         **  into corresponding UFCalls to make an non-affine set.
         */
         void preVisitTerm(Term * t) {
             if(visit){ nonAffineExp->addTerm( t->clone() ); }
         }
         //! Affine seta cannot have UFC terms!
         void preVisitUFCallTerm(UFCallTerm * t){
             throw assert_exception("VisitorReverseAffineSubstitution: \
                             An UFCall term found in affine set!");
         } 
         void preVisitTupleVarTerm(TupleVarTerm * t){
             if(visit){ nonAffineExp->addTerm( t->clone() ); }
         }
         void preVisitVarTerm(VarTerm * t){
             if(visit){
                 UFCallTerm* ufc = (UFCallTerm*)(ufcmap->find( t ));
                 if(!ufc){
                     nonAffineExp->addTerm( t->clone() );
                 } else {
                     ufc->setCoefficient(t->coefficient());
                     nonAffineExp->addTerm( ufc );
                 }
             }
         }
         void preVisitTupleExpTerm(TupleExpTerm * t){ 
             if(visit){ nonAffineExp->addTerm( t->clone() ); }
         }
         /*! Intialize an nonAffineExp if Exp is not a UFCall argument
         **  See VisitorsuperAffineSet::preVisitExp for more details
         */
         void preVisitExp(iegenlib::Exp * e){
             if( e->isExpression() ){
                 visit = false;
             } else {
                 visit = true;
                 nonAffineExp = new Exp();  
             }
         }
         /*! There cannot be argument to UFCalls in affine set, since there
         **  cannot be UFCalls in affine set. preVisitUFCallTerm for details 
         */ 
         void postVisitExp(iegenlib::Exp * e){
             if( e->isExpression() ){
                 // we need visit = true for nested expressions
                 visit = true;
                 return;
             }
             Exp* CaffineExp = nonAffineExp->clone();
             if( e->isInequality() ){
                 nonAffineConj->addInequality( CaffineExp );
             } else {
                 nonAffineConj->addEquality( CaffineExp );
             }
             delete nonAffineExp;
         }
         //! Initializes an nonAffineConj
         void preVisitConjunction(iegenlib::Conjunction * c){
             nonAffineConj = new Conjunction( c->getTupleDecl() );
             nonAffineConj->setInArity( c->inarity() );
         }
         //! adds the current nonAffineConj to nonMAffineConj
         void postVisitConjunction(iegenlib::Conjunction * c){
             nonMAffineConj.push_back(nonAffineConj->clone());
             delete nonAffineConj;
         }
         //! Add Conjunctions in nonMAffineConj to nonAffineSet
         void postVisitSet(iegenlib::Set * s){
             nonAffineSet = new Set( s->arity() );

             for (std::list<Conjunction*>::const_iterator i =
                  nonMAffineConj.begin(); i != nonMAffineConj.end(); i++){
                 nonAffineSet->addConjunction((*i));
             }
         }
         //! Add Conjunctions in nonMAffineConj to nonAffineRelation
         void postVisitRelation(iegenlib::Relation * r){
             nonAffineRelation = new Relation( r->inArity() , r->outArity() );
             for (std::list<Conjunction*>::const_iterator i =
                  nonMAffineConj.begin(); i != nonMAffineConj.end(); i++){
                 nonAffineRelation->addConjunction((*i));
             }
         }
         Set* getSet(){ return nonAffineSet; }
         Relation* getRelation(){ return nonAffineRelation; }
};

/*! Creates a super affine set from a non-affine set.
**  To do this:
**    (1) We add constraints due to all UFCalls' domain and range
**    (2) We replace all UFCalls with symbolic constants found in the ufc map.
**  The function does not own the ufcmap.
*/
Set* Set::reverseAffineSubstitution(UFCallMap* ufcmap)
{
    VisitorReverseAffineSubstitution* v = 
                  new VisitorReverseAffineSubstitution(ufcmap);
    this->acceptVisitor( v );
    
    Set* result = (v->getSet());
    delete v;

    return result;
}

//! Same as Set
Relation* Relation::reverseAffineSubstitution(UFCallMap* ufcmap)
{
    VisitorReverseAffineSubstitution* v = 
                  new VisitorReverseAffineSubstitution(ufcmap);
    this->acceptVisitor( v );

    Relation* result = (v->getRelation());
    delete v;

    return result;
}

/*****************************************************************************/
#pragma mark -
/*************** VisitorProjectOut *****************************/
//! Vistor Classes used in projection process

/*! The main class for projecting out tuple variables from affine Set/Relation
**  We use ISL library to project out tvar from each conjunction
*/
class VisitorProjectOut : public Visitor {
  private:
    int tvar;
    int inArity;
    Set* newSet;
    Relation* newRelation;
    std::list<Conjunction*> mNewConj;
  public:
    VisitorProjectOut(int itvar, int ia=0){
        tvar = itvar;
        // Adjust inArity for after projection
        if( tvar < ia && ia != 0 ){
            ia--;
        } 
        inArity = ia;
    }
    // Projects out tuple varrable No. tvar from current conjunction
    // And adds it to mNewConj
    void postVisitConjunction(iegenlib::Conjunction* c){
        Conjunction* cc = new Conjunction( *c );
        
        cc->setInArity(0);
        Set * cs = new Set(cc->arity() );
        cs->addConjunction(cc);

        // Send through ISL to project out desired tuple variable
        Set* islSet = islSetProjectOut(cs, tvar);

        if( !(islSet->mConjunctions.empty()) ){
            Conjunction* crc = new Conjunction ( *(islSet->mConjunctions.front()) );
            crc->setInArity( inArity );

            // Storing the result so we could add it to new Set\Relation later
            mNewConj.push_back( crc );
        }

        delete islSet;
        delete cs;
    }
    //! Add Conjunctions in mnewConj to newSet
    void postVisitSet(iegenlib::Set * s){
        newSet = new Set( (s->arity()-1) );

        for(std::list<Conjunction*>::const_iterator i=mNewConj.begin();
                      i != mNewConj.end(); i++) {
            newSet->addConjunction((*i));
        }
    }
    //! Add Conjunctions in mnewConj to newRelation
    void postVisitRelation(iegenlib::Relation * r){
        // Adjusting new in and out arity depending projected tvar 
        int ia = r->inArity(), oa = r->outArity();
        if( tvar < ia ){
            ia--;
        } else {
            oa--;
        }
        newRelation = new Relation( ia , oa );

        for(std::list<Conjunction*>::const_iterator i=mNewConj.begin();
                     i != mNewConj.end(); i++) {

            newRelation->addConjunction((*i));
        }
    }

    Set* getSet(){ return newSet; }
    Relation* getRelation(){ return newRelation; }
};

/*! This class is used in adjusting tuple variable indices for those
**  constraints that involved in UFCallTerms, after projection.
*/
class VisitorProjectOutCleanUp : public Visitor {
  private:
    int tvar;
    int nc_ufc;     // nested UFCallTerm count

  public:
    VisitorProjectOutCleanUp(int itvar){ tvar = itvar;  nc_ufc = 0;}
    void preVisitUFCallTerm(iegenlib::UFCallTerm * t){
        nc_ufc++;
    }
    void postVisitUFCallTerm(iegenlib::UFCallTerm * t){
        nc_ufc--;
    }
    void preVisitTupleVarTerm(iegenlib::TupleVarTerm * t){
        if( nc_ufc && t->tvloc() > tvar){
            TupleVarTerm newT( t->coefficient() , t->tvloc()-1 );
            *t = newT;
        }
    }
};

/*! Projects out tuple varrable No. tvar if tvar is not argument to any UFCall
**  tvar is calculated based on ariety starting from 0.
**  Consequently, to project out jp from S: tvar = 5
**     S = {[i,j,k,ip,jp,kp] : ...}
**
**  NOTE: if tvar is argument some UFCall, then we cannot project it out
**        and functions returns NULL.
**        Ex: if col(k) in constraints exists we cannot project out 'k'
*/
Set* Set::projectOut(int tvar)
{
    if (isUFCallParam(tvar)){
      return NULL;
    }
    
    // Geting a map of UFCalls 
    iegenlib::UFCallMap *ufcmap = new UFCallMap();;
    // Getting the super affine set of constraints with no UFCallTerms
    Set* sup_s = this->superAffineSet(ufcmap);

    // Projecting out tvar using ISL library
    VisitorProjectOut* pv = new VisitorProjectOut(tvar, 0);
    sup_s->acceptVisitor(pv);
    delete sup_s;
    sup_s = pv->getSet();

    // Getting the reverseAffineSubstitution
    Set* result = sup_s->reverseAffineSubstitution(ufcmap);

    // Adjusting changes in UFCTerms due to projection: _tvN -> _tvN-1
    VisitorProjectOutCleanUp* cv = new VisitorProjectOutCleanUp(tvar);
    result->acceptVisitor(cv);

    delete sup_s;
    delete cv;
    delete pv;
   
    return result;
}

/*! Projects out tuple varrable No. tvar if tvar is not argument to any UFCall
**  tvar is calculated based on total ariety (in+out) starting from 0.
**  Consequently, to project out jp from R: tvar = 5
**     R = {[i,j,k] -> [ip,jp,kp] : ...}
**
**  NOTE: if tvar is argument some UFCall, then we cannot project it out
**        and functions returns NULL.
**        Ex: if col(k) in constraints exists we cannot project out 'k'
*/
Relation* Relation::projectOut(int tvar)
{
    if (isUFCallParam(tvar)){
      return NULL;
    }
    
    // Geting a map of UFCalls 
    iegenlib::UFCallMap *ufcmap = new UFCallMap();
    // Getting the super affine set of constraints with no UFCallTerms
    Relation* sup_r = this->superAffineRelation(ufcmap);

    // Projecting out tvar using ISL library
    VisitorProjectOut* pv = new VisitorProjectOut(tvar, sup_r->inArity());
    sup_r->acceptVisitor(pv);
    delete sup_r;
    sup_r = pv->getRelation();

    // Getting the reverseAffineSubstitution
    Relation* result = sup_r->reverseAffineSubstitution(ufcmap);

    // Adjusting changes in UFCTerms due to projection: _tvN -> _tvN-1
    VisitorProjectOutCleanUp* cv = new VisitorProjectOutCleanUp(tvar);
    result->acceptVisitor(cv);

    delete sup_r;
    delete cv;
    delete pv;
   
    return result;
}

/*****************************************************************************/
#pragma mark -
/*************** VisitorNumUFCallConstsMustRemove *****************************/
/*! Vistor Class used in numUFCallConstsMustRemove
** 
*/
class VisitorNumUFCallConstsMustRemove : public Visitor {
  private:
    int mTupleID;
    std::set<Exp> ignore;
    bool seenTupleVar;
    int UFCLevel;
    int count;

  public:
    VisitorNumUFCallConstsMustRemove(int tupleID, std::set<Exp>& iignore)
                  : mTupleID(tupleID), ignore(iignore),   
                  seenTupleVar(false), UFCLevel(0), count(0){}

    virtual ~VisitorNumUFCallConstsMustRemove(){ }

    int getCount() { return count;}

    void preVisitTupleVarTerm(iegenlib::TupleVarTerm *t) {
        if ( UFCLevel > 0 && t->tvloc()== mTupleID) {
            seenTupleVar = true;
        }
    }
    void preVisitExp(iegenlib::Exp * e) {
        //
        if( e->isEquality() || e->isInequality() ){
             UFCLevel = 0;
             seenTupleVar = false;
        } else if( e->isExpression() ){
             UFCLevel++;
        }
    }
    void postVisitExp(iegenlib::Exp * e) {
        // 
        if( e->isExpression() ){
             UFCLevel--;
        } else if ( seenTupleVar) {
            if( ignore.find(*e) == ignore.end() ){
                count++;
            }
        }
    }
};

/*! This function considers tuple variable i; and counts the number of
**  constraints in the set where this tuple variable is argument to an UFC.
**  However, it excludes constraints that are in the ignore set.
**  Since, these constraints are related to domain/range of UFCs in the set.
*/
int SparseConstraints::numUFCallConstsMustRemove(int i, std::set<Exp>& ignore)
{
    VisitorNumUFCallConstsMustRemove *v = new VisitorNumUFCallConstsMustRemove(
                                               i, ignore);
    this->acceptVisitor(v);

    int count = v->getCount();
    delete v;
    
    return count;
}

/*****************************************************************************/
#pragma mark -
/*************** VisitornRemoveUFCallConsts *****************************/
/*! Vistor Class used in removeUFCallConsts
**
*/
class VisitorRemoveUFCallConsts : public Visitor {
  private:
    int mTupleID;
    bool seenTupleVar;
    int UFCLevel;
    SparseConstraints* redSparseConstraints;
    Conjunction* redConj;
    std::list<Conjunction*> mRedConj;

  public:
    VisitorRemoveUFCallConsts(int tupleID)
                  : mTupleID(tupleID), seenTupleVar(false), UFCLevel(0){}

    virtual ~VisitorRemoveUFCallConsts(){ }

    void preVisitTupleVarTerm(iegenlib::TupleVarTerm *t) {
        if ( UFCLevel > 0 && t->tvloc()== mTupleID) {
            seenTupleVar = true;
        }
    }
    void preVisitExp(iegenlib::Exp * e) {
        //
        if( e->isEquality() || e->isInequality() ){
             UFCLevel = 0;
             seenTupleVar = false;
        } else if( e->isExpression() ){
             UFCLevel++;
        }
    }
    void postVisitExp(iegenlib::Exp * e) {
        // 
        if( e->isExpression() ){
             UFCLevel--;
        } else if ( !seenTupleVar ) {
             if( e->isInequality() ){
                 redConj->addInequality( e->clone() );
             }else{
                 redConj->addEquality( e->clone() );
             }
        }
    }
    //! Initializes an redConj
    void preVisitConjunction(iegenlib::Conjunction * c){
        redConj = new Conjunction( c->getTupleDecl() );
        redConj->setInArity( c->inarity() );
    }
    //! adds the current redConj to mRedConj
    void postVisitConjunction(iegenlib::Conjunction * c){
         mRedConj.push_back(redConj->clone());
         delete redConj;
    }
    //! Add Conjunctions in mRedConj to redSparseConstraints
    void postVisitSparseConstraints(iegenlib::SparseConstraints * sc){
        redSparseConstraints = new SparseConstraints( );

        for(std::list<Conjunction*>::const_iterator i=mRedConj.begin();
                             i != mRedConj.end(); i++) {
            redSparseConstraints->addConjunction((*i));
        }
    }

    SparseConstraints* getSparseConstraints() { return redSparseConstraints; }
};

/*! This function removes any constraints where tuple variable i
**  is argument to an UFC. The function is inplace.
*/
void SparseConstraints::removeUFCallConsts(int i)
{
    VisitorRemoveUFCallConsts* v = new VisitorRemoveUFCallConsts(i);
    
    this->acceptVisitor(v);

    SparseConstraints* result = v->getSparseConstraints();

    *this = *result;

    delete result;
}

//! This function is implementation of a heuristic algorithm to remove
//  expensive contranits from the set.
void SparseConstraints::removeExpensiveConstraints(std::set<int> parallelTvs, 
                              int mNumConstsToRemove , std::set<Exp> ignore )
{
    int lastTV = this->arity()-1, nConstsToRemove = 0;

    for (int i = lastTV ; i >= 0 ; i-- ) {

        if ( parallelTvs.find(i) != parallelTvs.end() ){
            continue;
        }

        nConstsToRemove = this->numUFCallConstsMustRemove(i, ignore);

        if ( nConstsToRemove > mNumConstsToRemove ){
            continue;
        }else if( 0 <= nConstsToRemove && nConstsToRemove <= mNumConstsToRemove ){
            this->removeUFCallConsts( i );
            mNumConstsToRemove -= nConstsToRemove;
        }
    }
}

/*! This function simplifies constraints of non-affine sets that
**  are targeted for level set parallelism. These sets are representative
**  of data access dependency relations. For level set parallelism,
**  we need to create an optimized inspector code that checks 
**  data dependency based these constraints in run time. This function is
**  implementation of Simplification Algorithm that simplifies dependency
**  relations, so we can generate optimized inspector code from constraint sets.
*/
Set* Set::simplifyForPartialParallel(std::set<int> parallelTvs )
{
    Set *temp;
    int lastTV = this->arity()-1;
    Set* result = new Set(*this);

//  Commenting out these until we implement adding domain info based on
//  rule instantiation
//    Set* copySet = new Set(*this);
//    // Adding constraints dut to Monotonicity of UFCs (if any exists)
//    result = copySet->addConstraintsDueToMonotonicity();
//    delete copySet;

    result->normalize();

    // Projecting out any tuple variable that are not argument to a UFCall 
    // starting from inner most loops. We also do not project out indecies
    // specified in parallelTvs, since they are going to be parallelized.
    // [i1,i1p,i2, i2p, ...] :  we keep i1 and i1p

    for (int i = lastTV ; i >= 0 ; i-- ) {

        if ( parallelTvs.find(i) != parallelTvs.end() ){
            continue;
        }
    
        // Project out if it is not an UFCall argument
        temp = result->projectOut(i); 

        if ( temp ){
            delete result;  
            result = temp;
        }

        if( result->isDefault() ){

            return NULL;
        }
    }

    return result;    
}

/*! Same as Set.
*/
Relation* Relation::simplifyForPartialParallel(std::set<int> parallelTvs)
{
    Relation *temp;
    int lastTV = this->arity()-1;
    Relation* result = new Relation(*this);

//  Commenting out these until we implement adding domain info based on
//  rule instantiation
//    Relation* copyRelation = new Relation(*this);
//    // Adding constraints dut to Monotonicity of UFCs (if any exists)
//    result = this->addConstraintsDueToMonotonicity();
//    delete copyRelation;
    
    // Projecting out any tuple variable that are not argument to a UFCall 
    // starting from inner most loops. We also do not project out first 2 loops
    // that are outer most loops, since they are going to be parallelized.
    // [i1,i1p,i2, i2p, ...] :  we keep i1 and i1p
    for (int i = lastTV ; i >= 0 ; i-- ) {

        if ( parallelTvs.find(i) != parallelTvs.end() ){
            continue;
        }
    
        // Project out if it is not an UFCall argument
        temp = result->projectOut(i); 

        if ( temp ){
            delete result;        
            result = temp;
        }

        if( result->isDefault() ){
            return NULL;
        }
    }

    return result;    
}




/*****************************************************************************/
#pragma mark -
/*****************************************************************************/
/*! Vistor Class used in VisitorGatherAllTerms
**  The visitor class to gather all (unique) terms so they
**  can be used in rule instantiation.
*/
class VisitorGatherAllParameters : public Visitor {
  private:
    std::set<Exp> instExps; 

  public:
    VisitorGatherAllParameters(){}
    virtual ~VisitorGatherAllParameters(){}

    void preVisitUFCallTerm(UFCallTerm * t){
        instExps.insert( *(t->getParamExp(0)) );
    }

    std::set<Exp> getExps() { 
        return instExps;
    }
};

/* We get a rule looking like:
**   forall x1, x2: x1 <= x2 -> f(x1) <= g(x2)
** Note, left side (x1 <= x2), and right side (f(x1) <= g(x2))
** are stored as separate sets like: 
**  {[x1,x2]: x1 <= x2}
**  {[x1,x2]: f(x1) < g(x2)}
** We want to replace x1 and x2 with expressions like: i, col(j), ... 
** and returned instantiated rule, for instance:
   if we have 
     rule: forall x1, x2: x1 <= x2 -> f(x1) <= g(x2)
     x1 = i   and x2 = col(j)
   we want to return:
      < i <= col(j) , f(i) <= g(col(j)) >
*/
std::pair <std::string,std::string> instantiate(
          UniQuantRule* uqRule, Exp x1, Exp x2, 
          UFCallMap *ufcmap, TupleDecl origTupleDecl){
  
  Set *leftSideOfTheRule, *rightSideOfTheRule;

  // Create map for substituting uni. quant. vars. in a rule
  //  with expressions from our list. 
  SubMap subMap;
  TupleVarTerm *uQV = new TupleVarTerm( 0 );
  Exp *subExp = new Exp( x1 ); 
  subMap.insertPair( uQV , subExp );
  uQV = new TupleVarTerm( 1 );
  subExp = new Exp( x2 ); 
  subMap.insertPair( uQV , subExp );

  // Substitute universially quantified variables 
  // with terms from list of terms
  leftSideOfTheRule = new Set ( *(uqRule->getLeftSide()) );
  rightSideOfTheRule = new Set ( *(uqRule->getRightSide()) );
  leftSideOfTheRule->substituteInConstraints( subMap );
  rightSideOfTheRule->substituteInConstraints( subMap );

  // make rule's tuple declaration to match original constraints 
  leftSideOfTheRule->setTupleDecl(origTupleDecl); 
  rightSideOfTheRule->setTupleDecl(origTupleDecl);

  // create superAffine sets of left/right sides
  Set *supAffLeft, *supAffRight;
  supAffLeft  = leftSideOfTheRule->superAffineSet(ufcmap, false);
  supAffRight = rightSideOfTheRule->superAffineSet(ufcmap, false);
  delete leftSideOfTheRule;
  delete rightSideOfTheRule;

  // we only need the constraint part of left/right sides 
  // that are stored as separate sets.
  srParts subLeftSideParts, subRightSideParts;
  subLeftSideParts = getPartsFromStr(supAffLeft->prettyPrintString());
  subRightSideParts = getPartsFromStr(supAffRight->prettyPrintString());
  subLeftSideParts.constraints = trim(subLeftSideParts.constraints);
  subRightSideParts.constraints = trim(subRightSideParts.constraints);
  std::string leftStr = "false", rightStr = "false";
  if(subLeftSideParts.constraints == ""){  leftStr = "true"; } 
  else if(subLeftSideParts.constraints == "FALSE"){  leftStr = "false";}
  else { leftStr = subLeftSideParts.constraints; }
  if(subRightSideParts.constraints == ""){ rightStr = "true"; }
  else if(subRightSideParts.constraints == "FALSE"){ rightStr = "false";}
  else { rightStr = subRightSideParts.constraints; }

  return (std::make_pair( leftStr, rightStr));
}


// Builds list of symbolic constants from original constraints,
// super affine set, and instantiations, so we can use it for isl input. 
//  drOrigSet = original set with domain/range constraints
//  ufcmap    = list of UFCs from super super affine set, and instantiations
string symsForInstantiationSet(Set *drOrigSet, UFCallMap *ufcmap ){
  std::stringstream ss;
  StringIterator * symIter;
  bool foundSymbols = false;
  symIter = (*(drOrigSet->mConjunctions.begin()))->getSymbolIterator();
  while (symIter->hasNext()) {
    if (foundSymbols == false) {
      foundSymbols = true;
      ss << symIter->next();
    } else { ss << ", " << symIter->next(); }
  }
  delete drOrigSet;
  std::string syms = "";
  if( !((ss.str()).empty()) && !((ufcmap->varTermStrList()).empty()) ){
    syms = "[" + ss.str() + ", " 
                           + ufcmap->varTermStrList() + "] -> ";    
  } else if ( (ufcmap->varTermStrList()).empty() ) {
    syms = "[" + ss.str() + "] -> ";   
  } else if ( (ss.str()).empty() ){
    syms = "[" + ufcmap->varTermStrList() + "] -> "; 
  }

  return syms;
}

// This function creates a isl set from instantiations. The naive way to 
// do this would be to put all the instantiation inside one Set 
// and feed it isl. However, that would create a performance bottleneck,
// since isl has to try to colasce and simplify 
// lots of disjunctions at the same time. Instead of the naive way 
// this function iteratively adds the useful instantiation.
isl_set* instantiationSet( srParts supSetParts, 
           std::set<std::pair <std::string,std::string>> instantiations,
                               string syms , isl_ctx* ctx){

  // Build original Set with all symbolic constants from instantiations
  string origRel = syms + "{" + supSetParts.tupDecl + " : " + 
                   supSetParts.constraints + "}";

  // Iteratively add useful instantiation utilizing isl functions
  // We want to iterate until there is nothing useful to add.
  // This condition is checked at the end of the loop. For most examples
  // visiting everything 2 times must be enough. However, since
  // there might be some unusual example, we put a cap (up to 2 times), so
  // we would never end up looping many times even in rare occasions. 
  isl_set* set = isl_set_read_from_str(ctx, origRel.c_str() );
  isl_set* old_set = isl_set_copy(set);
  for (int i = 0; i < 2; i++) {
    for (std::set<std::pair <std::string,std::string>>::iterator 
          it=instantiations.begin(); it!=instantiations.end(); it++){ 
      string antecedentStr = syms + "{" + supSetParts.tupDecl + 
                             " : " + (*it).first + "}";
      string consequentStr = syms + "{" + supSetParts.tupDecl + 
                             " : " + (*it).second + "}";
      // If antecedent is true add the consequent of the instantiation
      int added = 0;
      {
        isl_set* ant_set = isl_set_read_from_str(ctx, antecedentStr.c_str());
        ant_set = isl_set_gist(ant_set, isl_set_copy(set));
        if (isl_set_plain_is_universe(ant_set)) {
          isl_set* con_set = isl_set_read_from_str(ctx, consequentStr.c_str());
          set = isl_set_intersect(set, con_set);
          set = isl_set_coalesce(set);
          added = 1;
        }
        isl_set_free(ant_set);
      }
      // If complement of consequent is true add the complement of antecedent 
      if (!added) {
        isl_set* con_set = isl_set_read_from_str(ctx, consequentStr.c_str());
        con_set = isl_set_complement(con_set);
        con_set = isl_set_gist(con_set, isl_set_copy(set));
        if (isl_set_plain_is_universe(con_set)) {
          isl_set* ant_set = isl_set_read_from_str(ctx, antecedentStr.c_str());
          ant_set = isl_set_complement(ant_set);
          set = isl_set_intersect(set, ant_set);
          set = isl_set_coalesce(set);
          added = 1;
        }
        isl_set_free(con_set);
      }
    }
    if( isl_set_is_empty(set) ) break;             // Set is UnSat
    if ( isl_set_is_equal( old_set, set ) ){ break; // We have converged
    } else {
      isl_set_free(old_set);
      old_set = isl_set_copy(set);
    }
  }

  return set;
}


/*! Vistor Class for finding only potential useful equalities from set of equalities
  Potential useful  equalities must directly define at least one tuple variable 
  in terms of other terms, i.e. tv_i = ...; something like following is not 
  a potential   useful equality: f(tv_i) = g(tv_j), because both tv_i and tv_j are 
  parameters to uninterpreted function calls. 
*/
class VisitorGetUsefulEqs : public Visitor {
  private:
         bool foundTvVar;
         int UFnestLevel;
         Conjunction* uEqsC;
         Set* uEqsS;
  public:
         VisitorGetUsefulEqs(){UFnestLevel=0;foundTvVar=false;}
         //! 
         void preVisitUFCallTerm(UFCallTerm* t){
           UFnestLevel++;
         }
         //! 
         void postVisitUFCallTerm(UFCallTerm* t){
           UFnestLevel--;
         }
         //!
         void preVisitTupleVarTerm(TupleVarTerm* t){
           if( UFnestLevel == 0) foundTvVar=true;
         }
         //!
         void preVisitExp(iegenlib::Exp* e){
           if( UFnestLevel == 0) foundTvVar=false;
         }
         //!
         void postVisitExp(iegenlib::Exp* e){
           if( UFnestLevel == 0 && foundTvVar ){
             uEqsC->addEquality(e->clone());
           }
         }
         //! 
         void preVisitConjunction(Conjunction* c){
           UFnestLevel=0;
           TupleDecl td = c->getTupleDecl();
           uEqsC = new Conjunction(td);
         }
         //! 
         void postVisitConjunction(Conjunction* c){
           TupleDecl td = c->getTupleDecl();
           uEqsS = new Set(td);
           uEqsS->addConjunction(uEqsC);
         }
         Set* getUsefulEqs(){ return uEqsS; }
};


// Check to see if the isl set is empty (the original Set is UnSat)
// or, it is not (the original Set is MaySat) in which case extract
// new equalities and add them to original Set
Set* checkIslSet(isl_set* set, isl_ctx* ctx, 
                      UFCallMap *ufcmap, Set *origSet ){
  Set *result = NULL;
  if( isl_set_is_empty(set) ){
    result = NULL;
  } else {
    isl_basic_set *bset = isl_set_affine_hull(set);
    // Get an isl printer and associate to an isl context
    isl_printer * ip = isl_printer_to_str(ctx);
    // get string back from ISL set
    isl_printer_set_output_format(ip , ISL_FORMAT_ISL);
    isl_printer_print_basic_set(ip ,bset);
    char *i_str = isl_printer_get_str(ip);
    // clean-up
    isl_printer_flush(ip);
    isl_printer_free(ip);
    isl_basic_set_free(bset);
    // Puting the newly found equalities into original constraint set.
    Set* affineEqs = new Set(i_str);
    Set* eQs = affineEqs->reverseAffineSubstitution(ufcmap);

    // Only keeping equalities that can potentially be useful. 
    VisitorGetUsefulEqs *v = new VisitorGetUsefulEqs();
    eQs->acceptVisitor( v );

    Set * uEqs = v->getUsefulEqs();
    result = origSet->Intersect(uEqs);

//    result = v->getUsefulEqs();

    delete affineEqs;
    delete eQs;
    delete uEqs;
    //free(i_str);
  }
  return result;
}


/*!
** This function takes an expression set, and instantiates 
** quantified rules stored in the environment using them.
** Although, only the rules that have their type set in 
** the useRule argument are instantiated.
** An instantiation is of the form: p1 -> q1, 
** the output includes set of tuples like (p1,q1).
*/
std::set<std::pair <std::string,std::string>> ruleInstantiation
                          (std::set<Exp> instExps, bool *useRule, 
                           TupleDecl origTupleDecl, UFCallMap *ufcmap){
  int noAvalRules = queryNoUniQuantRules();
  UniQuantRule* uqRule;
  std::set<std::pair <std::string,std::string>> instantiations;
  // If no rules are explicitly specified, we instantiate all of them  
  if (!useRule){
    useRule = new bool[ TheOthers ];
    for(int i = 0 ; i < TheOthers ; i++ ){ useRule[i] = 1; } 
  }
  // Instantiating the universially quantified rules available 
  // in the environment one by one, for the ones we want to check
  for(int i = 0 ; i < noAvalRules ; i++ ){

    // Query rule No. i from environment
    uqRule = queryUniQuantRuleEnv(i);
    // If we do not want to instantiate this rule move on to next one
    if( !(useRule[uqRule->getType()]) ) continue;
    // Go over our Expression Set (E), and replace uni. quant. vars.
    // in the rule with these expressions.
    for (std::set<Exp>::iterator it1=instExps.begin(); 
             it1!=instExps.end(); it1++){
      for (std::set<Exp>::iterator it2=instExps.begin(); 
               it2!=instExps.end(); it2++){
        instantiations.insert(instantiate(uqRule,*it1,*it2,
                              ufcmap,origTupleDecl));
      }
    }
  }

  return instantiations;
}


// Detect UnSat or MaySat for the set utilizing domain information 
// that are stored as universally quantified rules in the environment.
// To utilize the domain information, the function first gathers all 
// the parameter expression to all UFCs in the relation, then it uses 
// those expression to instantiate the quantified rules. Next, it creates 
// a isl map out of original relation and the instantiate rules. 
// Finally, it checks to see whither the isl map is empty or not, 
// in case it is not empty it extract the newly found equalities, 
// adds them the original relation and returns the result.
Set* Set::detectUnsatOrFindEqualities(bool *useRule){

  // Gather all UFCall Parameters for Expression Set (E) for rule instantiation
  VisitorGatherAllParameters *vGE = new VisitorGatherAllParameters;
  this->acceptVisitor(vGE);
  std::set<Exp> instExps = vGE->getExps();
  // Generate all instantiations of universialy quantified rules
  TupleDecl origTupleDecl = getTupleDecl();
  std::set<std::pair <std::string,std::string>> instantiations;
  UFCallMap *ufcmap = new UFCallMap();
  instantiations = ruleInstantiation(instExps, useRule, origTupleDecl, ufcmap);
  // Use ISL to add useful instantiations, refer to instantiationSet
  Set *supAffSet = superAffineSet(ufcmap);
  srParts supSetParts = getPartsFromStr(supAffSet->prettyPrintString());
  isl_ctx* ctx = isl_ctx_alloc();
  string syms = symsForInstantiationSet(boundDomainRange(), ufcmap);
  isl_set* set = instantiationSet(supSetParts, instantiations, syms, ctx);
  Set *result = checkIslSet(set, ctx, ufcmap, this);
  //isl_ctx_free(ctx);

  return result;
}


// Same as Set
Relation* Relation::detectUnsatOrFindEqualities(bool *useRule){

  // Gather all UFCall Parameters for Expression Set (E) for rule instantiation
  VisitorGatherAllParameters *vGE = new VisitorGatherAllParameters;
  this->acceptVisitor(vGE);
  std::set<Exp> instExps = vGE->getExps();

  // Generate all instantiations of universialy quantified rules
  TupleDecl origTupleDecl = getTupleDecl();
  std::set<std::pair <std::string,std::string>> instantiations;
  UFCallMap *ufcmap = new UFCallMap();
  instantiations = ruleInstantiation(instExps, useRule, origTupleDecl, ufcmap);

  // Here, we are going to utlize same functions that as Set class.
  // Set::detectUnsatOrFindEqualities uses. Therefore, we temporary
  // turn the Relation into a Set by simply changing its tuple declaration
  // using relationStr2SetStr function found in isl_str_manipulation.
  Set *eqSet = new Set( relationStr2SetStr(prettyPrintString(), 
                                  inArity(), outArity()) );
  Set *supAffSet = eqSet->superAffineSet(ufcmap);
  srParts supSetParts = getPartsFromStr(supAffSet->prettyPrintString());
  isl_ctx* ctx = isl_ctx_alloc();
  string syms = symsForInstantiationSet(eqSet->boundDomainRange(), ufcmap);
  isl_set* set = instantiationSet(supSetParts, instantiations, syms, ctx);

  // Check if the relation with new information is UnSat or MaySat
  Set *resultSet = checkIslSet(set, ctx, ufcmap, eqSet);
  //isl_ctx_free(ctx);
  Relation *result = NULL;
  // Turning results back into a Relation
  if( resultSet ){
    result = new Relation( setStr2RelationStr(resultSet->prettyPrintString(),
                                  inArity(), outArity()) );
  }
  return result;
}



/*****************************************************************************/
#pragma mark -
/*************** VisitorGetString *****************************/
/*! Vistor Class used in getString
**  to getting a string representation of a Set or Relation
*/
class VisitorGetString : public Visitor {
  private:
         string str;
         TupleDecl aTupleDecl;
         int aritySplit;
         bool firstConj;
         bool firstExp;
  public:
         VisitorGetString(){ str = ""; firstConj = firstExp = true;}

         /*! We build our string one expression at a time before visiting 
         **  each expression. Note, we cannot build the expression at term
         **  granularity, because at that level would not know which term 
         **  to put at which side of an inequality.
         */
         void preVisitExp(iegenlib::Exp * e){
             // ignoring parameters of an UFC
             if( e->isExpression() ){
                 return;
             }
             int constT = 0;
             bool haveConstT = false;
             std::list<Term*> terms, leftSide, rightSide;

             // Check if we are vising first expression to know whether we need &&
             if(firstExp) firstExp = false;
             else         str += string(" && ");

             // Get list of terms and separate them into 2 lists, 
             // the ones for the left side and the ones for right side 
             terms = e->getTermList();
             for (std::list<Term*>::const_iterator i=terms.begin(); 
                  i != terms.end(); ++i) {
                 if( (*i)->isConst() ){
                     haveConstT = true;
                     constT = (*i)->coefficient();
                 }
                 else if ((*i)->coefficient() < 0) {
                     leftSide.push_front(*i);
                 } else {
                     rightSide.push_front(*i);
                 }
             }

             bool absValue = true;
             char buffer[20]="";
             int tempT = 0;

             // print the terms in the left of the inequality ( < or <= )
             for (std::list<Term*>::const_iterator i=leftSide.begin(); 
                  i != leftSide.end(); ++i) {
                 str += (*i)->prettyPrintString(aTupleDecl, absValue);
             }
             // If there is no terms in the left, print 0
             if( leftSide.empty() && !haveConstT ) str += string("0");
     
             if( e->isEquality() ){
                 if( haveConstT && constT  < 0 ){
                     tempT = constT*(-1);
                     sprintf(buffer, "%d", tempT); 
                     if( leftSide.empty() ) str += string(buffer);
                     else str += string(" + ") + string(buffer);
                 }
                 str += string(" = ");
             } 
             // Notice we check if we need < or <=
             else if( e->isInequality()){
                 if( haveConstT && constT < 0 ){
                     tempT = constT*(-1);
                     tempT--;
                     if( tempT != 0 ){
                         sprintf(buffer, "%d", tempT); 
                         if( leftSide.empty() ) str += string(buffer);
                         else str += string(" + ") + string(buffer);
                     } else if( tempT == 0 && leftSide.empty() ){
                         str += string("0");
                     }
                     str += string(" < ");
                 } else if( haveConstT && constT > 0 && leftSide.empty() ){
                     tempT = constT*(-1);
                     tempT--;
                     sprintf(buffer, "%d", tempT); 
                     str += string(buffer);

                     str += string(" < ");
                 } else { //if( !haveConstT || !(leftSide.empty()) ){
                     str += string(" <= ");
                 }
             }

             for (std::list<Term*>::const_iterator i=rightSide.begin(); 
                  i != rightSide.end(); ++i) {
                 str += (*i)->prettyPrintString(aTupleDecl, absValue);
             }
             if( rightSide.empty() && !haveConstT ) str += string("0");

             if( e->isEquality() && haveConstT && constT > 0 ){
                 sprintf(buffer, "%d", constT); 
                 if( leftSide.empty() ) str += string(buffer);
                 else str += string(" + ") + string(buffer);
             } else if( e->isInequality() && haveConstT && 
                        constT > 0 && !(leftSide.empty()) ){
                 tempT = constT;
                 sprintf(buffer, "%d", tempT);
                 if( rightSide.empty() ) str += string(buffer);
                 else str += string(" + ") + string(buffer);
             }
         }
         //! 
         void preVisitConjunction(iegenlib::Conjunction * c){
             aTupleDecl = c->getTupleDecl();

             // If this is not the first conjunction print &&
             if(firstConj) firstConj = false;
             else          str += string(" union ");

             // Start the conjunction's string with tuple
             str += string("{ ") + 
                    aTupleDecl.toString(true,aritySplit) + string(" : ");

             firstExp = true;
         }
         //! 
         void postVisitConjunction(iegenlib::Conjunction * c){
             str += string(" }");
         }

         //! Indicate, we are traversing a Set
         void preVisitSet(iegenlib::Set * s){
             aritySplit = 0;

             // If there are no conjunctions then indicate we have an empty set
             // by printing out generic arity tuple declarations and FALSE as 
             // a constraint.
             if (s->getNumConjuncts()==0) {
                 str = string("{ ") + 
                       TupleDecl::sDefaultTupleDecl(s->arity()).toString(true) +
                       string(" : FALSE }");
             }
         }
         //! Indicate, we are traversing a Relation
         void preVisitRelation(iegenlib::Relation * r){
             aritySplit = r->inArity();

             // If there are no conjunctions then indicate we have an empty relation
             // by printing out generic arity tuple declarations and FALSE as 
             // a constraint.
             if (r->getNumConjuncts()==0) {
                 str = string("{ ") + 
                       TupleDecl::sDefaultTupleDecl(r->arity()).toString(true,aritySplit) +
                       string(" : FALSE }");
             }
         }

         string getString(){ return str; }
};

/*! 
**  This function generates a string representation of the Set.
**  There are two differences between this function and other string
**  genrators, like toString, and prettyPrintSring:
**  (1) it uses visitor patter
**  (2) The generated string is better formatted, for instance while other functions 
        generate something like following for some relation: 
           {[i,j] : i - j = 0 && i - 6 >= 0 && -j + 2 >= 0}
        This function genrates bellow for the same relation: 
           {[i,j] : i = j && 0 <= i && j <= 2}
    For more examples see the getString test case in set_relation_test.cc
*/
string Set::getString()
{
    string result;

    VisitorGetString* v = new VisitorGetString();
    this->acceptVisitor( v );
    
    result = v->getString();

    return result;
}

//! Same as Set
string Relation::getString()
{
    string result;

    VisitorGetString* v = new VisitorGetString();
    this->acceptVisitor( v );
    
    result = v->getString();

    return result;
}





/*****************************************************************************/
#pragma mark -
/*************** VisitorGetZ3form *****************************/
/*! Vistor Class used in getZ3form()
*/
class VisitorGetVarTerms : public Visitor {
  private:
         std::set<std::string> vtTs;
  public:
         void preVisitVarTerm(VarTerm * t){
           std::string str = t->toString(true);
           vtTs.insert( str );
         }
         std::set<std::string> getVarTerms(){ return vtTs; }
};


string int2str(int i){
  char buf[50];
  sprintf (buf, "%d",i);
  return string(buf);
}
class VisitorGetZ3form : public Visitor {
  private:
         // When we are using this class for generating domain/Range assertion 
         // we just need the constraints, and we should not generate 
         // the UF symbols definitions to avoid recursive calls.
         bool termDef;
         TupleDecl tupleDecl; // Need this for tuple variable name
         int cc;
         // Keep a list of different vars to define them.
         std::set<std::string> tvTs;
         std::set<std::string> vtTs;
         std::set<std::string> ufsTs;
         
         std::vector<std::string> expZ3Form;
         std::vector<std::string> conjZ3Form;
         std::vector<std::string> z3Form;

         int constT;
         std::string leftSide;
         std::string rightSide;
         // Need to consider nested UF Calls, e.g row(col(i)+1)  
         int UFnestLevel;
         std::string UFparamT;
         std::string UFparamExp;

  public:
         VisitorGetZ3form(std::set<std::string> inUfsTs, 
                          std::set<std::string> inVtTs, bool inTermDef){
           cc = 1;
           termDef = inTermDef;
           ufsTs = inUfsTs;
           vtTs = inVtTs;
         }
         //! Keeping track of const term in the expression
         void preVisitTerm(Term * t) {
           if(UFnestLevel > 0) return;
           constT = t->coefficient();
         }
         //! 
         void preVisitUFCallTerm(UFCallTerm * t){
           ufsTs.insert( t->name() );
           if(UFnestLevel == 0){
             UFparamT = "";
             UFparamExp = "";
           }
           UFnestLevel++;
         }
         //! 
         void postVisitUFCallTerm(UFCallTerm * t){
           UFnestLevel--;
           std::string str = "("+ t->name() + " " + UFparamExp + ")";
           if(UFnestLevel == 0){
             if( t->coefficient() < 0 ) leftSide = str;
             else                       rightSide = str;
           } else {
             UFparamExp = str;
           }
         }
         void preVisitTupleVarTerm(TupleVarTerm * t){
           std::string str = t->prettyPrintString(tupleDecl,true);
           tvTs.insert( str );

           if(UFnestLevel == 0){
             if( t->coefficient() < 0 ) leftSide = str;
             else                       rightSide = str;
           } else {
             // Mahdi: FIXME: for simplicity for now assuming all the parameters to UFCs have positive sign
             UFparamT = str;
           }
         }
         void preVisitVarTerm(VarTerm * t){
           std::string str = t->toString(true);
           vtTs.insert( str );

           if(UFnestLevel == 0){
             if( t->coefficient() < 0 ) leftSide = str;
             else                       rightSide = str;
           } else {
             // Mahdi: FIXME: for simplicity for now assuming all the parameters to UFCs have positive sign
             UFparamT = str;
           }
         }
         //!
         void preVisitExp(iegenlib::Exp * e){
           if( !(e->isExpression()) ){ 
             leftSide = "";
             rightSide = "";
             constT = 0;
             UFnestLevel = 0;
           }
         }
         /*! 
         */ 
         void postVisitExp(iegenlib::Exp * e){
           if( !(e->isExpression()) ) {
             string z3Str = "";
             string comp = "";
             if(e->isEquality()) comp = "=";
             else if(constT == 0) comp = "<=";
             else {
               comp = "<";
               constT++;
             }
             if(leftSide == "") leftSide = "0";
             if(rightSide == "") rightSide = "0";

             if( constT == 0 ){
               z3Str = "(" + comp + " " + leftSide + " " + rightSide + ")";
             } else if ( constT < 0 ){
               z3Str = "(" + comp + " (+ " + leftSide + " " + 
                       int2str((constT*-1)) + ") " + rightSide + ")";
             } else if ( constT > 0 ){
               z3Str = "(" + comp + " " + leftSide + " (+ " + 
                       rightSide + " " + int2str(constT) + ") )";
             }

             expZ3Form.push_back(z3Str);
           } else {
             Term* cons = e->getConstTerm();
             if(UFparamExp == "" && cons == NULL){
               UFparamExp = UFparamT;
             } else if(UFparamExp == ""){
               UFparamExp = "(+ " + UFparamT + " " + int2str(cons->coefficient()) + ")";
             } else if(cons == NULL){
               UFparamExp = UFparamExp;
             } else {
               UFparamExp = "(+ " + UFparamExp + " " + int2str(cons->coefficient()) + ")";
             } 
           }
         }
         //! 
         void preVisitConjunction(iegenlib::Conjunction * c){
             tupleDecl = c->getTupleDecl();
             expZ3Form.clear();
         }
         //! 
         void postVisitConjunction(iegenlib::Conjunction * c){
           if(expZ3Form.size()>0){
             if(termDef){
               for(int i=0; i<expZ3Form.size(); i++){
                 conjZ3Form.push_back(expZ3Form[i]);
               }
             } else {
               string z3Str = "(and";
               for(int i=0; i<expZ3Form.size(); i++){
                 z3Str += " " + expZ3Form[i];
               }
               z3Str += " )";
               conjZ3Form.push_back(z3Str);
             }
           }
         }
         //! 
         void preVisitSparseConstraints(iegenlib::SparseConstraints *sc){
           conjZ3Form.clear();
         }
         //! 
         void postVisitSparseConstraints(iegenlib::SparseConstraints *sc){

           if(termDef){
             // Adding tuple variables definitions.
             for (std::set<std::string>::iterator it=tvTs.begin(); it!=tvTs.end(); it++)
               z3Form.push_back("(declare-const "+ *it + " Int)");
             //
             string z3Str = "";
              for(int i=0; i<conjZ3Form.size(); i++){
                z3Str = "(assert (! " + conjZ3Form[i] + " :named c1"+int2str(cc++)+") )";
                z3Form.push_back(z3Str);
              }
           } else {
             z3Form.push_back("(assert " + conjZ3Form[0] + " )");
           }
         }

         std::vector<std::string> getZ3Form(){ return z3Form; }
         std::set<std::string> getUFSs(){ return ufsTs; }
         std::set<std::string> getVars(){ return vtTs; }
};

/**! SparseConstraints::getZ3form returns a vector of strings that include 
//   constraints represented in SMT-LIB format that SMT solvers like z3 
//   gets as input. This can be used to check the satisfiability of 
//   an IEGenLib Set/Relation with a SMT solver. The returned list
//   also includes tuple variable declarations, however they do not include UFSymbol
//   and global variable declarations. This is because, when checking satisfiability of
//   a set, we usually also want to define some user defined assertions along side original
//   constraints. The assertions might have UFSymbols and global variables of their
//   own that original constraints do not. We can put their UFSymbols and globals into 
//   UFSyms, and VarSyms std::set that SparseConstraints::getZ3form returns by reference,
//   and then a driver function can declare all the UFSymbols and globals at the beginning of 
//   the input file that it is going to generate and pass to a SMT solver. 

     Also note that: UniQuantRule::getZ3form (in environment.h/cc) can be used to get SMT-LIB
     format of an user defined assertion stored in IEGenLib environment about UFSymbols.
**/
std::vector<std::string> SparseConstraints::getZ3form
(std::set<std::string> &UFSyms, std::set<std::string> &VarSyms, bool termDef){
    std::vector<std::string> result;
    VisitorGetZ3form *v = new VisitorGetZ3form(UFSyms, VarSyms, termDef);
    this->acceptVisitor( v );
    UFSyms = v->getUFSs();
    VarSyms = v->getVars();

    result = v->getZ3Form();
    
    return result;
}



}//end namespace iegenlib
