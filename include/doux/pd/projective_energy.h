#pragma once

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

enum struct ProjEnergyType : uint32_t {
  TET_ASAP = 1,
  TRI_ASAP = 2,
};

/*
 * abstract class for projective energy term
 */
class ProjEnergy {
 public:
  virtual void project() = 0;
};

NAMESPACE_END(doux::pd)