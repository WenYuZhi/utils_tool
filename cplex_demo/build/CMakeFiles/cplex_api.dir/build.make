# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build

# Include any dependencies generated for this target.
include CMakeFiles/cplex_api.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cplex_api.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cplex_api.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cplex_api.dir/flags.make

CMakeFiles/cplex_api.dir/main.cpp.o: CMakeFiles/cplex_api.dir/flags.make
CMakeFiles/cplex_api.dir/main.cpp.o: ../main.cpp
CMakeFiles/cplex_api.dir/main.cpp.o: CMakeFiles/cplex_api.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cplex_api.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/cplex_api.dir/main.cpp.o -MF CMakeFiles/cplex_api.dir/main.cpp.o.d -o CMakeFiles/cplex_api.dir/main.cpp.o -c /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/main.cpp

CMakeFiles/cplex_api.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cplex_api.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/main.cpp > CMakeFiles/cplex_api.dir/main.cpp.i

CMakeFiles/cplex_api.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cplex_api.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/main.cpp -o CMakeFiles/cplex_api.dir/main.cpp.s

# Object files for target cplex_api
cplex_api_OBJECTS = \
"CMakeFiles/cplex_api.dir/main.cpp.o"

# External object files for target cplex_api
cplex_api_EXTERNAL_OBJECTS =

cplex_api: CMakeFiles/cplex_api.dir/main.cpp.o
cplex_api: CMakeFiles/cplex_api.dir/build.make
cplex_api: /home/wangyuan@sribd.cn/cplex_studio2210/cplex/lib/x86-64_linux/static_pic/libcplex.a
cplex_api: /home/wangyuan@sribd.cn/cplex_studio2210/cplex/lib/x86-64_linux/static_pic/libilocplex.a
cplex_api: CMakeFiles/cplex_api.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cplex_api"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cplex_api.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cplex_api.dir/build: cplex_api
.PHONY : CMakeFiles/cplex_api.dir/build

CMakeFiles/cplex_api.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cplex_api.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cplex_api.dir/clean

CMakeFiles/cplex_api.dir/depend:
	cd /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build /home/wangyuan@sribd.cn/codespace/utils_tool/cplex_demo/build/CMakeFiles/cplex_api.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cplex_api.dir/depend
