cmake_minimum_required(VERSION 3.5)

project(TetWildTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find CGAL for include paths
find_package(CGAL REQUIRED)

# Use system-installed GTest
find_package(GTest QUIET)

# Unit tests
add_executable(test_tetwild 
    test_tetmesh_elements.cpp
)

target_include_directories(test_tetwild PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CGAL_INCLUDE_DIRS}
)

target_link_libraries(test_tetwild
    PRIVATE
    libTetWild
)

if(GTest_FOUND)
    target_link_libraries(test_tetwild PRIVATE GTest::gtest GTest::gtest_main)
endif()

add_test(NAME TetWildTests COMMAND test_tetwild)

# Integration tests
add_executable(test_integration
    test_integration.cpp
)

target_include_directories(test_integration PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CGAL_INCLUDE_DIRS}
)

target_link_libraries(test_integration
    PRIVATE
    libTetWild
)

if(GTest_FOUND)
    target_link_libraries(test_integration PRIVATE GTest::gtest GTest::gtest_main)
endif()

add_test(NAME TetWildIntegrationTests COMMAND test_integration)