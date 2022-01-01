#pragma once

#include <assert.h>
#include "doux/shape/shape.h"

NAMESPACE_BEGIN(doux::pd)

enum struct ProjEnergyType : uint32_t {
  TET_ASAP = 1,
  TRI_ASAP = 2,
  PLANE_COLLISION = 3,
};

class ProjDynBody;

/*
 * abstract class for projective energy term
 */
class ProjEnergy {
 public:
  ProjEnergy() = delete;
  ProjEnergy(const ProjEnergy&) = default;
  ProjEnergy(ProjEnergy&&) = default;
  ProjEnergy& operator = (const ProjEnergy&) = default;
  ProjEnergy& operator = (ProjEnergy&&) = default;

  DOUX_ATTR(nonnull) ProjEnergy(ProjDynBody* sb) noexcept : body_{sb} {}
  DOUX_ATTR(nonnull) ProjEnergy(ProjDynBody* sb, real_t stiff) : 
      body_{sb}, stiffness_{stiff} {
    assert(stiff > 0);
  }

  virtual ~ProjEnergy() {}

  virtual void project() = 0;

 private:
  ProjDynBody*  body_;
  // stiffness of this energy term
  real_t stiffness_ {1};
};

class PlaneColliEnergy : public ProjEnergy {
 public:
  // The energy type info is needed when grouping energy terms together for 
  // batch processing on GPUs
  static constexpr ProjEnergyType Type = ProjEnergyType::PLANE_COLLISION;

  void project() override;

 private:
  uint32_t v_;
  Plane3<real_t>  plane_;
};

/*
 * Simple Corotationa energy for a tet
 * 
 */
class TetCorotEnergy : public ProjEnergy {
 public:
  // The energy type info is needed when grouping energy terms together for 
  // batch processing on GPUs
  static constexpr ProjEnergyType Type = ProjEnergyType::TET_ASAP;

  void project() override;

 private:
  uint32_t v_[4];
};

NAMESPACE_END(doux::pd)