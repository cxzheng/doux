#pragma once

#include "doux/core/platform.h"
#include "doux/core/point.h"
#include "doux/linalg/num_types.h"

NAMESPACE_BEGIN(doux::shape)

// D_ = 1: graph-like polyline
// D_ = 2: Triangle mesh
// D_ = 3: Tet mesh
template<size_t D_>
requires(D_ > 0 && D_ < 4)
class Mesh {
 public:
  Mesh() = delete;
  Mesh(const Mesh&) = delete;
  Mesh(Mesh&&) = default;
  Mesh& operator = (const Mesh&) = delete;
  Mesh& operator = (Mesh&&) = default;

  Mesh(linalg::matrix_r_t&& x, linalg::matrix_i_t&& ele) :
      x_{std::move(x)}, ele_{std::move(ele)} {
    if ( auto c = x_.cols(); c != 3 ) [[unlikely]] {
      throw std::invalid_argument(fmt::format(
	      "The provided x matrix must have 3 columns, yet it has {} columns",
	      c));
    }

    // triangle mesh: 3 columns 
    // tet mesh: 4 columns
    if ( auto c = ele_.cols(); c != D_+1 ) [[unlikely]] {
      throw std::invalid_argument(fmt::format(
	      "The provided ele matrix must have {} columns, yet it has {} columns",
	      D_, c));
    }
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  size_t num_elements() const { return ele_.rows(); }

  // Return vertex positions as a matrix
  [[nodiscard]] DOUX_ALWAYS_INLINE
  const linalg::matrix_r_t& vertices() const { return x_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  const linalg::matrix_i_t& elements() const { return ele_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  size_t num_vertices() const { return x_.rows(); }

  [[nodiscard]]
  std::vector<Point3r> vtx_pos() const;

 private:
  // vertex positions
  linalg::matrix_r_t x_;
  // element vertex indices
  linalg::matrix_i_t ele_;
};

// ---------------------------------------------------------------------------------------

template<size_t D_>
requires(D_ > 0 && D_ < 4)
[[nodiscard]] std::vector<Point3r> Mesh<D_>::vtx_pos() const {
  std::vector<Point3r> ret(x_.rows());
  for(size_t i = 0;i < ret.size();++ i) {
    ret[i].set(x_(i, 0), x_(i, 1), x_(i, 2));
  }
  //fmt::print("dd {}\n", (void*)ret.data());
  return ret; // TEST: check copy elision
}

NAMESPACE_END(doux::shape)