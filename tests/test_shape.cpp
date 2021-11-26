#include <gtest/gtest.h>

#include "doux/shape/shape.h"

TEST(ShapeTest, cuboid) {
  using ::doux::Point2d;
  using ::doux::Rect;
  using ::doux::Vec2d;

  Rect<double> rect{Point2d{1., 2.}, Point2d{4., 3.}};
  Vec2d s = rect.size();
  EXPECT_DOUBLE_EQ(3., s.x());
  EXPECT_DOUBLE_EQ(1., s.y());

#ifndef NDEBUG
  EXPECT_THROW(Rect<double> rect(Point2d{1., 2.}, Point2d{4., 1.}),
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
  using ::doux::Point2d;

  Circle<double> c{Point2d{1., 3.}, 2.};
  EXPECT_DOUBLE_EQ(2., c.r());

  Circle<int> ci;
  ASSERT_EQ(0, ci.c().x());
  ASSERT_EQ(0, ci.c().y());

  Circle<double> cd;
  ASSERT_DOUBLE_EQ(0., cd.c().x());
  ASSERT_DOUBLE_EQ(0., cd.c().y());

#ifndef NDEBUG
  EXPECT_THROW(Circle<double> c2(Point2d{1., 3.}, -2.), std::invalid_argument);
#endif
}

TEST(ShapeTest, ellipse) {
  using ::doux::Ellipse;
  using ::doux::Point2d;
  using ::doux::Vec2d;

  Ellipse<double> e{Point2d{1., 1.}, Vec2d{1., 2.}};
  ASSERT_DOUBLE_EQ(1., e.coeff<0>());
  ASSERT_DOUBLE_EQ(2., e.coeff<1>());

  {
    Ellipse<double> e2{Point2d{1., 1.}, Vec2d{1., 0.25}};
    auto bb = e2.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(0., bb.min_pt().x());
    ASSERT_DOUBLE_EQ(-1., bb.min_pt().y());
    ASSERT_DOUBLE_EQ(2., bb.max_pt().x());
    ASSERT_DOUBLE_EQ(3., bb.max_pt().y());
  }

  {
    Ellipse<double> e2{Point2d{1., 1.}, Vec2d{1./9., 0.25}};
    auto bb = e2.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(-2, bb.min_pt().x());
    ASSERT_DOUBLE_EQ(-1., bb.min_pt().y());
    ASSERT_DOUBLE_EQ(4., bb.max_pt().x());
    ASSERT_DOUBLE_EQ(3., bb.max_pt().y());
  }
}

TEST(ShapeTest, ellipse_contain) {
  using ::doux::Ellipse;
  using ::doux::Point2d;
  using ::doux::Vec2d;
  {
    Ellipse<double> e{Point2d{1., 1.}, Vec2d{0.25, 0.25}};
    ASSERT_TRUE(e.contain(Point2d{1.5, 1.5}));
    ASSERT_TRUE(e.contain(Point2d{2.9999, 1.}));
    ASSERT_FALSE(e.contain(Point2d{3.0001, 1.}));
  }
  {
    Ellipse<double> e{Point2d{1., 1.}, Vec2d{1., 0.25}};
    ASSERT_FALSE(e.contain(Point2d{0., -1.}));
  }
}

TEST(ShapeTest, ellipsoid) {
  using ::doux::Ellipsoid;
  using ::doux::Point3d;
  using ::doux::Vec3d;

  {
    Ellipsoid<double> e{Point3d{1., 1., 0.}, Vec3d{1., 0.25, 0.25}};
    auto bb = e.axis_aligned_bb();
    ASSERT_DOUBLE_EQ(-2., bb.min_pt().z());
    ASSERT_DOUBLE_EQ(2., bb.max_pt().z());
    ASSERT_TRUE(e.contain(Point3d{1., 1., 1.99999}));
    ASSERT_FALSE(e.contain(Point3d{1., 1., 2.00001}));
  }
}

TEST(ShapeTest, variant) {
  using ::doux::Point2d;
  doux::shape_var_t<double, 2> v{
      std::in_place_type<doux::Rect<double>>,
      Point2d{1., 2.}, Point2d{2.}};
  if (auto prect = std::get_if<doux::Rect<double>>(&v); prect) {
    doux::Vec2d s = prect->size();
    ASSERT_DOUBLE_EQ(1., s.x());
    ASSERT_DOUBLE_EQ(0., s.y());
  } else {
    FAIL() << "prect shouldn't be null";
  }
}
