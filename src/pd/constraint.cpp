#include "doux/pd/softbody.h"
#include "doux/pd/constraint.h"
#include "doux/elasty/continuum.h"

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

  auto const& X0 = sb->vtx_pos(v0);
  auto const& X1 = sb->vtx_pos(v1);
  auto const& X2 = sb->vtx_pos(v2);
}

[[nodiscard]] real_t StVKTriCFunc::c() const {
}

void StVKTriCFunc::grad(std::span<real_t> grad_ret) {
}

NAMESPACE_END(doux::pd)