/*!
 * \file setRelationship.cc
 *
 * \brief Implementation of functionality that tries to determine set type relationship 
 * (SuperSet, SubSetEqual, etc) between IEGenLib Sets and Relations that can have 
 * uninterpreted function symbols in their constraints.
 *
 * \date Started: 9/07/18
 *
 * \authors Mahdi Soltan Mohammadi
 *
 * Copyright (c) 2015-2018, University of Arizona <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include "set_relation.h"
#include "expression.h"
#include "UFCallMap.h"
#include "Visitor.h"

#include <util/util.h>
#include <gtest/gtest.h>

#include <utility>
#include <fstream>
#include <iostream>


namespace iegenlib{

/*****************************************************************************/
#pragma mark -

/*! Vistor Class to get implicit bounds on a tuple var
*/
class VisitorGetTVBounds : public Visitor {
  private:
         int UFnestLevel;
         int loc;
         std::vector<Exp> bExpS;
         bool found;
  public:
         VisitorGetTVBounds(int mloc){loc = mloc; UFnestLevel=0; found = false;}
         //! 
         void preVisitUFCallTerm(UFCallTerm * t){
           UFnestLevel++;
         }
         //! 
         void postVisitUFCallTerm(UFCallTerm * t){
           UFnestLevel--;
         }
         void preVisitTupleVarTerm(TupleVarTerm * t){
           if(UFnestLevel == 0 && t->tvloc() == loc) found = true;
         }
         //!
         void preVisitExp(iegenlib::Exp * e){
           if( UFnestLevel == 0 ) found = false;
         }
         //!
         void postVisitExp(iegenlib::Exp * e){
           if( UFnestLevel == 0 && found ){ 
             bExpS.push_back(*e);
           }
         }

         std::vector<Exp> getExps(){ return bExpS; }
};


// 
SetRelationshipType Conjunction::setRelationship(Conjunction* rightSide){

  int lEqC, lInEqC, lConstC, rEqC, rInEqC, rConstC, supSetEqC, supSetInEqC, supSetContC;

  lEqC = this->mEqualities.size();
  lInEqC = this->mInequalities.size();
  lConstC = lEqC+lInEqC;
  rEqC = rightSide->mEqualities.size();
  rInEqC = rightSide->mInequalities.size();
  rConstC = rEqC+rInEqC;

  // We are going to consider the set with fewer constraints as 
  // the candidate of being superset of the other one.
  Conjunction *supSetCand, *subSetCand;
  if(lConstC <= rConstC){
    supSetCand = this;
    subSetCand = rightSide;
    supSetEqC = lEqC;
    supSetInEqC = lInEqC;
    supSetContC = lConstC;
  } else {
    supSetCand = rightSide;
    subSetCand = this;
    supSetEqC = rEqC;
    supSetInEqC = rInEqC;
    supSetContC = rConstC;
  }

  // Putting Constraints of the subset candidate set into std::set's
  std::set<Exp> supSetEq, supSetInEq,subSetEq, subSetInEq;
  for(std::list<Exp*>::const_iterator i=subSetCand->mEqualities.begin();
              i != subSetCand->mEqualities.end(); i++) {
    subSetEq.insert( (*(*i)) ); 
  }
  for(std::list<Exp*>::const_iterator i=subSetCand->mInequalities.begin();
              i != subSetCand->mInequalities.end(); i++) {
    subSetInEq.insert( (*(*i)) );
  }

  std::set<Exp>::iterator it;
  // Comprae the constraints of the superset candidate with the subset candidate 
  for(std::list<Exp*>::const_iterator i=supSetCand->mEqualities.begin();
              i != supSetCand->mEqualities.end(); i++) {
    it = subSetEq.find( (*(*i)) );
    if( it == subSetEq.end() ){
      supSetEq.insert( (*(*i)) );
    } else {
      subSetEq.erase( it );
    }
  }
  for(std::list<Exp*>::const_iterator i=supSetCand->mInequalities.begin();
              i != supSetCand->mInequalities.end(); i++) {
    it = subSetInEq.find( (*(*i)) );
    if( it == subSetInEq.end() ){
      supSetInEq.insert( (*(*i)) );
    } else {
      subSetInEq.erase( it );
    }
  }

  // If subset candidate relation have all the constranits of the superset
  // relatrion, we  can say that superset candidate is trivially 
  // superset of subset candidate
  if( supSetEq.empty() && supSetInEq.empty() ){
    if(lConstC <= rConstC) return SuperSetEqual;
    else                   return SubSetEqual;
  }

  // Otherwise, it might be the case that the superset candidate is the 
  // superset of the subset candidate , but in a complicated manner.
  // Right now, we are looking into just one straight forward case 
  // of such situation.
  bool isSupSet = false;
  if(supSetEq.size() == 1 && supSetInEq.empty() ){
isSupSet=true;
/*
    Exp supEqCand = *(supSetEq.begin()), subEqCand;
    std::list<Term*> suptl = supEqCand.getTermList();
    if(suptl.size() == 2){
      for(std::set<Exp>::iterator i=subSetEq.begin(); i != subSetEq.end(); i++){
        std::list<Term*> subtl = (*it).getTermList();
        Term *supTerm, *subTerm;
        Exp copyE = *i;

        if(supTerm->type() != "TupleVarTerm" || subTerm->type() != "TupleVarTerm"){
          continue;
        }

        int supTvLoc = ((TupleVarTerm*)(supTerm))->tvloc();
        std::vector<Exp> bExpS;
        VisitorGetTVBounds *v = new VisitorGetTVBounds(supTvLoc);
        supSetCand->acceptVisitor( v );
        bExpS = v->getExps();
        SubMap subMap;
        Exp e1;
        e1.addTerm(new TupleVarTerm(((TupleVarTerm*)(subTerm))->tvloc()));
        subMap.insertPair(new TupleVarTerm(supTvLoc), new Exp(e1));
std::cout<<"\n\n>>>>>Bound Exps:\n\n";
        for(int j=0; j<bExpS.size() ; j++){
std::cout<<"BeS = "<<bExpS[j].toString();
          bExpS[j].substitute(subMap);
std::cout<<"   AfS = "<<bExpS[j].toString()<<"\n";
        }
        //
      }
    }
*/
  }

  if(isSupSet){
    std::cout<<"\n\n>>>The complicated case!!\n\n";
    if(lConstC <= rConstC) return SuperSetEqual;
    else                   return SubSetEqual;
  }
/*
  std::cout<<"\n\nLeftS = "<<this->toString()<<"\nRightS = "<<rightSide->toString()<<"\n\n";
  std::cout<<"\n\nConjunction::setRelationship:\n Left EqC = "<<lEqC<<" InEqC = "<<lInEqC<<"  ConstC = "<<lConstC<<" Right EqC = "<<rEqC <<" InEqC = "<<rInEqC <<"  ConstC = "<<rConstC<<"\n\n";
  std::cout<<"\n\n\n ----SupSetEqC = "<<supSetEq.size()<<" nSupSetInEqC = "<<supSetInEq.size()<<"\n";
  for(std::set<Exp>::const_iterator i=supSetEq.begin(); i != supSetEq.end(); i++) {
    std::cout<<"\n  EQ  = "<<(*i).toString();
  }
  for(std::set<Exp>::const_iterator i=supSetInEq.begin(); i != supSetInEq.end(); i++) {
    std::cout<<"\n  InEQ  = "<<(*i).toString();
  }
  std::cout<<"\n\n\n ----SubSetEqC = "<<subSetEq.size()<<" nSubSetInEqC = "<<subSetInEq.size()<<"\n";
  for(std::set<Exp>::const_iterator i=subSetEq.begin(); i != subSetEq.end(); i++) {
    std::cout<<"\n  EQ  = "<<(*i).toString();
  }
  for(std::set<Exp>::const_iterator i=subSetInEq.begin(); i != subSetInEq.end(); i++) {
    std::cout<<"\n  InEQ  = "<<(*i).toString();
  }
*/
  return UnKnown;
}

bool islUnSat(std::string str){
  srParts parts = getPartsFromStr(str);
  if( trim(parts.constraints) == "FALSE") return true;
  return false;
}

// 
SetRelationshipType Set::setRelationship(Set* rightSide){
  // get clones of the sets
  Set* ls = new Set(*this);
  Set* rs = new Set(*rightSide);
  ls->normalize(false); rs->normalize(false);
  // See if the inormalization has detected the sets as unsat 
  // * normalization can detect trivially unsat sets
  if( islUnSat(ls->getString()) || islUnSat(rs->getString()) ) return UnSatSet;
  // See if the sets are equal * normalization exposes the trivially equal sets
  if(*ls == *rs) return SetEqual;

  Conjunction *lside = *(ls->mConjunctions.begin());
  Conjunction *rside = *(rs->mConjunctions.begin());

  SetRelationshipType ret = lside->setRelationship(rside);

  if( ret == UnKnown) {
    lside = *(ls->mConjunctions.begin());
    rside = *(rs->mConjunctions.begin());
    return lside->setRelationship(rside);
  }

  return ret;
}

// 
SetRelationshipType Relation::setRelationship(Relation* rightSide){
  // get clones of the sets
  Relation* ls = new Relation(*this);
  Relation* rs = new Relation(*rightSide);
  ls->normalize(false); rs->normalize(false);
  // See if the inormalization has detected the sets as unsat 
  // * normalization can detect trivially unsat sets
  if( islUnSat(ls->getString()) || islUnSat(rs->getString()) ) return UnSatSet;
  // See if the sets are equal * normalization exposes the trivially equal sets
  if(*ls == *rs) return SetEqual;

  Conjunction *lside = *(this->mConjunctions.begin());
  Conjunction *rside = *(rightSide->mConjunctions.begin());

  SetRelationshipType ret = lside->setRelationship(rside);

  if( ret == UnKnown) {
    lside = *(ls->mConjunctions.begin());
    rside = *(rs->mConjunctions.begin());
    return lside->setRelationship(rside);
  }

  return ret;
}


}//end namespace iegenlib
