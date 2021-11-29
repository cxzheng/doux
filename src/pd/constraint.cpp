#include "doux/pd/constraint.h"

NAMESPACE_BEGIN(doux::pd)

[[nodiscard]] real_t DistCFunc::c() const {
  return (body_.vtx_pos(v_[0]) - body_.vtx_pos(v_[1])).norm() - d0_;
}

void DistCFunc::grad(std::span<real_t> grad_ret) const {
#ifndef NDEBUG
  // In debug mode, check the output array size
  if (auto s = grad_ret.size(); s < 6) {
    throw std::out_of_range(
        fmt::format("Insufficient output array space:"
                    "L = {0:d}, but 6 is needed", s));
  }
#endif
  const auto v = body_.vtx_pos(v_[0]) - body_.vtx_pos(v_[1]);
  
  Vec3r ret;
  if ( auto nrm2 = v.norm2(); nrm2 < eps<real_t>::v ) [[unlikely]] {
    ret = Vec3r(rg_(), rg_(), rg_()).normalize();
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

NAMESPACE_END(doux::pd)