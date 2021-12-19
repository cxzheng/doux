#pragma once

#include "doux/core/platform.h"
#if DOUX_X86_AVX
#include "smallvec/svec_avx.h"
#endif

#if DOUX_X86_SSE42
#include "smallvec/svec_sse42.h"
#endif

#include "smallvec/svec_generic.h"

NAMESPACE_BEGIN(doux)

template <typename T_, size_t D_>
using SVector = svec::SVector<T_, D_>;

using Vec2ui = svec::SVector<uint32_t, 2>;
using Vec2i = svec::SVector<int, 2>;
using Vec3ui = svec::SVector<uint32_t, 3>;
using Vec3i = svec::SVector<int, 3>;
using Vec2d = svec::SVector<double, 2>;
using Vec2f = svec::SVector<float, 2>;
using Vec3d = svec::SVector<double, 3>;
using Vec3f = svec::SVector<float, 3>;
using Vec4d = svec::SVector<double, 4>;
using Vec8f = svec::SVector<float, 8>;
using Vec2r = svec::SVector<real_t, 2>;
using Vec3r = svec::SVector<real_t, 3>;
using Vec4r = svec::SVector<real_t, 4>;

// used to locate a cell on a grid and grid res.
template <size_t D_> using VecUL = doux::SVector<size_t, D_>;
using Vec2UL = VecUL<2>;
using Vec3UL = VecUL<3>; 

// ------------------------------------------------------------------
// methods specific to certain dimension of vectors: 

// This is a naive impl of cross product
template <typename Value_> 
DOUX_ALWAYS_INLINE 
auto cross_naive(const SVector<Value_, 3>& v1, const SVector<Value_, 3>& v2) {
  return SVector<Value_, 3>(
          v1.y() * v2.z() - v2.y() * v1.z(),
          v1.z() * v2.x() - v2.z() * v1.x(),
          v1.x() * v2.y() - v2.x() * v1.y());
}

template <typename Value_> 
DOUX_ALWAYS_INLINE 
auto cross(const SVector<Value_, 3>& v1, const SVector<Value_, 3>& v2) {
  return v1.template shuffle<1, 2, 0>()*v2.template shuffle<2, 0, 1>() - 
         v2.template shuffle<1, 2, 0>()*v1.template shuffle<2, 0, 1>();
}
NAMESPACE_END(doux)
