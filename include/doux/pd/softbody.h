#pragma once

/*
 * This header defines a deformable body that can be simulated using PBD or projective dynamics.
 */

#include <vector>
#include "doux/core/platform.h"
#include "doux/core/point.h"
#include "doux/linalg/num_types.h"

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
    mass_.resize(pos_.size(), (real_t)1); // initalize with unit mass

    for_each(vel_.begin(), vel_.end(), [] (auto& v) { v.set_zero(); });
  }

  // return total number of vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE size_t num_vtx() const noexcept { return pos_.size(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  std::vector<Point3r>& vtx_pos() { return pos_; }

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
        : mass_[vid];
#else
    return mass_[vid]; 
#endif
  }

 protected:
  // list of vertices sampled on the body (volume or cloth)
  // position, velocity, mass
  std::vector<Point3r>  pos_;   // vertex positions
  std::vector<Vec3r>    vel_;   // vertex velocity
  std::vector<real_t>   mass_;  // mass

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
  using MotionFunc = std::function<Point3r(const Point3r&, real_t)>;

  template<typename POS_, typename FS_>
  MotiveBody(POS_&& pos, FS_&& fs) : 
      Softbody{std::forward<POS_>(pos), std::forward<FS_>(fs)} {}

  // This constructor will be called by `build_softbody` in motion_preset.h
  template<typename POS_, typename FS_>
  MotiveBody(POS_&& pos, FS_&& fs, size_t nfixed, 
             std::vector<Point3r>&& p0,
             std::vector<MotionFunc>&& script) : 
      Softbody{std::forward<POS_>(pos), std::forward<FS_>(fs)},
      num_fixed_{nfixed}, num_restricted_{nfixed + p0.size()},
      p0_{std::move(p0)}, script_{std::move(script)} {
    assert(p0_.size() == script_.size());
  }

  MotiveBody() = delete;
  MotiveBody(const MotiveBody&) = delete;
  MotiveBody(MotiveBody&&) = default;
  MotiveBody& operator = (const MotiveBody&) = delete;
  MotiveBody& operator = (MotiveBody&&) = default;

  // -------------------------------------------------

  // update the position of scripted vertices, if any
  inline void update_scripted(real_t t) {
    for(size_t i = num_fixed_;i < num_restricted_;++ i) {
      auto const j = i - num_fixed_;
      pos_[i] = script_[j](p0_[j], t);
    }
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_fixed_vs() const { return num_fixed_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_scripted_vs() const { return p0_.size(); }

  // return the initial positions of scripted vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  auto const& init_scripted_pos() const { return p0_; }

 protected:
  size_t    num_fixed_{0};         // number of fixed vertices
  size_t    num_restricted_{0};    // number of fixed + number of scripted
  std::vector<Point3r>    p0_;       // initial positions of scripted vertices
  std::vector<MotionFunc> script_;   // scripted vertex motion, one for each scripted vertex
};

// -----------------------------------------------------------------------

// Extend to use constraints to generate internal forces, so they can be used
// in PBD framework.
class PBDBody : public MotiveBody {
 public:
  // Explicitly update vel. and pos. by a uniform acceleration 
  // v += a*dt
  // p += v*dt
  void predict_vel_pos(const Vec3r& a, real_t dt);

  // update the position of scripted vertices, if any
  void update_scripted(real_t t) {
    MotiveBody::update_scripted(t);
    for(size_t i = num_fixed_;i < num_restricted_;++ i) {
      pred_pos_[i] = pos_[i];
    }
  }

 private:
  std::vector<Point3r>      pred_pos_; // predicted vertex positions
  // list of constraints for generating internal forces
};

NAMESPACE_END(doux::pd)