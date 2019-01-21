project(tests)

add_executable(tests TestMain.cpp)
target_link_libraries(tests PRIVATE lodge_lib catch)

