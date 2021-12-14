#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

void MotiveBody::update_scripted(real_t t) {
  for(size_t i = num_fixed_;i < num_restricted_;++ i) {
    auto const j = i - num_fixed_;
    pos_[i] = script_[j](p0_[j], t);
  }
}

NAMESPACE_END(doux::pd)