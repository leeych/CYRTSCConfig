#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T16:31:07
#
#-------------------------------------------------

TARGET = UIBase
TEMPLATE = lib

LIBS = -L$$PWD/../../../lib/ -lUtility

INCLUDEPATH += $$PWD/../../../include/

DESTDIR = $$PWD/../../../lib/

DEFINES += UIBASE_LIBRARY

SOURCES += mtabwidget.cpp

HEADERS += uibase_global.h \
    mtabwidget.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
