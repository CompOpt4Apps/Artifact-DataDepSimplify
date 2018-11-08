#Ensure that we do an out of source buld

MACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD MSG)
  STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
  GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
  STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir)
  IF( insourcesubdir)
    MESSAGE(FATAL_ERROR "${MSG} If you believe that you are attempting an out of source build be sure to make sure that you source directory does not have a CMakeCache.txt file.")
  ENDIF( insourcesubdir)
  IF(insource )
    MESSAGE(FATAL_ERROR "${MSG} If you believe that you are attempting an out of source build be sure to make sure that you source directory does not have a CMakeCache.txt file.")
  ENDIF(insource) 
ENDMACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD)
