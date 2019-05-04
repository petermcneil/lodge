project(lodge_lib)

set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -g")

set (LIBRARY_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib")

include_directories (${LIBRARY_INCLUDE_PATH} ${BRIDGE_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file (GLOB LIB_HEADER_FILES "${LIBRARY_INCLUDE_PATH}/*.h")
file (GLOB LIB_SOURCE_FILES "${LIBRARY_SRC_PATH}/*.cpp")


add_library (${PROJECT_NAME} STATIC ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES} ${BRIDGE_HEADER})

enable_testing()
add_subdirectory (test)