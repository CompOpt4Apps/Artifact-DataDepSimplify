/*!
 * \file util.h
 *
 * \brief Utilities for the IEGen project
 *
 * \date Started: 9/13/2010, but has been completely rewritten
 * # $Revision:: 622                $: last committed revision
 * # $Date:: 2013-01-18 13:11:32 -0#$: date of last committed revision
 * # $Author:: cathie               $: author of last committed revision
 *
 * \authors Cathie Olschanowsky
 *
 * Copyright (c) 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */
#ifndef UTIL_H_
#define UTIL_H_

#include <stdexcept>
#include <string>
#include <fstream>
#include <set>
#include <sstream>
#include <list>

using std::string;
using std::logic_error;
using std::runtime_error;
using std::set;
using std::stringstream;


namespace iegenlib{

bool compareFileContentsToString(string file, string contents);

/*!
 * A unique name created from the base name and the set of names that
 * are already in use
 * @param name The base name to create the unique name from
 * @param taken The set of names that are already in use.  This set is updated
 *              with the name returned.
 * @return the unique name that is created from base name and the taken names
 */
string createUniqueName(string name, set<string>& taken);


/*!
 * Exception used when we would normally use an assert.  We expect
 * certain conditions to apply and when they don't, we throw one of these.
 */
class assert_exception: public logic_error {
  public:
    explicit assert_exception(const string& error_message)  :
                logic_error(error_message) {}
};

/*!
 * A parse_exception occurs anytime a set, relation, or environment
 * specification has a parsing error in it.  The iegenlib_calc driver
 * will also use these.
 */
class parse_exception: public runtime_error {
  public:
        explicit parse_exception(const string& error_message)  :
                runtime_error(error_message) {}
};

/*!
 * \class StringIterator
 *
 * \brief An ordered iterator over strings.
 */
class StringIterator {
  public:
    StringIterator(std::list<std::string> aList);
    StringIterator(std::set<std::string> aSet);

    /// Returns a pointer to the next string.
    std::string next();
    
    /// Returns true if there is another string in the iteration.
    bool hasNext();
    
  private:
    std::list<std::string> mList;
    std::list<std::string>::iterator mIter;
};



}//end namespace iegenlib

#endif
