cmake_minimum_required(VERSION 3.16)
project(lodge)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wnon-virtual-dtor -g -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find packages installed by Conan
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem program_options regex)
find_package(spdlog REQUIRED)

enable_testing()

add_subdirectory(lib)
add_subdirectory(app)
