cmake_minimum_required(VERSION 3.5)

project("Quark Compiler" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Find LLVM
find_package(LLVM 12.0 REQUIRED CONFIG)

# Add necesary include files
include_directories(include)
include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

# Add librararies and tools
add_subdirectory(lib)
add_subdirectory(tools)
