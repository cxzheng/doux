//******************************************************************************
// num_types.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include <complex>
#include <Eigen/Core>
#include "doux/doux.h"

NAMESPACE_BEGIN(doux::linalg)

using complex_t = std::complex<real_t>;

// matrix of real values
using matrix_r_t = Eigen::Matrix<real_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_r_t = Eigen::Matrix<real_t, Eigen::Dynamic, 1>;

using mat2_r_t = Eigen::Matrix<real_t, 2, 2>;
using vec2_r_t = Eigen::Matrix<real_t, 2, 1>;
using mat2_f_t = Eigen::Matrix<float, 2, 2>;
using vec2_f_t = Eigen::Matrix<float, 2, 1>;
using mat2_d_t = Eigen::Matrix<double, 2, 2>;
using vec2_d_t = Eigen::Matrix<double, 2, 1>;

using mat3_r_t = Eigen::Matrix<real_t, 3, 3>;
using vec3_r_t = Eigen::Matrix<real_t, 3, 1>;
using mat3_f_t = Eigen::Matrix<float, 3, 3>;
using vec3_f_t = Eigen::Matrix<float, 3, 1>;
using mat3_d_t = Eigen::Matrix<double, 3, 3>;
using vec3_d_t = Eigen::Matrix<double, 3, 1>;

using matrix_ui_t = Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_ui_t = Eigen::Matrix<uint32_t, Eigen::Dynamic, 1>;

using matrix_i_t = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic>;
using vector_i_t = Eigen::Matrix<int, Eigen::Dynamic, 1>;

// matrix of complex values
using matrix_c_t = Eigen::Matrix<complex_t, Eigen::Dynamic, Eigen::Dynamic>;
using vector_c_t = Eigen::Matrix<complex_t, Eigen::Dynamic, 1>;

NAMESPACE_END(doux::linalg)