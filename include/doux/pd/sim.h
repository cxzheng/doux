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

  SimStats(real_t dt, size_t niter) : num_iter{niter}, dt{dt} {
    assert(dt > 0 && niter > 0);
  }

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
          class ExtForce_ = std::monostate> 
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
 * 
 * DataProc_: type for processing simulated data (e.g., storing into a file)
 */
template <class Scene_, 
          class GlobalSolver_,
          class ExtForce_ = std::monostate,
          class DataProc_ = std::monostate> 
class ProjDynSim {
 public:
  /// Timestep the simulation
  size_t step();

 private:
  // initialize the simulation
  // e.g., allocate memory / data structure
  void init(); 

 private:
  SimStats      status_;
  Scene_        scene_;   // simulation scene
  GlobalSolver_ solver_;
};

#include "sim-impl.h"

NAMESPACE_END(doux::pd)