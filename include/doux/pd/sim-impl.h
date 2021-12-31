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

template <class Scene_, class ExtForce_, class DataProc_> 
size_t ProjDynSim<Scene_, ExtForce_, DataProc_>::step() {
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
  for(auto i = 0;i < status_.num_iter;++ i) {
    // --- local solve ---
    for(auto& sb : scene_.deformables()) {
      sb.project();
    }
    auto const& cons = scene_.collision_constraints();
    for(auto& cf : cons) {
      cf.project();
    }

    // --- global solve ---
  } // end subiter

  // update vel. and pos
  for(auto& sb : scene_.deformables()) {
    sb.update_vel_pos(status_.dt);
  }

  if constexpr (!std::is_same_v<DataProc_, std::monostate>) {
    UNIMPLEMENTED
  }

  return status_.finished_steps;
}