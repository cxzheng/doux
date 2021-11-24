#set(TEST_SRC 
#    test_svec.cpp           test_point.cpp
#    test_shape.cpp          test_fdtd_field.cpp
#    test_gaussian.cpp       test_fdtd_src.cpp
#    test_fdtd_sim.cpp       test_parser.cpp
#    test_format.cpp         test_timer.cpp
#    test_mat_space.cpp      test_matrix.cpp
#    test_pml_params.cpp     test_rcwa_layer.cpp
#    test_quaternion.cpp     test_pml.cpp
#)

#set(TEST_LINK_LIBS
#    doux::core  
#    GTest::gtest
#    GTest::gtest_main)

#if (ACA_USE_TBB) 
#    list(APPEND TEST_SRC test_par_fdtd_sim.cpp)
#    list(APPEND TEST_LINK_LIBS TBB::tbb)
#endif()

#if (ACA_WITH_CUDA)
#    list(APPEND TEST_LINK_LIBS acacia::em_cuda MAGMA::MAGMA CUDA::cublas) # MKL::MKL)
#endif()

#add_executable(tests ${TEST_SRC})
#target_link_libraries(tests 
#  PRIVATE 
#    ${TEST_LINK_LIBS})
#target_include_directories(tests
#  PRIVATE
#    ${SRC_IMPL_DIR}
#)

## Disable clang-tidy for benchmarks
#set_target_properties(tests
#  PROPERTIES
#    CXX_CLANG_TIDY "")

# ----------------------------------------------------
add_executable(test_misc test_misc.cpp)
target_link_libraries(test_misc 
  PRIVATE
    doux::core)
