//******************************************************************************
// force.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/doux.h"
#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

// This is a slight generalization of `GravityForce`
class MassForce {

 public:
  MassForce() = default;
  MassForce(const MassForce&) = default;
  MassForce(MassForce&&) = default;
  MassForce& operator = (const MassForce&) = default;
  MassForce& operator = (MassForce&&) = default;

  MassForce(real_t gx, real_t gy, real_t gz) noexcept :
    g_{gx, gy, gz} {}

  MassForce(const Vec3r& g) noexcept : g_(g) {}

  [[nodiscard]] DOUX_ALWAYS_INLINE
  auto const& coeff() const { return g_; }

  // apply the force to predict the vel and pos at the next timestep
  inline void apply(PBDBody& body, real_t dt) const {
    body.predict_vel_pos(g_, dt);
  }

 private:
  Vec3r g_{(real_t)0, (real_t)(-9.8), (real_t)0}; // force coefficient
};

NAMESPACE_END(doux::pd)

#if 0
  // explicit euler timestepping the velocity
  void step_vel(MotiveBody& body, real_t dt) const;

  // explicit euler timestepping softbody vertex positions
  void step_pos(MotiveBody& body, real_t dt) const;
#endif
