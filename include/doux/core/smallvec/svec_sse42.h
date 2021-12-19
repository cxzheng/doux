
#pragma once

#ifndef DOUX_X86_SSE42
#error To compile svec_sse42.h, SSE4.2 feature on CPU must be enabled.
#endif

#if defined(DOUX_X86_64) || defined(DOUX_X86_32)
#include <immintrin.h>
#else
#error Architecture not supported yet.
#endif
#include "svec_base.h"
#include "svec_generic.h"

NAMESPACE_BEGIN(doux::svec)

namespace internal {
template <>
struct is_native<double, 2> : std::true_type {};
template <>
struct is_native<float, 4> : std::true_type {};
template <>
struct is_native<float, 3> : std::true_type {};
}  // namespace internal

template <typename Derived_>
struct SmallVecImpl<double, 2, Derived_>
    : SmallVecBase<double, 2, Derived_> {

  using Base = SmallVecBase<double, 2, Derived_>;
  using Data = __m128d;
  using typename Base::Derived;
  using typename Base::value_t;

  SVEC_DEFAULT_CONSTORS

  [[nodiscard]] DOUX_ALWAYS_INLINE double& _c(uint32_t i) { return data_[i]; }
  [[nodiscard]] DOUX_ALWAYS_INLINE double _c(uint32_t i) const {
    return data_[i];
  }

  SmallVecImpl(__m128d t) noexcept : m_{t} {}
  SmallVecImpl(double v) noexcept : m_{_mm_set1_pd(v)} {}
  SmallVecImpl(double d0, double d1) noexcept 
      : m_{_mm_setr_pd(d0, d1)} {}

  template <typename Val_, typename Der_>
  requires requires(Val_ v) { {static_cast<double>(v)}; }
  SmallVecImpl(const SmallVecBase<Val_, 2, Der_>& c)
      : m_{_mm_setr_pd(static_cast<double>(c[0]), 
                       static_cast<double>(c[1]))} {}

  void set(double d0, double d1) {
    m_ = _mm_setr_pd(d0, d1); 
  }

  DOUX_ALWAYS_INLINE Derived _add(const Derived& rhs) const {
    return Derived{_mm_add_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sub(const Derived& rhs) const {
    return Derived{_mm_sub_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _mul(const Derived& rhs) const {
    return Derived{_mm_mul_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _div(const Derived& rhs) const {
    return Derived{_mm_div_pd(m_, rhs.m_)};
  }

#define DOUX_VEC2D_HOP(name, op) \
  DOUX_ALWAYS_INLINE double _##name() const {  \
    __m128d t1 = _mm_unpackhi_pd(m_, m_);     \
    t1 = _mm_##op##_pd(m_, t1);               \
    return _mm_cvtsd_f64(t1);                 \
  }

  DOUX_VEC2D_HOP(hsum, add)
  DOUX_VEC2D_HOP(hprod, mul)

#undef DOUX_VEC2D_HOP

  DOUX_ALWAYS_INLINE Derived _sqrt() const { return Derived{_mm_sqrt_pd(m_)}; }
  DOUX_ALWAYS_INLINE Derived _sqr() const { return Derived{_mm_mul_pd(m_, m_)}; }

  // -------------- rcp --------------
  //// TODO: there should be a better way of computing this
  DOUX_ALWAYS_INLINE Derived _rcp() const {
    return Derived{1. / data_[0], 1. / data_[1]};
  }
  DOUX_ALWAYS_INLINE Derived _rcp(double v) const {
    return Derived{v / data_[0], v / data_[1]};
  }

  // -------------- ceil, floor, and round --------------
  /// Round the floating value vector
  DOUX_ALWAYS_INLINE Derived _round() const {
    return Derived{
        _mm_round_pd(m_, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
  }
  DOUX_ALWAYS_INLINE Derived _ceil() const { return Derived{_mm_ceil_pd(m_)}; }
  DOUX_ALWAYS_INLINE Derived _floor() const { return Derived{_mm_floor_pd(m_)}; }

  DOUX_ALWAYS_INLINE void _set_zero() noexcept { m_ = _mm_setzero_pd(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE const Data& _data() const { return m_; }

 private:
  union {
    double data_[2];
    __m128d m_;
  };
};

// --------------------------------------------------------------------------------------

template <size_t Size_, typename Derived_>
requires(Size_ == 3 || Size_ == 4) 
struct SmallVecImpl<float, Size_, Derived_>
    : SmallVecBase<float, Size_, Derived_> {

  using Base = SmallVecBase<float, Size_, Derived_>;
  using Data = __m128;
  using typename Base::Derived;
  using typename Base::value_t;

  SVEC_DEFAULT_CONSTORS

  [[nodiscard]] DOUX_ALWAYS_INLINE float& _c(uint32_t i) { return data_[i]; }
  [[nodiscard]] DOUX_ALWAYS_INLINE float _c(uint32_t i) const {
    return data_[i];
  }

  SmallVecImpl(__m128 t) noexcept : m_{t} {}
  SmallVecImpl(float v) noexcept : m_{_mm_set1_ps(v)} {}

  SmallVecImpl(float d0, float d1, float d2, float d3) noexcept 
      requires(Size_ == 4)
      : m_{_mm_setr_ps(d0, d1, d2, d3)} {}
  SmallVecImpl(float d0, float d1, float d2) noexcept 
      requires(Size_ == 3)
      : m_{_mm_setr_ps(d0, d1, d2, 0.f)} {}

  template <typename Val_, typename Der_>
  requires requires(Val_ v) { {static_cast<float>(v)}; }
  SmallVecImpl(const SmallVecBase<Val_, 3, Der_>& c)
      requires(Size_ == 3)
      : m_{_mm_setr_ps(static_cast<float>(c[0]), 
                       static_cast<float>(c[1]),
                       static_cast<float>(c[2]), 0.f)} {}

  template <typename Val_, typename Der_>
  requires requires(Val_ v) { {static_cast<float>(v)}; }
  SmallVecImpl(const SmallVecBase<Val_, 4, Der_>& c)
      requires(Size_ == 4)
      : m_{_mm_setr_ps(static_cast<float>(c[0]), 
                       static_cast<float>(c[1]),
                       static_cast<float>(c[2]),
                       static_cast<float>(c[3]))} {}

  void set(float d0, float d1, float d2, float d3) requires(Size_ == 4) {
      m_ = _mm_setr_ps(d0, d1, d2, d3); 
  }
  void set(float d0, float d1, float d2) requires(Size_ == 3) {
      m_ = _mm_setr_ps(d0, d1, d2, 0.);
  }
  void set(float d) { m_ = _mm_set1_ps(d); }

  DOUX_ALWAYS_INLINE Derived _add(const Derived& rhs) const {
    return Derived{_mm_add_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sub(const Derived& rhs) const {
    return Derived{_mm_sub_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _mul(const Derived& rhs) const {
    return Derived{_mm_mul_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _div(const Derived& rhs) const {
    return Derived{_mm_div_ps(m_, rhs.m_)};
  }

#define DOUX_M128_HOP(name, op) \
  DOUX_ALWAYS_INLINE float _##name() const {  \
    __m128 t1 = _mm_movehdup_ps(m_);          \
    __m128 t2 = _mm_##op##_ps(m_, t1);        \
    t1 = _mm_movehl_ps(t1, t2);               \
    t2 = _mm_##op##_ss(t2, t1);               \
    return _mm_cvtss_f32(t2);                 \
  }

  DOUX_M128_HOP(hsum, add)
  DOUX_M128_HOP(hprod, mul)

#undef DOUX_M128_HOP

  DOUX_ALWAYS_INLINE Derived _sqrt() const { return Derived{_mm_sqrt_ps(m_)}; }
  DOUX_ALWAYS_INLINE Derived _sqr() const { return Derived{_mm_mul_ps(m_, m_)}; }

  // -------------- rcp --------------
  //// TODO: there should be a better way of computing this
  DOUX_ALWAYS_INLINE Derived _rcp() const {
    return Derived{1.f / data_[0], 1.f / data_[1]};
  }

  DOUX_ALWAYS_INLINE Derived _rcp(float v) const {
    return Derived{v / data_[0], v / data_[1]};
  }

  // -------------- ceil, floor, and round --------------
  /// Round the floating value vector
  DOUX_ALWAYS_INLINE Derived _round() const {
    return Derived{_mm_round_ps(m_, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
  }
  DOUX_ALWAYS_INLINE Derived _ceil() const { return Derived{_mm_ceil_ps(m_)}; }
  DOUX_ALWAYS_INLINE Derived _floor() const { return Derived{_mm_floor_ps(m_)}; }

  DOUX_ALWAYS_INLINE void _set_zero() noexcept { m_ = _mm_setzero_ps(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE const Data& _data() const { return m_; }

  template <int I0, int I1, int I2, int I3>
  DOUX_ALWAYS_INLINE Derived _shuffle() const {
#if DOUX_X86_AVX
    return Derived{_mm_permute_ps(m_, _MM_SHUFFLE(I3, I2, I1, I0))};
#else
    return Derived{_mm_shuffle_ps(m_, m_, _MM_SHUFFLE(I3, I2, I1, I0))};
#endif
  }

  template <int I0, int I1, int I2>
  DOUX_ALWAYS_INLINE Derived _shuffle() const requires(Size_ == 3) {
    return Base::template shuffle<I0, I1, I2, 3>();
  }

 private:
  union {
    float data_[4];
    __m128 m_;
  };
};

NAMESPACE_END(doux::svec)
