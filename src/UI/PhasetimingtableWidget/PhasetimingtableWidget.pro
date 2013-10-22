#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T10:33:13
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = PhasetimingtableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lBottomButtonsWidget -lMDatabase

DESTDIR = $$PWD/../../../lib/

DEFINES += PHASETIMINGTABLEWIDGET_LIBRARY

SOURCES += phasetimingtablewidget.cpp \
    phasetimingeditdlg.cpp \
    phasetiminghandler.cpp

HEADERS += phasetimingtablewidget.h\
        phasetimingtablewidget_global.h \
    phasetimingeditdlg.h \
    phasetiminghandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
