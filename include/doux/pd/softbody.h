//******************************************************************************
// softbody.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

/*
 * This header defines a deformable body that can be simulated using PBD or projective dynamics.
 */

#include <vector>
#include <span>
#include "doux/core/platform.h"
#include "doux/core/svec.h"
#include "doux/linalg/num_types.h"
#include "constraint.h"

NAMESPACE_BEGIN(doux::pd)

// Maintain a basic data structure to describe a softbody
class Softbody {
 public:
  // ------ constructors ------
  Softbody() = delete;
  Softbody(const Softbody&) = delete;
  Softbody(Softbody&&) = default;
  Softbody& operator = (const Softbody&) = delete;
  Softbody& operator = (Softbody&&) = default;

  template<typename POS_, typename FS_>
  Softbody(POS_&& pos, FS_&& fs) : 
      pos_{std::forward<POS_>(pos)}, faces_{std::forward<FS_>(fs)} {
    assert(faces_.cols() == 3 && "faces must be a N x 3 index matrix");

    vel_.resize(pos_.size());
    mass_ = linalg::vector_r_t::Ones(pos_.size()); // initalize with unit mass

    for_each(vel_.begin(), vel_.end(), [] (auto& v) { v.set_zero(); });
  }

  // return total number of vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE size_t num_vtx() const noexcept { return pos_.size(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  std::vector<Vec3r>& vtx_pos() { return pos_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto const& vtx_pos(size_t vid) const { 
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
  std::vector<Vec3r>& vtx_vel() { return vel_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto const& vtx_vel(size_t vid) const { 
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
        : mass_(vid);
#else
    return mass_(vid); 
#endif
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  const linalg::vector_r_t& mass() const { return mass_; }

 protected:
  // list of vertices sampled on the body (volume or cloth)
  // position, velocity, mass
  std::vector<Vec3r>  pos_;   // vertex positions
  std::vector<Vec3r>  vel_;   // vertex velocity
  linalg::vector_r_t  mass_;  // mass

  // surface vertex information for collision detection
  // This is a M x N matrix, where M is the number of surface faces,
  // and N is the number of vertices on each face. E.g., for triangle 
  // surface mesh, N = 3
  linalg::matrix_i_t  faces_;
};

// -----------------------------------------------------------------------

// Extend the softbody to enable fixed and scripted vertices
class MotiveBody : public Softbody {
 public:
  using MotionFunc = std::function<Vec3r(const Vec3r&, real_t)>;

  template<typename POS_, typename FS_>
  MotiveBody(POS_&& pos, FS_&& fs) : 
      Softbody{std::forward<POS_>(pos), std::forward<FS_>(fs)},
      num_free_{pos_.size()}, pred_pos_{pos_.size()} {}

  // This constructor will be called by `build_softbody` in motion_preset.h
  template<typename POS_, typename FS_>
  MotiveBody(POS_&& pos, FS_&& fs, size_t nfixed, 
             std::vector<Vec3r>&& p0,
             std::vector<MotionFunc>&& script) : 
      Softbody{std::forward<POS_>(pos), std::forward<FS_>(fs)},
      num_fixed_{nfixed}, num_restricted_{nfixed + p0.size()},
      num_free_{pos_.size() - num_restricted_},
      p0_{std::move(p0)}, script_{std::move(script)}, 
      pred_pos_{pos_.size()} {
    assert(p0_.size() == script_.size() && num_restricted_ <= pos_.size());
  }

  MotiveBody() = delete;
  MotiveBody(const MotiveBody&) = delete;
  MotiveBody(MotiveBody&&) = default;
  MotiveBody& operator = (const MotiveBody&) = delete;
  MotiveBody& operator = (MotiveBody&&) = default;

  // -------------------------------------------------

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool is_fixed(size_t vid) const { return vid < num_fixed_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool is_restricted(size_t vid) const { return vid < num_restricted_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  std::span<Vec3r const> free_vtx_pos() const { 
    return std::span{pos_.data() + num_restricted_, num_free_};
  }

  // return the number of fixed vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_fixed_vs() const { return num_fixed_; }

  // return the number of scripted vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_scripted_vs() const { return p0_.size(); }

  // return the number of restricted vertices (fixed + scripted)
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_restricted_vs() const { return num_restricted_; }

  // return the number of free vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_free_vs() const { return num_free_; }

  // Return the vertex ID of a free vertex
  // Must ensure the input vid refers to a free vertex
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t free_vtx_id(size_t vid) const { 
    assert(vid >= num_restricted_);
    return vid - num_restricted_; 
  }

  // return the initial positions of scripted vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto const& init_scripted_pos() const { return p0_; }

  // Explicitly update vel. and pos. by a uniform acceleration 
  // v += a*dt
  // p += v*dt
  void predict_vel_pos(const Vec3r& a, real_t dt);

  // Explicitly update vel. and pos. by a uniform acceleration 
  // v stays unchanged
  // p += v*dt
  void predict_pos(real_t dt); 

  // finish iterations and update the vel. and positions
  void update_vel_pos(real_t dt);

  // update the position of scripted vertices, if any
  void update_scripted(real_t t) {
    for(size_t i = num_fixed_;i < num_restricted_;++ i) {
      auto const j = i - num_fixed_;    // index of the scripted vertex in p0 list
      pred_pos_[i] = pos_[i] = script_[j](p0_[j], t);
    }
  }

 protected:
  size_t    num_fixed_{0};          // number of fixed vertices
  size_t    num_restricted_{0};     // number of fixed + number of scripted
  size_t    num_free_{0};           // number of free vertices
  std::vector<Vec3r>      p0_;      // initial positions of scripted vertices
  std::vector<MotionFunc> script_;  // scripted vertex motion, one for each scripted vertex
  std::vector<Vec3r>      pred_pos_;// to store predicted vertex positions
};

// -----------------------------------------------------------------------

class GlobalSolver;  // linear solver for the global step
class ProjEnergy;

class ProjDynBody : public MotiveBody {
 friend class GlobalSolver;

 public:
  // the project (local solve) step
  // This method apply the local solve step on all internal energy terms of the softbody
  void project();

  [[nodiscard]] DOUX_ALWAYS_INLINE uint32_t id() const { return id_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  const std::vector<std::unique_ptr<ProjEnergy>>& internal_energies() const { return e_; } 

 private:
  // the ID will be set by `Glb`
  void set_id(uint32_t id) noexcept { id_ = id; }

 private:
  uint32_t id_;
  // the softbody's implicit energy terms
  std::vector<std::unique_ptr<ProjEnergy>> e_; 
};

// -----------------------------------------------------------------------

// Extend to use constraints to generate internal forces, so they can be used
// in PD framework.
class PBDBody : public MotiveBody {
 public:
 private:
  // list of constraints for generating internal forces
  std::vector<std::unique_ptr<CFunc>> cons_;        
};

NAMESPACE_END(doux::pd)