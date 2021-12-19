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