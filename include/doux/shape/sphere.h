//******************************************************************************
// sphere.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/core/svec.h"
#include "cuboid.h"

NAMESPACE_BEGIN(doux::shape)

// clang-format off
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Sphere {
 public:
  /// By default, a unit sphere centered at the origin
  Sphere() noexcept : ctr_{static_cast<T_>(0)}, rad_{static_cast<T_>(1)}, r2_{rad_} {}
  Sphere(const Sphere&) = default;
  Sphere(Sphere&&) noexcept = default;

  Sphere& operator=(const Sphere&) = default;
  Sphere& operator=(Sphere&&) noexcept = default;

  Sphere(const SVector<T_, D_>& c, T_ r) : ctr_{c}, rad_{r}, r2_{r * r} {
#ifndef NDEBUG
    if (r <= 0.) {
      throw std::invalid_argument(
          fmt::format("Sphere radius (r={}) must be positive", r));
    }
#endif
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  T_ r() const noexcept {
    return rad_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  auto const& c() const noexcept {
    return ctr_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto axis_aligned_bb() const noexcept {
    return Cuboid<T_, D_>(ctr_ - rad_, ctr_ + rad_);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool contain(const SVector<T_, D_>& pt) const noexcept {
      //requires requires(T_ a) { { a*a } ->std::same_as<T_>; } {
    return (pt - ctr_).sqr().hsum() < r2_;
  }

 private:
  /// Minimum point of the cuboid
  SVector<T_, D_> ctr_;
  /// Radius
  T_ rad_;
  /// Radius square
  T_ r2_;
};
// clang-format on

NAMESPACE_END(doux::shape)