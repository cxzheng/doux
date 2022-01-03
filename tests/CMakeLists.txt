# ******************************************************************************
#  CMakeLists.txt -- This file is part of Doux, a realtime softbody simulation library
# 
#  Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
# 
#  This Source Code Form is subject to the terms of the Mozilla Public License
#  v. 2.0. If a copy of the MPL was not distributed with this file, You can
#  obtain one at http://mozilla.org/MPL/2.0/.
# ******************************************************************************
set(TEST_SRC 
    test_svec.cpp       test_shape.cpp
    test_softbody.cpp   test_constraint.cpp 
    test_mesh.cpp       test_motion_preset.cpp
    test_elasty.cpp     test_motion_preset.cpp
    test_eigen.cpp
)

set(TEST_LINK_LIBS
    doux::core        doux::pd
    GTest::gtest      GTest::gtest_main)

#if (DOUX_USE_TBB) 
#    list(APPEND TEST_SRC test_par_fdtd_sim.cpp)
#    list(APPEND TEST_LINK_LIBS TBB::tbb)
#endif()

#if (DOUX_WITH_CUDA)
#    list(APPEND TEST_LINK_LIBS acacia::em_cuda MAGMA::MAGMA CUDA::cublas) # MKL::MKL)
#endif()

add_executable(tests ${TEST_SRC})
target_include_directories(tests
  PRIVATE
    ${SRC_IMPL_DIR}
)
target_link_libraries(tests 
  PRIVATE 
    ${TEST_LINK_LIBS})

# Disable clang-tidy for benchmarks
set_target_properties(tests
  PROPERTIES
    CXX_CLANG_TIDY "")

# ----------------------------------------------------
add_executable(test_misc test_misc.cpp)
target_link_libraries(test_misc 
  PRIVATE
    doux::core)
