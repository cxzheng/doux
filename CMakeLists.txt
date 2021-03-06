# ******************************************************************************
#  CMakeLists.txt -- This file is part of Doux, a realtime softbody simulation library
# 
#  This is the main CMake build file used to compile doux
#
#  Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
# 
#  This Source Code Form is subject to the terms of the Mozilla Public License
#  v. 2.0. If a copy of the MPL was not distributed with this file, You can
#  obtain one at http://mozilla.org/MPL/2.0/.
# ******************************************************************************

cmake_minimum_required (VERSION 3.20.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_BINARY_DIR})

include(HunterGate)
HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.320.tar.gz"
  SHA1 "9b4e732afd22f40482c11ad6342f7d336634226f"
)

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
option(DOUX_BUILD_BENCH  "Build benchmakrs" OFF)
# option(DOUX_WITH_CUDA  "Build GPU simulation on CUDA" OFF)
# option(DOUX_USE_TBB  "Use Intel TBB for parallel computing" OFF)

# ---------- Check for dependencies ----------
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        origin/master
)

set(SPDLOG_BUILD_TESTING OFF CACHE BOOL " " FORCE)
set(SPDLOG_BUILD_SHARED ON CACHE BOOL " " FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.9.2
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

set(EXT_LIBS fmt spdlog googletest)

# Enable Google benchmark if needed
if (DOUX_BUILD_BENCH)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.6.0
  )
  
  list(APPEND EXT_LIBS benchmark)
endif()

FetchContent_MakeAvailable(${EXT_LIBS})

# ----------------------------------
if (DOUX_USE_FLOAT64)
  set(doux_Float_Precision double)
  add_definitions(-DDOUX_USE_FLOAT64)
else()
  set(doux_Float_Precision float)
endif()

if (POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW) # add_test() supports arbitrary characters
endif()

include(CheckCXXCompilerFlag)

# ---------- Generate version header files ----------
configure_file(${PROJECT_SOURCE_DIR}/include/doux/doux.h.in ${PROJECT_SOURCE_DIR}/include/doux/doux.h)

set(SRC_INC_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_IMPL_DIR "${CMAKE_SOURCE_DIR}/src")

if (DOUX_BUILD_FOR_NATIVE) 
  CHECK_CXX_COMPILER_FLAG("-ffast-math -march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
  if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -march=native")
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

if (DOUX_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if (DOUX_BUILD_BENCH)
  add_subdirectory(bench)
endif()
