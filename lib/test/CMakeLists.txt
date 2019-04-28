project(tests)

set(BINARY_NAME lodge_tests)

include_directories(${LIBRARY_INCLUDE_PATH})

file (GLOB TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${BINARY_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib catch)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")

enable_testing()
