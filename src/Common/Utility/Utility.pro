#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T07:58:17
#
#-------------------------------------------------

TARGET = Utility
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/

DESTDIR = $$PWD/../../../lib/

DEFINES += UTILITY_LIBRARY

SOURCES += mutility.cpp

HEADERS += utility_global.h \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
