#pragma once

/*
 * This header defines the specification of a simulation scene
 */

#include <variant>
#include "doux/doux.h"
#include "softbody.h"
#include "motion_preset.h"

NAMESPACE_BEGIN(doux::pd)

//
// CD_: class for collision dection. If CD_ = std::monostate, no collision detection 
//      will be performed.
template <class CD_ = std::monostate>
class Scene {
 public:
   [[nodiscard]] DOUX_ALWAYS_INLINE 
   std::vector<MotiveBody>& deformables() { return sb_; }

   /// detect the collisions in the current scene
   /// NOTE: this may be implemented using coroutine in C++20

 private:
  /// a list of soft bodies to be simulated
  std::vector<PBDBody> sb_;

  /// a list of fixed objects (e.g., ground)

  /// a list of preset object motions
  std::vector<MotionPreset>

  CD_ coll_det_;	// collision detector
};

NAMESPACE_END(doux::pd)