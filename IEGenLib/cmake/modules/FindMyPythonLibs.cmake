# Initial module code taken from cmake bug tracker, issue 2257: http://www.paraview.org/Bug/view.php?id=2257
#
# - Find python libraries
# This module finds if Python is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  PYTHON_VERSION = version of Python found
#  PYTHON_PREFIX = prefix of python install that was found
#  PYTHON_INCLUDES = paths to include directories
#  PYTHON_LIBS = names of libraries to link to
#  PYTHON_LINK_DIRS = paths of directories to search for libraries
#

#Get the version of python
EXECUTE_PROCESS(COMMAND python --version ERROR_VARIABLE PYTHON_VERSION ERROR_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " " ";" PYTHON_VERSION "${PYTHON_VERSION}")
list(GET PYTHON_VERSION 1 PYTHON_VERSION)

#Get the python prefix
EXECUTE_PROCESS(COMMAND python-config --prefix OUTPUT_VARIABLE PYTHON_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)

#Get the python-required includes
EXECUTE_PROCESS(COMMAND python-config --includes OUTPUT_VARIABLE PYTHON_INCLUDES OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " " ";" PYTHON_INCLUDES "${PYTHON_INCLUDES}")
string(REGEX REPLACE "-I" "" PYTHON_INCLUDES "${PYTHON_INCLUDES}")
list(REMOVE_DUPLICATES PYTHON_INCLUDES)

#Get the python-required libraries to link to
EXECUTE_PROCESS(COMMAND python-config --libs OUTPUT_VARIABLE PYTHON_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " " ";" PYTHON_LIBS "${PYTHON_LIBS}")

#Get the python-required link directories
EXECUTE_PROCESS(COMMAND python-config --ldflags OUTPUT_VARIABLE PYTHON_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE " " ";" PYTHON_LDFLAGS "${PYTHON_LDFLAGS}")
set(PYTHON_LINK_DIRS "")
foreach(ldflag ${PYTHON_LDFLAGS})
  string(REGEX MATCH "-L.*" MATCHED ${ldflag})
  if(MATCHED)
    set(PYTHON_LINK_DIRS ${PYTHON_LINK_DIRS} ${MATCHED})
  endif(MATCHED)
endforeach(ldflag)
string(REGEX REPLACE "-L" "" PYTHON_LINK_DIRS "${PYTHON_LINK_DIRS}")
