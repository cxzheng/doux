//******************************************************************************
// shape.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/doux.h"
#include "doux/core/concepts.h"
#include "doux/core/variants.h"
#include "cuboid.h"
#include "sphere.h"
#include "elliptic.h"
#include "plane.h"

NAMESPACE_BEGIN(doux)

// clang-format off

template <typename T, size_t D> using Cube = shape::Cuboid<T, D>;
template <typename T, size_t D> using Sphere = shape::Sphere<T, D>;
template <typename T, size_t D> using Elliptic = shape::Elliptic<T, D>;
template <typename T, size_t D> using Plane = shape::Plane<T, D>;

template <typename T> using Rect = Cube<T, 2>;
template <typename T> using Circle = Sphere<T, 2>;
template <typename T> using Cube3 = Cube<T, 3>;
template <typename T> using Sphere3 = Sphere<T, 3>;
template <typename T> using Plane3 = Plane<T, 3>;     // 3D plane
template <typename T> using Ellipse = Elliptic<T, 2>;
template <typename T> using Ellipsoid = Elliptic<T, 3>;

// -----------------------------------------------------------------
template <typename T, size_t D>
struct ShapeVar {};

template <typename T>
struct ShapeVar<T, 2> {
  using type = std::variant<Sphere<T, 2>, Rect<T>, Ellipse<T>>;
};

template <typename T>
struct ShapeVar<T, 3> {
  using type = std::variant<Sphere<T, 3>, Cube<T, 3>, Ellipsoid<T>>;
};

template <typename T, size_t D>
using shape_var_t = typename ShapeVar<T, D>::type;
// -----------------------------------------------------------------

template <IsVariant Var_> 
requires is_variant_v<Var_>
[[nodiscard]] inline auto axis_aligned_bb(const Var_& sh) noexcept {
  return std::visit([](auto&& s) { return s.axis_aligned_bb(); }, sh);
}

template <typename Var_, class Point_> 
requires is_variant_v<Var_>
[[nodiscard]] bool contain(const Var_& sh, const Point_& pt) noexcept {
  return std::visit([&pt](auto&& s) { return s.contain(pt); }, sh);
}

// clang-format on

NAMESPACE_END(doux)
