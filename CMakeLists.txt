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

set(BRIDGE_LIB_LOC ${CMAKE_CURRENT_BINARY_DIR}/bridge/lib${BRIDGE_NAME}.dylib)

ExternalProject_Add(ffmpeg_proj DEPENDS ${BRIDGE_NAME}
        BINARY_DIR ${FFMPEG_DIR}
        SOURCE_DIR ${FFMPEG_DIR}
        CONFIGURE_COMMAND
        # Install step is very unreliable otherwise
        mkdir -p ${FFMPEG_DIR}/include/lodge &&
        mkdir -p ${FFMPEG_DIR}/lib &&
        cp ${BRIDGE_INCLUDE_PATH}/ffmpeg_connector.h ${FFMPEG_DIR}/include/lodge &&
        cp ${BRIDGE_LIB_LOC} /usr/local/lib &&
        cp ${BRIDGE_LIB_LOC} ${FFMPEG_DIR}/lib &&
        <SOURCE_DIR>/configure
        --prefix=<BINARY_DIR>
        --extra-cflags=-I<BINARY_DIR>/include
        --extra-ldflags=-L<BINARY_DIR>/lib\ -l${BRIDGE_NAME}
        --enable-gpl
        --enable-nonfree
        --enable-libfdk-aac
        --disable-doc
        --disable-pthreads
        --disable-programs
        --enable-shared
        --enable-static
        --enable-runtime-cpudetect
        BUILD_COMMAND make -j3
        INSTALL_COMMAND make install)

add_library(libavcodec  SHARED IMPORTED GLOBAL)
add_library(libavformat SHARED IMPORTED GLOBAL)
add_library(libavfilter SHARED IMPORTED GLOBAL)
add_library(libavutil   SHARED IMPORTED GLOBAL)
add_library(libswscale  SHARED IMPORTED GLOBAL)
add_library(lib${BRIDGE_NAME} SHARED IMPORTED GLOBAL)

ExternalProject_Get_Property(ffmpeg_proj BINARY_DIR)
set_target_properties(libavcodec        PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavcodec.dylib)
set_target_properties(libavformat       PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavformat.dylib)
set_target_properties(libavfilter       PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavfilter.dylib)
set_target_properties(libavutil         PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavutil.dylib)
set_target_properties(libswscale        PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libswscale.dylib)
set_target_properties(lib${BRIDGE_NAME} PROPERTIES IMPORTED_LOCATION ${BRIDGE_LIB_LOC})

add_dependencies(libavcodec ffmpeg_proj)
add_dependencies(libavformat ffmpeg_proj)
add_dependencies(libavfilter ffmpeg_proj)
add_dependencies(libavutil ffmpeg_proj)

ExternalProject_Get_Property(ffmpeg_proj SOURCE_DIR)

find_library(AVFOUNDATION AVFoundation)
find_library(COCOA Cocoa)
find_library(COREFOUNDATION CoreFoundation)
find_library(COREMEDIA CoreMedia)
find_library(COREVIDEO CoreVideo)
find_library(COREMEDIAIO CoreMediaIO)
find_library(VIDEOTOOLBOX VideoToolbox)

include_directories(${AVFOUNDATION}
        ${COCOA}
        ${COREFOUNDATION}
        ${COREMEDIA}
        ${COREVIDEO}
        ${COREMEDIAIO}
        ${VIDEOTOOLBOX}
        ${COCOA})

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(gui)