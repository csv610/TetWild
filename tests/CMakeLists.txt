cmake_minimum_required(VERSION 3.5)

project(TetWildTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt OFF)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Find CGAL for include paths
find_package(CGAL REQUIRED)

# Test executable
add_executable(test_tetwild 
    test_tetmesh_elements.cpp
)

# Add include directories - from main project and CGAL
target_include_directories(test_tetwild PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CGAL_INCLUDE_DIRS}
)

# Link to libTetWild which has all the proper dependencies
target_link_libraries(test_tetwild
    PRIVATE
    libTetWild
    GTest::gtest
    GTest::gtest_main
)

# Register tests
add_test(NAME TetWildTests COMMAND test_tetwild)