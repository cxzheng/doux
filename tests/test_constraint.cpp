#include <gtest/gtest.h>

#include "common.h"
#include "doux/pd/softbody.h"
#include "doux/pd/constraint.h"

TEST(TestPDConstraint, DistCons) {
  using namespace doux;

  std::vector<Vec3r> ps;
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)1, (real_t)3);
  linalg::matrix_i_t fs(1, 3);

  pd::Softbody sb(std::move(ps), std::move(fs));

  pd::DistCFunc func(&sb, 0, 1, (real_t)1.);

  EXPECT_APPROX_EQ(func.c(), std::sqrt(2.) - 1.);

  real_t grad[6];
  func.grad(grad);

  // finite difference est.
  auto& pos = sb.vtx_pos();
#if DOUX_USE_FLOAT64
  real_t eps = 1E-8;
#else
  real_t eps = 1E-4;
#endif
  pos[0].y() -= eps;
  auto v1 = func.c();

  pos[0].y() += 2*eps;
  auto v2 = func.c();

#if DOUX_USE_FLOAT64
  EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-7);
#else
  EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-3);
#endif

  pos[0].y() = 2.;
  pos[1].z() -= eps;
  auto z1 = func.c();
  pos[1].z() += eps*2;
  auto z2 = func.c();
#if DOUX_USE_FLOAT64
  EXPECT_NEAR(grad[5], (z2-z1)/(2*eps), 1E-7);
#else
  EXPECT_NEAR(grad[5], (z2-z1)/(2*eps), 1E-3);
#endif
}

TEST(TestPDConstraint, DistCons1) {
  using namespace doux;

  std::vector<Vec3r> ps;
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)1, (real_t)3);
  Vec3r p0((real_t)2, (real_t)1, (real_t)3);
  linalg::matrix_i_t fs(1, 3);

  pd::Softbody sb(std::move(ps), std::move(fs));

  pd::UnitaryDistCFunc func(&sb, 0, p0, (real_t)1.);

  EXPECT_APPROX_EQ(func.c(), std::sqrt(2.) - 1.);

  real_t grad[3];
  func.grad(grad);

  // finite difference est.
  auto& pos = sb.vtx_pos();
#if DOUX_USE_FLOAT64
  real_t eps = 1E-8;
#else
  real_t eps = 1E-4;
#endif
  pos[0].y() -= eps;
  auto v1 = func.c();

  pos[0].y() += 2*eps;
  auto v2 = func.c();

#if DOUX_USE_FLOAT64
  EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-7);
#else
  EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-3);
#endif
}

TEST(TestPDConstraint, StvkTriCons0) {
  using namespace doux;

  std::vector<Vec3r> ps;
  ps.emplace_back((real_t)0, (real_t)0, (real_t)0);
  ps.emplace_back((real_t)1, (real_t)0, (real_t)0);
  ps.emplace_back((real_t)0, (real_t)2, (real_t)0);
  linalg::matrix_i_t fs(1, 3);
  fs << 0, 1, 2;

  pd::Softbody sb(std::move(ps), std::move(fs));
  pd::StVKTriCFunc func(&sb, 0, 1, 2, 600, 0.45);
  EXPECT_APPROX_EQ(1, func.tri_area());
}

TEST(TestPDConstraint, StvkTriCons1) {
  using namespace doux;

  std::vector<Vec3r> ps;
  ps.emplace_back((real_t)0, (real_t)0, (real_t)0);
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)1, (real_t)3);
  linalg::matrix_i_t fs(1, 3);
  fs << 0, 1, 2;

  pd::Softbody sb(std::move(ps), std::move(fs));
  pd::StVKTriCFunc func(&sb, 0, 1, 2, 600, 0.45);
  {
    auto v1 = func.c();
    EXPECT_NEAR(0, v1, doux::eps<real_t>::v);
  }

  real_t grad[9];
  func.grad(grad);

  auto& pos = sb.vtx_pos();
#if DOUX_USE_FLOAT64
  real_t eps = 1E-8;
#else
  real_t eps = 1E-4;
#endif
  {
    pos[0].y() -= eps;
    auto v1 = func.c();
    pos[0].y() += 2*eps;
    auto v2 = func.c();

#if DOUX_USE_FLOAT64
    EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-7);
#else
    EXPECT_NEAR(grad[1], (v2-v1)/(2*eps), 1E-3);
#endif
  }

  {
    func.grad(grad);
    pos[1].z() -= eps;
    auto v1 = func.c();
    pos[1].z() += 2*eps;
    auto v2 = func.c();

#if DOUX_USE_FLOAT64
    EXPECT_NEAR(grad[5], (v2-v1)/(2*eps), 1E-7);
#else
    EXPECT_NEAR(grad[5], (v2-v1)/(2*eps), 1E-3);
#endif
  }

  {
    func.grad(grad);
    pos[2].x() -= eps;
    auto v1 = func.c();
    pos[2].x() += 2*eps;
    auto v2 = func.c();

#if DOUX_USE_FLOAT64
    EXPECT_NEAR(grad[6], (v2-v1)/(2*eps), 1E-7);
#else
    EXPECT_NEAR(grad[6], (v2-v1)/(2*eps), 1E-3);
#endif
  }
}