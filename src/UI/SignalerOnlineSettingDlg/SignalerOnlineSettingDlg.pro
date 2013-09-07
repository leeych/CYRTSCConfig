#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T10:48:03
#
#-------------------------------------------------

TARGET = SignalerOnlineSettingDlg
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase \
               $$PWD/../../Common/Utility
               $$PWD/../UIBase \
               $$PWD/../BottomButtonWidget \
               $$PWD/../UnitparamtableWidget \
               $$PWD/../ScheduleTableWidget \
               $$PWD/../TimingplanWidget \
               $$PWD/../PhaseTableWidget \
               $$PWD/../ChanneltableWidget \
               $$PWD/../DetectortableWidget \
               $$PWD/../TimesectiontableWidget \
               $$PWD/../PhasetimingtableWidget \
               $$PWD/../PhaseconflicttableWidget

LIBS += -L$$PWD/../../../lib/ -lBottomButtonWidget \
        -lUnitParamWidget -lScheduleTableWidget -lTimingplanWidget -lPhasetableWidget \
        -lChanneltableWidget -lDetectortableWidget -lTimesectiontableWidget -lPhaseconflicttableWidget \
        -lPhasetimingtableWidget -lUIBase \
        -lMDatabase -lUtility

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERONLINESETTINGDLG_LIBRARY

SOURCES += signaleronlinesettingdlg.cpp

HEADERS += signaleronlinesettingdlg.h\
        signaleronlinesettingdlg_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
