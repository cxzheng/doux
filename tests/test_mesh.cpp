//******************************************************************************
// test_mesh.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************
#include <gtest/gtest.h>

#include "common.h"
#include "doux/shape/mesh.h"

TEST(TestMesh, VertexList) {
  using namespace doux;

  linalg::matrix_r_t x(3, 3);
  linalg::matrix_i_t e(1, 3);
  x << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  shape::Mesh<2> msh(std::move(x), std::move(e));
  auto r = msh.vtx_pos();
  EXPECT_APPROX_EQ(r[0].x(), 1);
  EXPECT_APPROX_EQ(r[0].y(), 2);
  EXPECT_APPROX_EQ(r[0].z(), 3);
  EXPECT_APPROX_EQ(r[1].x(), 4);
  EXPECT_APPROX_EQ(r[1].y(), 5);
  EXPECT_APPROX_EQ(r[1].z(), 6);
  EXPECT_APPROX_EQ(r[2].x(), 7);
  EXPECT_APPROX_EQ(r[2].y(), 8);
  EXPECT_APPROX_EQ(r[2].z(), 9);
}