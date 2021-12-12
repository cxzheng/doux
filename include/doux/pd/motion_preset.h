#pragma once

/*
 * This header specifies preset motion, including fixed vertices and vertices undergoes 
 * scripted motion
 */

#include <unordered_map>
#include <assert.h>
#include <optional>
#include "softbody.h"
#include "doux/shape/mesh.h"

NAMESPACE_BEGIN(doux::pd)

// D_ = 2: 2D mesh (e.g., a cloth)
// D_ = 3: 3D mesh (i.e., a tet mesh)
// NOTE: The D_ template parameter here is different from that in shape::Mesh
template <size_t D_>
requires(D_ > 1 && D_ < 4)
class MotionPreset {
 public:
  using MotionFunc = std::function<Point3r(Point3r, real_t)>;

  MotionPreset() = delete;
  MotionPreset(MotionPreset&) = delete;
  MotionPreset(MotionPreset&&) = delete;

  explicit MotionPreset(const shape::Mesh<D_>& msh) : 
      mesh_{msh}, vtag_(msh.num_vertices(), 0) {}

  // Return if the underlying mesh is restricted, either with fixed vertices
  // or having scripted vertices
  [[nodiscard]] DOUX_ALWAYS_INLINE 
  bool restricted() const { return n_fixed_ > 0 || !script_.empty(); }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_fixed_vs() const { return n_fixed_; }

  [[nodiscard]] DOUX_ALWAYS_INLINE 
  size_t num_scripted_vs() const { return script_.size(); }

  // set the vertex indexed by vid to be fixed
  inline void fix_vertex(uint32_t vid) {
    assert(vid < vtag_.size() && "fixed vertex is out of the range");
    if ( vtag_[vid] == 2 ) [[unlikely]] {
      throw std::runtime_error(fmt::format(
          "the vertex [ID={}] supposed to be fixed has been scripted",
          vid));
    }
    // vtag_[vid] = 0 or 1
    n_fixed_ += (vtag_[vid]==0);
    vtag_[vid] = 1;
  }

  // set the vertex vid to move as scripted
  void script_vertex(uint32_t vid, MotionFunc&& script) {
    assert(vid < vtag_.size() && "fixed vertex is out of the range");
    if ( vtag_[vid] != 0 ) [[unlikely]] {
      throw std::runtime_error(fmt::format(
          "the vertex [ID={}] has already been assigned with tag: {}",
          vid, vtag_[vid]));
    } 
    vtag_[vid] = 2;
    script_.insert({vid, std::move(script)});
  }

  [[nodiscard]] DOUX_ALWAYS_INLINE
  const shape::Mesh<D_>& mesh() const { return mesh_; }

  // return a list of vertex IDs that re-order the mesh vertices according to
  // the fixed and scripted vertices, such that the fixed vertices are always 
  // at the beginning of the list followed by the scripted vertices and then 
  // free vertices.
  // If there is no fixed or scripted vertices, std::nullopt is returned
  [[nodiscard]] std::optional<std::vector<uint32_t>> reorder_vertices() const; 

 private:
  const shape::Mesh<D_>&  mesh_;
  size_t                  n_fixed_ {0};

  // vertex label: 
  // 0: free vertices
  // 1: fixed 
  // 2: scripted
  std::vector<uint8_t>    vtag_;

  std::unordered_map<uint32_t, MotionFunc> script_;
};

// -----------------------------------------------------------------------------------------------

template <size_t D_>
requires(D_ > 1 && D_ < 4)
[[nodiscard]] std::optional<std::vector<uint32_t>> MotionPreset<D_>::reorder_vertices() const {
  if (!restricted()) { return std::nullopt; }

  // now we need to re-order the vertices
  std::vector<uint32_t> ret(vtag_.size());
  size_t filled = 0;
  for(size_t i = 0;i < vtag_.size();++ i) {
    if ( vtag_[i] == 1 ) ret[filled ++] = i;
  }

  for(size_t i = 0;i < vtag_.size();++ i) {
    if ( vtag_[i] == 2 ) ret[filled ++] = i;
  }

  for(size_t i = 0;i < vtag_.size();++ i) {
    if ( vtag_[i] == 0 ) ret[filled ++] = i;
  }
  assert(filled == vtag_.size());
  return ret;
}

NAMESPACE_BEGIN(internal)

// idmap[i]: the i-th vertex of the ordered mesh --> vertex ID of the original mesh
// D_ = 2: tri mesh
// D_ = 3: tet mesh
template <size_t D_>
shape::Mesh<D_> ordered_mesh(const std::vector<uint32_t>& idmap, const shape::Mesh<D_>& mesh) {
  auto const& x = mesh.vertices();
  auto const& e = mesh.elements();

  // inv_map[i]: the i-th vertex of origional mesh --> vertex ID of the ordered mesh
  std::vector<uint32_t> inv_map(idmap.size());
  for(size_t i = 0;i < idmap.size();++ i) {
    inv_map[idmap[i]] = i;
  }

  // the following matrices will be moved into the returned mesh
  linalg::matrix_r_t ordered_x(x.rows(), x.cols());
  linalg::matrix_i_t ordered_e(e.rows(), e.cols());
  for(size_t i = 0;i < idmap.size();++ i) {
    ordered_x.row(i) = x.row(idmap[i]);
  }
  assert(ordered_e.cols() == D_ + 1);
  for(size_t i = 0;i < e.rows();++ i) 
    for(size_t j = 0;j <= D_;++ j) {
      assert(e(i, j) < inv_map.size());
      ordered_e(i, j) = inv_map[e(i, j)];
    }

  return shape::Mesh<D_>(std::move(ordered_x), std::move(ordered_e));
}

NAMESPACE_END(internal)

// consumes the softbody setup and produce immutable motion preset for simulation.
// This ensures that once the simulation starts, the motion preset won't be changed.
template <size_t D_>
requires(D_ > 1 && D_ < 4)
std::tuple<MotiveBody, std::optional<shape::Mesh<D_>>> 
build_softbody(const MotionPreset<D_>& preset) {

  // reorder mesh vertex order
  auto id_map = preset.reorder_vertices();
  if ( !id_map ) {
    if constexpr (D_ == 2) {
      // collect vertices
      return {MotiveBody(preset.mesh().vtx_pos(), preset.mesh().elements()), 
              std::nullopt};
    } else {
      // extract surface mesh
      UNIMPLEMENTED
    }
  } // end if id_map

  // create another mesh that uses the recorded vertices
  auto ordered_msh = internal::ordered_mesh(id_map.value(), preset.mesh());

  // populate p0: initial positions of scripted vertices
  std::vector<Point3r> p0(preset.num_scripted_vs());
  // populate script
  std::vector<typename MotionPreset<D_>::MotionFunc> script;

  if constexpr (D_ == 2) {
      return {MotiveBody(ordered_msh.vtx_pos(), ordered_msh.elements(), 
                         preset.num_fixed_vs(), std::move(p0), std::move(script)), 
              std::move(ordered_msh)};
  } else {
    // extract surface mesh
    UNIMPLEMENTED
  }
}

NAMESPACE_END(doux::pd)