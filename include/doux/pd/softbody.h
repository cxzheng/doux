#pragma once

/*
 * This header defines a deformable body that can be simulated using PBD or projective dynamics.
 */

#include <vector>
#include "doux/core/point.h"

NAMESPACE_BEGIN(doux::pd)

class SoftBody {
 public:
  // ------ constructors ------
  SoftBody() = delete;
  SoftBody(const SoftBody&) = delete;
  SoftBody(SoftBody&&) = delete;
  SoftBody& operator = (const SoftBody&) = delete;
  SoftBody& operator = (SoftBody&&) = delete;

  SoftBody(std::vector<Point3r>&& pos);

  // return total number of vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE size_t num_vtx() const noexcept { return pos_.size(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  decltype(auto) vtx_pos(size_t vid) const { 
#ifndef NDEBUG
    return ( vid >= pos_.size() ) ? throw std::out_of_range(fmt::format(
        "vtx_pos(vid) call: out of range access. Tried to access index {0:d}, "
        "but only {1:d} vertices exists", vid, pos_.size()))
        : pos_[vid];
#else
    return pos_[vid]; 
#endif
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  decltype(auto) vtx_vel(size_t vid) const { 
#ifndef NDEBUG
    return ( vid >= vel_.size() ) ? throw std::out_of_range(fmt::format(
        "vtx_vel(vid) call: out of range access. Tried to access index {0:d}, "
        "but only {1:d} vertices exists", vid, vel_.size()))
        : vel_[vid];
#else
    return vel_[vid]; 
#endif
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  decltype(auto) vtx_mass(size_t vid) const { 
#ifndef NDEBUG
    return ( vid >= mass_.size() ) ? throw std::out_of_range(fmt::format(
        "vtx_mass(vid) call: out of range access. Tried to access index {0:d}, "
        "but only {1:d} vertices exists", vid, mass_.size()))
        : mass_[vid];
#else
    return mass_[vid]; 
#endif
  }

 private:
  // list of vertices sampled on the body (volume or cloth)
  // position, velocity, mass
  std::vector<Point3r>  pos_;   // vertex positions
  std::vector<Vec3r>    vel_;   // vertex velocity
  std::vector<real_t>   mass_;  // mass

  // list of constraints for generating internal forces
};

NAMESPACE_END(doux::pd)