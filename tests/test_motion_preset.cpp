#include <gtest/gtest.h>

#include "common.h"
#include "doux/pd/motion_preset.h"

TEST(TestPDMotionPreset, NoFixedSetup) {
  using namespace doux;

  linalg::matrix_r_t x(3, 3);
  linalg::matrix_i_t e(1, 3);
  x << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  e << 0, 1, 2;
  shape::Mesh<2> msh(std::move(x), std::move(e));
  pd::MotionPreset<2> preset(msh);
  EXPECT_FALSE(preset.restricted());

  auto [a, b] = pd::build_softbody(std::move(preset));
  EXPECT_FALSE(b);
  const auto& r = a.vtx_pos();
  //fmt::print(">> {}\n", (void*)r.data());
  EXPECT_APPROX_EQ(r[0].x(), 1);
  EXPECT_APPROX_EQ(r[0].y(), 2);
  EXPECT_APPROX_EQ(r[0].z(), 3);
  EXPECT_APPROX_EQ(r[1].x(), 4);
  EXPECT_APPROX_EQ(r[1].y(), 5);
  EXPECT_APPROX_EQ(r[1].z(), 6);
  EXPECT_APPROX_EQ(r[2].x(), 7);
  EXPECT_APPROX_EQ(r[2].y(), 8);
  EXPECT_APPROX_EQ(r[2].z(), 9);
}

TEST(TestPDMotionPreset, FixedSetup1) {
  using namespace doux;

  linalg::matrix_r_t x(3, 3);
  linalg::matrix_i_t e(1, 3);
  x << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  e << 0, 1, 2;
  shape::Mesh<2> msh(std::move(x), std::move(e));
  pd::MotionPreset<2> preset(msh);
  preset.fix_vertex(1);
  auto vid = preset.reorder_vertices().value();
  EXPECT_EQ(vid[0], 1);
  EXPECT_EQ(vid[1], 0);
  EXPECT_EQ(vid[2], 2);

  auto rmsh = pd::internal::ordered_mesh(vid, msh);
  //fmt::print("V## {}\n", (void*)rmsh.vertices().data());
  //fmt::print("E## {}\n", (void*)rmsh.elements().data());
  const auto r = rmsh.vtx_pos();
  EXPECT_APPROX_EQ(r[0].x(), 4);
  EXPECT_APPROX_EQ(r[0].y(), 5);
  EXPECT_APPROX_EQ(r[0].z(), 6);
  EXPECT_APPROX_EQ(r[1].x(), 1);
  EXPECT_APPROX_EQ(r[1].y(), 2);
  EXPECT_APPROX_EQ(r[1].z(), 3);
  EXPECT_APPROX_EQ(r[2].x(), 7);
  EXPECT_APPROX_EQ(r[2].y(), 8);
  EXPECT_APPROX_EQ(r[2].z(), 9);
  //std::cout << rmsh.elements() << std::endl;
  //auto [a, b] = pd::build_softbody(std::move(preset));
  //EXPECT_TRUE(b);
}