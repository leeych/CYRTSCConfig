#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T11:21:19
#
#-------------------------------------------------

TARGET = SignalerStatusWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../SignalerOnlineSettingDlg/

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lSignalerOnlineSettingDlg

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERSTATUSWIDGET_LIBRARY

SOURCES += signalerstatuswidget.cpp \
    signalerbasiceditdlg.cpp \
    signalerhandler.cpp

HEADERS += signalerstatuswidget.h\
        signalerstatuswidget_global.h \
    signalerbasiceditdlg.h \
    signalerhandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
