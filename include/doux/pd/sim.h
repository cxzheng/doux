//******************************************************************************
// sim.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

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
  real_t dt2; // dt^2

  // ----------------------------------------------------------

  SimStats() = delete;

  SimStats(const SimStats&) = delete;
  SimStats(SimStats&&) = delete;
  SimStats& operator=(const SimStats&) = delete;
  SimStats& operator=(SimStats&&) = delete;

  SimStats(real_t dt, size_t niter) : num_iter{niter}, dt{dt}, dt2{dt*dt} {
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
 * GlobalSolver_: Linear solver for the global step
 * ExtForce_: model of external forces (e.g., gravity)
 * DataProc_: type for processing simulated data (e.g., storing into a file)
 */
template <class Scene_, 
          class GlobalSolver_,
          class ExtForce_ = std::monostate,
          class DataProc_ = std::monostate> 
class ProjDynSim {
 public:
  ProjDynSim() = delete;

  // -------------------------------------------------------
  //  Constructors

  // Construct without external force and data processing
  ProjDynSim(SimStats s, Scene_ scene, GlobalSolver_ solver)
      requires (std::same_as<ExtForce_, std::monostate> && std::same_as<DataProc_, std::monostate>) :
      status_{s}, scene_{std::forward<Scene_>(scene)},
      solver_{std::forward<GlobalSolver_>(solver)} {
    solver_.init(scene_.deformables(), status_.dt2);
  }

  // Construct with external force and without data processing
  ProjDynSim(SimStats s, Scene_ scene, GlobalSolver_ solver, ExtForce_ f)
      requires (!std::same_as<ExtForce_, std::monostate> && std::same_as<DataProc_, std::monostate>) :
      status_{s}, scene_{std::forward<Scene_>(scene)},
      solver_{std::forward<GlobalSolver_>(solver)},
      ext_f_{std::forward<ExtForce_>(f)} {
    solver_.init(scene_.deformables(), status_.dt2);
  }
      

  /// Timestep the simulation
  size_t step();

 private:
  SimStats      status_;
  Scene_        scene_;   // simulation scene
  GlobalSolver_ solver_;

  ExtForce_   ext_f_;       // external force
  DataProc_   data_proc_;
};

#include "sim-impl.h"

NAMESPACE_END(doux::pd)