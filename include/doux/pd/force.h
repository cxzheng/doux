#pragma once

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

class MassForce {
 public:

  // explicit euler timestepping the velocity
  void apply_on_vel(SoftBody& body, real_t dt) const;

 private:
  Vec3r g_;
};

NAMESPACE_END(doux::pd)
