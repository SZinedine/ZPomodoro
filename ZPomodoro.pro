#-------------------------------------------------
#
# Project created by QtCreator 2018-11-07T15:01:08
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = ZPomodoro
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/zpomodoro.cpp \
    src/timelabel.cpp \
    src/ztime.cpp \
    src/item.cpp \
    src/ztreewidget.cpp

HEADERS += \
    src/zpomodoro.h \
    src/common.h \
    src/timelabel.h \
    src/ztime.h \
    src/item.h \
    src/ztreewidget.h

RESOURCES += zpomodoro.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
