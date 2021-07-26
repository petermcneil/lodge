cmake_minimum_required(VERSION 3.13)
project(lodge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wnon-virtual-dtor -g -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.76.0 COMPONENTS filesystem program_options regex REQUIRED)

find_package(spdlog CONFIG REQUIRED)

enable_testing()

add_subdirectory(lib)
add_subdirectory(app)
