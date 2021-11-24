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
using Vec3d = svec::SVector<double, 3>;
using Vec4d = svec::SVector<double, 4>;
using Vec8f = svec::SVector<float, 8>;
using Vec2r = svec::SVector<Real, 2>;
using Vec3r = svec::SVector<Real, 3>;
using Vec4r = svec::SVector<Real, 4>;

// used to locate a cell on a grid and grid res.
template <size_t D_> using VecUL = doux::SVector<size_t, D_>;
using Vec2UL = VecUL<2>;
using Vec3UL = VecUL<3>; 

NAMESPACE_END(doux)
