#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T17:29:13
#
#-------------------------------------------------

QT       -= gui

TARGET = FileReaderWriter
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase

LIBS += -L$$PWD/../../../lib/ -lMDatabase

DESTDIR = $$PWD/../../../lib/

DEFINES += FILEREADERWRITER_LIBRARY

SOURCES += filereaderwriter.cpp

HEADERS += filereaderwriter.h\
        filereaderwriter_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
