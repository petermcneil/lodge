cmake_minimum_required(VERSION 3.13)
project(lodge)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.68 COMPONENTS filesystem program_options regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

enable_testing()

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(gui)