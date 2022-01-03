//******************************************************************************
// projective_energy.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include <assert.h>
#include <unordered_map>
#include "doux/shape/shape.h"
#include "doux/linalg/num_types.h"

NAMESPACE_BEGIN(doux::pd)

enum struct ProjEnergyType : uint32_t {
  TET_ASAP = 1,
  TRI_ASAP = 2,
  PLANE_COLLISION = 3,
};

class ProjDynBody;
class GlobalSolver;

/*
 * abstract class for projective energy term
 */
class ProjEnergy {
 public:
  ProjEnergy() = delete;
  ProjEnergy(const ProjEnergy&) = default;
  ProjEnergy(ProjEnergy&&) = default;
  ProjEnergy& operator = (const ProjEnergy&) = default;
  ProjEnergy& operator = (ProjEnergy&&) = default;

  DOUX_ATTR(nonnull) ProjEnergy(ProjDynBody* sb) noexcept : body_{sb} {}
  DOUX_ATTR(nonnull) ProjEnergy(ProjDynBody* sb, real_t stiff) : 
      body_{sb}, stiffness_{stiff} {
    assert(stiff > 0);
  }

  virtual ~ProjEnergy() {}

  virtual void project() = 0;

  // evaluate the energy value
  virtual real_t val() const = 0;
  /*
   * Add elements introduced by this energy term to the A matrix for global solve
   */
  virtual void register_global_solve_elems(GlobalSolver* solver) = 0;
  // update the RHS in global system
  virtual void update_global_solve_rhs(GlobalSolver* solver) = 0;

 protected:
  ProjDynBody*  body_;
  // stiffness of this energy term
  real_t stiffness_ {1};
};

class PlaneColliEnergy : public ProjEnergy {
 public:
  // The energy type info is needed when grouping energy terms together for 
  // batch processing on GPUs
  static constexpr ProjEnergyType Type = ProjEnergyType::PLANE_COLLISION;

  void project() override;

 private:
  size_t v_;
  Plane3<real_t>  plane_;
  Vec3r  p_;              // the projected vertex position
};

/*
 * Simple Corotationa energy for a tet
 * 
 * WithFixedVtx_: indicate if this energy term involves fixed vertices
 */
class TetCorotEnergy : public ProjEnergy {
 public:
  // The energy type info is needed when grouping energy terms together for 
  // batch processing on GPUs
  static constexpr ProjEnergyType Type = ProjEnergyType::TET_ASAP;

  // ------------------------------------------------
  TetCorotEnergy() = delete;
  TetCorotEnergy(const TetCorotEnergy&) = default;
  TetCorotEnergy(TetCorotEnergy&&) = default;
  TetCorotEnergy& operator = (const TetCorotEnergy&) = default;
  TetCorotEnergy& operator = (TetCorotEnergy&&) = default;

  TetCorotEnergy(ProjDynBody* b, real_t s, size_t v0, size_t v1, size_t v2, size_t v3);

  // ------------------------------------------------

  // evaluate the energy value
  [[nodiscard]] real_t val() const override;

  void project() override;
  DOUX_ATTR(nonnull) void register_global_solve_elems(GlobalSolver* solver) override;
  // update the RHS in global system
  DOUX_ATTR(nonnull) void update_global_solve_rhs(GlobalSolver* solver) override;

 private:
  size_t v_[4];
  bool restricted_vtx_[4];

  // D^{-1} to compute the deformation gradient
  linalg::mat3_r_t D_inv_;
  linalg::vec3_r_t d_sum_;
  linalg::mat3_r_t r_;      // projected def. gradient (should be a rotational matrix)
};

NAMESPACE_END(doux::pd)