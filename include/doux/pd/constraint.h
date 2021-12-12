#pragma once

/*
 * This header includes the definitions of PBD constraints
 */

#include "doux/doux.h"
#include "doux/core/point.h"
#include <random>
#include <span>

NAMESPACE_BEGIN(doux::pd)

class Softbody;

// Constrait function that keeps the distance of two vertices
class DistCFunc {
 public:
  // ------ constructors ------
  DistCFunc() = delete;
  DistCFunc(const DistCFunc&) = delete;
  DistCFunc(DistCFunc&&) = delete;
  DistCFunc& operator = (const DistCFunc&) = delete;
  DistCFunc& operator = (DistCFunc&&) = delete;

  DistCFunc(Softbody& sb, uint32_t v1, uint32_t v2, real_t d0) :
      body_(sb), v_{v1, v2}, d0_(d0) {
#ifndef NDEBUG
    if ( d0 < 0 ) {
      throw std::invalid_argument(fmt::format("Provided d0 value must be positive: {}", d0));
    }
#endif
  }

  [[nodiscard]] real_t c() const;

  void grad(std::span<real_t> grad_ret);

 private:
  Softbody& body_;
  uint32_t  v_[2];  // vertex IDs
  real_t    d0_;    // rest distance

  // NOTE: use a simple random generator to handle the degenerated case
  // where the two vertices are colocated. 
  std::minstd_rand rg_{123456789};
};

// Constrait function that keeps the distance of a vertex from another fixed point.
class UnitaryDistCFunc {
 public:
  // ------ constructors ------
  UnitaryDistCFunc() = delete;
  UnitaryDistCFunc(const UnitaryDistCFunc&) = delete;
  UnitaryDistCFunc(UnitaryDistCFunc&&) = delete;
  UnitaryDistCFunc& operator = (const UnitaryDistCFunc&) = delete;
  UnitaryDistCFunc& operator = (UnitaryDistCFunc&&) = delete;

  UnitaryDistCFunc(Softbody& sb, uint32_t v, const Point3r& p0, real_t d0) :
      body_(sb), v_{v}, p0_{p0}, d0_{d0} {
#ifndef NDEBUG
    if ( d0 < 0 ) {
      throw std::invalid_argument(fmt::format("Provided d0 value must be positive: {}", d0));
    }
#endif
  }

  [[nodiscard]] real_t c() const;

  void grad(std::span<real_t> grad_ret) const;

 private:
  Softbody&       body_;
  uint32_t        v_;
  Point3r         p0_;
  real_t          d0_;
};

NAMESPACE_END(doux::pd)

/*
// Constraint with a fix number of vertices involved. This is the contraint
// typically used for generating internl forces
//
template<size_t N_, class CFunc_>
requires(N_ > 0)
class FixedConstraint {
 public:
  static constexpr size_t VtxNum = N_;

  // project the vertices
  void project();
};

// Constraint that involves a dynamic set of vertices. The vertices are determined
// at runtime. E.g., the constraints introduced by collisions
class DynConstraint {
 public:

  // project the vertices
  void project();
};
*/