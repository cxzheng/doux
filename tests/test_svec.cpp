#include <gtest/gtest.h>

#include <random>

#include "doux/core/svec.h"

class SVecTests : public ::testing::Test {
 protected:
  void SetUp() override {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-5.0, 10.0);

    v2_ = doux::Vec2d{dist(mt), dist(mt)};
    v3_ = doux::Vec3d{dist(mt), dist(mt), dist(mt)};
    v4_ = doux::Vec4d{dist(mt), dist(mt), dist(mt), dist(mt)};
    v8_ = doux::Vec8f{static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt)), 
                      static_cast<float>(dist(mt))};
    for (int i = 0; i < N_; ++i) {
      vN_[i] = dist(mt);
    }
  }

  static constexpr int N_{56};
  doux::Vec2d v2_;
  doux::Vec3d v3_;
  doux::Vec4d v4_;
  doux::Vec8f v8_;
  doux::SVector<double, N_> vN_;
};

TEST_F(SVecTests, Round) {
  {
    auto r = v2_.round();
    EXPECT_DOUBLE_EQ(std::round(v2_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::round(v2_.y()), r.y());
  }
  {
    auto r = v3_.round();
    EXPECT_DOUBLE_EQ(std::round(v3_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::round(v3_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::round(v3_.z()), r.z());
  }
  {
    auto r = v4_.round();
    EXPECT_DOUBLE_EQ(std::round(v4_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::round(v4_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::round(v4_.z()), r.z());
    EXPECT_DOUBLE_EQ(std::round(v4_.w()), r.w());
  }
  {
    auto r = vN_.round();
    for (int i = 0; i < N_; ++i) EXPECT_DOUBLE_EQ(std::round(vN_[i]), r[i]);
  }
  {
    auto r = v8_.round();
    for(int i = 0;i < 8;++ i) {
      EXPECT_FLOAT_EQ(std::round(v8_[i]), r[i]);
    }
  }
}
TEST_F(SVecTests, Floor) {
  {
    auto r = v2_.floor();
    EXPECT_DOUBLE_EQ(std::floor(v2_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::floor(v2_.y()), r.y());
  }
  {
    auto r = v3_.floor();
    EXPECT_DOUBLE_EQ(std::floor(v3_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::floor(v3_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::floor(v3_.z()), r.z());
  }
  {
    auto r = v4_.floor();
    EXPECT_DOUBLE_EQ(std::floor(v4_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::floor(v4_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::floor(v4_.z()), r.z());
    EXPECT_DOUBLE_EQ(std::floor(v4_.w()), r.w());
  }
  {
    auto r = vN_.floor();
    for (int i = 0; i < N_; ++i) EXPECT_DOUBLE_EQ(std::floor(vN_[i]), r[i]);
  }
  {
    auto r = v8_.floor();
    for(int i = 0;i < 8;++ i) {
      EXPECT_FLOAT_EQ(std::floor(v8_[i]), r[i]);
    }
  }
}
TEST_F(SVecTests, Ceil) {
  {
    auto r = v2_.ceil();
    EXPECT_DOUBLE_EQ(std::ceil(v2_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::ceil(v2_.y()), r.y());
  }
  {
    auto r = v3_.ceil();
    EXPECT_DOUBLE_EQ(std::ceil(v3_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::ceil(v3_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::ceil(v3_.z()), r.z());
  }
  {
    auto r = v4_.ceil();
    EXPECT_DOUBLE_EQ(std::ceil(v4_.x()), r.x());
    EXPECT_DOUBLE_EQ(std::ceil(v4_.y()), r.y());
    EXPECT_DOUBLE_EQ(std::ceil(v4_.z()), r.z());
    EXPECT_DOUBLE_EQ(std::ceil(v4_.w()), r.w());
  }
  {
    auto r = vN_.ceil();
    for (int i = 0; i < N_; ++i) EXPECT_DOUBLE_EQ(std::ceil(vN_[i]), r[i]);
  }
  {
    auto r = v8_.ceil();
    for(int i = 0;i < 8;++ i) {
      EXPECT_FLOAT_EQ(std::ceil(v8_[i]), r[i]);
    }
  }
}
TEST_F(SVecTests, HSum) {
  {
    auto r = v2_.hsum();
    EXPECT_DOUBLE_EQ(v2_[0] + v2_[1], r);
  }
  {
    auto r = v3_.hsum();
    EXPECT_NEAR(v3_[0] + v3_[1] + v3_[2], r, 1E-14);
  }
  {
    auto r = v4_.hsum();
    EXPECT_NEAR(v4_[0] + v4_[1] + v4_[2] + v4_[3], r, 1E-14);
  }
  {
    auto r = vN_.hsum();
    double e = 0;
    for (int i = 0; i < N_; ++i) e += vN_[i];
    EXPECT_DOUBLE_EQ(e, r);
  }
  {
    auto r = v8_.hsum();
    double e = 0;
    for(int i = 0;i < 8;++ i) e += v8_[i];
    //EXPECT_FLOAT_EQ(e, r);
    EXPECT_NEAR(e, r, 1E-6);
  }
}
TEST_F(SVecTests, HProd) {
  {
    auto r = v2_.hprod();
    EXPECT_DOUBLE_EQ(v2_[0] * v2_[1], r);
  }
  {
    auto r = v3_.hprod();
    EXPECT_NEAR(v3_[0] * v3_[1] * v3_[2], r, 1E-12);
  }
  {
    auto r = v4_.hprod();
    EXPECT_NEAR(v4_[0] * v4_[1] * v4_[2] * v4_[3], r, 1E-12);
  }
  {
    auto r = vN_.hprod();
    double e = 1;
    for (int i = 0; i < N_; ++i) e *= vN_[i];
    EXPECT_NEAR(e, r, std::abs(e*1E-12));
  }
  {
    auto r = v8_.hprod();
    double e = 1;
    for(int i = 0;i < 8;++ i) e *= v8_[i];
    EXPECT_FLOAT_EQ(e, r);
  }
}
TEST_F(SVecTests, Rcp) {
  {
    auto r = v3_.rcp();
    EXPECT_DOUBLE_EQ(1./v3_.x(), r.x());
    EXPECT_DOUBLE_EQ(1./v3_.y(), r.y());
    EXPECT_DOUBLE_EQ(1./v3_.z(), r.z());
  }
  {
    auto r = v4_.rcp();
    EXPECT_DOUBLE_EQ(1./v4_.x(), r.x());
    EXPECT_DOUBLE_EQ(1./v4_.y(), r.y());
    EXPECT_DOUBLE_EQ(1./v4_.z(), r.z());
    EXPECT_DOUBLE_EQ(1./v4_.w(), r.w());
  }
  {
    auto r = v8_.rcp();
    EXPECT_NEAR(1./v8_.val<0>(), r.val<0>(), std::abs(r.val<0>())*1E-6);
    EXPECT_NEAR(1./v8_.val<1>(), r.val<1>(), std::abs(r.val<1>())*1E-6);
    EXPECT_NEAR(1./v8_.val<7>(), r.val<7>(), std::abs(r.val<2>())*1E-6);
  }
}

TEST(SVecTest, Vec2dRecp) {
  doux::Vec2d p1(1., 2.), p2(3., 4.);
  const auto r1 = p1.rcp();
  EXPECT_DOUBLE_EQ(r1.x(), 1.);
  EXPECT_DOUBLE_EQ(r1.y(), 0.5);

  const auto r2 = p2.rcp(2.);
  EXPECT_DOUBLE_EQ(r2.x(), 2. / 3.);
  EXPECT_DOUBLE_EQ(r2.y(), 0.5);
}

TEST(SVecTest, Round) {
  doux::Vec2d p1(1.4999, 2.5001);
  {
    auto r = p1.round();
    EXPECT_DOUBLE_EQ(1., r.x());
    EXPECT_DOUBLE_EQ(3., r.y());
  }

  doux::Vec3d p2(1.4999, 9.00001, 2.5001);
  {
    auto r = p2.round();
    EXPECT_DOUBLE_EQ(1., r.x());
    EXPECT_DOUBLE_EQ(9., r.y());
    EXPECT_DOUBLE_EQ(3., r.z());
  }
}

TEST(SVecTest, VecNormalize) {
  doux::Vec3d p1(1., 2., 3.);
  {
    const auto nrm = p1.norm();
    const auto nn = p1.normalize();

    EXPECT_DOUBLE_EQ(nn.x(), 1. / nrm);
    EXPECT_DOUBLE_EQ(nn.y(), 2. / nrm);
    EXPECT_DOUBLE_EQ(nn.z(), 3. / nrm);
  }

  doux::Vec4d p2(1., 2., 3., 1.);
  {
    const auto nrm = p2.norm();
    const auto nn = p2.normalize();

    EXPECT_DOUBLE_EQ(nn.x(), 1. / nrm);
    EXPECT_DOUBLE_EQ(nn.y(), 2. / nrm);
    EXPECT_DOUBLE_EQ(nn.z(), 3. / nrm);
    EXPECT_DOUBLE_EQ(nn.w(), 1. / nrm);
  }
}

TEST(SVecTest, Vec3dRecp) {
  doux::Vec3d p1(1., 2., 3.), p2(3., 4., 5.);
  const auto r1 = p1.rcp();
  EXPECT_DOUBLE_EQ(r1.x(), 1.);
  EXPECT_DOUBLE_EQ(r1.y(), 0.5);
  EXPECT_DOUBLE_EQ(r1.z(), 1. / 3.);

  const auto r2 = p2.rcp(2.);
  EXPECT_DOUBLE_EQ(r2.x(), 2. / 3.);
  EXPECT_DOUBLE_EQ(r2.y(), 0.5);
  EXPECT_DOUBLE_EQ(r2.z(), 0.4);
}

TEST(SVecTest, Vec3dMul) {
  doux::Vec3d p1(1., 2., 3.);
  auto ret = p1 * 1.5;
  EXPECT_DOUBLE_EQ(1.5, ret.x());
  EXPECT_DOUBLE_EQ(3., ret.y());
  EXPECT_DOUBLE_EQ(4.5, ret.z());
}
TEST(SVecTest, Vec3dCompoundOp) {
  doux::Vec3d p1(1., 2., 3.);
  doux::Vec3d p2(2., 3., 3.);
  p1 += p2; 
  EXPECT_DOUBLE_EQ(3., p1.x());
  EXPECT_DOUBLE_EQ(5., p1.y());
  EXPECT_DOUBLE_EQ(6., p1.z());
}

TEST(SVecTest, Vec2dOper) {
  doux::Vec2d p1(1., 2.), p2(3., 4.);
  EXPECT_DOUBLE_EQ(p1.x(), 1.);
  EXPECT_DOUBLE_EQ(p1.y(), 2.);
  EXPECT_DOUBLE_EQ(p2.x(), 3.);
  EXPECT_DOUBLE_EQ(p2.y(), 4.);

  {
    doux::Vec2d p = p1 + p2;
    EXPECT_DOUBLE_EQ(p.x(), 4.);
    EXPECT_DOUBLE_EQ(p.y(), 6.);
  }
  {
    doux::Vec2d p = p1 - p2;
    EXPECT_DOUBLE_EQ(p.x(), -2.);
    EXPECT_DOUBLE_EQ(p.y(), -2.);
  }
  {
    doux::Vec2d p = p1 * p2;
    EXPECT_DOUBLE_EQ(p.x(), 1 * 3.);
    EXPECT_DOUBLE_EQ(p.y(), 8.);
  }
  {
    doux::Vec2d p = p1 / p2;
    EXPECT_DOUBLE_EQ(p.x(), 1. / 3.);
    EXPECT_DOUBLE_EQ(p.y(), 2. / 4.);
  }
  {
    doux::Vec2d p = p1.sqrt();
    EXPECT_DOUBLE_EQ(p.x(), 1.);
    EXPECT_DOUBLE_EQ(p.y() * p.y(), 2.);
  }

  {
    auto p = p1 + 3.5;
    EXPECT_DOUBLE_EQ(4.5, p.x());
    EXPECT_DOUBLE_EQ(5.5, p.y());
  }
}
TEST(SVecTest, Vec3dOper) {
  doux::Vec3d p1(1., 2., 3.), p2(3., 2., 4.);
  EXPECT_DOUBLE_EQ(p1.norm2(), 1+4.+9.);
  EXPECT_DOUBLE_EQ(p1.norm(), std::sqrt(1+4.+9.));

  EXPECT_DOUBLE_EQ(p1.x(), 1.);
  EXPECT_DOUBLE_EQ(p1.y(), 2.);
  EXPECT_DOUBLE_EQ(p1.z(), 3.);
  EXPECT_DOUBLE_EQ(p2.x(), 3.);
  EXPECT_DOUBLE_EQ(p2.y(), 2.);
  EXPECT_DOUBLE_EQ(p2.z(), 4.);
  {
    doux::Vec3d p = p1 + p2;
    EXPECT_DOUBLE_EQ(p.x(), 4.);
    EXPECT_DOUBLE_EQ(p.y(), 4.);
    EXPECT_DOUBLE_EQ(p.z(), 7.);
  }
  {
    doux::Vec3d p = p1 - p2;
    EXPECT_DOUBLE_EQ(p.x(), -2.);
    EXPECT_DOUBLE_EQ(p.y(), 0.);
    EXPECT_DOUBLE_EQ(p.z(), -1.);
  }
  {
    doux::Vec3d p = p1 * p2;
    EXPECT_DOUBLE_EQ(p.x(), 1 * 3.);
    EXPECT_DOUBLE_EQ(p.y(), 4.);
    EXPECT_DOUBLE_EQ(p.z(), 12.);
  }
  {
    doux::Vec3d p = p1 / p2;
    EXPECT_DOUBLE_EQ(p.x(), 1. / 3.);
    EXPECT_DOUBLE_EQ(p.y(), 1.);
    EXPECT_DOUBLE_EQ(p.z(), 3. / 4.);
  }
}

TEST(SVecTest, Vec3iOper) {
  doux::Vec3i p1(1, 2, 3), p2(3, 2, 4);
  doux::Vec3i p = p1 / p2;
  EXPECT_EQ(0, p.x());
  EXPECT_EQ(1, p.y());
  EXPECT_EQ(0, p.z());

  EXPECT_EQ(p1.norm2(), 1+4+9);
}

TEST(SVecTest, PosSize) {
  EXPECT_EQ(sizeof(size_t) * 2, sizeof(doux::Vec2UL));
  EXPECT_EQ(sizeof(size_t) * 3, sizeof(doux::Vec3UL));

  doux::Vec2UL p(1UL, 2UL);
  EXPECT_EQ(1, p.x());
  EXPECT_EQ(2, p.y());
}

TEST(SVecTest, Vec4dOper) {
  doux::Vec4d p1(1., 2., 3., 3.), p2(3., 2., 4., 2.);
  EXPECT_DOUBLE_EQ(p1.w(), 3.);
  EXPECT_DOUBLE_EQ(p2.w(), 2.);
  {
    doux::Vec4d p = p1 + p2;
    EXPECT_DOUBLE_EQ(p.x(), 4.);
    EXPECT_DOUBLE_EQ(p.y(), 4.);
    EXPECT_DOUBLE_EQ(p.z(), 7.);
    EXPECT_DOUBLE_EQ(p.w(), 5.);
  }
  {
    doux::Vec4d p = p1 - p2;
    EXPECT_DOUBLE_EQ(p.x(), -2.);
    EXPECT_DOUBLE_EQ(p.y(), 0.);
    EXPECT_DOUBLE_EQ(p.z(), -1.);
    EXPECT_DOUBLE_EQ(p.w(), 1.);
  }
  {
    doux::Vec4d p = p1 * p2;
    EXPECT_DOUBLE_EQ(p.x(), 1 * 3.);
    EXPECT_DOUBLE_EQ(p.y(), 4.);
    EXPECT_DOUBLE_EQ(p.z(), 12.);
    EXPECT_DOUBLE_EQ(p.w(), 6.);
  }
  {
    doux::Vec4d p = p1 / p2;
    EXPECT_DOUBLE_EQ(p.x(), 1. / 3.);
    EXPECT_DOUBLE_EQ(p.y(), 1.);
    EXPECT_DOUBLE_EQ(p.z(), 3. / 4.);
    EXPECT_DOUBLE_EQ(p.w(), 3. / 2.);
  }
  {
    doux::Vec4d p{1.2};
    EXPECT_DOUBLE_EQ(p.x(), 1.2);
    EXPECT_DOUBLE_EQ(p.y(), 1.2);
    EXPECT_DOUBLE_EQ(p.z(), 1.2);
    EXPECT_DOUBLE_EQ(p.w(), 1.2);
  }
}

TEST(SVecTest, Vec5iOper) {
  doux::SVector<int, 5> p1{1, 2, 3, 0, 1}, p2{3, 2, 4, 9, -1};
  doux::SVector<int, 5> p = p1 * p2;
  EXPECT_EQ(3, p.x());
  EXPECT_EQ(4, p.y());
  EXPECT_EQ(12, p.z());
  EXPECT_EQ(0, p.w());
  EXPECT_EQ(-1, p[4]);

  {
    doux::SVector<int, 5> v{p};
    EXPECT_EQ(3, v.x());
    EXPECT_EQ(4, v.y());
    EXPECT_EQ(12, v.z());
    EXPECT_EQ(0, v.w());
    EXPECT_EQ(-1, v[4]);
  }
  {
    doux::SVector<int, 5> v;
    v = p;
    EXPECT_EQ(3, v.x());
    EXPECT_EQ(4, v.y());
    EXPECT_EQ(12, v.z());
    EXPECT_EQ(0, v.w());
    EXPECT_EQ(-1, v[4]);
  }
  {
    doux::SVector<double, 5> pd{1., 2., 3., 0., 1.};
    doux::SVector<double, 5> v = pd.sqrt();
    EXPECT_DOUBLE_EQ(1, v.x());
    EXPECT_DOUBLE_EQ(2, v.y() * v.y());
    EXPECT_DOUBLE_EQ(3, v.z() * v.z());
    EXPECT_DOUBLE_EQ(0, v.w());
    EXPECT_DOUBLE_EQ(1, v[4]);
  }
  {
    doux::SVector<int, 5> v{4};
    EXPECT_EQ(4, v.x());
    EXPECT_EQ(4, v.y());
    EXPECT_EQ(4, v.z());
    EXPECT_EQ(4, v.w());
    EXPECT_EQ(4, v[4]);
  }
  {
    auto v = p1 * 2;
    EXPECT_EQ(2, v.x());
    EXPECT_EQ(4, v.y());
    EXPECT_EQ(6, v.z());
    EXPECT_EQ(0, v.w());
    EXPECT_EQ(2, v[4]);
  }
}

TEST(SVecTest, Vec8f) {
  doux::Vec8f p1(1.5f);
  {
    auto ret = p1.hsum();
    EXPECT_FLOAT_EQ(1.5f*8, ret);
  }
  {
    auto ret = p1.hprod();
    EXPECT_FLOAT_EQ(std::pow(1.5f, 8.f), ret);
  }
}
