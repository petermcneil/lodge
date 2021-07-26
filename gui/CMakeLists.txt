# DOES NOT STATICALLY LINK THE BINARY
cmake_minimum_required(VERSION 3.16)
project(lodge_gui)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)
find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Widgets Quick)
include_directories (${LIBRARY_INCLUDE_PATH})
include("$ENV{LODGE}/build/lib/lodge-exports.cmake")

find_package(spdlog CONFIG REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.76.0 COMPONENTS filesystem program_options regex REQUIRED)

qt5_add_resources(${PROJECT_NAME}_SRCS qml.qrc)
qt5_add_resources(${PROJECT_NAME}_SRCS resources.qrc)

include_directories(${Qt5Core_PRIVATE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    ${PROJECT_NAME}_SRCS
    src/main.cpp
    src/backend.cpp
    src/backend.h
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Quick spdlog::spdlog_header_only lodge)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} ${INSTALL_TARGETS_DEFAULT_ARGS})

# install(PROGRAMS org.flatpak.qtdemo.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
# install(FILES org.flatpak.qtdemo.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
# install(FILES flatpak-print-cheatsheet.pdf DESTINATION ${DATA_INSTALL_DIR}/org.flatpak.qtdemo/)