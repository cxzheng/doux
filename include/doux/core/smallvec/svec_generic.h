#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <span>

#include "doux/doux.h"
#include "doux/core/math_func.h"
#include "svec_base.h"

NAMESPACE_BEGIN(doux::svec)

namespace internal {
//
// A set of type traits useful in the following
//

// -------------------------------------------
template <typename Value_, size_t Size_>
struct is_native {
  static constexpr bool value = false;
};

template <typename Value_, size_t Size_>
inline constexpr bool is_native_v = is_native<Value_, Size_>::value;

// ------------------------------------------

template <typename Value_, size_t Size_>
concept UseGenericImpl = !is_native_v<Value_, Size_>;

}  // end namespace internal

// **NOTE**: This is only for internal use.
// It should only be used when Value_ type if trivially copiable
#define SVEC_DEFAULT_CONSTORS                             \
  SmallVecImpl() = default;                               \
  SmallVecImpl(const SmallVecImpl&) = default;            \
  SmallVecImpl(SmallVecImpl&&) = default;                 \
  SmallVecImpl& operator=(const SmallVecImpl&) = default; \
  SmallVecImpl& operator=(SmallVecImpl&&) = default;

// ========================================================================

template <typename Value_, size_t Size_, typename Derived_>
struct SmallVecImpl;

// clang-format off

/// \brief Fall-back implementation of SmallVec
template <typename Value_, size_t Size_, typename Derived_>
requires internal::UseGenericImpl<Value_, Size_> 
struct SmallVecImpl<Value_, Size_, Derived_> : SmallVecBase<Value_, Size_, Derived_> {
  static_assert(Size_ > 0, "The template size argument must be positive");

  using Base = SmallVecBase<Value_, Size_, Derived_>;
  using Data = std::array<Value_, Size_>;

  using typename Base::Derived;
  using typename Base::value_t;
  using Ref = std::remove_reference_t<value_t>&;
  using ConstRef = const std::remove_reference_t<value_t>&;
  [[nodiscard]] DOUX_ALWAYS_INLINE Ref _c(uint32_t i) { return data_[i]; }
  [[nodiscard]] DOUX_ALWAYS_INLINE ConstRef _c(uint32_t i) const {
    return data_[i];
  }

  DOUX_ALWAYS_INLINE
  SmallVecImpl() requires std::is_default_constructible_v<Value_> = default;

  // template <typename... Ts>
  template <typename... Ts>
  explicit SmallVecImpl(Ts&&... ts) noexcept 
      requires IsValidVecInitList<Size_, std::decay<Ts>...>
      : data_{std::forward<Ts>(ts)...} {}

  template <typename Val_, typename Der_>
  explicit SmallVecImpl(const SmallVecBase<Val_, Size_, Der_>& c) {
    for(size_t i = 0;i < Size_;++ i) {
      data_[i] = static_cast<Value_>(c[i]);
    }
  }

  SmallVecImpl(Value_ v) noexcept { data_.fill(v); }
  // cppcheck-suppress [noExplicitConstructor]
  SmallVecImpl(const Data& data) : data_{data} {}

  template<size_t S>
  SmallVecImpl(std::span<const Value_, S> d) noexcept requires(S >= Size_) {
    memcpy(data_.data(), d.data(), sizeof(Value_) * Size_);
  }
  // -----------------------------------------------------------------

  template <typename... Ts>
  DOUX_ALWAYS_INLINE void set(Ts&&... ts) noexcept 
      requires IsValidVecInitList<Size_, std::decay<Ts>...> {
    data_ = {std::forward<Ts>(ts)...};
  }

  DOUX_ALWAYS_INLINE Derived _add(const Derived& rhs) const
      requires(!std::is_pointer_v<Value_>) {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] + rhs.data_[i];
    return ret;
  }
  DOUX_ALWAYS_INLINE Derived _add(const value_t& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] + rhs;
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _sub(const Derived& rhs) const
      requires(!std::is_pointer_v<Value_>) {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] - rhs.data_[i];
    return ret;
  }
  DOUX_ALWAYS_INLINE Derived _sub(const value_t& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] - rhs;
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _mul(const Derived& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] * rhs.data_[i];
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _mul(const value_t& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] * rhs;
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _div(const Derived& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] / rhs.data_[i];
    return ret;
  }
  DOUX_ALWAYS_INLINE Derived _div(const value_t& rhs) const {
    Derived ret;
    for (size_t i = 0; i < Size_; ++i) ret.data_[i] = data_[i] / rhs;
    return ret;
  }

  DOUX_ALWAYS_INLINE Value_ _hsum() const {
    Value_ ret = 0;
    for (size_t i = 0;i < Size_;++ i) ret += data_[i];
    return ret;
  }
  DOUX_ALWAYS_INLINE Value_ _hprod() const {
    Value_ ret = 1;
    for (size_t i = 0;i < Size_;++ i) ret *= data_[i];
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _sqrt() const 
      requires is_complex_or_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) ret.data_[i] = std::sqrt(data_[i]);
    return ret;
  }

  DOUX_ALWAYS_INLINE Derived _sqr() const 
      requires std::is_arithmetic_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) ret.data_[i] = sqr(data_[i]);
    return ret;
  }


  // -------------- rcp --------------
  /// returns 1 / vec
  DOUX_ALWAYS_INLINE Derived _rcp() const 
      requires is_complex_or_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) {
      ret.data_[i] = static_cast<Value_>(1) / data_[i];
    }
    return ret;
  }

  /// returns v / vec
  DOUX_ALWAYS_INLINE Derived _rcp(const Value_ v) const 
      requires is_complex_or_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) { ret.data_[i] = v / data_[i]; }
    return ret;
  }

  // -------------- ceil, floor, and round --------------
  /// Round the floating value vector
  [[nodiscard]] DOUX_ALWAYS_INLINE Derived _round() const
      requires std::is_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) ret.data_[i] = std::round(data_[i]);
    return ret;
  }
  [[nodiscard]] DOUX_ALWAYS_INLINE Derived _ceil() const
      requires std::is_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) ret.data_[i] = std::ceil(data_[i]);
    return ret;
  }
  [[nodiscard]] DOUX_ALWAYS_INLINE Derived _floor() const
      requires std::is_floating_point_v<Value_> {
    Derived ret;
    for (size_t i = 0;i < Size_;++ i) ret.data_[i] = std::floor(data_[i]);
    return ret;
  }

  // --------------- zero ------------
  DOUX_ALWAYS_INLINE void _set_zero() noexcept {
    data_.fill(0);
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  const Data& _data() const noexcept { return data_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(returns_nonnull)
  const Value_* data_ptr() const noexcept { return data_.data(); }

  /// Shuffle operation fallback implementation
  template <size_t... Indices> inline Derived _shuffle() const {
    static_assert(sizeof...(Indices) == Size_, "shuffle(): Invalid size!");
    Derived out;
    size_t idx = 0;
    bool result[] = { (out._c(idx++) = data_[Indices % Size_], false)... };
    (void) idx; (void) result;
    return out;
  }

 private:
  std::array<Value_, Size_> data_;
};

template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator+(const T& a1, const T& a2) {
  return a1.derived()._add(a2.derived());
}
template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator+(const T& a1, const typename T::value_t& a2) {
  return a1.derived()._add(a2);
}

template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator-(const T& a1, const T& a2) {
  return a1.derived()._sub(a2.derived());
}
template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator-(const T& a1, const typename T::value_t& a2) {
  return a1.derived()._sub(a2);
}

template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator*(const T& a1, const T& a2) {
  return a1.derived()._mul(a2.derived());
}
template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator*(const T& a1, const typename T::value_t& a2) {
  return a1.derived()._mul(a2);
}

template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator/(const T& a1, const T& a2) {
  return a1.derived()._div(a2.derived());
}
template <typename T, typename = typename T::is_svec>
DOUX_ALWAYS_INLINE auto operator/(const T& a1, const typename T::value_t& a2) {
  return a1.derived()._div(a2);
}

#define DOUX_SVEC_COMPOUND_OPERATOR(op)                    \
template <typename T, typename = typename T::is_svec>     \
DOUX_ALWAYS_INLINE T& operator op##=(T& a1, const T& a2) { \
  a1 = a1 op a2;                                          \
  return a1;                                              \
}                                                         \
template <typename T, typename = typename T::is_svec>     \
DOUX_ALWAYS_INLINE T& operator op##=(T& a1, const typename T::value_t& a2) { \
  a1 = a1 op a2;                                          \
  return a1;                                              \
}
DOUX_SVEC_COMPOUND_OPERATOR(+)
DOUX_SVEC_COMPOUND_OPERATOR(-)
DOUX_SVEC_COMPOUND_OPERATOR(*)
DOUX_SVEC_COMPOUND_OPERATOR(/)

#undef DOUX_SVEC_COMPOUND_OPERATOR

// --------------------------------------------------------------------------------------

template <typename Value_, size_t Size_>
struct SVector : SmallVecImpl<Value_, Size_, SVector<Value_, Size_>> {
  // for self-identification
  using is_svec = void;
  using Base = SmallVecImpl<Value_, Size_, SVector<Value_, Size_>>;
  using typename Base::value_t;

  SVector() = default;
  SVector(const SVector&) = default;
  SVector(SVector&&) = default;
  explicit SVector(Base&& b) : Base(std::move(b)) {}
  /// Broadcast the value to every element
  SVector(Value_ v) noexcept : Base(v) {}

  template <size_t S>
  SVector(std::span<const Value_, S> d) noexcept requires(S >= Size_) 
      : Base(d) {}

  template<typename Val_>
  explicit SVector(const SVector<Val_, Size_>& s) : Base(s) {}

  SVector& operator=(const SVector&) = default;
  SVector& operator=(SVector&&) = default;

  template <typename... Ts>
  SVector(Ts&&... ts) noexcept 
      requires doux::IsValidVecInitList<Size_, std::decay<Ts>...>
      : Base{std::forward<Ts>(ts)...} {}
};

// clang-format on

NAMESPACE_END(doux::svec)