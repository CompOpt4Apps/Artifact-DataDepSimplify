/*!
 * \file UninterpFunc.h
 *
 * \brief Interface of UninterpFunc class
 *
 * \date Started: 2013-06-20
 * # $Revision:: 727                $: last committed revision
 * # $Date:: 2013-07-25 22:45:15 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2013, Colorado State University 
 * All rights reserved. 
 * See ../../COPYING for details. <br>
 */
 
#ifndef SYMBOLIC_FUNC_H_
#define SYMBOLIC_FUNC_H_

#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>

namespace iegenlib{

class Set;

/*!
 * \class UninterpFunc
 *
 * \brief Contains info about uninterpreted function such as name, domain,
 *        range, whether bijective, and if has any monotonicity characteristic.
 */
typedef enum {
    Monotonic_NONE,
    Monotonic_Nondecreasing,
    Monotonic_Increasing,
    Monotonic_Nonincreasing,
    Monotonic_Decreasing
    // Here is where others would go when we get them
} MonotonicType;

class UninterpFunc {
public:
    UninterpFunc(std::string name, Set* domain, Set* range,
                 bool bijective, MonotonicType monoType);
    ~UninterpFunc();
    
    //! Copy constructor.
    UninterpFunc( const UninterpFunc& other );
    
    //! Assignment operator.
    UninterpFunc& operator=( const UninterpFunc& other);
    
    //! Return the function name.
    std::string getName() const { return mName; }

    //! Return the function domain.
    Set* getDomain() const { return mDomain; }

    //! Return the function range.
    Set* getRange() const { return mRange; }

    //! Returns true if function is bijective, false otherwise.
    bool isBijective() const { return mIsBijective; }
    
    //! Getter for the monotonicity type.
    MonotonicType getMonoType() const { return mMonoType; }
        
    //! Creates a string with information about function.
    std::string toString() const;
            
private:
    std::string         mName;
    Set*                mDomain;
    Set*                mRange;
    bool                mIsBijective;
    MonotonicType       mMonoType;
};

}

#endif
