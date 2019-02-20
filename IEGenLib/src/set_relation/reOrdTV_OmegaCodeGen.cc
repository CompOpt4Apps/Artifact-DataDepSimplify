/*!
 * \file reOrdTV_OmegaCodeGen.cc
 *
 * \brief Implementations of the reOrdTV_OmegaCodeGen.
 *
 * \date Started: 2/19/19
 *
 * \authors Mahdi Soltan Mohammadi 
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
#include "isl_str_manipulation.h"


namespace iegenlib{


/*****************************************************************************/
#pragma mark -
/*****************************************************************************/
/*! Vistor Class used in VisitorCalculateComplexityNoEq
**  This isa  simplified versiopn of complexityForPartialParallel
    that do not considers useful equalities effect on complexity.
    This functionality is useful particularly here.
    For more details on how to calculate complexity please 
    see complexityForPartialParallel.cc.
*/
class VisitorCalculateComplexityNoEqs : public Visitor {
  private:
    int *complexity; 
    Exp *curr;
    TupleVarTerm *tv1;  // First TupleVar that we see in an in/equality
    TupleVarTerm *tv2;  // Second (possible) TupleVar that we see
    UFCallTerm *ufcUB;
    VarTerm *varUB;
    int nestedness;

  public:
    VisitorCalculateComplexityNoEqs(int *iComplexity){
        complexity = iComplexity;
        nestedness = 0;
    }
    virtual ~VisitorCalculateComplexityNoEqs(){}
    void preVisitUFCallTerm(UFCallTerm * t){ nestedness++; }
    void postVisitUFCallTerm(UFCallTerm * t){
      nestedness--;
      if( nestedness ) return;
      if( ufcUB ){
        //throw assert_exception("VisitorCalculateComplexityNoEqs: "
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
        //throw assert_exception("VisitorCalculateComplexityNoEqs: "
        //                   "more than two tuple variable in a constraint.");
      } 
    }
    void preVisitExp(iegenlib::Exp * e) {
      if( nestedness > 0) return; 
      curr = e; 
      nestedness = 0;
      tv1 = NULL; tv2 = NULL; ufcUB = NULL; varUB = NULL;
    }
    void postVisitExp(iegenlib::Exp * e) {
      if( nestedness > 0) return; 
      if( !tv1 ) { return; }
      if(complexity[tv1->tvloc()] == 0 || complexity[tv1->tvloc()] == 1 ) { return; }
      if( curr->isInequality() ) {                  // An inequlity constraint{
        if( !tv2 ){
          if(tv1->coefficient() > 0 || complexity[tv1->tvloc()] > -1 ){
            return; // If this is a lower bound or if complexity has already been determined 
          } else if( varUB ){     // i < n
            if(varUB->symbol() == "n" || varUB->symbol() == "m"){
              complexity[tv1->tvloc()] = 3;
            } else if(varUB->symbol() == "nnz"){
              complexity[tv1->tvloc()] = 2;
            } else {
              //throw assert_exception("VisitorCalculateComplexityNoEqs: "
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



/*! Vistor Class for finding iterators that get defined by useful equalities hence code generator
    can potentially remove their loops from inspector code, i.e. tv_i = ...;
    Note however, something like following is not a potential useful equality: f(tv_i) = g(tv_j),
    because both tv_i and tv_j are parameters to uninterpreted function calls. 
*/
class VisitorZeroCompTVs : public Visitor {
  private:
         int *zTVs; 
         int *complexity; 
         int tv1;
         int tv2;
         int UFnestLevel;
  public:
         VisitorZeroCompTVs(int *mzTVs, int *mcomps){
           zTVs = mzTVs;
           complexity = mcomps;
           UFnestLevel=0;
           tv1=-1;
           tv2=-1;
         }
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
           if( UFnestLevel == 0){
             if(tv1==-1) tv1 = t->tvloc();
             else        tv2 = t->tvloc();
           }
         }
         //!
         void preVisitExp(iegenlib::Exp* e){
           if( UFnestLevel == 0){
              tv1=tv2=-1;
           }
         }
         //!
         void postVisitExp(iegenlib::Exp* e){
           if( UFnestLevel == 0 && tv1 > -1 && e->isEquality()){
             if(tv2 == -1){
               zTVs[tv1] = 1;
             } else {
               if( complexity[tv1] > complexity[tv2] ){
                 zTVs[tv1] = 1;
               } else if( complexity[tv1] < complexity[tv2] ){
                 zTVs[tv2] = 1;
               } else if( tv1 > tv2 ){
                 zTVs[tv1] = 1;
               } else if( tv1 < tv2 ){
                 zTVs[tv2] = 1;
               } else {  // Should be impossible: i = i === TRUE
                 zTVs[tv1] = 1;
               }
             }
           }
         }
         int* getZTVs(){ return zTVs; }
};




/**! This function 
**/ 
void Set::reOrdTV_OmegaCodeGen(std::set<int> parallelTvs){

  if( getNumConjuncts() != 1 ){
    throw assert_exception("SparseConstraints::complexityForPartialParallel:"
                           "number of conjunctions is not 1.");
  }

//std::cout<<"\n\nHello from reOrdTV_OmegaCodeGen!!\n\n";

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

  VisitorCalculateComplexityNoEqs *vComp = new VisitorCalculateComplexityNoEqs(complexities);
  this->acceptVisitor(vComp);
//  this->acceptVisitor(vComp);
//  string result = calculateComplexityStr( complexities , arity() );

  int* zTVs = new int[arity()+1];
  for(int i=0; i < arity() ; i++){
    zTVs[i] = 0;
  }
  VisitorZeroCompTVs *vZComp = new VisitorZeroCompTVs(zTVs,complexities);
  this->acceptVisitor(vZComp);

  int candidateTV = -1;
  // 
  for(int i=0; i < arity() ; i++){
    if (zTVs[i] == 1){
      if( candidateTV == -1){
        candidateTV = i;
      } else if (complexities[candidateTV] <= complexities[i]){
        candidateTV = i;
      }
    }
  }

/*
  std::cout<<"\n\nzTV[";
  for(int i=0; i < arity() ; i++){
    std::cout<<zTVs[i]<<", ";
  }
  std::cout<<"]\n";

  std::cout<<"\ncomplexities[";
  for(int i=0; i < arity() ; i++){
    std::cout<<complexities[i]<<", ";
  }
  std::cout<<"]\n\n";
*/

  // == -1 means there is no useful equalityto begin with
  // == (arity()-1) means the iterator involved in the useful equality is alread last TV
  if(candidateTV == -1 || candidateTV == (arity()-1) ){
//std::cout<<"\n\nNo need for reordering\n\n";
    return;
  }

//std::cout<<"\n\nNeed reorder, candidate = "<<candidateTV<<"\n\n";
  
  // reordering
  std::string setStr = this->getString();
  srParts setParts;
  setParts = getPartsFromStr(setStr);
  std::queue<std::string> iters, newIters;
  std::string iter="", candIter="";  
  std::string newTupDecl = "[";
  bool b=true;
  iters = tupVarsExtract(setParts.tupDecl, arity(), 0);
  for(int i=0; i < arity() ; i++){
    std::string iter = iters.front();
    iters.pop();
    if( i == candidateTV){
      candIter = iter;
    } else {
      if(b){
        b = false;
        newTupDecl = newTupDecl+iter;
      } else {
        newTupDecl = newTupDecl+","+iter;
      }
    }
  }
  newTupDecl = newTupDecl+","+candIter+"]";  
  setParts.tupDecl = newTupDecl;
  std::string newSetStr = getFullStrFromParts(setParts);

  Set *newSet = new Set(newSetStr.c_str());

  *this = *newSet;

  delete newSet;
}




/*! Visitor Class for removing uninterpreted predicates
*/
class VisitorRmvUPs : public Visitor {
  private:
         Set* newSet;
         Conjunction* newConj;
         VarTerm *vt;
         int ct;
         int UFnestLevel;
  public:
         VisitorRmvUPs(){
           vt = NULL;
           UFnestLevel=0;
           ct=-100;
         }
         //! 
         void preVisitUFCallTerm(UFCallTerm* t){
           UFnestLevel++;
         }
         //! 
         void postVisitUFCallTerm(UFCallTerm* t){
           UFnestLevel--;
         }
         void preVisitTerm(Term * t) {
           if( UFnestLevel == 0){
             ct = t->coefficient();
           }
         }
         //!
         void postVisitVarTerm(VarTerm* t){
           if( UFnestLevel == 0){
             vt = t;
           }
         }
         //!
         void preVisitExp(iegenlib::Exp* e){
           if( UFnestLevel == 0){
             vt = NULL;
             ct=-100;
           }
         }
         //!
         void postVisitExp(iegenlib::Exp* e){
           if( UFnestLevel > 0 ) return;

           if( e->getTerm() != NULL ) ct =0;
           if( vt != NULL && ct != -100 && e->isEquality()) // removing UPs
             return;

           if( e->isInequality() ){
             newConj->addInequality( e->clone() );
           }else{
             newConj->addEquality( e->clone() );
           }
         }


         void preVisitConjunction(iegenlib::Conjunction * c){
             newConj = new Conjunction( c->getTupleDecl() );
         }
         //! Add Conjunctions in maffineConj to affineSet
         void postVisitSet(iegenlib::Set * s){
             newSet = new Set( s->arity() );

             newSet->addConjunction( newConj );
         }

         Set* getNewSet(){ return newSet; }
};


/**! This function 
**/ 
void Set::removeUPs(){
  
  VisitorRmvUPs *v = new VisitorRmvUPs();
  this->acceptVisitor(v);
 
  Set* newSet = v->getNewSet();

  *this = *newSet;
 
}


}//end namespace iegenlib
