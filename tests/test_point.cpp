#include <gtest/gtest.h>

#include "doux/core/point.h"

TEST(PointTest, Point2dOper) {
  doux::Point2d p1(1., 2.);
  doux::Vec2d v2(3., 4.);
  EXPECT_DOUBLE_EQ(p1.x(), 1.);
  EXPECT_DOUBLE_EQ(p1.y(), 2.);
  EXPECT_DOUBLE_EQ(v2.x(), 3.);
  EXPECT_DOUBLE_EQ(v2.y(), 4.);

  doux::Point2d pp{p1};
  EXPECT_DOUBLE_EQ(1., pp.x());
  EXPECT_DOUBLE_EQ(2., pp.y());
  {
    doux::Point2d p = p1 + v2;
    EXPECT_DOUBLE_EQ(p.x(), 4.);
    EXPECT_DOUBLE_EQ(p.y(), 6.);
  }
  {
    doux::Point2d p = p1 - v2;
    EXPECT_DOUBLE_EQ(p.x(), -2.);
    EXPECT_DOUBLE_EQ(p.y(), -2.);
  }

  {
    doux::Point3d p0 = doux::Point3d::zero();
    EXPECT_DOUBLE_EQ(p0.x(), 0.);
    EXPECT_DOUBLE_EQ(p0.y(), 0.);
    EXPECT_DOUBLE_EQ(p0.z(), 0.);
  }
  {
    doux::Point2d p0 = doux::Point2d::zero();
    EXPECT_DOUBLE_EQ(p0.x(), 0.);
    EXPECT_DOUBLE_EQ(p0.y(), 0.);
  }
}

TEST(PointTest, Point3dOper) {
  doux::Point3d p1(1., 2., 3.);
  doux::Point3d p2(0., 1.5, 4.);
  {
    auto v = p1 - p2;
    EXPECT_DOUBLE_EQ(v.x(), 1.);
    EXPECT_DOUBLE_EQ(v.y(), 0.5);
    EXPECT_DOUBLE_EQ(v.z(), -1.);
  }
}