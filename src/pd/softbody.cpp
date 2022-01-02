#include "doux/pd/softbody.h"

NAMESPACE_BEGIN(doux::pd)

void MotiveBody::predict_vel_pos(const Vec3r& a, real_t dt) {
  for(size_t i = num_restricted_;i < vel_.size();++ i) {
    vel_[i] += a*dt;
    pred_pos_[i] = pos_[i] + vel_[i]*dt;
  }
}

void MotiveBody::predict_pos(real_t dt) {
  for(size_t i = num_restricted_;i < vel_.size();++ i) {
    pred_pos_[i] = pos_[i] + vel_[i]*dt;
  }
}

void MotiveBody::update_vel_pos(real_t dt) {
  const real_t inv_dt = static_cast<real_t>(1) / dt;
  UNIMPLEMENTED

  for(size_t i = num_restricted_;i < vel_.size();++ i) {
    vel_[i] = (pred_pos_[i] - pos_[i]) * inv_dt;
    pos_[i] = pred_pos_[i];
  }
}

NAMESPACE_END(doux::pd)