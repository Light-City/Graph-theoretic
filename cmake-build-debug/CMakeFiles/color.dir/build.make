# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/light/CLION/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/light/CLION/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/light/Graph-theoretic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/light/Graph-theoretic/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/color.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/color.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/color.dir/flags.make

CMakeFiles/color.dir/graphAlg/color.cpp.o: CMakeFiles/color.dir/flags.make
CMakeFiles/color.dir/graphAlg/color.cpp.o: ../graphAlg/color.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/light/Graph-theoretic/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/color.dir/graphAlg/color.cpp.o"
	/home/light/gcc8.3/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/color.dir/graphAlg/color.cpp.o -c /home/light/Graph-theoretic/graphAlg/color.cpp

CMakeFiles/color.dir/graphAlg/color.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/color.dir/graphAlg/color.cpp.i"
	/home/light/gcc8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/light/Graph-theoretic/graphAlg/color.cpp > CMakeFiles/color.dir/graphAlg/color.cpp.i

CMakeFiles/color.dir/graphAlg/color.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/color.dir/graphAlg/color.cpp.s"
	/home/light/gcc8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/light/Graph-theoretic/graphAlg/color.cpp -o CMakeFiles/color.dir/graphAlg/color.cpp.s

# Object files for target color
color_OBJECTS = \
"CMakeFiles/color.dir/graphAlg/color.cpp.o"

# External object files for target color
color_EXTERNAL_OBJECTS =

color: CMakeFiles/color.dir/graphAlg/color.cpp.o
color: CMakeFiles/color.dir/build.make
color: CMakeFiles/color.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/light/Graph-theoretic/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable color"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/color.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/color.dir/build: color

.PHONY : CMakeFiles/color.dir/build

CMakeFiles/color.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/color.dir/cmake_clean.cmake
.PHONY : CMakeFiles/color.dir/clean

CMakeFiles/color.dir/depend:
	cd /home/light/Graph-theoretic/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/light/Graph-theoretic /home/light/Graph-theoretic /home/light/Graph-theoretic/cmake-build-debug /home/light/Graph-theoretic/cmake-build-debug /home/light/Graph-theoretic/cmake-build-debug/CMakeFiles/color.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/color.dir/depend
