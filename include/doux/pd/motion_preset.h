#pragma once

/*
 * This header specifies preset motion, including fixed vertices and vertices undergoes 
 * scripted motion
 */

#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

class MotionConfig {
 public:
  // set the vertex indexed by vid to be fixed
  void fix_vertex(uint32_t vid);

 private:
  SoftBody& body_;
};

class MotionPreset {
 public:

 private:
  // reference to the softbody 
  SoftBody& body_;

  uint32_t num_fixed_{0};      // number of fixed vertices
  uint32_t num_scripted_{0};   // number of scripted vertices
};

// Consumes the motion configuration and produce immutable motion preset for simulation.
// This ensures that once the simulation starts, the motion preset won't be changed.

NAMESPACE_END(doux::pd)