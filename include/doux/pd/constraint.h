#pragma once

/*
 * This header includes the definitions of PBD constraints
 */
NAMESPACE_BEGIN(doux::pd)

template<size_t N_>
requires(N_ > 0)
class FixNumConstraint {
 public:
  static constexpr size_t VtxNum = N_;
};

NAMESPACE_END(doux::pd)