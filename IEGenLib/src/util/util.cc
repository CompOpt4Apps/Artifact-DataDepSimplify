/*!
 * \file util.cc
 *
 * \brief Implementation for the util classes/routines
 *
 * \date Started: 9/13/10
 * # $Revision:: 738                $: last committed revision
 * # $Date:: 2013-08-04 23:04:41 -0#$: date of last committed revision
 * # $Author:: mstrout              $: author of last committed revision
 *
 * \authors Alan LaMielle
 *
 * Copyright (c) 2009, 2010, 2011, 2012, Colorado State University <br>
 * All rights reserved. <br>
 * See ../../COPYING for details. <br>
 */

#include <util/util.h>
#include <iostream>

namespace iegenlib{

  /*! Compare the contents of a file to the contents of a string
  **  (diff)
  **
  ** \param filename and string with contents
  ** \return true if they match; false otherwise
  */
  bool compareFileContentsToString(string file, string contents){

    // create a stream from the filename
    std::ifstream t(file.c_str());
    if(!t.good()){
      throw assert_exception("Error opening " + file);
    }
    // This is simply a call to the string constructor.
    // There is a constructor with the following declaration:
    // template<class InputIterator> 
    //    string (InputIterator begin, InputIterator end);
    // in this case InpuIterator is char. Therefore, this constructor 
    // simply calls string ( size_t n, char c );
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    //std::cout << "file = " << str << std::endl;    
    //std::cout << "contents = " << contents << std::endl;    
    if(str.compare(contents) == 0){
      return true;
    }else{
      return false;
    }
  }


  string createUniqueName(string name, set<string>& taken) {
    //Is the name already taken?
    if(taken.find(name) == taken.end()){
      return name;
    }

    int count = 1;
    string retval;

    //loop until we find a unique name
    do {
      stringstream ss;
      ss << name << count++;
      retval = ss.str();
    } while(taken.find(retval) != taken.end());
    taken.insert(retval);
    return retval;
  }


  StringIterator::StringIterator(std::list<std::string> aList) : mList(aList) {
    mIter = mList.begin();
  }


  StringIterator::StringIterator(std::set<std::string> aSet) {
  
    std::list<std::string> symbolList;
    for (std::set<std::string>::iterator iter = aSet.begin();
            iter != aSet.end(); iter++) {
        mList.push_back( *iter );
    }
    
    mIter = mList.begin();
  }



  /// Returns a pointer to the next symbol.
  std::string StringIterator::next() {
    std::string retval = *mIter;
    mIter++;
    return retval;
  }
    
  /// Returns true if there is another symbol in the iteration.
  bool StringIterator::hasNext() {
    return mIter!=mList.end();
  }


}//end namespace iegenlib
