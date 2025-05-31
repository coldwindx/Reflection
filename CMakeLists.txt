cmake_minimum_required(VERSION 3.17)
project(Reflection)
# Find CMake file for Clang
FIND_LIBRARY(LIBCLANG_PATH clang)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-fno-rtti)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using Clang ${Clang_VERSION}")
# Add path to LLVM modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${LLVM_CMAKE_DIR}"
  )

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
FILE(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_definitions(${LLVM_DEFINITIONS})
add_definitions(${CLANG_DEFINITIONS})

add_executable(main main.cpp ${SRC_LIST})
set_property(TARGET main PROPERTY CXX_STANDARD 17)
target_link_libraries(main ${LIBCLANG_PATH})