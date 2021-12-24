#pragma once

#include <Eigen/Core>
#include "doux/core/platform.h"
#include <assert.h>

// References:
//
// [1] Eftychios Sifakis and Jernej Barbic. 2012. FEM simulation of 3D deformable solids: a practitioner's guide to
// theory, discretization and model reduction. In ACM SIGGRAPH 2012 Courses (SIGGRAPH '12). Association for Computing
// Machinery, New York, NY, USA, Article 20, 1–50. DOI:https://doi.org/10.1145/2343483.2343501
//

NAMESPACE_BEGIN(doux::elasty)

// See sec. 3.2 in [1]
// return \lambda, \mu
// \lambda: first Lame coefficient 
// \mu: second Lame coefficient 
inline std::pair<real_t, real_t> lame_coeff(real_t youngs_modulus, real_t poisson_ratio) {  // tested
  assert(poisson_ratio >= 0 && poisson_ratio <= 0.5 && youngs_modulus > 0);

  auto sv = (real_t)1 / ((real_t)1 + poisson_ratio);
  return std::make_pair(youngs_modulus * poisson_ratio * sv / ((real_t)1 - poisson_ratio * (real_t)2),
                        youngs_modulus * sv * 0.5);
}

/*
 * Calculate the Green strain tensor for a finite element.
 *
 * \param F The deformation gradient matrix, which should be either 2x2 (2D element in 2D), 3x3 (3D
 * element in 3D), or 3x2 (2D element in 3D).
 */
template <typename Derived>
Eigen::Matrix<typename Derived::Scalar, Derived::ColsAtCompileTime, Derived::ColsAtCompileTime>
green_strain(const Eigen::MatrixBase<Derived>& F)
{
    using Mat = Eigen::Matrix<typename Derived::Scalar, Derived::ColsAtCompileTime, Derived::ColsAtCompileTime>;

    return 0.5 * (F.transpose() * F - Mat::Identity());
}

/*
 * See Eq. (3.2) in [1]
 * F:     deformation gradient
 * lame1: first Lame parameter
 * lame2: second Lame parameter
 */
template <typename Derived>
typename Derived::Scalar stvk_energy_density(const Eigen::MatrixBase<Derived>& F, 
                                             const typename Derived::Scalar lame1,
                                             const typename Derived::Scalar lame2) {
  auto const E = green_strain(F);
  auto const t = E.trace();

  return lame2 * E.squaredNorm() + 0.5 * lame1 * t * t;
}

NAMESPACE_END(doux::elasty)