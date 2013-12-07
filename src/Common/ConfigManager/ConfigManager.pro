#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T08:42:12
#
#-------------------------------------------------

QT       -= gui

TARGET = ConfigManager
TEMPLATE = lib

DESTDIR = $$PWD/../../../lib/

DEFINES += CONFIGMANAGER_LIBRARY

SOURCES += configmanager.cpp \
    appsettinginfo.cpp

HEADERS += configmanager.h\
        configmanager_global.h \
    appsettinginfo.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
