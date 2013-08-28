#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T11:21:19
#
#-------------------------------------------------

TARGET = SignalerStatusWidget
TEMPLATE = lib

INCLUDEPATH == $$PWD/../../../include/

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERSTATUSWIDGET_LIBRARY

SOURCES += signalerstatuswidget.cpp \
    signalerbasiceditdlg.cpp

HEADERS += signalerstatuswidget.h\
        signalerstatuswidget_global.h \
    signalerbasiceditdlg.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
