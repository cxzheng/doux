//******************************************************************************
// test_shape.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************
#include <gtest/gtest.h>

#include "doux/shape/shape.h"
#include "doux/shape/tet.h"

TEST(ShapeTest, cuboid) {
  using ::doux::Rect;
  using ::doux::Vec2d;

  Rect<double> rect{Vec2d{1., 2.}, Vec2d{4., 3.}};
  Vec2d s = rect.size();
  EXPECT_DOUBLE_EQ(3., s.x());
  EXPECT_DOUBLE_EQ(1., s.y());

#ifndef NDEBUG
  EXPECT_THROW(Rect<double> rect(Vec2d{1., 2.}, Vec2d{4., 1.}),
               std::invalid_argument);
#endif
  {
    doux::Cube<double, 3> cc;
    EXPECT_DOUBLE_EQ(0., cc.min_pt().x());
    EXPECT_DOUBLE_EQ(0., cc.min_pt().y());
    EXPECT_DOUBLE_EQ(0., cc.min_pt().z());

    doux::Vec3d ss = cc.size();
    EXPECT_DOUBLE_EQ(1., ss.x());
    EXPECT_DOUBLE_EQ(1., ss.y());
    EXPECT_DOUBLE_EQ(1., ss.z());
  }
}

TEST(ShapeTest, sphere) {
  using ::doux::Circle;
  using ::doux::Vec2d;

  Circle<double> c{Vec2d{1., 3.}, 2.};
  EXPECT_DOUBLE_EQ(2., c.r());

  Circle<int> ci;
  ASSERT_EQ(0, ci.c().x());
  ASSERT_EQ(0, ci.c().y());

  Circle<double> cd;
  ASSERT_DOUBLE_EQ(0., cd.c().x());
  ASSERT_DOUBLE_EQ(0., cd.c().y());

#ifndef NDEBUG
  EXPECT_THROW(Circle<double> c2(Vec2d{1., 3.}, -2.), std::invalid_argument);
#endif
}

TEST(ShapeTest, ellipse) {
  using ::doux::Ellipse;
  using ::doux::Vec2d;

  Ellipse<double> e{Vec2d{1., 1.}, Vec2d{1., 2.}};
  ASSERT_DOUBLE_EQ(1., e.coeff<0>());
  ASSERT_DOUBLE_EQ(2., e.coeff<1>());

  {
    Ellipse<double> e2{Vec2d{1., 1.}, Vec2d{1., 0.25}};
    auto bb = e2.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(0., bb.min_pt().x());
    ASSERT_DOUBLE_EQ(-1., bb.min_pt().y());
    ASSERT_DOUBLE_EQ(2., bb.max_pt().x());
    ASSERT_DOUBLE_EQ(3., bb.max_pt().y());
  }

  {
    Ellipse<double> e2{Vec2d{1., 1.}, Vec2d{1./9., 0.25}};
    auto bb = e2.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(-2, bb.min_pt().x());
    ASSERT_DOUBLE_EQ(-1., bb.min_pt().y());
    ASSERT_DOUBLE_EQ(4., bb.max_pt().x());
    ASSERT_DOUBLE_EQ(3., bb.max_pt().y());
  }
}

TEST(ShapeTest, ellipse_contain) {
  using ::doux::Ellipse;
  using ::doux::Vec2d;
  {
    Ellipse<double> e{Vec2d{1., 1.}, Vec2d{0.25, 0.25}};
    ASSERT_TRUE(e.contain(Vec2d{1.5, 1.5}));
    ASSERT_TRUE(e.contain(Vec2d{2.9999, 1.}));
    ASSERT_FALSE(e.contain(Vec2d{3.0001, 1.}));
  }
  {
    Ellipse<double> e{Vec2d{1., 1.}, Vec2d{1., 0.25}};
    ASSERT_FALSE(e.contain(Vec2d{0., -1.}));
  }
}

TEST(ShapeTest, ellipsoid) {
  using ::doux::Ellipsoid;
  using ::doux::Vec3d;

  {
    Ellipsoid<double> e{Vec3d{1., 1., 0.}, Vec3d{1., 0.25, 0.25}};
    auto bb = e.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(-2., bb.min_pt().z());
    ASSERT_DOUBLE_EQ(2., bb.max_pt().z());
    ASSERT_TRUE(e.contain(Vec3d{1., 1., 1.99999}));
    ASSERT_FALSE(e.contain(Vec3d{1., 1., 2.00001}));
  }
}

TEST(ShapeTest, variant) {
  using ::doux::Vec2d;
  doux::shape_var_t<double, 2> v{
      std::in_place_type<doux::Rect<double>>,
      Vec2d{1., 2.}, Vec2d{2.}};
  if (auto prect = std::get_if<doux::Rect<double>>(&v); prect) {
    doux::Vec2d s = prect->size();
    ASSERT_DOUBLE_EQ(1., s.x());
    ASSERT_DOUBLE_EQ(0., s.y());
  } else {
    FAIL() << "prect shouldn't be null";
  }
}

TEST(ShapeTest, plane) {
  using ::doux::Plane;
  using ::doux::Vec3d;

  {
    Plane<double, 3> plane(Vec3d{0., 1., 0.}, Vec3d{1., 1., 1.});
    EXPECT_DOUBLE_EQ(-1., plane.distance(Vec3d{0., 0., 0.}));
    EXPECT_DOUBLE_EQ(1., plane.distance(Vec3d{2., 2., -1.}));
  }
  {
    Plane<double, 3> plane(Vec3d{1., 1., 1.}, Vec3d{0., 0., 0.});
    EXPECT_DOUBLE_EQ(sqrt(3.), plane.distance(Vec3d{1., 1., 1.}));
  }
}
TEST(ShapeTest, TetVolume) {
  using namespace doux;

  {
    Vec3d a(0., 1., 0.), b(0., 0., 1.), c(1., 0., 0.), d(1., 1., 1.);
    auto v = shape::signed_tet_volume(a, b, c, d);
    auto r = std::sqrt(2.);
    EXPECT_DOUBLE_EQ(r*r*r/(6 * std::sqrt(2.)), v);
  }
  {
    Vec3d a(0., 1., 0.), b(0., 0., 1.), c(1., 0., 0.), d(1., 1., 1.);
    auto v = shape::signed_tet_volume(a, c, b, d);
    auto r = std::sqrt(2.);
    EXPECT_DOUBLE_EQ(-r*r*r/(6 * std::sqrt(2.)), v);
  }
}