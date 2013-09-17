#-------------------------------------------------
#
# Project created by QtCreator 2013-08-10T11:21:19
#
#-------------------------------------------------

QT     += xml network

TARGET = SignalerStatusWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../UIBase/ \
               $$PWD/../SignalerOnlineSettingDlg/ \
               $$PWD/../BottomButtonsWidget \
               $$PWD/../UnitparamtableWidget \
               $$PWD/../ScheduleTableWidget \
               $$PWD/../TimingplanWidget \
               $$PWD/../PhasetableWidget \
               $$PWD/../ChanneltableWidget \
               $$PWD/../DetectortableWidget \
               $$PWD/../TimesectiontableWidget \
               $$PWD/../PhasetimingtableWidget \
               $$PWD/../PhaseconflicttableWidget \
               $$PWD/../../Common/SignalerManager \
               $$PWD/../../Common/Communication

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lSignalerOnlineSettingDlg -lSignalerManager -lUtility -lCommunication

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERSTATUSWIDGET_LIBRARY

SOURCES += signalerstatuswidget.cpp \
    signalerbasiceditdlg.cpp \
    signalerhandler.cpp \
    buttonwidget.cpp

HEADERS += signalerstatuswidget.h\
        signalerstatuswidget_global.h \
    signalerbasiceditdlg.h \
    signalerhandler.h \
    buttonwidget.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
