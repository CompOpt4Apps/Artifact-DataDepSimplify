#Tell gtest it has pthreads support
add_definitions(-DGTEST_HAS_PTHREAD=0)

#Tell gtest to NOT use tuple support.
add_definitions(-DGTEST_HAS_TR1_TUPLE=0)

#Compile gtests as a static library
add_library(gtest STATIC gtest-all.cc)
