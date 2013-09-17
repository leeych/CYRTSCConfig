#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T18:56:28
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = CommandCallback
TEMPLATE = lib

INCLUDEPATH += $$PWD/../Commnication/

LIBS += -L$$PWD/../../../lib/ -lCommunication

DEFINES += COMMANDCALLBACK_LIBRARY

DESTDIR = $$PWD/../../../lib/

SOURCES += commandcallback.cpp

HEADERS += commandcallback.h\
        commandcallback_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
