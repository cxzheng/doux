# This is the main CMake build file used to compile doux

cmake_minimum_required (VERSION 3.20.0)
project(doux)

set(doux_VERSION_MAJOR 0)
set(doux_VERSION_MINOR 0)
set(doux_VERSION_PATCH 1)

# ---------- User options ----------
option(DOUX_BUILD_TESTS  "Build unit tests" ON)
option(DOUX_BUILD_FOR_NATIVE "Build with -march=native" ON)
option(DOUX_USE_FLOAT64  "Use double precision computation" OFF)
option(DOUX_USE_CPPCHECK "Run cppcheck for static analysis" OFF)
option(DOUX_USE_CLANG_TIDY "Run clang-tidy for static analysis" OFF)

option(DOUX_ENABLE_STDOUT "Display messages on the console" ON)
# option(DOUX_BUILD_BENCH  "Build benchmakrs" OFF)
# option(DOUX_WITH_CUDA  "Build GPU simulation on CUDA" OFF)
# option(DOUX_USE_TBB  "Use Intel TBB for parallel computing" OFF)

# ---------- Check for submodules ----------

# ----------------------------------
if (DOUX_USE_FLOAT64)
  set(doux_Float_Precision double)
  add_definitions(-DDOUX_USE_FLOAT64)
else()
  set(doux_Float_Precision float)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_BINARY_DIR})

if (POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW) # add_test() supports arbitrary characters
endif()

include(CheckCXXCompilerFlag)

# ---------- Generate version header files ----------
configure_file(${PROJECT_SOURCE_DIR}/include/doux/doux.h.in ${PROJECT_SOURCE_DIR}/include/doux/doux.h)

set(SRC_INC_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_IMPL_DIR "${CMAKE_SOURCE_DIR}/src")

if (DOUX_BUILD_FOR_NATIVE) 
  CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
  if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
  endif()
endif()

# ---------- Setup CppCheck for static analysis -------------

if (DOUX_USE_CPPCHECK) 
  find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
  if (CMAKE_CXX_CPPCHECK)
    message(STATUS "Will use cppcheck for static code analysis")
    list(APPEND CMAKE_CXX_CPPCHECK
        "--enable=all"
        "--inline-suppr"
        "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
  else()
    message(WARNING "cppcheck is not found, and will be diabled")
    set(CMAKE_CXX_CPPCHECK "")
  endif()
else()
  set(CMAKE_CXX_CPPCHECK "")
endif()

# ---------- Setup clang-tidy for static analysis ------------
if (DOUX_USE_CLANG_TIDY)
  find_program(CMAKE_CXX_CLANG_TIDY NAMES "clang-tidy")
  if (CMAKE_CXX_CLANG_TIDY)
    message(STATUS "Will use clang-tidy for static code analysis")
  else()
    message(WARNING "clang-tidy is not found, and will be diabled")
    set(CMAKE_CXX_CLANG_TIDY "")
  endif()
else()
  set(CMAKE_CXX_CLANG_TIDY "")
endif()

# ---------- Set up ClangFormat ----------
find_package(ClangFormat)

if (CLANG_FORMAT_FOUND)
  set(CLANG_FORMAT_DIRECTORIES  "tests/" "src/" "bench/" "include/" "app/") 
  set(CLANG_FORMAT_FILE_EXTENSIONS ${CLANG_FORMAT_FILE_EXTENSIONS} ".cpp;.h;.cxx;.hxx;.hpp;.cc;.ipp")
  # A CMake script to find all source files and setup clang-format targets for them
  include(clang-format)
else()
  message(WARNING "clang-format not found. Not setting up format targets")
endif()

# ---------------------------------------------------
add_subdirectory(src)
