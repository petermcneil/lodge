set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g")

set(BINARY_NAME lodge)
file (GLOB APP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories (${LIBRARY_INCLUDE_PATH})
add_executable(${BINARY_NAME} ${APP_SOURCE_FILES})
target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")
