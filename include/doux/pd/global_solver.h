#pragma once

#include "doux/doux.h"
#include "softbody.h"
#include <unordered_map>
#include <cstring>

NAMESPACE_BEGIN(doux::pd)

class GlobalSolver {
 public:
  /* 
   * initialize the solver
   * 1. allocate space for sparse matrix A
   * 2. set softbody ID
   * 
   * dt2: dt^2
   */
  void init(std::vector<ProjDynBody>& sb, real_t dt2);

  /*
   * This method is called at the beginning of subiterations in each PD timestep 
   *
   * - load the predicted positions
   */
  void begin_iter(const std::vector<ProjDynBody>& sb);

  /*
   * This method is called at the end of subiterations in each PD timestep 
   * to store the solving results in the softbodies positions
   */
  void store_pos(std::vector<ProjDynBody>& sb);

  /*
   * prepare the RHS
   */
  void begin_solve() {
    std::memcpy(b_.data(), b0_.data(), sizeof(real_t) * b_.size());
  }

  // Solve Ax = b
  virtual void solve() = 0;

  // return the index of a vertex of a particular softbody
  [[nodiscard]] DOUX_ALWAYS_INLINE
  DOUX_ATTR(nonnull) size_t vtx_id(const ProjDynBody* sb, size_t vid) const {
    assert(sb);
    return body_vec_map_[sb->id()] + sb->free_vtx_id(vid);
  }

  // -------------------------------------------------------------
  // these two methods will be called by ProjEnergy instances to 
  // fill in matrix elements according to their definitions
  void add_elem(const ProjDynBody* sb, size_t vid, real_t val) {
    diag_[vtx_id(sb, vid)] += val;
  }

  void add_elem(const ProjDynBody* sb1, size_t vid1,
			          const ProjDynBody* sb2, size_t vid2, real_t val) {
    auto const v1 = vtx_id(sb1, vid1);
    auto const v2 = vtx_id(sb2, vid2);
    off_diag_map_[v1][v2] += val;
    off_diag_map_[v2][v1] += val;
  }
  // -------------------------------------------------------------

 protected:
  struct MatElem {
    uint32_t cid {0}; // column ID of the matrix element
    real_t   val {0}; // the matrix entry value

    MatElem() = default;
    MatElem(uint32_t c, real_t v) noexcept : cid{c}, val{v} {}
  };

  real_t dt2_;

  std::vector<real_t> diag_;
  std::vector<real_t> b_;  // RHS vector for Ax = b
  std::vector<real_t> x_;  // x vector for storing solving results
  std::vector<real_t> b0_; // M * s_n
  std::vector<std::vector<MatElem>> off_diag_;

  // map each softbody ID to its starting position in the sparse matrix/vector
  // body_vec_map_[i]: the starting index in the sparse matrix/vector for 
  //                   the softbody with ID i
  std::vector<size_t> body_vec_map_;

  // backup the diag. elements, as the collision enregy terms 
  // may change them
  std::vector<real_t> diag_backup_; 
  std::vector<std::unordered_map<uint32_t, real_t>> off_diag_map_;
};

class GlbGaussSeidelSolver : public GlobalSolver {
 public:

 private:
};

NAMESPACE_END(doux::pd)