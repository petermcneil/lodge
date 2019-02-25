project(tests)

set(BINARY_NAME lodge_tests)

add_executable(${BINARY_NAME} TestMain.cpp TestEncoder.cpp TestVideoFile.cpp TestSubtitleFile.cpp)
target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib catch)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")

