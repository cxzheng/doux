#pragma once

#include "doux/doux.h"
#include "svec.h"

NAMESPACE_BEGIN(doux)

template <typename Value_, size_t Size_>
struct Point : svec::SmallVecImpl<Value_, Size_, Point<Value_, Size_>> {
  using Base = svec::SmallVecImpl<Value_, Size_, Point<Value_, Size_>>;
  using typename Base::value_t;

  using Base::_data;
  using Base::set_zero;

  DOUX_ALWAYS_INLINE
  Point() noexcept { set_zero(); }
  Point(const Point& p) = default;
  Point(Point&&) noexcept = default;

  Point& operator=(const Point&) = default;
  Point& operator=(Point&&) noexcept = default;

  explicit Point(Value_ v) noexcept : Base(v) {}
  explicit Point(Base&& b) : Base(std::move(b)) {}

  template <typename... Ts>
  explicit Point(Ts&&... ts) noexcept
      requires IsValidVecInitList<Size_, std::decay<Ts>...>
      : Base(std::forward<Ts>(ts)...) {}

  explicit Point(const typename Base::Data& d) : Base(d) {}

  // clang-format off
  // Only Point +/- Vector make sense
  [[nodiscard]] DOUX_ATTR(flatten)  
  Point operator+( const svec::SVector<value_t, Size_>& rhs) const {
    return Point((svec::SVector<value_t, Size_>(_data()) + rhs)._data());
  }

  [[nodiscard]] DOUX_ATTR(flatten) 
  Point operator+(const value_t& rhs) const {
    return Point((svec::SVector<value_t, Size_>(_data()) + rhs)._data());
  }

  [[nodiscard]] DOUX_ATTR(flatten) 
  Point operator-(const svec::SVector<value_t, Size_>& rhs) const {
    return Point((svec::SVector<value_t, Size_>(_data()) - rhs)._data());
  }

  [[nodiscard]] DOUX_ATTR(flatten) 
  Point operator*(const svec::SVector<value_t, Size_>& rhs) const {
    return Point((svec::SVector<value_t, Size_>(_data()) * rhs)._data());
  }

  [[nodiscard]] DOUX_ATTR(flatten) 
  Point operator-(const value_t& rhs) const {
    return Point((svec::SVector<value_t, Size_>(_data()) - rhs)._data());
  }

  // Point - Point -> Vector
  [[nodiscard]] DOUX_ATTR(flatten) 
  svec::SVector<value_t, Size_> operator-(const Point& rhs) const {
    return svec::SVector<value_t, Size_>(_data()) -
           svec::SVector<value_t, Size_>(rhs._data());
  }
  // clang-format on
};

template <typename T_>
using Point2 = Point<T_, 2>;
using Point2i = Point<int, 2>;
using Point2d = Point<double, 2>;
using Point2r = Point<real_t, 2>;

template <typename T_>
using Point3 = Point<T_, 3>;
using Point3i = Point<int, 3>;
using Point3d = Point<double, 3>;
using Point3r = Point<real_t, 3>;

NAMESPACE_END(doux)
