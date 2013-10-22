#-------------------------------------------------
#
# Project created by QtCreator 2013-08-23T18:30:20
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = BottomButtonsWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/

DESTDIR = $$PWD/../../../lib/

DEFINES += BOTTOMBUTTONSWIDGET_LIBRARY

SOURCES += bottombuttonswidget.cpp

HEADERS += bottombuttonswidget.h\
        bottombuttonswidget_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
