/*!
 * \file UninterpFunc.cc
 *
 * \brief Implementation of UninterpFunc class
 *
 * \date Started: 2013-06-20
 * # $Revision:: 727                $: last committed revision
 * # $Date:: 2013-07-25 22:45:15 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013 Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */

#include "UninterpFunc.h"
#include "set_relation.h"



namespace iegenlib{

UninterpFunc::UninterpFunc( std::string name, Set* domain, Set* range, 
                            bool bijective, MonotonicType monoType) :
        mName(name), mDomain(domain), mRange(range), 
        mIsBijective(bijective), mMonoType(monoType)
{}

UninterpFunc::~UninterpFunc() {
    delete mDomain;
    delete mRange;
}
    
//! Copy constructor.
UninterpFunc::UninterpFunc( const UninterpFunc& other ) {
    *this = other;
}
    
//! Assignment operator.  Clones the domain and range sets.
UninterpFunc& UninterpFunc::operator=( const UninterpFunc& other) {
    mName = other.mName;
    mDomain = new Set(*(other.mDomain));
    mRange = new Set(*(other.mRange));
    mIsBijective = other.mIsBijective;
    mMonoType = other.mMonoType;
    return *this;
}
        
//! Creates a string with information about function.
std::string UninterpFunc::toString() const {
    std::stringstream ss;
    ss << "UninterpFunc(mName=" << mName;
    ss << ", mDomain=" << mDomain->toString();
    ss << ", mRange=" << mRange->toString();
    ss << ", mIsBijective=" << mIsBijective;
    ss << ", mIsBijective=" << mMonoType << ")";
    
    return ss.str();
}
            

}
