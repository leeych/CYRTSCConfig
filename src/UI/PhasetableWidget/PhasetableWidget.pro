#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T18:56:57
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = PhasetableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lBottomButtonsWidget

DESTDIR = $$PWD/../../../lib/

DEFINES += PHASETABLEWIDGET_LIBRARY

SOURCES += phasetablewidget.cpp \
    phaseeditdlg.cpp \
    phasehandler.cpp

HEADERS += phasetablewidget.h\
        phasetablewidget_global.h \
    phaseeditdlg.h \
    phasehandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
