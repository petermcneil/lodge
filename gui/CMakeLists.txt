project(gui)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(Qt5_DIR "/usr/local/opt/qt/lib/cmake/Qt5")

find_package(Qt5 COMPONENTS Core Quick Widgets REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc")
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE lodge_lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC "libavformat.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavutil.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "swscale.a")
