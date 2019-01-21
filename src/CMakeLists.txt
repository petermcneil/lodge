project(Lodge_prog)

add_library(lodge_lib STATIC fact.cpp)
target_include_directories(lodge_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lodge Main.cpp)
target_link_libraries(lodge PRIVATE lodge_lib)