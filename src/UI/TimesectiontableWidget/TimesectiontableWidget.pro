#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T09:41:27
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = TimesectiontableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lBottomButtonsWidget

DESTDIR = $$PWD/../../../lib/

DEFINES += TIMESECTIONTABLEWIDGET_LIBRARY

SOURCES += timesectiontablewidget.cpp \
    timesectioneditdlg.cpp \
    timesectionhandler.cpp

HEADERS += timesectiontablewidget.h\
        timesectiontablewidget_global.h \
    timesectioneditdlg.h \
    timesectionhandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
