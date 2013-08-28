#-------------------------------------------------
#
# Project created by QtCreator 2013-08-07T20:34:45
#
#-------------------------------------------------

TARGET = ChanneltableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lBottomButtonsWidget -lMDatabase

DESTDIR = $$PWD/../../../lib/

DEFINES += CHANNELTABLEWIDGET_LIBRARY

SOURCES += channeltablewidget.cpp \
    channeleditdlg.cpp \
    channelhandler.cpp

HEADERS += channeltablewidget.h\
        channeltablewidget_global.h \
    channeleditdlg.h \
    channelhandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
