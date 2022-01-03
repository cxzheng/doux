//******************************************************************************
// env_collision.h -- This file is part of Doux, a realtime softbody simulation library
//
// Copyright (C) 2021 Changxi Zheng <cxz@cs.columbia.edu>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
//******************************************************************************

#pragma once

#include "doux/doux.h"
#inlcude "constraint.h"
#include "softbody.h"

NAMESPACE_BEGIN(doux::pd)

/*
 * Collision detector to detect collisions between a simulated object
 * and the environment. Provide a virtual method for detecting the collisions.
 * 
 */
class EnvColliConsBuilder {
 public:
  // return how many collision constraints are added
  virtual int update(MotiveBody& b, std::vector<std::unique_ptr<CFunc>>& ret_cons) = 0;
};

class PlaneColliConsBuilder {
 public:
  // return how many collision constraints are added
  int update(MotiveBody& b, std::vector<std::unique_ptr<CFunc>>& ret_cons) override;
};

NAMESPACE_END(doux::pd)