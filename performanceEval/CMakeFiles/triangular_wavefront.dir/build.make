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
include CMakeFiles/triangular_wavefront.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/triangular_wavefront.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/triangular_wavefront.dir/flags.make

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o: CMakeFiles/triangular_wavefront.dir/flags.make
CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o: test/fs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kingmahdi/workstation/simplification_kazem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o -c /home/kingmahdi/workstation/simplification_kazem/test/fs.cpp

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/triangular_wavefront.dir/test/fs.cpp.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kingmahdi/workstation/simplification_kazem/test/fs.cpp > CMakeFiles/triangular_wavefront.dir/test/fs.cpp.i

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/triangular_wavefront.dir/test/fs.cpp.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kingmahdi/workstation/simplification_kazem/test/fs.cpp -o CMakeFiles/triangular_wavefront.dir/test/fs.cpp.s

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.requires:

.PHONY : CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.requires

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.provides: CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.requires
	$(MAKE) -f CMakeFiles/triangular_wavefront.dir/build.make CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.provides.build
.PHONY : CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.provides

CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.provides.build: CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o


# Object files for target triangular_wavefront
triangular_wavefront_OBJECTS = \
"CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o"

# External object files for target triangular_wavefront
triangular_wavefront_EXTERNAL_OBJECTS =

triangular_wavefront: CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o
triangular_wavefront: CMakeFiles/triangular_wavefront.dir/build.make
triangular_wavefront: CMakeFiles/triangular_wavefront.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kingmahdi/workstation/simplification_kazem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable triangular_wavefront"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/triangular_wavefront.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/triangular_wavefront.dir/build: triangular_wavefront

.PHONY : CMakeFiles/triangular_wavefront.dir/build

CMakeFiles/triangular_wavefront.dir/requires: CMakeFiles/triangular_wavefront.dir/test/fs.cpp.o.requires

.PHONY : CMakeFiles/triangular_wavefront.dir/requires

CMakeFiles/triangular_wavefront.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/triangular_wavefront.dir/cmake_clean.cmake
.PHONY : CMakeFiles/triangular_wavefront.dir/clean

CMakeFiles/triangular_wavefront.dir/depend:
	cd /home/kingmahdi/workstation/simplification_kazem && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem /home/kingmahdi/workstation/simplification_kazem/CMakeFiles/triangular_wavefront.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/triangular_wavefront.dir/depend
