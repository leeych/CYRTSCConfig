#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T15:26:35
#
#-------------------------------------------------

QT       += widgets

TARGET = ConfigoptWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include \

DEFINES += CONFIGOPTWIDGET_LIBRARY

SOURCES += configoptwidget.cpp

HEADERS += configoptwidget.h\
        configoptwidget_global.h

#LIBS += -L $$PWD/../../../lib

DESTDIR = $$PWD/../../../lib

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
