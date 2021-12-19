#pragma once

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
std::pair<real_t, real_t> lame_coeff(real_t youngs_modulus, real_t poisson_ratio) {  // tested
  assert(poisson_ratio >= 0 && poisson_ratio <= 0.5 && youngs_modulus > 0);

  auto sv = (real_t)1 / ((real_t)1 + poisson_ratio);
  return std::make_pair(youngs_modulus * poisson_ratio * sv / ((real_t)1 - poisson_ratio * (real_t)2),
                        youngs_modulus * sv * 0.5);
}

NAMESPACE_END(doux::elasty)