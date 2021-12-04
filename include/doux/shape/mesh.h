#pragma once

#include "doux/doux.h"
#include "doux/linalg/num_types.h"

NAMESPACE_BEGIN(doux::shape)

// D_ = 3: Triangle mesh
// D_ = 4: Tet mesh
template<size_t D_>
requires(D_ > 1 && D_ < 5)
class Mesh {
 public:
  Mesh() = delete;

  Mesh(linalg::matrix_r_t&& x, linalg::matrix_i_t&& ele) :
      x_{std::move(x)}, ele_{std::move(ele)} {
    if ( auto c = x.cols(); c != 3 ) [[unlikely]] {
      throw std::invalid_argument(fmt::format(
	  "The provided x matrix must have 3 columns, yet it has {} columns",
	  c));
    }

    if ( auto c = ele.cols(); c != D_ ) [[unlikely]] {
      throw std::invalid_argument(fmt::format(
	  "The provided ele matrix must have {} columns, yet it has {} columns",
	  D_, c));
    }
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  size_t num_elements() const { return ele_.rows(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  size_t num_vertices() const { return x_.rows(); }

 private:
  // vertex positions
  linalg::matrix_r_t x_;
  // element vertex indices
  linalg::matrix_i_t ele_;
};

NAMESPACE_END(doux::shape)