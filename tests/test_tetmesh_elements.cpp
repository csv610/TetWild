#include <gtest/gtest.h>
#include <tetwild/TetmeshElements.h>
#include <tetwild/State.h>
#include <tetwild/Common.h>
#include <tetwild/Args.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <Eigen/Dense>
#include <geogram/basic/geometry.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_3 = Kernel::Point_3;

namespace tetwild {

TEST(TetVertex, DefaultConstructor) {
    tetwild::TetVertex v;
    EXPECT_EQ(v.on_fixed_vertex, -1);
    EXPECT_FALSE(v.is_on_surface);
    EXPECT_FALSE(v.is_rounded);
    EXPECT_FALSE(v.is_on_bbox);
    EXPECT_FALSE(v.is_on_boundary);
    EXPECT_FALSE(v.is_locked);
    EXPECT_FALSE(v.is_inside);
    EXPECT_EQ(v.adaptive_scale, 1.0);
}

TEST(TetVertex, PointConstructor) {
    Point_3 p(1.0, 2.0, 3.0);
    tetwild::TetVertex v(p);
    EXPECT_NEAR(CGAL::to_double(v.pos[0]), 1.0, 1e-6);
    EXPECT_NEAR(CGAL::to_double(v.pos[1]), 2.0, 1e-6);
    EXPECT_NEAR(CGAL::to_double(v.pos[2]), 3.0, 1e-6);
}

TEST(TetVertex, RoundFunction) {
    Point_3 p(1.5, 2.5, 3.5);
    tetwild::TetVertex v(p);
    v.round();
    EXPECT_TRUE(v.is_rounded);
    EXPECT_FLOAT_EQ(v.posf[0], 1.5f);
    EXPECT_FLOAT_EQ(v.posf[1], 2.5f);
    EXPECT_FLOAT_EQ(v.posf[2], 3.5f);
}

TEST(TetVertex, Flags) {
    tetwild::TetVertex v;
    v.is_on_surface = true;
    v.is_on_bbox = true;
    v.is_on_boundary = true;
    v.is_locked = true;
    v.is_inside = true;
    EXPECT_TRUE(v.is_on_surface);
    EXPECT_TRUE(v.is_on_bbox);
    EXPECT_TRUE(v.is_on_boundary);
    EXPECT_TRUE(v.is_locked);
    EXPECT_TRUE(v.is_inside);
}

TEST(TetVertex, AdaptiveScale) {
    tetwild::TetVertex v;
    v.adaptive_scale = 2.5;
    EXPECT_DOUBLE_EQ(v.adaptive_scale, 2.5);
}

TEST(TetQuality, DefaultConstructor) {
    tetwild::TetQuality q;
    EXPECT_EQ(q.min_d_angle, 0.0);
    EXPECT_EQ(q.max_d_angle, 0.0);
    EXPECT_EQ(q.slim_energy, 0.0);
    EXPECT_EQ(q.volume, 0.0);
}

TEST(TetQuality, ParameterizedConstructor) {
    tetwild::TetQuality q(10.0, 20.0, 1.0);
    EXPECT_EQ(q.min_d_angle, 10.0);
    EXPECT_EQ(q.max_d_angle, 20.0);
}

TEST(TetQuality, Setters) {
    tetwild::TetQuality q;
    q.min_d_angle = 5.0;
    q.max_d_angle = 15.0;
    q.slim_energy = 2.5;
    q.volume = 100.0;
    
    EXPECT_EQ(q.min_d_angle, 5.0);
    EXPECT_EQ(q.max_d_angle, 15.0);
    EXPECT_EQ(q.slim_energy, 2.5);
    EXPECT_EQ(q.volume, 100.0);
}

TEST(TetQuality, IsBetterThan_AMIPS) {
    tetwild::TetQuality q1, q2;
    q1.slim_energy = 1.0;
    q2.slim_energy = 2.0;
    
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_TRUE(q1.isBetterThan(q2, state.ENERGY_AMIPS, state));
    EXPECT_FALSE(q2.isBetterThan(q1, state.ENERGY_AMIPS, state));
}

TEST(TetQuality, IsBetterThan_AD) {
    tetwild::TetQuality q1, q2;
    q1.min_d_angle = 30.0;
    q1.max_d_angle = 80.0;
    q2.min_d_angle = 20.0;
    q2.max_d_angle = 90.0;
    
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_TRUE(q1.isBetterThan(q2, state.ENERGY_AD, state));
    EXPECT_FALSE(q2.isBetterThan(q1, state.ENERGY_AD, state));
}

TEST(TetQuality, IsBetterOrEqualThan_AMIPS) {
    tetwild::TetQuality q1, q2;
    q1.slim_energy = 1.0;
    q2.slim_energy = 1.0;
    
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_TRUE(q1.isBetterOrEqualThan(q2, state.ENERGY_AMIPS, state));
}

TEST(State, Constants) {
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_EQ(state.EPSILON_INFINITE, -2);
    EXPECT_EQ(state.EPSILON_NA, -1);
    EXPECT_EQ(state.ENERGY_NA, 0);
    EXPECT_EQ(state.ENERGY_AD, 1);
    EXPECT_EQ(state.ENERGY_AMIPS, 2);
    EXPECT_EQ(state.ENERGY_DIRICHLET, 3);
}

TEST(State, DefaultValues) {
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_GE(state.bbox_diag, 0.0);
    EXPECT_GE(state.eps, 0.0);
    EXPECT_GE(state.initial_edge_len, 0.0);
}

TEST(State, Options) {
    tetwild::Args args;
    Eigen::MatrixXd V(1, 3);
    V << 0, 0, 0;
    tetwild::State state(args, V);
    
    EXPECT_TRUE(state.use_energy_max);
    EXPECT_TRUE(state.use_sampling);
    EXPECT_FALSE(state.use_onering_projection);
}

TEST(MeshRecord, Constructor1) {
    tetwild::MeshRecord rec(1, 1.5, 100, 200, 10.0, 15.0, 80.0, 85.0, 1.0, 2.0);
    
    EXPECT_EQ(rec.op, 1);
    EXPECT_DOUBLE_EQ(rec.timing, 1.5);
    EXPECT_EQ(rec.n_v, 100);
    EXPECT_EQ(rec.n_t, 200);
    EXPECT_DOUBLE_EQ(rec.min_min_d_angle, 10.0);
    EXPECT_DOUBLE_EQ(rec.avg_min_d_angle, 15.0);
    EXPECT_DOUBLE_EQ(rec.max_max_d_angle, 80.0);
    EXPECT_DOUBLE_EQ(rec.avg_max_d_angle, 85.0);
    EXPECT_DOUBLE_EQ(rec.max_energy, 1.0);
    EXPECT_DOUBLE_EQ(rec.avg_energy, 2.0);
}

TEST(MeshRecord, Constructor2) {
    tetwild::MeshRecord rec(tetwild::MeshRecord::OP_INIT, 0.5, 50, 75);
    
    EXPECT_EQ(rec.op, tetwild::MeshRecord::OP_INIT);
    EXPECT_DOUBLE_EQ(rec.timing, 0.5);
    EXPECT_EQ(rec.n_v, 50);
    EXPECT_EQ(rec.n_t, 75);
}

TEST(Common, IsHaveCommonEle) {
    std::unordered_set<int> s1 = {1, 2, 3};
    std::unordered_set<int> s2 = {3, 4, 5};
    std::unordered_set<int> s3 = {7, 8, 9};
    
    EXPECT_TRUE(tetwild::isHaveCommonEle(s1, s2));
    EXPECT_FALSE(tetwild::isHaveCommonEle(s1, s3));
}

TEST(Common, SampleTriangle) {
    std::array<GEO::vec3, 3> tri;
    tri[0] = GEO::vec3(0.0, 0.0, 0.0);
    tri[1] = GEO::vec3(1.0, 0.0, 0.0);
    tri[2] = GEO::vec3(0.0, 1.0, 0.0);
    
    std::vector<GEO::vec3> ps;
    tetwild::sampleTriangle(tri, ps, 0.5);
    
    EXPECT_GE(ps.size(), 3u);
}

TEST(Common, SampleTriangle_SmallDist) {
    std::array<GEO::vec3, 3> tri;
    tri[0] = GEO::vec3(0.0, 0.0, 0.0);
    tri[1] = GEO::vec3(1.0, 0.0, 0.0);
    tri[2] = GEO::vec3(0.0, 1.0, 0.0);
    
    std::vector<GEO::vec3> ps;
    tetwild::sampleTriangle(tri, ps, 0.1);
    
    EXPECT_GT(ps.size(), 3u);
}

TEST(Stage, DefaultConstructor) {
    tetwild::Stage stage;
    EXPECT_TRUE(stage.tet_vertices.empty());
    EXPECT_TRUE(stage.tets.empty());
    EXPECT_TRUE(stage.is_surface_fs.empty());
    EXPECT_FALSE(stage.resolution > 0);
}

TEST(Stage, Constructor) {
    std::vector<tetwild::TetVertex> vs = {tetwild::TetVertex(), tetwild::TetVertex()};
    std::vector<std::array<int, 4>> ts = {{{0, 1, 2, 3}}};
    std::vector<std::array<int, 4>> isf = {{{0, 0, 0, 0}}};
    std::vector<bool> vir = {false, false};
    std::vector<bool> tir = {false};
    std::vector<tetwild::TetQuality> tqs = {tetwild::TetQuality()};
    
    tetwild::Stage stage(vs, ts, isf, vir, tir, tqs);
    
    EXPECT_EQ(stage.tet_vertices.size(), 2u);
    EXPECT_EQ(stage.tets.size(), 1u);
    EXPECT_EQ(stage.is_surface_fs.size(), 1u);
}

}  // namespace tetwild