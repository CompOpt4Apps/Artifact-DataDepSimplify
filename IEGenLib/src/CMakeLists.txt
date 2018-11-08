# steps:
# 0: Echo build parameters to the user
# 1: compile a list of sources to be used
# 2: Create the targets to gen the parser
# 3: Create executables and libraries
# 4: Install
# 5: Add in some testing

#Search for a threading library
find_package(Threads)
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")

############################### STEP 0 ########################################
########## Verify the input parameters and echo them to the user ##############

set(GEN_PARSER_YESNO "No")
if(GEN_PARSER)
  set(GEN_PARSER_YESNO "Yes")
endif()
message(STATUS "Generating parser code: ${GEN_PARSER_YESNO}")

set(BUILD_PYTHON_YESNO "No")
if(BUILD_PYTHON)
  set(BUILD_PYTHON_YESNO "Yes")
endif()
message(STATUS "Building Python bindings: ${BUILD_PYTHON_YESNO}")

############################### STEP 1 ########################################
##### compile a list of all of the sources in our subdirectories     ##########

## get all of the sources (testing and not)
file(GLOB_RECURSE iegenlib_SOURCES *.cc)
file(GLOB_RECURSE iegenlib_t_SOURCES *test.cc)

## remove the tests from the regular list 
list(REMOVE_ITEM iegenlib_SOURCES ${iegenlib_t_SOURCES})

## remove anything with a main function
list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENLIB_SOURCE_DIR}/src/iegenlib_calc.cc)
list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENLIB_SOURCE_DIR}/src/cpp_api_example.cc)
list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENCC_SOURCE_DIR}/src/iegenlib/iegenlib_calc.cc)
list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENCC_SOURCE_DIR}/src/iegenlib/cpp_api_example.cc)

list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENLIB_SOURCE_DIR}/src/drivers/DEPRECATED_simplifyDriver.cc)

list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENLIB_SOURCE_DIR}/src/drivers/simplification.cc)
list(REMOVE_ITEM iegenlib_SOURCES 
	${IEGENLIB_SOURCE_DIR}/src/drivers/superAffSet.cc)


############################### STEP 2 ########################################
######################  Generate the Parser Code ##############################
if(GEN_PARSER)
  #Find the flex and bison tools
  find_package(FLEX)
  find_package(BISON)

  if(${FLEX_VERSION} LESS 2.5)
    message(FATAL_ERROR "Flex version too low: ${FLEX_VERSION}")
  else()
    message(STATUS "Flex version ok: ${FLEX_VERSION}")
  endif()

  if(${BISON_VERSION} LESS 2.4)
    message(FATAL_ERROR "Bison version too low: ${BISON_VERSION}")
  else()
    message(STATUS "Bison version ok: ${BISON_VERSION}")
  endif()

  if(${FLEX_FOUND} AND ${BISON_FOUND})
    #Generate gen_scanner.cc file
    FLEX_TARGET(gen_scanner parser/parser.l
	    ${CMAKE_CURRENT_SOURCE_DIR}/parser/gen_scanner.cc)

    #Generate the gen_parser.cc/gen_parser.hh files
    BISON_TARGET(gen_parser parser/parser.y
	    ${CMAKE_CURRENT_SOURCE_DIR}/parser/gen_parser.cc)

    #Setup the proper dependency between the flex and bison files
    ADD_FLEX_BISON_DEPENDENCY(gen_scanner gen_parser)

  endif()
endif()

############################### STEP 3 ########################################
######################  Continue to build the targets #########################
## searching for header files
include_directories(.)
include_directories(parser)
include_directories(set_relation)
include_directories(util)

## set the compile flags
SET_SOURCE_FILES_PROPERTIES(${iegenlib_SOURCES} 
	                    PROPERTIES 
			    COMPILE_FLAGS "-Wall -Wno-unknown-pragmas")
SET_SOURCE_FILES_PROPERTIES(${iegenlib_t_SOURCES} 
	                    PROPERTIES 
			    COMPILE_FLAGS "-Wall -Wno-unknown-pragmas")

## create the library
add_library(iegenlib STATIC ${iegenlib_SOURCES})

#Compile and link the iegenlib_calc executable
add_executable(iegenlib_calc iegenlib_calc.cc)
target_link_libraries(iegenlib_calc iegenlib isl gmp ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(iegenlib_calc isl gmp)

cmake_policy(SET CMP0037 OLD)
#Compile and link the simplifyDriver executable
add_executable(../bin/simplifyDriver drivers/simplification.cc)
target_link_libraries(../bin/simplifyDriver iegenlib isl gmp)

cmake_policy(SET CMP0037 OLD)
#Compile and link the superAffSet executable
add_executable(../bin/superAffSet drivers/superAffSet.cc)
target_link_libraries(../bin/superAffSet iegenlib isl gmp)


### this executable hold our unit tests
add_executable(iegenlib_t ${iegenlib_SOURCES} ${iegenlib_t_SOURCES})
#Tell included gtest headers to NOT use tuple support.
add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
target_link_libraries(iegenlib_t gtest isl gmp)

#Process subdirectories
add_subdirectory(bindings)

############################### STEP 4 ########################################
######################  Install the library ###################################

#Install the iegenlib library
install(TARGETS iegenlib
        ARCHIVE DESTINATION lib)

#Install the iegenlib_calc executable
install(TARGETS iegenlib_calc
        RUNTIME DESTINATION bin)

#Install the test binary
install(TARGETS iegenlib_t
	RUNTIME DESTINATION bin)

#Install the header files
install(DIRECTORY ./ 
	DESTINATION include/iegenlib
	FILES_MATCHING PATTERN "*.h"
	PATTERN ".svn" EXCLUDE)

#Install jsoncon specific header files
install(DIRECTORY ./parser/jsoncons/
	DESTINATION include/iegenlib/parser/jsoncons/
	FILES_MATCHING PATTERN "*.hpp"
	PATTERN ".svn" EXCLUDE)

#Install the oracle files for testing dot output
install(DIRECTORY ./set_relation/dotTest 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/test_data
	FILES_MATCHING 
	PATTERN "*.in.*"
        PATTERN "*.sh"
        PATTERN ".svn" EXCLUDE)

# print install message
install (CODE "MESSAGE (\"\n\nSet IEGEN_HOME to ${CMAKE_INSTALL_PREFIX}\n\n\")")                                                          
############################### STEP 5 ########################################
######################  Add testing Code ######################################


## copy some needed data over for testing
add_custom_command(TARGET iegenlib_t POST_BUILD
  COMMAND mkdir -p ../test_data/dotTest
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/set_relation/dotTest/*
              ../test_data/dotTest)
add_custom_command(TARGET run_iegen_tests POST_BUILD
  COMMAND mkdir -p ../test_data/islTest
  COMMAND cp  ${CMAKE_CURRENT_SOURCE_DIR}/iegen/loopnest/islTest/*
              ../test_data/islTest)

## add the test executable as a test in ctest
add_test(iegenlib_unit_test iegenlib_t)
set_property(TEST iegenlib_unit_test 
	     PROPERTY ENVIRONMENT "IEGEN_HOME=${PROJECT_BINARY_DIR}")
