#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

SoftBody::SoftBody(std::vector<Point3r>&& pos) :
        pos_(std::move(pos)) {
  vel_.resize(pos_.size());
  mass_.resize(pos_.size(), (real_t)1); // initalize with unit mass

  for_each(vel_.begin(), vel_.end(), [] (auto& v) { v.set_zero(); });
}

NAMESPACE_END(doux::pd)