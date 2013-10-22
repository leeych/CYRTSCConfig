#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T15:37:10
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = FileListWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
#               $$PWD/../../Common/DataInitializer
#               $$PWD/../../Common/FileReaderWriter

LIBS += -L$$PWD/../../../lib/ -lMDatabase

DESTDIR = $$PWD/../../../lib/

DEFINES += FILELISTWIDGET_LIBRARY

SOURCES += filelistwidget.cpp \
    filereaderwriter.cpp \
    datainitializer.cpp

HEADERS += filelistwidget.h\
        filelistwidget_global.h \
    filereaderwriter.h \
    datainitializer.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
