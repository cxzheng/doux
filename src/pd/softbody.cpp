//******************************************************************************
// softbody.cpp -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

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