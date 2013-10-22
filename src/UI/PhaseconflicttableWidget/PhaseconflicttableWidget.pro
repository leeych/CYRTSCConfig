#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T19:58:23
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = PhaseconflicttableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lBottomButtonsWidget -lMDatabase

DESTDIR = $$PWD/../../../lib/

DEFINES += PHASECONFLICTTABLEWIDGET_LIBRARY

SOURCES += phaseconflicttablewidget.cpp \
    phaseconflicteditdlg.cpp \
    phaseconflicthandler.cpp

HEADERS += phaseconflicttablewidget.h\
        phaseconflicttablewidget_global.h \
    phaseconflicteditdlg.h \
    phaseconflicthandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
