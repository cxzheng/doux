#pragma once

/*
 * This header defines the core PD-based simulation algorithm
 */

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

struct SimStatus {
};

/*
 * XPBD simulator
 */
template <class Scene_>
class XPBDSim {
 public:
  /// Timestep the simulation
  size_t step();

 private:
  SimStatus status_;
};

// --------------------------------------------------------------------------------

template <class Scene_>
size_t XPBDSim<Scene_>::step() {
}

NAMESPACE_END(doux::pd)