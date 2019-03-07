project(lodge)

set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g")

set(CMAKE_CXX_CPPCHECK "cppcheck")
if (CMAKE_CXX_CPPCHECK)
    list(
            APPEND CMAKE_CXX_CPPCHECK
            "--enable=all"
            "--inconclusive"
            "--force"
    )
endif()

add_library(lodge_lib STATIC VideoFile.cpp VideoFile.h Encoder.h SubtitleFile.cpp SubtitleFile.h)
target_include_directories(lodge_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${BINARY_NAME} Main.cpp)

target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")

target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib)
