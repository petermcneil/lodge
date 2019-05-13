QMAKE_CXXFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -mmacosx-version-min=10.12
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
INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/
LIBS += "$$PWD/build/lib/liblodge_lib.a"
LIBS += "/usr/local/lib/libboost_filesystem.a" "/usr/local/lib/libboost_regex.a"
LIBS += -lavcodec -lavutil -lavformat -lswscale -lavfilter
LIBS += -dead_strip