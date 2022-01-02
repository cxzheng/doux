#include "doux/pd/global_solver.h"
#include "doux/pd/projective_energy.h"

NAMESPACE_BEGIN(doux::pd)

void GlobalSolver::init(std::vector<ProjDynBody>& sb, real_t dt2) {
  dt2_ = dt2;

  // total size of the global system
  size_t N = 0;
  uint32_t id = 0;
  for(auto& b : sb) {
    body_vec_map_[id] = N;
    b.set_id(id ++);
    N += b.num_free_vs();
  }

  // allocate memory
  diag_.assign(N, static_cast<real_t>(0));
  b_.resize(N);
  x_.resize(N);
  b0_.resize(N);
  off_diag_map_.resize(N);
  
  for(auto const& b : sb) {
    for(auto const& e : b.internal_energies()) {
      e->register_global_solve_elems(this);
    }
  } // end for

  // now convert the map into a plain vector
  off_diag_.resize(N);
}

/*
 * Store the solving result in pred_pos_ list of ProjDynBody
 */
void GlobalSolver::store_pos(std::vector<ProjDynBody>& sb) {
}

NAMESPACE_END(doux::pd)