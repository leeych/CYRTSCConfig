#-------------------------------------------------
#
# Project created by QtCreator 2013-09-13T11:34:13
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Communication
TEMPLATE = lib

DESTDIR = $$PWD/../../../lib/

DEFINES += COMMUNICATION_LIBRARY

SOURCES += communication.cpp \
    command.cpp \
    cmdthread.cpp

HEADERS += communication.h\
        communication_global.h \
    command.h \
    cmdthread.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
