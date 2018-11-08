/*!
 * \file complexityForPartialParallel.cc
 *
 * \brief Implementations of the complexityForPartialParallel.
 *
 * \date Started: 2/05/18
 *
 * \authors Michelle Strout, and Mahdi Soltan Mohammadi 
 *
 * Copyright (c) 2015-2018, University of Arizona <br>
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


/*****************************************************************************/
#pragma mark -
/*****************************************************************************/
/*! Vistor Class used in VisitorCalculateComplexity
**  The visitor class calculates complexity for TupleVarTerms
The way it works in general is that: we want to determine whether 
a (smart) code generator would generate loop for any of the iterators and if so, 
what is the complexity of that loop. For instance, consider following example:

{[i,j] : i = j && 0 <= j < n }

(Smart) Code genrator would not generate a loop for i-iterator, 
because we have the constraints: i=j. 
But we need a loop with complexity of O(n) in that example. 
So, the output of this visitor would be:

[1,3] : indicating i has complexity O(1), and j has complexity O(n).

We use inequalities to determine  upper bounds of each term. 
If we have already find an upper bound but we see a smaller one, 
we would replace the bigger one.
We also use useful equalities to make complexity of one of 
the tuple terms in equalities like i=j, or i=f(j), to be O(1).
The important thing to note here is that we make the complexity of an iterator 
with bigger complexity O(1).
If a term is in an equality constraint with another constraints, 
'and we know the upper bound of the other iterator, 
then we do not have to find the upper bounds of this iterator. 

Note: we need to call this visitor at least twice since, it might be 
the case that while traversing the constraints (iegenlib::Exp's) 
we pass by an useful equality like i=j, but we do not know 
upper bound of neither i or j. 
So, we would not make complexity of any of them O(1).
*/
class VisitorCalculateComplexity : public Visitor {
  private:
    int *complexity; 
    Exp *curr;
    TupleVarTerm *tv1;  // First TupleVar that we see in an in/equality
    TupleVarTerm *tv2;  // Second (possible) TupleVar that we see
    UFCallTerm *ufcUB;
    VarTerm *varUB;
    int nestedness;

  public:
    VisitorCalculateComplexity(int *iComplexity){
        complexity = iComplexity;
        nestedness = 0;
    }
    virtual ~VisitorCalculateComplexity(){}
    void postVisitUFCallTerm(UFCallTerm * t){
      if( nestedness ) return;
      if( ufcUB ){
        //throw assert_exception("VisitorCalculateComplexity: "
        //                   "more than one UFC in a constraint.");
      }
      ufcUB = t;
    }
    void postVisitVarTerm(VarTerm * t){
      if( nestedness ) return;
      varUB = t;
    }
    void postVisitTupleVarTerm(TupleVarTerm *t) {
      if( nestedness ) return;
      if( !tv1 ){ tv1 = t;
      } else if( !tv2 ){ 
        tv2 = t;
      } else {
        //throw assert_exception("VisitorCalculateComplexity: "
        //                   "more than two tuple variable in a constraint.");
      } 
    }

    void preVisitExp(iegenlib::Exp * e) {
      // Record if our current expression is a constraint or param to an UFC
      if( e->isExpression() ){ nestedness++; }
      if( e->isEquality() || e->isInequality() ){ 
        curr = e; 
        nestedness = 0;
        tv1 = NULL; tv2 = NULL; ufcUB = NULL; varUB = NULL;
      }
    }
    void postVisitExp(iegenlib::Exp * e) {
      if( e->isExpression() ){ nestedness--; return; }
      if( !tv1 ) { return; }   // Nothing useful to investigate
      // Mahdi FIXME: complexity[tv1->tvloc()] == 0 is not correct in general.
      // If there are two equalities like this: i = j && j = k, 
      // this could generate the wrong complexity. However, I have made
      // this limitation for now to conveniently avoid getting O(1) for 
      // both iterators in one equality like i=j (only i or j should get O(1) complexity). 
      // Without it we need a complex scheme to avoid such recursive wrong result. 
      if(complexity[tv1->tvloc()] == 0 || complexity[tv1->tvloc()] == 1 ) { return; }

      if( curr->isEquality() ){ // An equlity constraint
        // Do we have something like i = jp  ** useful equality
        if( tv2 ) {
          if(complexity[tv1->tvloc()] == -1 && complexity[tv2->tvloc()] == -1){
            return;  // We need to know the complexity of at least one of the iterators 
          }          // to know that only less expensive one needs a loop.
          if(complexity[tv1->tvloc()] == 0 || complexity[tv2->tvloc()] == 0){
            return;  // The equality is useless since at least one of the iterators 
          }          // is going to be projected out.  
          // Mahdi FIXME: related to the previous fixme.
          if(complexity[tv1->tvloc()] == 1 || complexity[tv2->tvloc()] == 1){
            return;  // One of the iterators already have O(1) complexity
          }
          if(complexity[tv1->tvloc()] == -1 || complexity[tv2->tvloc()] > 1){
            complexity[tv1->tvloc()] = 1;
          } else if(complexity[tv1->tvloc()] > 1 || complexity[tv2->tvloc()] == -1){
            complexity[tv2->tvloc()] = 1;
          } else if(complexity[tv2->tvloc()] <= complexity[tv1->tvloc()] ){
            complexity[tv1->tvloc()] = 1;
          } else {
            complexity[tv2->tvloc()] = 1;
          } 
        // Do we have something like i = f(jp)  ** useful equality
        // No need for complicated checks since we can only use f(jp) 
        // to produce values to remove the need for a for-loop for i 
        } else if( ufcUB ){
          complexity[tv1->tvloc()] = 1;
        }
      } else {                  // An inequlity constraint{
        if( !tv2 ){
          if(tv1->coefficient() > 0 || complexity[tv1->tvloc()] > -1 ){
            return; // If this is a lower bound or if complexity has already been determined 
          } else if( varUB ){     // i < n
            if(varUB->symbol() == "n" || varUB->symbol() == "m"){
              complexity[tv1->tvloc()] = 3;
            } else if(varUB->symbol() == "nnz"){
              complexity[tv1->tvloc()] = 2;
            } else {
              //throw assert_exception("VisitorCalculateComplexity: "
              //                         "unknown symbolic constant.");
            }
          } else if ( ufcUB ) {     // i < UFC(XX)
             complexity[tv1->tvloc()] = 2; //ufcUpBound( ufcUB );
          }
        } else {  ////
          if(tv1->coefficient() < 0 && tv2->coefficient() < 0 ){
            return;
          } else if(tv1->coefficient() < 0 && tv2->coefficient() > 0 ){
            if( complexity[tv2->tvloc()] < 2 ){
              return;
            } else if( complexity[tv2->tvloc()] < complexity[tv1->tvloc()] ){
              complexity[tv1->tvloc()]  = complexity[tv2->tvloc()] ;
            }
          } else if(tv1->coefficient() > 0 && tv2->coefficient() < 0 ){
            if( complexity[tv1->tvloc()] < 2 ){
              return;
            } else if( complexity[tv1->tvloc()] < complexity[tv2->tvloc()] ){
              complexity[tv2->tvloc()]  = complexity[tv1->tvloc()] ;
            }
          }
        }  ////
      }
    }

    int* getComplexity() { 
        return complexity;
    }
};


/*! Calculates complexity string, e.g O(n^2*nnz*4), based on complexity
    of individual tuple variables given.
*/
string calculateComplexityStr(int *complexities, int arr){
  std::string result="O(";
  int numOfNs = 0, numOfNNZdNs = 0, diff;
  string strNumOfNs, strNumOfNNZdNs, strDiff;

  for(int i=0; i < arr ; i++){
    if (complexities[i] == 2)      numOfNNZdNs++;
    else if (complexities[i] == 3) numOfNs++;
  }
  diff = numOfNs - numOfNNZdNs;
  
  char buffer [50];
  sprintf (buffer, "%d", numOfNs);
  strNumOfNs = buffer;
  sprintf (buffer, "%d", numOfNNZdNs);
  strNumOfNNZdNs = buffer;
  sprintf (buffer, "%d", abs(diff) );
  strDiff = buffer;

  if( numOfNNZdNs == 0 ){
    result = result + "n^" + strNumOfNs + ")";
  } else if( numOfNs == 0){
    result = result+"nnz^"+strNumOfNNZdNs +"/"+"n^"+strNumOfNNZdNs+")";
  } else if( diff < 0 ) {
    result = result + "nnz^" + strNumOfNNZdNs + "/" + "n^" + strDiff + ")";
  } else if( diff > 0 ) {
    result = result + "n^" + strDiff + "*" + "nnz^" + strNumOfNNZdNs + ")";
  } else {
    result = result + "nnz^" + strNumOfNNZdNs + ")";
  }

  return result;
}

/**! This function calculates the algorithmic complexity of a Set/Relation
**   that is representing a data dependence. Also, it takes into
**   account the fact that the set is meant for dependency analysis
**   for partial parallelism. Basically, it calculates the complexity of
**   efficient inspector that we need to generate for the dependence.  
**   Therefore, it considers two things: 
**   1) It ignores any tuple variable that we can project out, other 
**   than those that we want to parallelize.
**   2) It takes into account the useful equalities (e.g i = col(jp), 
**   where we can get values of i from col(jp)). Nonetheless, note that 
**   it does not consider any sort of approximations that 
**   we might be able to do to further optimize the inspector.

**   The way it works is that we are trying to find the range of iterators
**   that are going to be in the final inspector, and multiply together.

**   The out is a string of the form O(n^2*nnz^4)
**/ 
std::string SparseConstraints::
            complexityForPartialParallel(std::set<int> parallelTvs){

  if( getNumConjuncts() != 1 ){
    throw assert_exception("SparseConstraints::complexityForPartialParallel:"
                           "number of conjunctions is not 1.");
  }
  // What each type of complexity mean for an iterator:
  // -1: has not been determined yet
  //  0: ignore the iterator since it can be projected out
  //  1: O(1): the values for the iterator can be generated from another one
  //  2: O(nnz/n)
  //  3: O(n)
  int* complexities = new int[arity()+1];
  for(int i=0; i < arity() ; i++){
    if ( parallelTvs.find(i) != parallelTvs.end() ){
      complexities[i] = -1;     // We do not want to project out the iterator
    } else if ( isUFCallParam(i) ){
      complexities[i] = -1;     // We cannot project out the iterator
    } else {
      complexities[i] = 0;      // We can project out the iterator
    }
  }

  // We visit the constraints twice, so we can determine the usefulness of 
  // any potential useful equality.  First round is to determine the initial
  // range of iterators (e.g i = colIdx(kp), we calculate the range of i and kp)
  // Then, in the second round we determine which iterator would have a loop
  // and which iterator can be calculated based on equalities: for example:
  // we have i = colIdx(kp) then we found out  i == n and kp == nnz/n 
  // =>  it is better to remove i from complexity since: n > nnz/n    
  VisitorCalculateComplexity *vComp = new VisitorCalculateComplexity(complexities);
  this->acceptVisitor(vComp);
  this->acceptVisitor(vComp);

  string result = calculateComplexityStr( complexities , arity() );

  return result;
}

}//end namespace iegenlib
