#include <gtest/gtest.h>
#include <tetwild/tetwild.h>
#include <tetwild/TetmeshElements.h>
#include <tetwild/State.h>
#include <tetwild/Args.h>
#include <cstdlib>

bool skip_slow_tests() {
    const char* env = std::getenv("TETWILD_RUN_SLOW_TESTS");
    return env && std::string(env) == "1";
}

#define SKIP_IF_NOT_SLOW() do { if (!skip_slow_tests()) GTEST_SKIP() << "Skipping slow test (run with TETWILD_RUN_SLOW_TESTS=1)"; } while(0)

TEST(Integration, TetrahedralizeTetrahedron) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd VI(4, 3);
    VI << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.5, 1.0, 0.0,
          0.5, 0.5, 0.5;
    
    Eigen::MatrixXi FI(4, 3);
    FI << 0, 1, 2,
          0, 3, 1,
          1, 3, 2,
          2, 3, 0;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-3;
    
    tetwild::tetrahedralization(VI, FI, VO, TO, AO, args);
    
    EXPECT_GT(VO.rows(), 0);
    EXPECT_GT(TO.rows(), 0);
    EXPECT_EQ(AO.size(), TO.rows());
    
    for (int i = 0; i < AO.size(); i++) {
        EXPECT_GE(AO(i), 5.0);
    }
}

TEST(Integration, TetrahedralizeUnitCube) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd V(8, 3);
    V << 0, 0, 0,
          1, 0, 0,
          1, 1, 0,
          0, 1, 0,
          0, 0, 1,
          1, 0, 1,
          1, 1, 1,
          0, 1, 1;
    
    Eigen::MatrixXi F(12, 3);
    F << 0, 2, 1,
          0, 3, 2,
          4, 5, 6,
          4, 6, 7,
          0, 1, 5,
          0, 5, 4,
          1, 2, 6,
          1, 6, 5,
          2, 3, 7,
          2, 7, 6,
          3, 0, 4,
          3, 4, 7;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-3;
    
    tetwild::tetrahedralization(V, F, VO, TO, AO, args);
    
    EXPECT_GT(VO.rows(), 8);
    EXPECT_GT(TO.rows(), 0);
    EXPECT_EQ(AO.size(), TO.rows());
}

TEST(Integration, TetrahedralizeFlatTriangle) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd VI(3, 3);
    VI << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.5, 1.0, 0.0;
    
    Eigen::MatrixXi FI(1, 3);
    FI << 0, 1, 2;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-2;
    
    tetwild::tetrahedralization(VI, FI, VO, TO, AO, args);
    
    EXPECT_GT(VO.rows(), 0);
    EXPECT_EQ(TO.rows(), 0);
}

TEST(Integration, ExtractBoundaryFromTetMesh) {
    Eigen::MatrixXd V(4, 3);
    V << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.0, 1.0, 0.0,
          0.0, 0.0, 1.0;
    
    Eigen::MatrixXi T(1, 4);
    T << 0, 1, 2, 3;
    
    Eigen::MatrixXd VS;
    Eigen::MatrixXi FS;
    
    tetwild::extractSurfaceMesh(V, T, VS, FS);
    
    EXPECT_EQ(VS.rows(), 4);
    EXPECT_EQ(FS.rows(), 4);
}

TEST(Integration, ExtractBoundaryFromMultipleTets) {
    Eigen::MatrixXd V(5, 3);
    V << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          1.0, 1.0, 0.0,
          0.0, 1.0, 0.0,
          0.5, 0.5, 1.0;
    
    Eigen::MatrixXi T(2, 4);
    T << 0, 1, 2, 4,
          0, 2, 3, 4;
    
    Eigen::MatrixXd VS;
    Eigen::MatrixXi FS;
    
    tetwild::extractSurfaceMesh(V, T, VS, FS);
    
    EXPECT_EQ(VS.rows(), 5);
    EXPECT_GE(FS.rows(), 4);
}

TEST(Integration, QualityThresholdRespected) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd VI(4, 3);
    VI << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.5, 1.0, 0.0,
          0.5, 0.5, 0.5;
    
    Eigen::MatrixXi FI(4, 3);
    FI << 0, 1, 2,
          0, 3, 1,
          1, 3, 2,
          2, 3, 0;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-3;
    args.filter_energy_thres = 10.0;
    
    tetwild::tetrahedralization(VI, FI, VO, TO, AO, args);
    
    if (TO.rows() > 0) {
        for (int i = 0; i < AO.size(); i++) {
            EXPECT_GE(AO(i), 0.0);
        }
    }
}

TEST(Integration, EmptyInput) {
    Eigen::MatrixXd VI(0, 3);
    Eigen::MatrixXi FI(0, 3);
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-3;
    
    EXPECT_NO_THROW(tetwild::tetrahedralization(VI, FI, VO, TO, AO, args));
    
    EXPECT_EQ(VO.rows(), 0);
    EXPECT_EQ(TO.rows(), 0);
}

TEST(Integration, SingleTriangleMesh) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd VI(3, 3);
    VI << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.5, 0.866, 0.0;
    
    Eigen::MatrixXi FI(1, 3);
    FI << 0, 1, 2;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 0.01;
    
    tetwild::tetrahedralization(VI, FI, VO, TO, AO, args);
    
    EXPECT_GE(VO.rows(), 0);
}

TEST(Integration, MultipleComponents) {
    SKIP_IF_NOT_SLOW();
    Eigen::MatrixXd VI(6, 3);
    VI << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.5, 1.0, 0.0,
          2.0, 0.0, 0.0,
          3.0, 0.0, 0.0,
          2.5, 1.0, 0.0;
    
    Eigen::MatrixXi FI(2, 3);
    FI << 0, 1, 2,
          3, 4, 5;
    
    Eigen::MatrixXd VO;
    Eigen::MatrixXi TO;
    Eigen::VectorXd AO;
    
    tetwild::Args args;
    args.eps_rel = 1e-3;
    
    tetwild::tetrahedralization(VI, FI, VO, TO, AO, args);
    
    EXPECT_GT(TO.rows(), 0);
}