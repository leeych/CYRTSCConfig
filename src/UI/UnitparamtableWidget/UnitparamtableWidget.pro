#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T10:54:43
#
#-------------------------------------------------

TARGET = UnitparamtableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase

DESTDIR = $$PWD/../../../lib/

LIBS += -L$$PWD/../../../lib/ -lMDatabase

DEFINES += UNITPARAMTABLEWIDGET_LIBRARY

SOURCES += unitparamtablewidget.cpp \
    unitparamhandler.cpp

HEADERS += unitparamtablewidget.h\
        unitparamtablewidget_global.h \
    unitparamhandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
