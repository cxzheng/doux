#include <Eigen/LU>
#include "doux/pd/projective_energy.h"
#include "doux/shape/tet.h"
#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

void PlaneColliEnergy::project() {
}

// ------------------------------------------------------

TetCorotEnergy::TetCorotEnergy(ProjDynBody* b, real_t s, size_t v0, size_t v1, size_t v2, size_t v3) :
    ProjEnergy(b, s), v_{v0, v1, v2, v3} {
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
}

void TetCorotEnergy::project() {
}

void TetCorotEnergy::register_global_solve_elems(GlobalSolver* solver) {
}

void TetCorotEnergy::update_global_solve_rhs(GlobalSolver* solver) {
}
NAMESPACE_END(doux::pd)