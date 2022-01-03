//******************************************************************************
// sim-impl.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

template <class Scene_, class ExtForce_> 
size_t XPBDSim<Scene_, ExtForce_>::step() {
  // timestep by external forces
  if constexpr (!std::is_same_v<ExtForce_, std::monostate>) {
    for(auto& sb : scene_.deformables()) {
      ext_f_.apply(sb, status_.dt);
    }
  } else { 
    // If no external force
    for(auto& sb : scene_.deformables()) {
      sb.predict_pos(status_.dt);
    }
  }

  status_.step();
  // timestep preset object motion
  const real_t t = status_.t();
  for(auto& sb : scene_.deformables()) {
    sb.update_scripted(t);
  }

  scene_.update_colli_cons(); // update collision constraints

  // substep iterations
  for(auto i = 0;i < status_.num_iter;++ i) {
    // go over all constraints to project particle positions
    // TODO: constraint projections can be implemented in parallel
    for(auto& sb : scene_.deformables()) {
    }
    
    auto const& cons = scene_.collision_constraints();
    for(auto& cf : cons) {
    }
  } // end for

  // update vel. and pos

  UNIMPLEMENTED
  return status_.finished_steps;
}

// --------------------------------------------------------------------------------

template <class Scene_, class GlobalSolver_, class ExtForce_, class DataProc_> 
size_t ProjDynSim<Scene_, GlobalSolver_, ExtForce_, DataProc_>::step() {
  auto& bodies = scene_.deformables();

  // timestep by external forces
  if constexpr (!std::is_same_v<ExtForce_, std::monostate>) {
    for(auto& sb : bodies) {
      ext_f_.apply(sb, status_.dt);
    }
  } else { 
    // If no external force
    for(auto& sb : bodies) {
      sb.predict_pos(status_.dt);
    }
  }

  status_.step();

  // timestep preset object motion
  const real_t t = status_.t();
  for(auto& sb : bodies) {
    sb.update_scripted(t);
  }

  scene_.update_colli_cons(); // update collision constraints

  auto& cons = scene_.collision_constraints();

  // load position data in a single vector
  solver_.begin_iter(bodies, cons);
  for(auto i = 0;i < status_.num_iter;++ i) {
    // --- local solve ---
    // TODO: parallelize it
    for(auto& sb : bodies) { sb.project(); }
    for(auto& cf : cons) { cf->project(); }

    // --- global solve ---
    solver_.begin_solve();
    // populate the RHS vector b
    // TODO: parallelize
    for(auto const& sb : bodies) {
      for(auto const& e : sb.internal_energies()) {
        e->update_global_solver_rhs(&solver);
      }
    }
    for(auto& cf : cons) { cf->update_global_solver_rhs(&solver); }
    // solve Ax = b
    solver_.solve();
    solver_.store_pos(bodies);
  } // end subiter

  // update vel. and pos
  for(auto& sb : bodies) {
    sb.update_vel_pos(status_.dt);
  }

  if constexpr (!std::is_same_v<DataProc_, std::monostate>) {
    UNIMPLEMENTED
  }

  return status_.finished_steps;
}