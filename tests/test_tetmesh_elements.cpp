#include <gtest/gtest.h>
#include <tetwild/TetmeshElements.h>
#include <CGAL/Simple_cartesian.h>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_3 = Kernel::Point_3;

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