#pragma once

/*
 * This header defines the specification of a simulation scene
 */

#include <variant>
#include "doux/doux.h"
#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

//
// CD_: class for collision dection. If CD_ = std::monostate, no collision detection 
//      will be performed.
template <class CD_ = std::monostate>
class Scene {
 public:

 private:
  /// a list of soft bodies to be simulated
  std::vector<SoftBoday> sb_;

  /// a list of fixed objects (e.g., ground)

  CD_ coll_det_;	// collision detector
};

NAMESPACE_END(doux::pd)