#pragma once

#include <cmath>
#include <numbers>

#include "doux/core/platform.h"
#include "concepts.h"
#include "fmt/core.h"

NAMESPACE_BEGIN(doux)

template <typename T_>
[[nodiscard]] constexpr DOUX_ALWAYS_INLINE DOUX_ATTR(const) T_ sqr(T_ x) {
  return x * x;
}

// clang-format off
template <typename T_>
requires std::is_floating_point_v<T_> 
class Gaussian {
 public:
  /// Default Gaussian
  Gaussian() noexcept
      : mu_(0.), amp_(sqrt(std::numbers::inv_pi_v<T_> * 0.5)), denom_(-0.5) {}

  /// Construct a Gaussian with provided amplitude, mean (\mu), and standard
  /// deviation (\sigma)
  Gaussian(T_ amp, T_ mu, T_ sigma) : amp_(amp), mu_(mu) {
    assert(!std::isnan(amp) && !std::isnan(mu) && !std::isnan(sigma));
    if DOUX_UNLIKELY(sigma < std::numeric_limits<T_>::epsilon() * 100) { 
      throw std::invalid_argument(
          fmt::format("Standard deviation ({:f}) must be positive", sigma));
    }

    denom_ = -0.5 / (sigma * sigma);
  }

  Gaussian(const Gaussian&) = default;
  Gaussian(Gaussian&&) = default;
  Gaussian& operator=(const Gaussian&) = default;
  Gaussian& operator=(Gaussian&&) = default;

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure) 
  T_ operator()(T_ v) const noexcept {
    return amp_ * std::exp(denom_ * sqr(v - mu_));
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE DOUX_ATTR(pure)
  T_ mu() const noexcept { return mu_; }

 private:
  /// Gaussian amplitude
  T_ amp_;
  /// $\mu$: expected value
  T_ mu_;
  /// `denom`: indictes the standard deviation, i.e.,
  /// $$
  ///   \text{denom} = -\frac{1}{2\sigma^2}.
  /// $$
  T_ denom_;
};
// clang-format on

NAMESPACE_END(doux)
