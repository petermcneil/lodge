
project(Lodge_prog)

# All sources that also need to be tested in unit tests go into a static library
add_library(lodge_lib STATIC fact.cpp)
target_include_directories(lodge_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# The main program
add_executable(lodge Main.cpp)
target_link_libraries(lodge PRIVATE lodge_lib)