#pragma once

#include <fmt/core.h>

#include <cstddef>
#include <string>

#include "doux/doux.h"
#include "doux/core/concepts.h"

NAMESPACE_BEGIN(doux::svec)

// clang-format off

/// \brief Bae class to provide generic interface methods for svec.
template <typename Value_, size_t Size_, typename Derived_>
requires(Size_ > 0) 
struct SmallVecBase {
  /// Alias to the derived type
  using value_t = Value_;
  using Derived = Derived_;

  /// Cast to derived type
  DOUX_ALWAYS_INLINE Derived_ &derived() { return (Derived_ &)*this; }
  /// Cast to derived type (const version)
  DOUX_ALWAYS_INLINE const Derived_ &derived() const {
    return (Derived_ &)*this;
  }

  /// Number of array entries
  static constexpr size_t Size = Size_;

  [[nodiscard]] constexpr size_t size() const { return Size; }

  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) operator[](size_t i) {
#ifndef NDEBUG
    if (i >= Size_)
      throw std::out_of_range(
          fmt::format("SmallVecBase: out of range access (tried to access "
                      "index {0:d} in a vector of dim {1:d})",
                      i, Size_));
#endif
    return derived()._c(i);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  constexpr decltype(auto) operator[](size_t i) const {
#ifndef NDEBUG
    // This is the enable compile-time check if possible
    // see: https://akrzemi1.wordpress.com/2011/05/06/compile-time-computations/
    return (i >= Size_) ? throw std::out_of_range(fmt::format(
                              "SmallVecBase: out of range access (tried to "
                              "access index {0:d} in a vector of dim {1:d}",
                              i, Size_))
                        : derived()._c(i);
#else
    return derived()._c(i);
#endif
  }

  template<size_t Id>
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) val() const requires(Size_ > Id) { 
    return derived()._c(Id); 
  }
  template<size_t Id>
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) val() requires(Size_ > Id) { 
    return derived()._c(Id); 
  }

  // This method is always enabled due to the check of Size_ > 0
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) x() const { return derived()._c(0); }
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) x() { return derived()._c(0); }

  /// retrive y-component
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) y() const requires(Size_ > 1) {
    return derived()._c(1);
  }

  /// retrive y-component
  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) y() requires(Size_ > 1) {
    return derived()._c(1);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) z() const requires(Size_ > 2) {
    return derived()._c(2);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) z() requires(Size_ > 2) {
    return derived()._c(2);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) w() const requires(Size_ > 3) {
    return derived()._c(3);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE decltype(auto) w() requires(Size_ > 3) {
    return derived()._c(3);
  }

  DOUX_ALWAYS_INLINE void set_zero() noexcept(
      noexcept(std::declval<Derived_>()._set_zero())) {
    return derived()._set_zero();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived sqrt() const requires is_complex_or_floating_point_v<Value_> {
    return derived()._sqrt();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived round() const requires std::is_floating_point_v<Value_> {
    return derived()._round();
  }
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived ceil() const requires std::is_floating_point_v<Value_> {
    return derived()._ceil();
  }
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived floor() const requires std::is_floating_point_v<Value_> {
    return derived()._floor();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived sqr() const requires std::is_arithmetic_v<Value_> {
    return derived()._sqr();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived rcp() const requires is_complex_or_floating_point_v<Value_> {
    return derived()._rcp();
  }

  // square of the 2-norm
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Value_ norm2() const requires std::is_arithmetic_v<Value_> {
    return derived()._sqr()._hsum();
  }

  // return the 2-norm
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Value_ norm() const requires std::is_arithmetic_v<Value_> {
    return std::sqrt(norm2());
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived normalize() const requires is_complex_or_floating_point_v<Value_> {
    // on Clang 13, for AVX, this will emit vsqrtsd instruction, which is what we want
    const auto s = (Value_)1 / std::sqrt(norm2());
    return derived() * s;
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Derived rcp(const Value_ v) const 
      requires is_complex_or_floating_point_v<Value_> {
    return derived()._rcp(v);
  }

  /// summation over all elements
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Value_ hsum() const requires std::is_arithmetic_v<Value_> {
    return derived()._hsum();
  }
  /// product over all elements
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Value_ hprod() const requires std::is_arithmetic_v<Value_> {
    return derived()._hprod();
  }

  [[nodiscard]] static DOUX_ALWAYS_INLINE Derived_ zero() {
    Derived_ ret;
    ret._set_zero();
    return ret;
  }

  template <size_t... Indices> DOUX_ALWAYS_INLINE 
  Derived shuffle() const {
    return derived().template _shuffle<Indices...>();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  Value_ dot(const Derived& rhs) const requires std::is_arithmetic_v<Value_> {
    return derived()._dot(rhs);
  }
};

// clang-format on

NAMESPACE_END(doux::svec)