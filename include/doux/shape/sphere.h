#pragma once

#include "acacia/core/point.h"
#include "cuboid.h"

NAMESPACE_BEGIN(acacia::shape)

// clang-format off
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Sphere {
 public:
  /// By default, a unit sphere centered at the origin
  Sphere() noexcept : rad_{static_cast<T_>(1.)}, r2_{rad_} {}
  Sphere(const Sphere&) = default;
  Sphere(Sphere&&) noexcept = default;

  Sphere& operator=(const Sphere&) = default;
  Sphere& operator=(Sphere&&) noexcept = default;

  Sphere(const Point<T_, D_>& c, T_ r) : ctr_{c}, rad_{r}, r2_{r * r} {
#ifndef NDEBUG
    if (r <= 0.) {
      throw std::invalid_argument(
          fmt::format("Sphere radius (r={}) must be positive", r));
    }
#endif
  }

  [[nodiscard]] ACA_ALWAYS_INLINE ACA_ATTR(pure) 
  T_ r() const noexcept {
    return rad_;
  }

  [[nodiscard]] ACA_ALWAYS_INLINE ACA_ATTR(pure) 
  auto const& c() const noexcept {
    return ctr_;
  }

  [[nodiscard]] ACA_ALWAYS_INLINE 
  auto axis_aligned_bb() const noexcept {
    return Cuboid<T_, D_>(ctr_ - rad_, ctr_ + rad_);
  }

  [[nodiscard]] ACA_ALWAYS_INLINE 
  bool contain(const Point<T_, D_>& pt) const noexcept {
      //requires requires(T_ a) { { a*a } ->std::same_as<T_>; } {
    return (pt - ctr_).sqr().hsum() < r2_;
  }

 private:
  /// Minimum point of the cuboid
  Point<T_, D_> ctr_;
  /// Radius
  T_ rad_;
  /// Radius square
  T_ r2_;
};
// clang-format on

NAMESPACE_END(acacia::shape)