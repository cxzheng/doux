//******************************************************************************
// cuboid.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/core/svec.h"

NAMESPACE_BEGIN(doux::shape)

// clang-format off

// A n-dimentional cuboid specificied by the smallest and largest corner points.
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Cuboid {
 public:
  /// By default, a unit cube with the min point at the origin
  Cuboid() noexcept : min_pt_{static_cast<T_>(0)}, max_pt_{static_cast<T_>(1)} {}
  Cuboid(const Cuboid&) = default;
  Cuboid(Cuboid&&) noexcept = default;

  Cuboid& operator=(const Cuboid&) = default;
  Cuboid& operator=(Cuboid&&) noexcept = default;

  Cuboid(const SVector<T_, D_>& p1, const SVector<T_, D_>& p2)
      : min_pt_{p1}, max_pt_{p2} {
#ifndef NDEBUG
    // Check if this is a valid cuboid at debugging time
    for (auto i = 0; i < D_; ++i)
      if (min_pt_[i] > max_pt_[i])
        throw std::invalid_argument(
            fmt::format("Cuboid: the min point is larger than max point on "
                        "{0:d}-th axis ({1} > {2})",
                        i, min_pt_[i], max_pt_[i]));
#endif
  }

  template<size_t N_>
  [[nodiscard]] DOUX_ALWAYS_INLINE auto const& pt() const noexcept 
      requires(N_== 0 || N_==1) {
    if constexpr (N_ == 0) {
      return min_pt_;
    } else if constexpr (N_ == 1) {
      return max_pt_;
    }
    DOUX_UNREACHABLE;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  svec::SVector<T_, D_> size() const noexcept {
    return max_pt_ - min_pt_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE auto const& min_pt() const noexcept {
    return min_pt_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE auto& min_pt() noexcept {
    return min_pt_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE auto const& max_pt() const noexcept {
    return max_pt_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE auto& max_pt() noexcept {
    return max_pt_;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE auto axis_aligned_bb() const noexcept {
    return *this;
  }

  [[nodiscard]] bool contain(const SVector<T_, D_>& pt) const noexcept {
    if constexpr (D_ == 2) {
      return pt.x() >= min_pt_.x() && pt.x() <= max_pt_.x() &&
             pt.y() >= min_pt_.y() && pt.y() <= max_pt_.y();
    } else if constexpr (D_ == 3) {
      return pt.x() >= min_pt_.x() && pt.x() <= max_pt_.x() &&
             pt.y() >= min_pt_.y() && pt.y() <= max_pt_.y() &&
             pt.z() >= min_pt_.z() && pt.z() <= max_pt_.z();
    } else {
      for(size_t i = 0;i < D_;++ i) {
        if ( pt[i] < min_pt_[i] || pt[i] > max_pt_[i] ) return false;
      }
      return true;
    }
  }

  // return the center of the cuboid
  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  const SVector<T_, D_> c() const noexcept {
    return (min_pt_ + max_pt_)*(T_)0.5;
  }

 private:
  /// Minimum point of the cuboid
  SVector<T_, D_> min_pt_;
  /// Maximum point of the cuboid
  SVector<T_, D_> max_pt_;
};
// clang-format on

NAMESPACE_END(doux::shape)
