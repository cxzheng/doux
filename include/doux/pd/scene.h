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
class ProjDynScene {
 public:
};

// ------------------------------------------------------------------------------------

template <class CD_>
void PBDScene<CD_>::update_colli_cons() {
}

NAMESPACE_END(doux::pd)