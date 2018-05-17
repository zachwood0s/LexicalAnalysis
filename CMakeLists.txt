cmake_minimum_required(VERSION 3.4.3)
set(CMAKE_BUILD_TYPE Debug)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_executable(compiler src/main.cpp src/parser.cpp src/lexar.cpp src/print_ast.cpp src/codegen_ast.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core bitwriter)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Link against LLVM libraries
target_link_libraries(compiler ${llvm_libs})
# $ clang++ -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs all` -o toy
#   clang++ -g toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native` -O3 -o toy
