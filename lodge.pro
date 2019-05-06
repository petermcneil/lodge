QMAKE_CXXFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
CONFIG += app_bundle
ICON = gui/resources/icon.icns

TARGET = Lodge
CONFIG += c++17
CONFIG += qt debug

QT += core widgets quick

HEADERS = gui/src/backend.h
SOURCES = gui/src/backend.cpp gui/src/main.cpp
RESOURCES += gui/qml.qrc

INCLUDEPATH += lib/include
INCLUDEPATH += /usr/local/Cellar/ffmpeg/4.1.3/include
INCLUDEPATH += /usr/local/include/
LIBS += "$$PWD/build/lib/liblodge_lib.a"
LIBS += "/usr/local/Cellar/boost/1.69.0_2/lib/libboost_filesystem.a" "/usr/local/Cellar/boost/1.69.0_2/lib/libboost_regex.a"
LIBS += "-L/usr/local/Cellar/ffmpeg/4.1.3/lib" -lavcodec -lavutil -lavformat -lswscale -lavfilter
LIBS += -dead_strip