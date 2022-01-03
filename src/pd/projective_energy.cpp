#include <Eigen/LU>
#include "doux/pd/projective_energy.h"
#include "doux/shape/tet.h"
#include "doux/pd/softbody.h"
#include "doux/pd/global_solver.h"

NAMESPACE_BEGIN(doux::pd)

void PlaneColliEnergy::project() {
}

// ------------------------------------------------------

TetCorotEnergy::TetCorotEnergy(ProjDynBody* b, real_t s, size_t v0, size_t v1, size_t v2, size_t v3) :
    ProjEnergy(b, s), v_{v0, v1, v2, v3},
    restricted_vtx_{b->is_restricted(v0), b->is_restricted(v1), 
                    b->is_restricted(v2), b->is_restricted(v3)} {
  assert(b);

  // rest positions
  auto const& X0 = b->vtx_pos(v0); // vec3r
  auto const& X1 = b->vtx_pos(v1);
  auto const& X2 = b->vtx_pos(v2);
  auto const& X3 = b->vtx_pos(v3);

  // compute D_inv_
  Vec3r XS[3] = {X1 - X0, X2 - X0, X3 - X0};
  // check the tet has non-zero volume
  assert(shape::signed_tet_volume(XS[0], XS[1], XS[2]) > eps<real_t>::v);
  linalg::mat3_r_t m = Eigen::Map<linalg::mat3_r_t, 0, 
                                  Eigen::OuterStride<sizeof(Vec3r)/sizeof(real_t)>>
                                  (reinterpret_cast<real_t*>(XS));
  D_inv_ = m.inverse();
  d_sum_ = D_inv_.colwise().sum();
}

real_t TetCorotEnergy::val() const {
  // current positions
  auto const& x0 = body_->vtx_pos(v_[0]); // vec3r
  auto const& x1 = body_->vtx_pos(v_[1]);
  auto const& x2 = body_->vtx_pos(v_[2]);
  auto const& x3 = body_->vtx_pos(v_[3]);

  // compute D_inv_
  Vec3r xs[3] = {x1 - x0, x2 - x0, x3 - x0};
  linalg::mat3_r_t m = Eigen::Map<linalg::mat3_r_t, 0, 
                                  Eigen::OuterStride<sizeof(Vec3r)/sizeof(real_t)>>
                                  (reinterpret_cast<real_t*>(xs));
  return (m * D_inv_ - r_).squaredNorm() * stiffness_ * static_cast<real_t>(0.5);
}

void TetCorotEnergy::project() {
}

void TetCorotEnergy::register_global_solve_elems(GlobalSolver* solver) {
  for(int i = 0;i < 3;++ i) {
    for(int j = 0;j < 4;++ j) {
      // diagonal element
      if ( restricted_vtx_[j] ) [[unlikely]] continue;

      real_t cj = j == 0 ? -d_sum_(i) : D_inv_(j-1, i);
      solver->add_elem(body_, v_[j], cj*cj*stiffness_);
      // off-diagonal element
      for(int k = 0;k < j;++ k) {
        if ( restricted_vtx_[k] ) [[unlikely]] continue;

        real_t ck = k == 0 ? -d_sum_(i) : D_inv_(k-1, i);
        solver->add_elem(body_, v_[j], body_, v_[k], cj*ck*stiffness_);
      }
    }
  } // end for i
}

void TetCorotEnergy::update_global_solve_rhs(GlobalSolver* solver) {
}

NAMESPACE_END(doux::pd)