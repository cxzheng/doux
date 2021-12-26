#pragma once

/*
 * This header defines the core PD-based simulation algorithm
 */

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

struct SimStats {
  /// finished number of steps
  size_t finished_steps {0};

  /// number of solver iterations
  size_t num_iter;

  /// physical timestep size
  real_t dt;

  // ----------------------------------------------------------

  SimStats() = delete;

  SimStats(const SimStats&) = delete;
  SimStats(SimStats&&) = delete;
  SimStats& operator=(const SimStats&) = delete;
  SimStats& operator=(SimStats&&) = delete;

  /// Return the current simulation time
  [[nodiscard]] DOUX_ALWAYS_INLINE real_t t() const noexcept {
    return dt * finished_steps;
  }

  DOUX_ALWAYS_INLINE void step() {
    ++ finished_steps;
  }
};

/*
 * XPBD simulator
 *
 * ExtForce_: model for applying external force (e.g., gravity)
 * ExtCons_: generate and process external constraints.
 */
template <class Scene_, 
          class ExtForce_ = std::monostate, 
          class ExtCons_ = std::monostate>
class XPBDSim {
 public:
  /// Timestep the simulation
  size_t step();

 private:
  SimStats  status_;
  Scene_    scene_;   // simulation scene
  ExtForce_ ext_f_;   // external force
};

/*
 * Projective dynamics simulator
 */
class ProjDynSim {
 public:
  /// Timestep the simulation
  size_t step();
};

// --------------------------------------------------------------------------------

template <class Scene_>
size_t XPBDSim<Scene_>::step() {
  // timestep by external forces
  if constexpr (!std::is_same_v<ExtForce_, std::monostate>) {
    for(auto& sb : scene_.deformables()) {
      ext_f_.apply(sb, status_.dt);
      // possibly damp the velocity
    }
  }

  status_.step();
  // timestep preset object motion
  const real_t t = status_.t();
  for(auto& sb : scene_.deformables()) {
    sb.update_scripted(t);
  }

  if constexpr (!std::is_same_v<ExtCons_, std::monostate>) {
    // detect collisions in the scene
    // generate external constraints
    UNIMPLEMENTED
  }

  // substep iterations
  for(auto i = 0;i < status_.num_iter;++ i) {
    // go over all constraints to project particle positions
    // TODO: this can be implemented in parallel
  }

  // update vel. and pos
}

NAMESPACE_END(doux::pd)