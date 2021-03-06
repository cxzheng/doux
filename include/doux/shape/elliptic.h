//******************************************************************************
// elliptic.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include <cassert>
#include "doux/core/svec.h"
#include "doux/core/format.h"

NAMESPACE_BEGIN(doux::shape)

// elliptic shape: a*x^2 + b*y^2 + c*z^2 + ... = 1
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Elliptic {
 public:
  Elliptic() = delete;
  Elliptic(const Elliptic& v) = default;
  Elliptic(Elliptic&& v) noexcept = default;
  Elliptic& operator = (const Elliptic& v) = default;
  Elliptic& operator = (Elliptic&& v) noexcept = default;

  Elliptic(const SVector<T_, D_>& c, const SVector<T_, D_>& abc) 
      : ctr_{c}, coeff_{abc} {
#ifndef NDEBUG
    for(size_t i = 0;i < D_;++ i) 
      if (abc[i] <= 0.) {
        throw std::invalid_argument(
            fmt::format("Elliptic coefficients ({}) must be positive", abc));
      }
#endif
  }
    
  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  auto const& c() const noexcept {
    return ctr_;
  }

  template<size_t DimId>
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) coeff() const requires(D_ > DimId) { 
    return coeff_.template val<DimId>();
  }
  template<size_t DimId>
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) coeff(uint32_t id) const { 
    assert(id < D_);
    return coeff_.val[id];
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto axis_aligned_bb() const noexcept {
    auto d = (SVector<T_, D_>((T_)1) / coeff_).sqrt();
    return Cuboid<T_, D_>(ctr_ - d, ctr_ + d);
  }

  [[nodiscard]] 
  bool contain(const SVector<T_, D_>& pt) const noexcept {
      // requires requires(T_ a) { { a*a } ->std::same_as<T_>; } {
    return ((pt - ctr_).sqr() * coeff_).hsum() < (T_)1;
  }

 private:
  SVector<T_, D_> ctr_;
  SVector<T_, D_> coeff_;
};

NAMESPACE_END(doux::shape)