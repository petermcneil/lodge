cmake_minimum_required(VERSION 3.13)
project(lodge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g -fvisibility=hidden -fvisibility-inlines-hidden -mmacosx-version-min=10.12")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.69 COMPONENTS filesystem program_options regex REQUIRED)

enable_testing()

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(app)