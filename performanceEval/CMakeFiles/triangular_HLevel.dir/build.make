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
CMAKE_SOURCE_DIR = /home/kingmahdi/workstation/simplification_kazem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kingmahdi/workstation/simplification_kazem

# Include any dependencies generated for this target.
include CMakeFiles/triangular_HLevel.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/triangular_HLevel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/triangular_HLevel.dir/flags.make

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o: CMakeFiles/triangular_HLevel.dir/flags.make
CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o: test/triangular_ordered.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kingmahdi/workstation/simplification_kazem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o -c /home/kingmahdi/workstation/simplification_kazem/test/triangular_ordered.cpp

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kingmahdi/workstation/simplification_kazem/test/triangular_ordered.cpp > CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.i

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kingmahdi/workstation/simplification_kazem/test/triangular_ordered.cpp -o CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.s

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.requires:

.PHONY : CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.requires

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.provides: CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.requires
	$(MAKE) -f CMakeFiles/triangular_HLevel.dir/build.make CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.provides.build
.PHONY : CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.provides

CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.provides.build: CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o


# Object files for target triangular_HLevel
triangular_HLevel_OBJECTS = \
"CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o"

# External object files for target triangular_HLevel
triangular_HLevel_EXTERNAL_OBJECTS =

triangular_HLevel: CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o
triangular_HLevel: CMakeFiles/triangular_HLevel.dir/build.make
triangular_HLevel: CMakeFiles/triangular_HLevel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kingmahdi/workstation/simplification_kazem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable triangular_HLevel"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/triangular_HLevel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/triangular_HLevel.dir/build: triangular_HLevel

.PHONY : CMakeFiles/triangular_HLevel.dir/build

CMakeFiles/triangular_HLevel.dir/requires: CMakeFiles/triangular_HLevel.dir/test/triangular_ordered.cpp.o.requires

.PHONY : CMakeFiles/triangular_HLevel.dir/requires

CMakeFiles/triangular_HLevel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/triangular_HLevel.dir/cmake_clean.cmake
.PHONY : CMakeFiles/triangular_HLevel.dir/clean

CMakeFiles/triangular_HLevel.dir/depend:
	cd /home/kingmahdi/workstation/simplification_kazem && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem/CMakeFiles/triangular_HLevel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/triangular_HLevel.dir/depend

