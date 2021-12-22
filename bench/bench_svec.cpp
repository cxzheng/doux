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

static DOUX_NEVER_INLINE auto point_oper(const Vec3d& p1, const Vec3d& v2,
                                        const Vec3d& v3) {
  asm("changxi_label_2:");
  return (p1 + v2) - v3;
}
// cppcheck-suppress [constParameterCallback]
static void BM_point3d_oper(benchmark::State& state) {
  Vec3d p1(1., 2., 3.);
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
  doux::Vec3f p1(1.f, 2.f, 3.f);
  doux::Vec3f p2(2.f, -3.f, 4.f);
  for (auto _ : state) {
    auto r = doux::cross(p1, p2);
    benchmark::DoNotOptimize(r);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_cross);

#if 0
// cppcheck-suppress [constParameterCallback]
static void BM_vector3d_cross_naive(benchmark::State& state) {
  doux::Vec3f p1(1.f, 2.f, 3.f);
  doux::Vec3f p2(2.f, -3.f, 4.f);
  for (auto _ : state) {
    auto r = doux::cross_naive(p1, p2);
    benchmark::DoNotOptimize(r);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector3d_cross_naive);
#endif

static DOUX_NEVER_INLINE auto vec4d_dot(const Vec4d& p1, const Vec4d& p2) {
  asm("changxi_vec4_dot_:");
  return p1.dot(p2);
}

// cppcheck-suppress [constParameterCallback]
static void BM_vector4d_dot(benchmark::State& state) {
  doux::Vec4d p1(1., 2., 3., 1.2);
  doux::Vec4d p2(0.7, -2., 2.7, -0.8);
  for (auto _ : state) {
    benchmark::DoNotOptimize(vec4d_dot(p1, p2));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_vector4d_dot);
BENCHMARK_MAIN();
