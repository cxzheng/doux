//******************************************************************************
// common.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************
#pragma once

#include "doux/doux.h"

#ifdef DOUX_USE_FLOAT64
#  define ASSERT_APPROX_EQ(x, y) ASSERT_DOUBLE_EQ(x, y)
#  define EXPECT_APPROX_EQ(x, y) EXPECT_DOUBLE_EQ(x, y)
#else
#  define ASSERT_APPROX_EQ(x, y) ASSERT_FLOAT_EQ(x, y)
#  define EXPECT_APPROX_EQ(x, y) EXPECT_FLOAT_EQ(x, y)
#endif
