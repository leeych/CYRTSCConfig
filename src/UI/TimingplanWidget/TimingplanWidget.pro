#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T15:31:54
#
#-------------------------------------------------

TARGET = TimingplanWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib -lMDatabase -lBottomButtonsWidget

DESTDIR = $$PWD/../../../lib/

DEFINES += TIMINGPLANWIDGET_LIBRARY

SOURCES += timingplanwidget.cpp \
    timingeditdlg.cpp \
    timinghandler.cpp

HEADERS += timingplanwidget.h\
        timingplanwidget_global.h \
    timingeditdlg.h \
    timinghandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
