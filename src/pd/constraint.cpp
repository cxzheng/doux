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

}

NAMESPACE_END(doux::pd)