#pragma once

#include "doux/doux.h"
#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

class GravityForce {

 public:
  GravityForce() = default;
  GravityForce(GravityForce&) = default;
  GravityForce(GravityForce&&) = default;
  GravityForce& operator = (GravityForce&) = default;
  GravityForce& operator = (GravityForce&&) = default;

  GravityForce(real_t g) noexcept : g_{g} {}
  
  // --------------------------------------------------
  void step_vel_pos(MotiveBody& body, real_t dt) const;

 private:
  real_t g_ {9.8};  // gravitational coefficient (m/s)
};

// This is a slight generalization of `GravityForce`
class MassForce {
 public:

  // explicit euler timestepping the velocity
  void step_vel(MotiveBody& body, real_t dt) const;

  // explicit euler timestepping softbody vertex positions
  void step_pos(MotiveBody& body, real_t dt) const;

  // This is a composite of `step_vel` and `step_pos`
  void step_vel_pos(MotiveBody& body, real_t dt) const;

 private:
  Vec3r g_; // force coefficient
};

NAMESPACE_END(doux::pd)
