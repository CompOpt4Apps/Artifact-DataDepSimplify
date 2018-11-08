#Only build bindings if requested to
if(${BUILD_PYTHON})
  #Search for SWIG
  find_package(SWIG REQUIRED)

  #Check if we actually found SWIG
  if(not ${FOUND_SWIG})
    message(STATUS "SWIG Not Found")
  #SWIG was found, so build the Python bindings
  else()

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/example.i "%module example %{%}")
    execute_process(COMMAND ${SWIG_EXECUTABLE} -python
    ${CMAKE_CURRENT_BINARY_DIR}/example.i
    ERROR_VARIABLE SWIG_PYTHON_ERR)
    if (SWIG_PYTHON_ERR)
      message(FATAL_ERROR "Swig installation doesn't support Python wrapper generation: ${SWIG_PYTHON_ERR}")
    endif ()

    message(STATUS "Found SWIG version ${SWIG_VERSION}: ${SWIG_EXECUTABLE} ${SWIG_DIR}")

    #Find the Python headers and libraries to link to
    find_package(MyPythonLibs)
    #find_package(PythonLibs)

    message(STATUS "Found Python version ${PYTHON_VERSION}: ${PYTHON_PREFIX}")
    message(STATUS "Python includes: ${PYTHON_INCLUDES}")
    #message(STATUS "Python libs: ${PYTHON_LIBRARIES}")

    #Run swig to produce the wrapper python and c++ code
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/iegenlib.py ${CMAKE_CURRENT_BINARY_DIR}/iegenlib_wrap.cc
      COMMAND ${SWIG_EXECUTABLE}
      ARGS -python -c++ -outdir ${CMAKE_CURRENT_BINARY_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/iegenlib_wrap.cc ${CMAKE_CURRENT_SOURCE_DIR}/iegenlib.i
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/iegenlib.i iegenlib
      COMMENT "Compiling swig interface file iegenlib.i for Python")
     
    include_directories(${PYTHON_INCLUDES})
    add_library(_iegenlib SHARED ${CMAKE_CURRENT_BINARY_DIR}/iegenlib_wrap.cc)
    #Link _iegenlib to the Python libraries
    target_link_libraries(_iegenlib iegenlib ${PYTHON_LIBRARIES})
    target_link_libraries(_iegenlib cloog-isl isl gmp)
    #Add Link Flags when the code is built under MAC OSX
    if(APPLE)
    set_target_properties(_iegenlib PROPERTIES LINK_FLAGS "-flat_namespace -undefined suppress")
    endif()
    set_target_properties(_iegenlib PROPERTIES PREFIX "")
    set_target_properties(_iegenlib PROPERTIES SUFFIX ".so")

    install(TARGETS _iegenlib
            LIBRARY DESTINATION bindings)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/iegenlib.py
            DESTINATION bindings)

    # print install message
    install (CODE "MESSAGE (\"\n\nSet IEGEN_HOME to ${CMAKE_INSTALL_PREFIX}\n\n\")")
  endif()
endif()
