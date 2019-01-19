cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME lodge)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*${SRC_EXT_PATTERN})
source_group("Source Files" FILES ${SRC_FILES})

# build target
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} "libavformat.a")
target_link_libraries(${PROJECT_NAME} "libavutil.a")
target_link_libraries(${PROJECT_NAME} "libavcodec.a")
target_link_libraries(${PROJECT_NAME} "libavfilter.a")
target_link_libraries(${PROJECT_NAME} "swscale.a")