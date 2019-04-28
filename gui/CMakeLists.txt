project(gui)

set(BINARY_NAME Lodge)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_PREFIX_PATH "/usr/local/opt/qt5/")

find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB QML_FILES "${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml")
file(GLOB RES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.*" "${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc")

set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns)

include_directories(${LIBRARY_INCLUDE_PATH})

add_executable(${BINARY_NAME} ${SRC_FILES} ${QML_FILES} ${RES_FILES} ${APP_ICON})

target_compile_definitions(${BINARY_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${BINARY_NAME} PRIVATE lodge_lib)
target_link_libraries(${BINARY_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets)
target_link_libraries(${BINARY_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${BINARY_NAME} PUBLIC "libavformat.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavutil.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${BINARY_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${BINARY_NAME} PUBLIC "swscale.a")

#Bundle properties
set_target_properties(${BINARY_NAME} PROPERTIES MACOSX_BUNDLE YES)
set_target_properties(${BINARY_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/gui/resources/Info.plist)
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
