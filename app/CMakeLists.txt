set(BINARY_NAME ldge)
file (GLOB APP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories (${LIBRARY_INCLUDE_PATH})
add_executable(${BINARY_NAME} ${APP_SOURCE_FILES})

target_include_directories(${BINARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(${BINARY_NAME} PRIVATE lodge)
target_link_libraries(${BINARY_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
