#pragma once

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

class MassForce {
 public:

  // explicit euler timestepping the velocity
  void step_vel(SoftBody& body, real_t dt) const;

 private:
  Vec3r g_; // gravitational coefficient
};

NAMESPACE_END(doux::pd)
