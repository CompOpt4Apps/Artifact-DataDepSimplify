# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kingmahdi/workstation/simplification

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kingmahdi/workstation/simplification

# Include any dependencies generated for this target.
include CMakeFiles/ic0_csc_performance.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ic0_csc_performance.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ic0_csc_performance.dir/flags.make

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o: CMakeFiles/ic0_csc_performance.dir/flags.make
CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o: test/ic0_csc_performance.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kingmahdi/workstation/simplification/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o -c /home/kingmahdi/workstation/simplification/test/ic0_csc_performance.cpp

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kingmahdi/workstation/simplification/test/ic0_csc_performance.cpp > CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.i

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kingmahdi/workstation/simplification/test/ic0_csc_performance.cpp -o CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.s

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.requires:

.PHONY : CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.requires

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.provides: CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.requires
	$(MAKE) -f CMakeFiles/ic0_csc_performance.dir/build.make CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.provides.build
.PHONY : CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.provides

CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.provides.build: CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o


# Object files for target ic0_csc_performance
ic0_csc_performance_OBJECTS = \
"CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o"

# External object files for target ic0_csc_performance
ic0_csc_performance_EXTERNAL_OBJECTS =

ic0_csc_performance: CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o
ic0_csc_performance: CMakeFiles/ic0_csc_performance.dir/build.make
ic0_csc_performance: CMakeFiles/ic0_csc_performance.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kingmahdi/workstation/simplification/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ic0_csc_performance"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ic0_csc_performance.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ic0_csc_performance.dir/build: ic0_csc_performance

.PHONY : CMakeFiles/ic0_csc_performance.dir/build

CMakeFiles/ic0_csc_performance.dir/requires: CMakeFiles/ic0_csc_performance.dir/test/ic0_csc_performance.cpp.o.requires

.PHONY : CMakeFiles/ic0_csc_performance.dir/requires

CMakeFiles/ic0_csc_performance.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ic0_csc_performance.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ic0_csc_performance.dir/clean

CMakeFiles/ic0_csc_performance.dir/depend:
	cd /home/kingmahdi/workstation/simplification && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kingmahdi/workstation/simplification /home/kingmahdi/workstation/simplification /home/kingmahdi/workstation/simplification /home/kingmahdi/workstation/simplification /home/kingmahdi/workstation/simplification/CMakeFiles/ic0_csc_performance.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ic0_csc_performance.dir/depend

