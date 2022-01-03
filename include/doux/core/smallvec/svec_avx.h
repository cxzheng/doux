//******************************************************************************
// svec_avx.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#ifndef DOUX_X86_AVX
#error To compile svec_avx.h, AVX feature on CPU must be enabled.
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
struct is_native<double, 4> : std::true_type {};
template <>
struct is_native<double, 3> : std::true_type {};
template <>
struct is_native<float, 8> : std::true_type {};
}  // namespace internal

// clang-format off
template <size_t Size_, typename Derived_>
requires(Size_ == 3 || Size_ == 4) 
struct SmallVecImpl<double, Size_, Derived_>
    : SmallVecBase<double, Size_, Derived_> {

  using Base = SmallVecBase<double, Size_, Derived_>;
  using Data = __m256d;
  using typename Base::Derived;
  using typename Base::value_t;

  SVEC_DEFAULT_CONSTORS

  [[nodiscard]] DOUX_ALWAYS_INLINE double& _c(uint32_t i) { return data_[i]; }
  [[nodiscard]] DOUX_ALWAYS_INLINE double _c(uint32_t i) const {
    return data_[i];
  }

  SmallVecImpl(__m256d t) noexcept : m_{t} {}
  SmallVecImpl(double v) noexcept : m_{_mm256_set1_pd(v)} {}

  SmallVecImpl(double d0, double d1, double d2, double d3) noexcept 
      requires(Size_ == 4)
      : m_{_mm256_setr_pd(d0, d1, d2, d3)} {}
  SmallVecImpl(double d0, double d1, double d2) noexcept 
      requires(Size_ == 3)
      : m_{_mm256_setr_pd(d0, d1, d2, 0.)} {}

  void set(double d0, double d1, double d2, double d3) requires(Size_ == 4) {
      m_ = _mm256_setr_pd(d0, d1, d2, d3); 
  }
  void set(double d0, double d1, double d2) requires(Size_ == 3) {
      m_ = _mm256_setr_pd(d0, d1, d2, 0.);
  }
  void set(double d) { m_ = _mm256_set1_pd(d); }

  // --------- converter ---------
  template <typename Val_, typename Der_>
  requires(Size_ == 3 && 
      requires(Val_ v) { {static_cast<double>(v)}; })
  SmallVecImpl(const SmallVecBase<Val_, 3, Der_>& c)
      : SmallVecImpl{static_cast<double>(c[0]), 
                     static_cast<double>(c[1]),
                     static_cast<double>(c[2])} {}

  template <typename Val_, typename Der_>
  requires(Size_ == 4 && 
      requires(Val_ v) { {static_cast<double>(v)}; })
  SmallVecImpl(const SmallVecBase<Val_, 4, Der_>& c)
      : SmallVecImpl{static_cast<double>(c[0]), 
                     static_cast<double>(c[1]),
                     static_cast<double>(c[2]),
                     static_cast<double>(c[3])} {}

  // --------- operators ---------
  DOUX_ALWAYS_INLINE Derived _add(const Derived& rhs) const {
    return Derived{_mm256_add_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sub(const Derived& rhs) const {
    return Derived{_mm256_sub_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _mul(const Derived& rhs) const {
    return Derived{_mm256_mul_pd(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _div(const Derived& rhs) const {
    return Derived{_mm256_div_pd(m_, rhs.m_)};
  }

#define DOUX_VEC4D_HOP(name, op) \
  DOUX_ALWAYS_INLINE double _##name() const {    \
    __m128d t1 = _mm256_extractf128_pd(m_,1);   \
    __m128d t2 = _mm256_castpd256_pd128(m_);    \
    if constexpr (Size_ == 4) {                 \
      t1 = _mm_##op##_pd(t1, t2);               \
      t2 = _mm_unpackhi_pd(t1, t1);             \
      t2 = _mm_##op##_pd(t1, t2);               \
    } else {                                    \
      t1 = _mm_##op##_sd(t1, t2);               \
      t2 = _mm_permute_pd(t2, 1);               \
      t2 = _mm_##op##_sd(t2, t1);               \
    }                                           \
    return _mm_cvtsd_f64(t2);                   \
  }

  DOUX_VEC4D_HOP(hsum, add)
  DOUX_VEC4D_HOP(hprod, mul)

  DOUX_ALWAYS_INLINE Derived _sqrt() const {
    return Derived{_mm256_sqrt_pd(m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sqr() const {
    return Derived{_mm256_mul_pd(m_, m_)};
  }
  DOUX_ALWAYS_INLINE Derived _rcp() const {
#ifdef __AVX512ER__  // AVX512ER: full precision
    return Derived{_mm512_castpd512_pd256(_mm512_rcp28_pd(_mm512_castpd256_pd512(m_)))};
#elif DOUX_INSTRSET >= 10  // AVX512VL: 14 bit precision
    __m256d r = _mm256_rcp14_pd(m_);
    __m256d ro = r, t0, t1;
    (void) ro;

    // Refine using 2 Newton-Raphson iterations
    t0 = _mm256_add_pd(r, r);
    t1 = _mm256_mul_pd(r, m_);
    r = _mm256_fnmadd_pd(t1, r, t0);

    t0 = _mm256_add_pd(r, r);
    t1 = _mm256_mul_pd(r, m_);
    r = _mm256_fnmadd_pd(t1, r, t0);

    return Derived{_mm256_fixupimm_pd(r, m_, _mm256_set1_epi32(0x0087A622), 0)};
#else
    return Derived{_mm256_div_pd(_mm256_set1_pd(1.), m_)};
#endif
  }
  DOUX_ALWAYS_INLINE Derived _rcp(double v) const {
#ifdef __AVX512ER__  // AVX512ER: full precision
    return _rcp() * v;
#else
    return Derived{_mm256_div_pd(_mm256_set1_pd(v), m_)};
#endif
  }

  // -------------- ceil, floor, and round --------------
  /// Round the floating value vector
  DOUX_ALWAYS_INLINE Derived _round() const {
    return Derived{_mm256_round_pd(m_, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
  }
  // function ceil: round towards plus infinity. (result as float vector)
  DOUX_ALWAYS_INLINE Derived _ceil() const { return Derived{_mm256_ceil_pd(m_)}; }
  // function floor: round towards minus infinity. (result as float vector)
  DOUX_ALWAYS_INLINE Derived _floor() const { return Derived{_mm256_floor_pd(m_)}; }

  DOUX_ALWAYS_INLINE void _set_zero() noexcept {
    m_ = _mm256_setzero_pd();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE const Data& _data() const { return m_; }

  template <int I0, int I1, int I2, int I3>
  DOUX_ALWAYS_INLINE Derived _shuffle() const {
#if DOUX_X86_AVX2
    return Derived{_mm256_permute4x64_pd(m_, _MM_SHUFFLE(I3, I2, I1, I0))};
#else
    UNIMPLEMENTED
#endif
  }

  template <int I0, int I1, int I2>
  DOUX_ALWAYS_INLINE Derived _shuffle() const requires(Size_ == 3) {
    return Base::template shuffle<I0, I1, I2, 3>();
  }

  // Dot product
  DOUX_ALWAYS_INLINE value_t _dot(const Derived& rhs) const {
    // TODO: Is there better impl?
    return (Base::derived() * rhs)._hsum();
  }

  union {
    double data_[4];
    __m256d m_;
  };
};

// ---------------------------------------------------------------------------------------
//  Vec8f
template <typename Derived_>
struct SmallVecImpl<float, 8, Derived_> : SmallVecBase<float, 8, Derived_> {

  using Base = SmallVecBase<float, 8, Derived_>;
  using Data = __m256;
  using typename Base::Derived;
  using typename Base::value_t;

  SVEC_DEFAULT_CONSTORS

  [[nodiscard]] DOUX_ALWAYS_INLINE float& _c(uint32_t i) { return data_[i]; }
  [[nodiscard]] DOUX_ALWAYS_INLINE float _c(uint32_t i) const { return data_[i]; }

  SmallVecImpl(__m256 t) noexcept : m_{t} {}
  SmallVecImpl(float v) noexcept : m_{_mm256_set1_ps(v)} {}

  SmallVecImpl(float d0, float d1, float d2, float d3,
               float d4, float d5, float d6, float d7) noexcept 
      : m_{_mm256_setr_ps(d0, d1, d2, d3, d4, d5, d6, d7)} {}

  void set(float v) { m_ = _mm256_set1_ps(v); }

  void set(float d0, float d1, float d2, float d3,
           float d4, float d5, float d6, float d7) {
    m_ = _mm256_setr_ps(d0, d1, d2, d3, d4, d5, d6, d7);
  }

  // --------- converter ---------
  template <typename Val_, typename Der_>
  SmallVecImpl(const SmallVecBase<Val_, 8, Der_>& c)
      : SmallVecImpl{static_cast<double>(c[0]),
                     static_cast<double>(c[1]),
                     static_cast<double>(c[2]),
                     static_cast<double>(c[3]),
                     static_cast<double>(c[4]),
                     static_cast<double>(c[5]),
                     static_cast<double>(c[6]),
                     static_cast<double>(c[7])} {}

  // --------- operators ---------
  DOUX_ALWAYS_INLINE Derived _add(const Derived& rhs) const {
    return Derived{_mm256_add_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sub(const Derived& rhs) const {
    return Derived{_mm256_sub_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _mul(const Derived& rhs) const {
    return Derived{_mm256_mul_ps(m_, rhs.m_)};
  }
  DOUX_ALWAYS_INLINE Derived _div(const Derived& rhs) const {
    return Derived{_mm256_div_ps(m_, rhs.m_)};
  }

#define DOUX_VEC8F_HOP(name, op) \
  DOUX_ALWAYS_INLINE double _##name() const {    \
    __m128 t1 = _mm256_extractf128_ps(m_,1);    \
    __m128 t2 = _mm256_castps256_ps128(m_);     \
    t2 = _mm_##op##_ps(t1, t2);                 \
    t1 = _mm_movehl_ps(t2, t2);                 \
    t2 = _mm_##op##_ps(t2, t1);                 \
    t1 = _mm_shuffle_ps(t2, t2, 1);             \
    __m128 tt = _mm_##op##_ss(t2, t1);          \
    return _mm_cvtss_f32(tt);                   \
  }

  DOUX_VEC8F_HOP(hsum, add)
  DOUX_VEC8F_HOP(hprod, mul)

  DOUX_ALWAYS_INLINE Derived _sqrt() const {
    return Derived{_mm256_sqrt_ps(m_)};
  }
  DOUX_ALWAYS_INLINE Derived _sqr() const {
    return Derived{_mm256_mul_ps(m_, m_)};
  }

  // NOTE: this is an approximation of 1./v
  DOUX_ALWAYS_INLINE Derived _rcp() const {
#ifdef __AVX512ER__  // AVX512ER: full precision
    return Derived{_mm512_castps512_ps256(_mm512_rcp28_round_ps(_mm512_castps256_ps512(m_), _MM_FROUND_NO_EXC))};
#else

    /* Use best reciprocal approximation available on the current hardware and refine */
    __m256 r;
#if DOUX_INSTRSET >= 10  // AVX512VL: 14 bit precision
    r = _mm256_rcp14_ps(m_);
#elif DOUX_INSTRSET >= 9   // AVX512F: 14 bit precision
    r = _mm512_castps512_ps256(_mm512_rcp14_ps(_mm512_castps256_ps512(m_)));
#else  // AVX: 11 bit precision
    r = _mm256_rcp_ps(m_);
#endif
    /* Refine using one Newton-Raphson iteration */
    __m256 t0 = _mm256_add_ps(r, r),
           t1 = _mm256_mul_ps(r, m_),
           ro = r;
    (void) ro;

#ifdef DOUX_X86_FMA
    r = _mm256_fnmadd_ps(t1, r, t0);
#else
    r = _mm256_sub_ps(t0, _mm256_mul_ps(r, t1));
#endif

#ifdef __AVX512VL__
    return Derived{_mm256_fixupimm_ps(r, m_, _mm256_set1_epi32(0x0087A622), 0)};
#endif
    return Derived{_mm256_blendv_ps(r, ro, t1)};
#endif
  }

  // -------------- ceil, floor, and round --------------
  /// Round the floating value vector
  // function round: round to nearest integer (even). (result as float vector)
  DOUX_ALWAYS_INLINE Derived _round() const {
    return Derived{_mm256_round_ps(m_, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
  }
  // function ceil: round towards plus infinity. (result as float vector)
  DOUX_ALWAYS_INLINE Derived _ceil() const { return Derived{_mm256_ceil_ps(m_)}; }
  // function floor: round towards minus infinity. (result as float vector)
  DOUX_ALWAYS_INLINE Derived _floor() const { return Derived{_mm256_floor_ps(m_)}; }

  DOUX_ALWAYS_INLINE void _set_zero() noexcept {
    m_ = _mm256_setzero_ps();
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE const Data& _data() const { return m_; }

  // Dot product
  DOUX_ALWAYS_INLINE value_t _dot(const Derived& rhs) const {
    __m256 dp = _mm256_dp_ps(m_, rhs.m_, 0b11110001);
    __m128 m0 = _mm256_castps256_ps128(dp);
    __m128 m1 = _mm256_extractf128_ps(dp, 1);
    __m128 m = _mm_add_ss(m0, m1);
    return _mm_cvtss_f32(m); 
  }

  union {
    float data_[8];
    __m256 m_;
  };
};

// clang-format on

NAMESPACE_END(doux::svec)
