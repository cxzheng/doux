#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

void PBDBody::predict_vel_pos(const Vec3r& a, real_t dt) {
  for(size_t i = num_restricted_;i < vel_.size();++ i) {
    vel_[i] += a*dt;
    pred_pos_[i] = pos_[i] + vel_[i]*dt;
  }
}

NAMESPACE_END(doux::pd)