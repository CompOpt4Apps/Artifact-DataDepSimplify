## This is the CMakeLists file for the doc directory.
## Documentation is not generated as part of make all or make install,
## you have to explicitly call make docs from the directory above this one.
## 
## The value of the CMAKE_SOURCE_DIR is IEGen, where src exists and build
## is created. Thus, the build will create documents in the doc directory at
## this level. Doxygen works by visiting all the source files in the src
## directory.

add_custom_target(docs COMMAND doxygen doc/doxyconfig WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
