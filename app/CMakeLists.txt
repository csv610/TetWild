cmake_minimum_required(VERSION 3.5)

add_executable(tetwild app/main.cpp)
target_link_libraries(tetwild
    libTetWild
    CLI11::CLI11
    CGAL::CGAL
    warnings::all
)
target_include_directories(tetwild PRIVATE src)

install(TARGETS tetwild RUNTIME DESTINATION bin)