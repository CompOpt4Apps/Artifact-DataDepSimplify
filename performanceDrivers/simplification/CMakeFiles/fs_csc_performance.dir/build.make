# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/kazem/programs/clion-2017.3.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/kazem/programs/clion-2017.3.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification

# Include any dependencies generated for this target.
include CMakeFiles/fs_csc_performance.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fs_csc_performance.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fs_csc_performance.dir/flags.make

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o: CMakeFiles/fs_csc_performance.dir/flags.make
CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o: test/fs_csc_performance.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o -c /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/test/fs_csc_performance.cpp

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/test/fs_csc_performance.cpp > CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.i

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/test/fs_csc_performance.cpp -o CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.s

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.requires:

.PHONY : CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.requires

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.provides: CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.requires
	$(MAKE) -f CMakeFiles/fs_csc_performance.dir/build.make CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.provides.build
.PHONY : CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.provides

CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.provides.build: CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o


# Object files for target fs_csc_performance
fs_csc_performance_OBJECTS = \
"CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o"

# External object files for target fs_csc_performance
fs_csc_performance_EXTERNAL_OBJECTS =

fs_csc_performance: CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o
fs_csc_performance: CMakeFiles/fs_csc_performance.dir/build.make
fs_csc_performance: CMakeFiles/fs_csc_performance.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fs_csc_performance"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fs_csc_performance.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fs_csc_performance.dir/build: fs_csc_performance

.PHONY : CMakeFiles/fs_csc_performance.dir/build

CMakeFiles/fs_csc_performance.dir/requires: CMakeFiles/fs_csc_performance.dir/test/fs_csc_performance.cpp.o.requires

.PHONY : CMakeFiles/fs_csc_performance.dir/requires

CMakeFiles/fs_csc_performance.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fs_csc_performance.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fs_csc_performance.dir/clean

CMakeFiles/fs_csc_performance.dir/depend:
	cd /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification /home/kazem/development/Artifact-DataDepSimplify/performanceDrivers/simplification/CMakeFiles/fs_csc_performance.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fs_csc_performance.dir/depend

