cmake_minimum_required(VERSION 3.13)
set(BINARY_NAME lodge)
project(Lodge)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.68 COMPONENTS filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)
