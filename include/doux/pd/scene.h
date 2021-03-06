//******************************************************************************
// scene.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

/*
 * This header defines the specification of a simulation scene
 */

#include <variant>
#include "doux/doux.h"
#include "softbody.h"
#include "motion_preset.h"
#include "projective_energy.h"

NAMESPACE_BEGIN(doux::pd)

//
// CD_: class for collision dection. If CD_ = std::monostate, no collision detection 
//      will be performed.
template <class CD_ = std::monostate>
class PBDScene {
 public:
   [[nodiscard]] DOUX_ALWAYS_INLINE 
   std::vector<MotiveBody>& deformables() { return sb_; }

   /// detect the collisions in the current scene
   /// and update the collison constraints
   void update_colli_cons();

   [[nodiscard]] DOUX_ALWAYS_INLINE
   const std::vector<std::unique_ptr<CFunc>>& collision_constraints() const {
     return colli_cons_;
   }

 private:
  /// a list of soft bodies to be simulated
  std::vector<PBDBody> sb_;

  /// Collision detector between a softbody and a fixed object (e.g., the environment)
  std::vector<std::unique_ptr<EnvColliConsBuilder>> evn_colli_;
  CD_ coll_det_;	// collision detector among softbodies

  std::vector<std::unique_ptr<CFunc>> colli_cons_;
};

/*
 * Scene for projective dyamics simulation
 */
template <class CD_ = std::monostate>
class ProjDynScene {
 public:
  ProjDynBody(std::vector<ProjDynBody>&& b);

   [[nodiscard]] DOUX_ALWAYS_INLINE 
   std::vector<ProjDynBody>& deformables() { return sb_; }

   [[nodiscard]] DOUX_ALWAYS_INLINE 
   const std::vector<ProjDynBody>& deformables() const { return sb_; }

   [[nodiscard]] DOUX_ALWAYS_INLINE
   const std::vector<std::unique_ptr<CFunc>>& collision_constraints() const {
     return colli_cons_;
   }

 private:
  /// a list of soft bodies to be simulated
  std::vector<ProjDynBody> sb_;

  CD_ coll_det_;	// collision detector among softbodies

  // energy terms introduced by softbody colliding with each other and
  // with the environment
  std::vector<std::unique_ptr<ProjEnergy>>  colli_cons_;
};

// ------------------------------------------------------------------------------------

NAMESPACE_END(doux::pd)