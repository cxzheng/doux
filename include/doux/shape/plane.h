//******************************************************************************
// plane.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include <cassert>
#include "doux/core/math_func.h"
#include "doux/core/svec.h"
#include "doux/core/math_func.h"

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

  Plane(const SVector<T_, D_>& n, const SVector<T_, D_>& p) :
	    n_{n.normalize()}, p_{p} {
    assert(n.norm2() > eps<T_>::v);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool on_plane(const SVector<T_, D_>& x) const noexcept {
    auto v = n_.dot(x - p_);
    return std::abs(v) < eps<T_>::v;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  T_ distance(const SVector<T_, D_>& x) const noexcept {
    return n_.dot(x - p_);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  auto const& n() const noexcept { return n_; }

 private:
  SVector<T_, D_> n_;	// normal dir
  SVector<T_, D_> p_;	// a point on the plane
};

NAMESPACE_END(doux::shape)