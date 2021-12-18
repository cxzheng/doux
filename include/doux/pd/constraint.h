#pragma once

/*
 * This header includes the definitions of PBD constraints
 */

#include "doux/doux.h"
#include "doux/core/point.h"
#include "doux/core/variants.h"
#include <random>
#include <span>

NAMESPACE_BEGIN(doux::pd)

class Softbody;

// Base class of constraint to provide basic data members
class CFunc {
 public:
  CFunc() = delete;
  CFunc(const CFunc&) = default;
  CFunc(CFunc&&) = default;
  CFunc& operator = (const CFunc&) = default;
  CFunc& operator = (CFunc&&) = default;

  CFunc(Softbody* sb) noexcept : body_{sb} {}

 protected:
  Softbody* body_;
};

// Constrait function that keeps the distance of two vertices
class DistCFunc : public CFunc {
 public:
  // ------ constructors ------
  DistCFunc() = delete;
  DistCFunc(const DistCFunc&) = default;
  DistCFunc(DistCFunc&&) = default;
  DistCFunc& operator = (const DistCFunc&) = default;
  DistCFunc& operator = (DistCFunc&&) = default;

  DistCFunc(Softbody* sb, uint32_t v1, uint32_t v2, real_t d0) :
      CFunc(sb), v_{v1, v2}, d0_(d0) {
#ifndef NDEBUG
    if ( d0 < 0 ) {
      throw std::invalid_argument(fmt::format("Provided d0 value must be positive: {}", d0));
    }
#endif
  }

  // constrait value
  [[nodiscard]] real_t c() const;

  // gradient of constraint
  void grad(std::span<real_t> grad_ret);

 private:
  uint32_t  v_[2];  // vertex IDs
  real_t    d0_;    // rest distance

  // NOTE: use a simple random generator to handle the degenerated case
  // where the two vertices are colocated. 
  std::minstd_rand rg_{123456789};
};

// Constrait function that keeps the distance of a vertex from another fixed point.
class UnitaryDistCFunc : public CFunc {
 public:
  // ------ constructors ------
  UnitaryDistCFunc() = delete;
  UnitaryDistCFunc(const UnitaryDistCFunc&) = default;
  UnitaryDistCFunc(UnitaryDistCFunc&&) = default;
  UnitaryDistCFunc& operator = (const UnitaryDistCFunc&) = default;
  UnitaryDistCFunc& operator = (UnitaryDistCFunc&&) = default;

  UnitaryDistCFunc(Softbody* sb, uint32_t v, const Point3r& p0, real_t d0) :
      CFunc(sb), v_{v}, p0_{p0}, d0_{d0} {
#ifndef NDEBUG
    if ( d0 < 0 ) {
      throw std::invalid_argument(fmt::format("Provided d0 value must be positive: {}", d0));
    }
#endif
  }

  [[nodiscard]] real_t c() const;

  void grad(std::span<real_t> grad_ret);

 private:
  uint32_t        v_;
  Point3r         p0_;
  real_t          d0_;

  // NOTE: use a simple random generator to handle the degenerated case
  // where the vertex and p0_ are colocated. 
  std::minstd_rand rg_{123456789};
};

/*
 * Bending constraint for a surface (2D manifold)
 */         
class SurfBendingCFunc : public CFunc {
 public:
  // ------ constructors ------
  SurfBendingCFunc() = delete;
  SurfBendingCFunc(const SurfBendingCFunc&) = delete;
  SurfBendingCFunc(SurfBendingCFunc&&) = delete;
  SurfBendingCFunc& operator = (const SurfBendingCFunc&) = delete;
  SurfBendingCFunc& operator = (SurfBendingCFunc&&) = delete;

  [[nodiscard]] real_t c() const;

  void grad(std::span<real_t> grad_ret);

 private:
 /* Indices of v_ arrays are:
  *
  *                  (1)
  *                 / | \
  *                /  |  \ 
  *              (2)  |  (3)
  *                \  |  /
  *                 \ | /
  *                  (0)
  */
  uint32_t        v_[4];
};

using cfunc_var_t = std::variant<DistCFunc, UnitaryDistCFunc>;

inline real_t cfunc_val(cfunc_var_t& func) {
  return std::visit(
    [](const auto& f) { return f.c(); }, 
    func);
}

inline void cfunc_grad(cfunc_var_t& func, std::span<real_t> g) {
  std::visit(
    [&](auto& f) { f.grad(g); }, 
    func);
}

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