//******************************************************************************
// tet.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include <cmath>
#include "doux/core/svec.h"

NAMESPACE_BEGIN(doux::shape)

// ------------------------------------------------------------
// Tet Volumn

// formula for a tet volume with vertices (a,b,c,d) is:
// |(a - d) dot ((b - d) cross (c - d))| / 6
template <typename T>
T tet_volume(const SVector<T, 3>& v10, const SVector<T, 3>& v20, const SVector<T, 3>& v30) {
  return std::abs(dot(v10, cross(v20, v30)) / static_cast<T>(6));
}

template <typename T>
T tet_volume(const SVector<T, 3>& v0, 
             const SVector<T, 3>& v1, 
             const SVector<T, 3>& v2, 
	     const SVector<T, 3>& v3) {
  auto const v10 = v1 - v0;
  auto const v20 = v2 - v0;
  auto const v30 = v3 - v0;
  return tet_volume(v10, v20, v30);
}

template <typename T>
T signed_tet_volume(const SVector<T, 3>& v10, const SVector<T, 3>& v20, const SVector<T, 3>& v30) {
  return dot(v10, cross(v20, v30)) / static_cast<T>(6);
}

template <typename T>
T signed_tet_volume(const SVector<T, 3>& v0, 
                    const SVector<T, 3>& v1, 
                    const SVector<T, 3>& v2, 
	            const SVector<T, 3>& v3) {
  auto const v10 = v1 - v0;
  auto const v20 = v2 - v0;
  auto const v30 = v3 - v0;
  return signed_tet_volume(v10, v20, v30);
}
// ------------------------------------------------------------

NAMESPACE_END(doux::shape)