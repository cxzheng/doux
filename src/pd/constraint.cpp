#include <Eigen/LU>
#include "doux/core/math_func.h"
#include "doux/pd/softbody.h"
#include "doux/pd/constraint.h"
#include "doux/elasty/continuum.h"

#include <iostream>
#include "doux/core/format.h"

NAMESPACE_BEGIN(doux::pd)

[[nodiscard]] real_t DistCFunc::c() const {
  return (body_->vtx_pos(v_[0]) - body_->vtx_pos(v_[1])).norm() - d0_;
}

void DistCFunc::grad(std::span<real_t> grad_ret) {
#ifndef NDEBUG
  // In debug mode, check the output array size
  if (auto s = grad_ret.size(); s < 6) {
    throw std::out_of_range(
        fmt::format("Insufficient output array space:"
                    "L = {0:d}, but 6 is needed", s));
  }
#endif
  const auto v = body_->vtx_pos(v_[0]) - body_->vtx_pos(v_[1]);
  
  Vec3r ret;
  if ( auto nrm2 = v.norm2(); nrm2 < eps<real_t>::v ) [[unlikely]] {
    ret = Vec3r(static_cast<real_t>(rg_()), 
                static_cast<real_t>(rg_()),
                static_cast<real_t>(rg_())).normalize();
  } else {
    auto s = (real_t)1 / std::sqrt(nrm2);
    ret = v * s;
  }
  grad_ret[0] =  ret.x();
  grad_ret[1] =  ret.y();
  grad_ret[2] =  ret.z();
  grad_ret[3] = -ret.x();
  grad_ret[4] = -ret.y();
  grad_ret[5] = -ret.z();
}

// -------------------------------------------------------------------------------

[[nodiscard]] real_t UnitaryDistCFunc::c() const {
  return (body_->vtx_pos(v_) - p0_).norm() - d0_;
}

void UnitaryDistCFunc::grad(std::span<real_t> grad_ret) {
#ifndef NDEBUG
  // In debug mode, check the output array size
  if (auto s = grad_ret.size(); s < 3) {
    throw std::out_of_range(
        fmt::format("Insufficient output array space:"
                    "L = {0:d}, but 3 is needed", s));
  }
#endif
  const auto v = body_->vtx_pos(v_) - p0_;

  Vec3r ret;
  if ( auto nrm2 = v.norm2(); nrm2 < eps<real_t>::v ) [[unlikely]] {
    ret = Vec3r(static_cast<real_t>(rg_()), 
                static_cast<real_t>(rg_()),
                static_cast<real_t>(rg_())).normalize();
  } else {
    auto s = (real_t)1 / std::sqrt(nrm2); 
    ret = v * s;
  }
  grad_ret[0] =  ret.x();
  grad_ret[1] =  ret.y();
  grad_ret[2] =  ret.z();
}

// -------------------------------------------------------------------------------

StVKTriCFunc::StVKTriCFunc(Softbody* sb, uint32_t v0, uint32_t v1, uint32_t v2, 
                           real_t youngs_modulus, real_t poisson_ratio) : 
                           CFunc(sb), v_{v0, v1, v2} {
  assert(sb);
  // lame_coeff_ 
  auto const [a, b] = elasty::lame_coeff(youngs_modulus, poisson_ratio);
  lame_coeff_[0] = a;
  lame_coeff_[1] = b;

  // rest positions
  auto const& X0 = sb->vtx_pos(v0); // vec3r
  auto const& X1 = sb->vtx_pos(v1);
  auto const& X2 = sb->vtx_pos(v2);

  auto const X10 = X1 - X0;
  auto const X20 = X2 - X0;

  auto const cxs = cross(X10, X20);
  area_ = cxs.norm();
  assert(area_ > eps<real_t>::v);
  area_ *= 0.5;                     // triangle area
  auto const ax1 = X10.normalize(); // normalize X10
  auto const ax2 = cross(cxs, ax1).normalize();
  
  const linalg::vec2_r_t proj_x_0(ax1.dot(X0), ax2.dot(X0));
  const linalg::vec2_r_t proj_x_1(ax1.dot(X1), ax2.dot(X1));
  const linalg::vec2_r_t proj_x_2(ax1.dot(X2), ax2.dot(X2));

  linalg::mat2_r_t D;
  D.col(0) = proj_x_1 - proj_x_0; // x10
  D.col(1) = proj_x_2 - proj_x_0; // x10

  assert(D.determinant() > eps<real_t>::v);
  D_inv_ = D.inverse();
}

[[nodiscard]] real_t StVKTriCFunc::c() const {
  auto const& x0 = body_->vtx_pos(v_[0]); // vec3r
  auto const& x1 = body_->vtx_pos(v_[1]);
  auto const& x2 = body_->vtx_pos(v_[2]);

  auto const x10 = x1 - x0;
  auto const x20 = x2 - x0;

  Eigen::Matrix<real_t, 3, 2> D;
  D << x10.x(), x20.x(),
       x10.y(), x20.y(),
       x10.z(), x20.z();
  
  auto const F = D * D_inv_;  // Deformation gradient (3x2 matrix)
  // compute strain energy density
  return area_ * elasty::stvk_energy_density(F, lame_coeff_[0], lame_coeff_[1]);
}

void StVKTriCFunc::grad(std::span<real_t> grad_ret) {
#ifndef NDEBUG
  // In debug mode, check the output array size
  if (auto s = grad_ret.size(); s < 9) {
    throw std::out_of_range(
        fmt::format("Insufficient output array space:"
                    "L = {0:d}, but 9 is needed", s));
  }
#endif

  auto const& x0 = body_->vtx_pos(v_[0]); // vec3r
  auto const& x1 = body_->vtx_pos(v_[1]);
  auto const& x2 = body_->vtx_pos(v_[2]);

  auto const x10 = x1 - x0;
  auto const x20 = x2 - x0;

  Eigen::Matrix<real_t, 3, 2> D;
  D << x10.x(), x20.x(),
       x10.y(), x20.y(),
       x10.z(), x20.z();
  
  auto const F = D * D_inv_;  // Deformation gradient (3x2 matrix)
  auto const P = elasty::stvk_1st_pk_stress(F, lame_coeff_[0], lame_coeff_[1]);

  // Calculate the gradient of the constraint
  const Eigen::Matrix<real_t, 3, 2> grad_12 = area_ * P * D_inv_.transpose();
  const Eigen::Matrix<real_t, 3, 1>  grad_0 = -grad_12.col(0) - grad_12.col(1);

    // Copy the results
    std::memcpy(grad_ret.data(), grad_0.data(), sizeof(real_t) * 3);
    std::memcpy(grad_ret.data() + 3, grad_12.data(), sizeof(real_t) * 6);
}

NAMESPACE_END(doux::pd)