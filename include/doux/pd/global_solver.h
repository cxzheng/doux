#pragma once

#include "doux/doux.h"

NAMESPACE_BEGIN(doux::pd)

class GblSolver {
 protected:
  struct MatElem {
    uint32_t cid; // column ID of the matrix element
    real_t   val; // the matrix entry value
  };

  std::vector<real_t> diag_;
  std::vector<std::vector<real_t>> off_diag_;
};

class GblGaussSeidelSolver : public GblSolver {
 public:

 private:
};

NAMESPACE_END(doux::pd)