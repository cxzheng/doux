#pragma once

/*
 * This header specifies preset motion, including fixed vertices and vertices undergoes 
 * scripted motion
 */

#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

template <size_t D_>
class MotionPreset {
 public:
  // set the vertex indexed by vid to be fixed
  void fix_vertex(uint32_t vid);

  // set the vertex vid to move as scripted
  void script_vertex(uint32_t vid, std::function<Point3r(uint32_t, Point3r, real_t)>&& script);

 private:
  const Mesh<D_>& mesh_;
  size_t          n_fixed_ {0};

  // vertex label: 
  // 0: free vertices
  // 1: fixed 
  // 2: scripted
  std::vector<uint32_t> tag_;
};

// consumes the softbody setup and produce immutable motion preset for simulation.
// This ensures that once the simulation starts, the motion preset won't be changed.
template <size_t D_>
std::tuple<MotiveSoftbody, std::optional<std::vector<uint32_t>>> 
build_softbody(MotionPreset<D_>&& preset) {
}

NAMESPACE_END(doux::pd)