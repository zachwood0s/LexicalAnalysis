# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical

# Include any dependencies generated for this target.
include CMakeFiles/compiler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compiler.dir/flags.make

CMakeFiles/compiler.dir/src/main.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compiler.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/main.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp

CMakeFiles/compiler.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp > CMakeFiles/compiler.dir/src/main.cpp.i

CMakeFiles/compiler.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/main.cpp -o CMakeFiles/compiler.dir/src/main.cpp.s

CMakeFiles/compiler.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/compiler.dir/src/main.cpp.o.requires

CMakeFiles/compiler.dir/src/main.cpp.o.provides: CMakeFiles/compiler.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/compiler.dir/build.make CMakeFiles/compiler.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/compiler.dir/src/main.cpp.o.provides

CMakeFiles/compiler.dir/src/main.cpp.o.provides.build: CMakeFiles/compiler.dir/src/main.cpp.o


CMakeFiles/compiler.dir/src/parser.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/parser.cpp.o: src/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compiler.dir/src/parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/parser.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp

CMakeFiles/compiler.dir/src/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp > CMakeFiles/compiler.dir/src/parser.cpp.i

CMakeFiles/compiler.dir/src/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/parser.cpp -o CMakeFiles/compiler.dir/src/parser.cpp.s

CMakeFiles/compiler.dir/src/parser.cpp.o.requires:

.PHONY : CMakeFiles/compiler.dir/src/parser.cpp.o.requires

CMakeFiles/compiler.dir/src/parser.cpp.o.provides: CMakeFiles/compiler.dir/src/parser.cpp.o.requires
	$(MAKE) -f CMakeFiles/compiler.dir/build.make CMakeFiles/compiler.dir/src/parser.cpp.o.provides.build
.PHONY : CMakeFiles/compiler.dir/src/parser.cpp.o.provides

CMakeFiles/compiler.dir/src/parser.cpp.o.provides.build: CMakeFiles/compiler.dir/src/parser.cpp.o


CMakeFiles/compiler.dir/src/lexar.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/lexar.cpp.o: src/lexar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/compiler.dir/src/lexar.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/lexar.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp

CMakeFiles/compiler.dir/src/lexar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/lexar.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp > CMakeFiles/compiler.dir/src/lexar.cpp.i

CMakeFiles/compiler.dir/src/lexar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/lexar.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/lexar.cpp -o CMakeFiles/compiler.dir/src/lexar.cpp.s

CMakeFiles/compiler.dir/src/lexar.cpp.o.requires:

.PHONY : CMakeFiles/compiler.dir/src/lexar.cpp.o.requires

CMakeFiles/compiler.dir/src/lexar.cpp.o.provides: CMakeFiles/compiler.dir/src/lexar.cpp.o.requires
	$(MAKE) -f CMakeFiles/compiler.dir/build.make CMakeFiles/compiler.dir/src/lexar.cpp.o.provides.build
.PHONY : CMakeFiles/compiler.dir/src/lexar.cpp.o.provides

CMakeFiles/compiler.dir/src/lexar.cpp.o.provides.build: CMakeFiles/compiler.dir/src/lexar.cpp.o


CMakeFiles/compiler.dir/src/print_ast.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/print_ast.cpp.o: src/print_ast.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/compiler.dir/src/print_ast.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/print_ast.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/print_ast.cpp

CMakeFiles/compiler.dir/src/print_ast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/print_ast.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/print_ast.cpp > CMakeFiles/compiler.dir/src/print_ast.cpp.i

CMakeFiles/compiler.dir/src/print_ast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/print_ast.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/print_ast.cpp -o CMakeFiles/compiler.dir/src/print_ast.cpp.s

CMakeFiles/compiler.dir/src/print_ast.cpp.o.requires:

.PHONY : CMakeFiles/compiler.dir/src/print_ast.cpp.o.requires

CMakeFiles/compiler.dir/src/print_ast.cpp.o.provides: CMakeFiles/compiler.dir/src/print_ast.cpp.o.requires
	$(MAKE) -f CMakeFiles/compiler.dir/build.make CMakeFiles/compiler.dir/src/print_ast.cpp.o.provides.build
.PHONY : CMakeFiles/compiler.dir/src/print_ast.cpp.o.provides

CMakeFiles/compiler.dir/src/print_ast.cpp.o.provides.build: CMakeFiles/compiler.dir/src/print_ast.cpp.o


CMakeFiles/compiler.dir/src/codegen_ast.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/codegen_ast.cpp.o: src/codegen_ast.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/compiler.dir/src/codegen_ast.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compiler.dir/src/codegen_ast.cpp.o -c /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/codegen_ast.cpp

CMakeFiles/compiler.dir/src/codegen_ast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/codegen_ast.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/codegen_ast.cpp > CMakeFiles/compiler.dir/src/codegen_ast.cpp.i

CMakeFiles/compiler.dir/src/codegen_ast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/codegen_ast.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/src/codegen_ast.cpp -o CMakeFiles/compiler.dir/src/codegen_ast.cpp.s

CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.requires:

.PHONY : CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.requires

CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.provides: CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.requires
	$(MAKE) -f CMakeFiles/compiler.dir/build.make CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.provides.build
.PHONY : CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.provides

CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.provides.build: CMakeFiles/compiler.dir/src/codegen_ast.cpp.o


# Object files for target compiler
compiler_OBJECTS = \
"CMakeFiles/compiler.dir/src/main.cpp.o" \
"CMakeFiles/compiler.dir/src/parser.cpp.o" \
"CMakeFiles/compiler.dir/src/lexar.cpp.o" \
"CMakeFiles/compiler.dir/src/print_ast.cpp.o" \
"CMakeFiles/compiler.dir/src/codegen_ast.cpp.o"

# External object files for target compiler
compiler_EXTERNAL_OBJECTS =

compiler: CMakeFiles/compiler.dir/src/main.cpp.o
compiler: CMakeFiles/compiler.dir/src/parser.cpp.o
compiler: CMakeFiles/compiler.dir/src/lexar.cpp.o
compiler: CMakeFiles/compiler.dir/src/print_ast.cpp.o
compiler: CMakeFiles/compiler.dir/src/codegen_ast.cpp.o
compiler: CMakeFiles/compiler.dir/build.make
compiler: /usr/local/lib/libLLVMSupport.a
compiler: /usr/local/lib/libLLVMCore.a
compiler: /usr/local/lib/libLLVMIRReader.a
compiler: /usr/local/lib/libLLVMAsmParser.a
compiler: /usr/local/lib/libLLVMBitReader.a
compiler: /usr/local/lib/libLLVMCore.a
compiler: /usr/local/lib/libLLVMBinaryFormat.a
compiler: /usr/local/lib/libLLVMSupport.a
compiler: /usr/local/lib/libLLVMDemangle.a
compiler: CMakeFiles/compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable compiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compiler.dir/build: compiler

.PHONY : CMakeFiles/compiler.dir/build

CMakeFiles/compiler.dir/requires: CMakeFiles/compiler.dir/src/main.cpp.o.requires
CMakeFiles/compiler.dir/requires: CMakeFiles/compiler.dir/src/parser.cpp.o.requires
CMakeFiles/compiler.dir/requires: CMakeFiles/compiler.dir/src/lexar.cpp.o.requires
CMakeFiles/compiler.dir/requires: CMakeFiles/compiler.dir/src/print_ast.cpp.o.requires
CMakeFiles/compiler.dir/requires: CMakeFiles/compiler.dir/src/codegen_ast.cpp.o.requires

.PHONY : CMakeFiles/compiler.dir/requires

CMakeFiles/compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compiler.dir/clean

CMakeFiles/compiler.dir/depend:
	cd /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical /home/zach/Documents/Projects/C++/ProgrammingLanguagesAndCompilers/FirstLexical/CMakeFiles/compiler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/compiler.dir/depend
