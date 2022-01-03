//******************************************************************************
// test_elasty.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************
#include <gtest/gtest.h>

#include "doux/elasty/continuum.h"

TEST(TestElasty, LameParam) {
  using namespace doux;
  real_t y = 70.;
  real_t p = 0.3;
  auto [a, b] = elasty::lame_coeff(y, p);
  //fmt::print("{} {}\n", a, b);
  EXPECT_NEAR(a, 40.38462, 1E-3);
  EXPECT_NEAR(b, 26.92308, 1E-3);
}