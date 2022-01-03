//******************************************************************************
// test_softbody.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************
#include <gtest/gtest.h>

#include "doux/pd/softbody.h"
#include "common.h"

TEST(TestPDSoftBody, Constr) {
  using namespace doux;

  std::vector<Vec3r> ps;
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)3, (real_t)3);
  linalg::matrix_i_t fs(1, 3);

  //fmt::print("data: {}\n", (void*)fs.data());
  pd::Softbody sb(std::move(ps), std::move(fs));
  //pd::Softbody sb(std::move(ps), fs);
  //fmt::print("SIZE: {}\n", ps.size());
  for(size_t i = 0;i < sb.num_vtx();++ i) {
    EXPECT_APPROX_EQ(sb.vtx_mass(i), (real_t)1);
  }

  EXPECT_APPROX_EQ(sb.vtx_pos(1).x(), (real_t)2);
  EXPECT_APPROX_EQ(sb.vtx_pos(1).y(), (real_t)3);
  EXPECT_APPROX_EQ(sb.vtx_pos(1).z(), (real_t)3);
}