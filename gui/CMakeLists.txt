project(lodge_gui)

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

set(SRC_FILES main.cpp backend.h backend.cpp)
set(QML_FILES
        qml/BackButton.qml
        qml/decodeView.qml
        qml/encodeView.qml
        qml/FileLoader.qml
        main.qml
        qml/subtitleView.qml
        qml/videoTranscription.qml
        qml/subtitleDecision.qml
        qml/StackScene.qml)
set(RES_FILES qml.qrc resources/icon.icns resources/logo_transparent.png resources/Info.plist)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${QML_FILES} ${RES_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE lodge_lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC "libavformat.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavutil.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavcodec.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "libavfilter.a")
target_link_libraries(${PROJECT_NAME} PUBLIC "swscale.a")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE YES)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST /Users/mcneip01/uni/CI301/lodge/gui/resources/Info.plist)
