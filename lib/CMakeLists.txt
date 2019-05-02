project(lodge_lib)

set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g")

set (LIBRARY_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib")
set (LIBRARY_SRC_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set (LIBRARY_INCLUDE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/include")
set (LIBRARY_INCLUDE_PATH ${LIBRARY_INCLUDE_PATH} PARENT_SCOPE)

include_directories (${LIBRARY_INCLUDE_PATH})

file (GLOB LIB_HEADER_FILES "${LIBRARY_INCLUDE_PATH}/*.h")
file (GLOB LIB_SOURCE_FILES "${LIBRARY_SRC_PATH}/*.cpp")

add_library (${PROJECT_NAME} SHARED ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME})

enable_testing()
add_subdirectory (test)