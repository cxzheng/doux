/*
 * This is a benchmark test to decide how to use constraints in PBD
 */

#include "benchmark/benchmark.h"
#include "doux/pd/constraint.h"
#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

class CFunc_V {
 public:
  CFunc_V(Softbody& sb) noexcept : body_{sb} {}

  virtual ~CFunc_V() {}

  [[nodiscard]] virtual real_t c() const = 0;
  virtual void grad(std::span<real_t> grad_ret) = 0;

 protected:
  Softbody& body_;
};

class DistCFunc_V : public CFunc_V {
 public:
  DistCFunc_V(Softbody& sb, uint32_t v1, uint32_t v2, real_t d0) :
      CFunc_V(sb), v_{v1, v2}, d0_(d0) {
#ifndef NDEBUG
    if ( d0 < 0 ) {
      throw std::invalid_argument(fmt::format("Provided d0 value must be positive: {}", d0));
    }
#endif
  }

  // constrait value
  [[nodiscard]] real_t c() const override;

  // gradient of constraint
  void grad(std::span<real_t> grad_ret) override;

 private:
  uint32_t  v_[2];  // vertex IDs
  real_t    d0_;    // rest distance

  // NOTE: use a simple random generator to handle the degenerated case
  // where the two vertices are colocated. 
  std::minstd_rand rg_{123456789};
};

[[nodiscard]] real_t DistCFunc_V::c() const {
  return (body_.vtx_pos(v_[0]) - body_.vtx_pos(v_[1])).norm() - d0_;
}

void DistCFunc_V::grad(std::span<real_t> grad_ret) {
#ifndef NDEBUG
  // In debug mode, check the output array size
  if (auto s = grad_ret.size(); s < 6) {
    throw std::out_of_range(
        fmt::format("Insufficient output array space:"
                    "L = {0:d}, but 6 is needed", s));
  }
#endif
  const auto v = body_.vtx_pos(v_[0]) - body_.vtx_pos(v_[1]);
  
  Vec3r ret;
  if ( auto nrm2 = v.norm2(); nrm2 < eps<real_t>::v ) [[unlikely]] {
    ret = Vec3r(static_cast<real_t>(rg_()), 
                static_cast<real_t>(rg_()),
                static_cast<real_t>(rg_())).normalize();
  } else {
    auto s = (real_t)1 / std::sqrt(nrm2);
    ret = v * s;
  }
  grad_ret[0] =  ret.x();
  grad_ret[1] =  ret.y();
  grad_ret[2] =  ret.z();
  grad_ret[3] = -ret.x();
  grad_ret[4] = -ret.y();
  grad_ret[5] = -ret.z();
}

NAMESPACE_END(doux::pd)

static void BM_cons_virtual(benchmark::State& state) {
  using namespace doux;

  std::vector<Point3r> ps;
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)1, (real_t)3);
  linalg::matrix_i_t fs(1, 3);

  pd::Softbody sb(std::move(ps), std::move(fs));

  std::vector<std::unique_ptr<pd::CFunc_V>> cons; // a list of constraints
  for(size_t i = 0;i < 10;++ i) {
    cons.emplace_back(new pd::DistCFunc_V(sb, 0, 1, (real_t)i));
  } // end for

  real_t grad[6];
  for (auto _ : state) {
    real_t b = 0;
    real_t c = 0;
    for(auto& a : cons) {
      b += a->c();
      a->grad({grad, grad+6});
      c += grad[1];
    }
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

// Register the function as a benchmark
BENCHMARK(BM_cons_virtual);

static void BM_cons_variant(benchmark::State& state) {
  using namespace doux;

  std::vector<Point3r> ps;
  ps.emplace_back((real_t)1, (real_t)2, (real_t)3);
  ps.emplace_back((real_t)2, (real_t)1, (real_t)3);
  linalg::matrix_i_t fs(1, 3);

  pd::Softbody sb(std::move(ps), std::move(fs));

  std::vector<pd::cfunc_var_t> cons; // a list of constraints
  for(size_t i = 0;i < 10;++ i) {
    cons.emplace_back(std::in_place_type<pd::DistCFunc>, &sb, 0, 1, (real_t)i);
  } // end for

  real_t grad[6];
  for (auto _ : state) {
    real_t b = 0;
    real_t c = 0;
    for(auto& a : cons) {
      b += pd::cfunc_val(a);
      pd::cfunc_grad(a, {grad, grad+6});
      c += grad[1];
    }
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

// Register the function as a benchmark
BENCHMARK(BM_cons_variant);

BENCHMARK_MAIN();