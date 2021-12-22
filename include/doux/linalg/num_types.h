#pragma once

#include <complex>
#include "Eigen/Core"
#include "doux/doux.h"

NAMESPACE_BEGIN(doux::linalg)

using complex_t = std::complex<real_t>;

// matrix of real values
using matrix_r_t = Eigen::Matrix<real_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_r_t = Eigen::Matrix<real_t, Eigen::Dynamic, 1>;

using mat2_r_t = Eigen::Matrix<real_t, 2, 2>;
using vec2_r_t = Eigen::Matrix<real_t, 2, 1>;

using matrix_ui_t = Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_ui_t = Eigen::Matrix<uint32_t, Eigen::Dynamic, 1>;

using matrix_i_t = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;
using vector_i_t = Eigen::Matrix<int, Eigen::Dynamic, 1>;

// matrix of complex values
using matrix_c_t = Eigen::Matrix<complex_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_c_t = Eigen::Matrix<complex_t, Eigen::Dynamic, 1>;

NAMESPACE_END(doux::linalg)