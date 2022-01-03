//******************************************************************************
// global_solver.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#include "doux/pd/global_solver.h"
#include "doux/pd/projective_energy.h"

// References:
//
// [1] Bouaziz, S., Martin, S., Liu, T., Kavan, L. and Pauly, M., 2014. 
// Projective dynamics: Fusing constraint projections for fast simulation. 
// ACM transactions on graphics (TOG), 33(4), pp.1-11.
//
NAMESPACE_BEGIN(doux::pd)

void GlobalSolver::init(std::vector<ProjDynBody>& sb, real_t dt2) {
  dt2_ = dt2;

  body_vec_map_.resize(sb.size());
  // total size of the global system
  size_t N = 0;
  uint32_t id = 0;
  for(auto& b : sb) {
    body_vec_map_[id] = N;
    b.set_id(id ++);
    N += b.num_free_vs();
  }

  // allocate memory
  diag_.resize(N);	// method from Eigen
  b_.resize(N);
  x_.resize(N);
  b0_.resize(N);
  off_diag_map_.resize(N);
  
  // fill diagonal & off-diagonal elements in A
  id = 0;
  for(auto const& b : sb) {
    // diagonal elements has mass
    auto const& mass = b.mass();
    assert(body_vec_map_[id] + b.num_free_vs() <= diag_.size());
    diag_.segment(body_vec_map_[id ++], b.num_free_vs()) = mass.tail(b.num_free_vs());

    // M + h^2 \sum (w_i A^T A x): Eq.(10) in [1]
    for(auto const& e : b.internal_energies()) {
      e->register_global_solve_elems(this);
    }
  } // end for

  diag_backup_ = diag_;
  // now convert the map into a plain vector
  off_diag_.resize(N);
}

/*
 * Store the solving result in pred_pos_ list of ProjDynBody
 */
void GlobalSolver::store_pos(std::vector<ProjDynBody>& sb) {
}

NAMESPACE_END(doux::pd)