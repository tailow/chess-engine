# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Projects\C++\seggz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Projects\C++\seggz\build

# Include any dependencies generated for this target.
include CMakeFiles/seggz.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/seggz.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/seggz.dir/flags.make

CMakeFiles/seggz.dir/src/main.cpp.obj: CMakeFiles/seggz.dir/flags.make
CMakeFiles/seggz.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Projects\C++\seggz\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/seggz.dir/src/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\seggz.dir\src\main.cpp.obj -c C:\Projects\C++\seggz\src\main.cpp

CMakeFiles/seggz.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/seggz.dir/src/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Projects\C++\seggz\src\main.cpp > CMakeFiles\seggz.dir\src\main.cpp.i

CMakeFiles/seggz.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/seggz.dir/src/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-W\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Projects\C++\seggz\src\main.cpp -o CMakeFiles\seggz.dir\src\main.cpp.s

# Object files for target seggz
seggz_OBJECTS = \
"CMakeFiles/seggz.dir/src/main.cpp.obj"

# External object files for target seggz
seggz_EXTERNAL_OBJECTS =

seggz.exe: CMakeFiles/seggz.dir/src/main.cpp.obj
seggz.exe: CMakeFiles/seggz.dir/build.make
seggz.exe: CMakeFiles/seggz.dir/linklibs.rsp
seggz.exe: CMakeFiles/seggz.dir/objects1.rsp
seggz.exe: CMakeFiles/seggz.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Projects\C++\seggz\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable seggz.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\seggz.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/seggz.dir/build: seggz.exe

.PHONY : CMakeFiles/seggz.dir/build

CMakeFiles/seggz.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\seggz.dir\cmake_clean.cmake
.PHONY : CMakeFiles/seggz.dir/clean

CMakeFiles/seggz.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Projects\C++\seggz C:\Projects\C++\seggz C:\Projects\C++\seggz\build C:\Projects\C++\seggz\build C:\Projects\C++\seggz\build\CMakeFiles\seggz.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/seggz.dir/depend

