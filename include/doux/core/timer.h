#pragma once

#include <array>
#include <chrono>

#include "doux/core/platform.h"

NAMESPACE_BEGIN(doux)

// clang-format off

// A timer that records N time moments, and report the N-1 time intervals
// between every two consecutive recorded time moments
template <size_t N_>
requires(N_ > 1) 
class Timer {
 private:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

 public:
  ACA_ALWAYS_INLINE void reset() noexcept { n_ = 0; }

  ACA_ALWAYS_INLINE void click() noexcept {
    if ACA_LIKELY (n_ < N_) {
      clicks_[n_++] = Clock::now();
    }
  }

  // Period: indicates the time units for reporting the time intervals
  template<class Period = std::micro>
  void report() const {
    fmt::print("------------------\n");
    if (n_ < 2) return;
    for (size_t i = 1; i < n_; ++i) {
      fmt::print("⏱  {:.6f} ", std::chrono::duration<double, Period>(
          clicks_[i] - clicks_[i - 1]).count());
      if constexpr (std::is_same_v<Period, std::micro>) {
        fmt::print("µs\n");
      } else if constexpr (std::is_same_v<Period, std::milli>) {
        fmt::print("ms\n");
      } else if constexpr (std::is_same_v<Period, std::nano>) {
        fmt::print("ns\n");
      } else if constexpr (std::is_same_v<Period, std::ratio<1>>) {
        fmt::print("sec\n");
      } else {
        fmt::print("\n");
      }
    }
    fmt::print("------------------\n");
  }

  template<class Period = std::micro>
  double span(uint32_t ii) const noexcept {
    if (ii < 1 || ii >= N_) return 0;
    return std::chrono::duration<double, Period>(
        clicks_[ii] - clicks_[ii - 1]).count();
  }

  template<class Period = std::micro>
  double span(uint32_t si, uint32_t ti) const noexcept {
    if (si >= N_ || ti >= N_ || si == ti) return 0;
    return std::chrono::duration<double, Period>(
        clicks_[ti] - clicks_[si]).count();
  }

 private:
  std::array<TimePoint, N_> clicks_;
  int n_{0};
};

// ----------------------------------------------------------------------------------

template <size_t N_, class Period = std::micro>
requires(N_ > 0) 
class AccuTimer {
 private:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using Duration = std::chrono::duration<double, Period>;

 public:
  AccuTimer() {
    ts_.fill(Duration::zero());
    count_.fill(0);
  }

  template<size_t N>
  ACA_ALWAYS_INLINE void start() requires(N < N_) {
    clicks_[N] = Clock::now();
  }

  template<size_t N>
  ACA_ALWAYS_INLINE void stop() requires(N < N_) {
    ts_[N] += Duration(Clock::now() - clicks_[N]);
    ++ count_[N];
  }

  template<class P = Period>
  void report() const {
    fmt::print("------------------\n");
    for (size_t i = 0; i < N_; ++i) {
      print_unit<P>();
      double v;
      if constexpr (std::is_same_v<Period, P>) {
        v = ts_[i].count();
      } else {
        v = std::chrono::duration_cast<std::chrono::duration<double, P>>(ts_[i]).count();
      }
      fmt::print("{:.6f}, Avg: {:.6f}\n", v, v/count_[i]);
    }
    fmt::print("------------------\n");
  }

 private:

  template<class P>
  void print_unit() const {
    if constexpr (std::is_same_v<P, std::micro>) {
      fmt::print("⏱  Total(µs): ");
    } else if constexpr (std::is_same_v<P, std::nano>) {
      fmt::print("⏱  Total(ns): ");
    } else if constexpr (std::is_same_v<P, std::milli>) {
      fmt::print("⏱  Total(ms): ");
    } else if constexpr (std::is_same_v<P, std::ratio<1>>) {
      fmt::print("⏱  Total(sec): ");
    } else {
      fmt::print("\n");
    }
  }

 private:
  std::array<TimePoint, N_> clicks_;
  std::array<Duration, N_> ts_;
  std::array<size_t, N_> count_;
};

// clang-format on

NAMESPACE_END(doux)