project(lodge)

add_library(lodge_lib STATIC fact.cpp)
target_include_directories(lodge_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${BINARY_NAME} Main.cpp)

target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")

target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib)
