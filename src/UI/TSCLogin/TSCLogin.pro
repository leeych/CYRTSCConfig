#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T14:56:08
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = TSCLogin
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/

LIBS += -L$$PWD/../../../lib/ -lUtility

DESTDIR = $$PWD/../../../lib/

DEFINES += TSCLOGIN_LIBRARY

SOURCES += login.cpp

HEADERS += login.h\
        tsclogin_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
