#include "doux/core/point.h"
#include "doux/core/svec.h"
#include "benchmark/benchmark.h"

using namespace doux;

static DOUX_NEVER_INLINE auto vec_oper(const Vec3d& p1, const Vec3d& p2,
                                      const Vec3d& p3) {
  asm("changxi_label_:");
  return ((p1 + p2) * p3 + p1 * p2) / p3 + p2 * p3 + p1 * p1 + p3 * p3 * p1;
}

// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_oper(benchmark::State& state) {
  doux::Vec3d p1(1., 2., 3.), p2(3., 2., 4.), p3(1.2, 3.2, -1100.9);
  for (auto _ : state) {
    benchmark::DoNotOptimize(vec_oper(p1, p2, p3));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_oper);

// ------------------------------------------------------------------------

static DOUX_NEVER_INLINE auto point_oper(const Point3d& p1, const Vec3d& v2,
                                        const Vec3d& v3) {
  asm("changxi_label_2:");
  return (p1 + v2) - v3;
}
// cppcheck-suppress [constParameterCallback]
static void BM_point3d_oper(benchmark::State& state) {
  Point3d p1(1., 2., 3.);
  Vec3d v2(3., 2., 4.), v3(1.2, 3.2, -1100.9);
  for (auto _ : state) {
    benchmark::DoNotOptimize(point_oper(p1, v2, v3));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_point3d_oper);

// ------------------------------------------------------------------------

static DOUX_NEVER_INLINE auto vec3d_rcp(const Vec3d& p1, double v) {
  asm("changxi_vec3d_rcp_:");
  return p1.rcp().rcp().rcp().rcp().rcp() * v;
}

// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_rcp(benchmark::State& state) {
  doux::Vec3d p1(1., 2., 3.);
  for (auto _ : state) {
    benchmark::DoNotOptimize(vec3d_rcp(p1, 12.));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_rcp);

// ------------------------------------------------------------------------

static DOUX_NEVER_INLINE auto vec3d_normalize(const Vec3d& p1) {
  asm("changxi_vec3_normalize_:");
  return p1.normalize();
}

// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_normalize(benchmark::State& state) {
  doux::Vec3d p1(1., 2., 3.);
  for (auto _ : state) {
    benchmark::DoNotOptimize(vec3d_normalize(p1));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_normalize);

// ------------------------------------------------------------------------

// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_cross(benchmark::State& state) {
  doux::Vec3d p1(1., 2., 3.);
  doux::Vec3d p2(2., -3., 4.);
  for (auto _ : state) {
    auto r = doux::cross(p1, p2);
    benchmark::DoNotOptimize(r);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_cross);

// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_cross_naive(benchmark::State& state) {
  doux::Vec3d p1(1., 2., 3.);
  doux::Vec3d p2(2., -3., 4.);
  for (auto _ : state) {
    auto r = doux::cross_naive(p1, p2);
    benchmark::DoNotOptimize(r);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_cross_naive);

BENCHMARK_MAIN();
