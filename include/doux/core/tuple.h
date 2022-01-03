//******************************************************************************
// tuple.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/core/platform.h"

NAMESPACE_BEGIN(doux)

template <typename T>
inline void sort_triple(T& a, T& b, T& c) {
  if ( b < a ) std::swap(a, b);
  if ( c < a ) std::swap(a, c);
  if ( c < b ) std::swap(b, c);
}

NAMESPACE_END(doux)