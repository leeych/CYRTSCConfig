#-------------------------------------------------
#
# Project created by QtCreator 2013-08-23T18:30:20
#
#-------------------------------------------------

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
