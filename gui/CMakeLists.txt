# Lodge GUI - Qt 6 QML Application
# Note: This does not statically link Qt by default (system Qt 6 is used)
# Static linking can be added as a future enhancement

cmake_minimum_required(VERSION 3.21)
project(lodge_gui VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Enable Qt automatic handling
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)

# Use Conan-installed dependencies from parent build
# CMAKE_CURRENT_SOURCE_DIR is gui/, so parent is one level up
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/build/Release/generators;${CMAKE_PREFIX_PATH}")

# Find other dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
find_package(ffmpeg CONFIG REQUIRED)

# Create executable with Qt 6
qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Backend.cpp
    src/Backend.h
)

# Add QML resources using traditional qrc files for compatibility
qt_add_resources(${PROJECT_NAME} "qml_files"
    PREFIX "/"
    FILES
        qml/main.qml
        qml/encodeView.qml
        qml/decodeView.qml
        qml/BackButton.qml
        qml/FileLoader.qml
        qml/subtitleView.qml
        qml/videoTranscription.qml
        qml/subtitleDecision.qml
        qml/StackScene.qml
)

# Add image/asset resources
qt_add_resources(${PROJECT_NAME} "assets"
    PREFIX "/"
    FILES
        resources/logo_transparent.png
        resources/icon.icns
        resources/Info.plist
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Qt 6 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

# Link other dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog::spdlog
    boost::boost
    ffmpeg::ffmpeg
)

# Link to lodge library
# Try to find lodge library in parent build directory
set(LODGE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/build/Release/lib/liblodge.a")
if(EXISTS "${LODGE_LIB_PATH}")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LODGE_LIB_PATH})
    message(STATUS "Using lodge library: ${LODGE_LIB_PATH}")
else()
    message(WARNING "Lodge library not found at ${LODGE_LIB_PATH}")
    message(WARNING "Build the core library first: cd .. && make build")
endif()

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# macOS bundle configuration
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist"
        MACOSX_BUNDLE_ICON_FILE "icon.icns"
    )
endif()

message(STATUS "")
message(STATUS "Lodge GUI Configuration:")
message(STATUS "  Qt Version: ${Qt6_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
