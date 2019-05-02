cmake_minimum_required(VERSION 3.13)
project(lodge)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.68 COMPONENTS filesystem program_options regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
set(FFMPEG_DIR ${CMAKE_SOURCE_DIR}/include/ffmpeg)

enable_testing()

add_subdirectory(include)
add_subdirectory(bridge)

ExternalProject_Add(ffmpeg_proj DEPENDS lodgebridge
#        LOG_CONFIGURE true
#        LOG_DIR ${CMAKE_SOURCE_DIR}/log
        BINARY_DIR ${FFMPEG_DIR}
        SOURCE_DIR ${FFMPEG_DIR}
        CONFIGURE_COMMAND
        # Install step is very unreliable otherwise
        mkdir -p ${FFMPEG_DIR}/include/lodge &&
        mkdir -p ${FFMPEG_DIR}/lib &&
        cp ${BRIDGE_INCLUDE_PATH}/ffmpeg_connector.h ${FFMPEG_DIR}/include/lodge &&
        cp ${CMAKE_CURRENT_BINARY_DIR}/bridge/liblodgebridge.so ${FFMPEG_DIR}/lib &&
        env LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/bridge/liblodgebridge.so <SOURCE_DIR>/configure
        --prefix=<BINARY_DIR>
        --extra-cflags=-I<BINARY_DIR>/include
        --extra-ldflags=-L<BINARY_DIR>/lib
#        --extra-libs=-llodgebridge
        --enable-gpl
        --enable-nonfree
#        --enable-libfdk-aac
        --disable-doc
        --disable-pthreads
        --disable-programs
        --enable-shared
        --enable-static
        --enable-runtime-cpudetect)


add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(gui)