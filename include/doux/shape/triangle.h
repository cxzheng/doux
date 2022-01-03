//******************************************************************************
// triangle.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

NAMESPACE_BEGIN(doux::shape)

// A triangle in n-dimensional space.
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Triangle {
 public:

 private:
  Point<T_, D_>	v_[3];	// three vertices
};

NAMESPACE_END(doux::shape)