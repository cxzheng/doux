#include <gtest/gtest.h>
#include "doux/linalg/num_types.h"
#include "doux/core/svec.h"


TEST(TestEigen, DISABLED_MapVec3) {
  using namespace doux;

  {
    Vec3d a(1.2), b(2.4), c(0.1), d(1.5);
    Vec3d x[] = {b - a, c - a, d - a};
    std::cout << "SIZE (vec3d): " << sizeof(a) / sizeof(double) << std::endl;
    Eigen::Map<linalg::mat3_d_t, 0, Eigen::OuterStride<sizeof(Vec3d)/sizeof(double)>> m(reinterpret_cast<double*>(x));
    std::cout << m << std::endl;
    std::cout << " ----------------- " << std::endl;
    linalg::mat3_d_t ss = m;
    std::cout << ss << std::endl;
  }
  {
    Vec3f a(1.2), b(2.4), c(0.1), d(1.5);
    Vec3f x[] = {b - a, c - a, d - a};
    std::cout << "SIZE (vec3f): " << sizeof(a) / sizeof(float) << std::endl;
    Eigen::Map<linalg::mat3_f_t, 0, Eigen::OuterStride<sizeof(Vec3f)/sizeof(float)>> m(reinterpret_cast<float*>(x));
    std::cout << m << std::endl;
    std::cout << " ----------------- " << std::endl;
    linalg::mat3_f_t ss = m;
    std::cout << ss << std::endl;
  }
}

TEST(TestEigen, ColwiseSum) {
  using namespace doux;
  linalg::mat3_d_t m;
  m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  std::cout << m << std::endl;
  linalg::vec3_d_t v = m.colwise().sum();
  std::cout << v << std::endl;
}