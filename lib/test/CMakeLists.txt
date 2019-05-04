project(tests)

set(BINARY_NAME lodge_tests)

include_directories(${LIBRARY_INCLUDE_PATH})

file (GLOB TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file (GLOB BRIDGE_HEADER ${BRIDGE_INCLUDE_PATH}/*.h)

add_executable(${BINARY_NAME} ${TEST_SOURCE_FILES} ${BRIDGE_HEADER})
target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib catch)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC libavcodec)
target_link_libraries(${BINARY_NAME} PUBLIC libavformat)
target_link_libraries(${BINARY_NAME} PUBLIC libavfilter)
target_link_libraries(${BINARY_NAME} PUBLIC libavutil)
target_link_libraries(${BINARY_NAME} PUBLIC libswscale)
target_link_libraries(${BINARY_NAME} PUBLIC lib${BRIDGE_NAME})

enable_testing()
