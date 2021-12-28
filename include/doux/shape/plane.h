#pragma once

#include <cassert>
#include "doux/core/math_func.h"
#include "doux/core/svec.h"

NAMESPACE_BEGIN(doux::shape)

// A plane (or a hyper-plane) defined by its normal n and a point p on the plane 
// The plane equation is:
//     (x - p).n = 0
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Plane {
 public:
  Plane() = delete;
  Plane(const Plane&) = default;
  Plane(Plane&&) = default;
  Plane& operator = (const Plane&) = default;
  Plane& operator = (Plane&&) = default;

  Plane(const SVector<T_, D_>& n, const SVector<T_, D_>& p) noexcept 
	: n_{n}, p_{p} {}

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool on_plane(const SVector<T_, D_>& x) const noexcept {
    auto v = n_.dot(x - p_);
    return std::abs(v) < eps<T_>::v;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  T_ distance(const SVector<T_, D_>& x) const noexcept {
    return n_.dot(x - p_);
  }

 private:
  SVector<T_, D_> n_;	// normal dir
  SVector<T_, D_> p_;	// a point on the plane
};

NAMESPACE_END(doux::shape)