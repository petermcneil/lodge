project(tests)

set(BINARY_NAME lodge_tests)

include_directories(${LIBRARY_INCLUDE_PATH})

file (GLOB TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
find_package(Catch2 CONFIG REQUIRED)

add_executable(${BINARY_NAME} ${TEST_SOURCE_FILES})

# Define TEST_RESOURCES_DIR to point to source directory resources
target_compile_definitions(${BINARY_NAME} PRIVATE
    TEST_RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources"
)

target_link_libraries(${BINARY_NAME} PRIVATE lodge Catch2::Catch2)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
find_package(FFMPEG REQUIRED)

include_directories(${FFMPEG_INCLUDE_DIRS})
link_directories(${FFMPEG_LIBRARY_DIRS})

target_link_libraries(${BINARY_NAME} PRIVATE spdlog::spdlog ${FFMPEG_LIBRARIES})
target_include_directories(${BINARY_NAME} PRIVATE spdlog::spdlog )

enable_testing()
add_test(NAME ${BINARY_NAME} COMMAND ${BINARY_NAME})
