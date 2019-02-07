cmake_minimum_required(VERSION 3.13)
set(BINARY_NAME lodge)
project(Lodge)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

set(CMAKE_CXX_CPPCHECK "cppcheck")
if (CMAKE_CXX_CPPCHECK)
    list(
            APPEND CMAKE_CXX_CPPCHECK
            "--enable=all"
            "--inconclusive"
            "--force"
            "--inline-suppr"
    )
endif()

FIND_PACKAGE(Boost 1.68 COMPONENTS filesystem program_options regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)
