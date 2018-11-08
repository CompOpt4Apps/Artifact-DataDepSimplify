#A few definitions for glog
add_definitions(-DHAVE_CONFIG_H -Wwrite-strings -Woverloaded-virtual -Wno-sign-compare -DNO_FRAME_POINTER)

#Compile glog as a static library
add_library(glog STATIC logging.cc raw_logging.cc vlog_is_on.cc utilities.cc demangle.cc symbolize.cc signalhandler.cc)
