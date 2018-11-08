// iegenlib.i - SWIG interface
%exception  {
   try {
      $action
   } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      PyErr_SetString(PyExc_Exception, const_cast<char*>(e.what()));
   }
}

%module iegenlib
%{
#include <iegenlib.h>
%}

%include "std_string.i"
%include "std_list.i"
%include "std_map.i"
%include "std_set.i"
%include "std_vector.i"

%ignore SparseConstraints::operator=;
%ignore operator+;

%rename(__assign__) *::operator=;

/* We can't figure out how to not override the __str__ method for a string 
object.  Solution: make all examples in python NOT use prettyPrintString(). */
%rename(__str__) prettyPrintString;

/* Set/Relation includes 
   Apparently these have to be relative paths.
*/

%include <../set_relation/UninterpFunc.h>
%include <../set_relation/SubMap.h>
%include <../set_relation/TupleDecl.h>
%include <../set_relation/environment.h>
%include <../util/util.h>
%include <../set_relation/expression.h>
%include <../set_relation/set_relation.h>
%include <../parser/parser.h>















