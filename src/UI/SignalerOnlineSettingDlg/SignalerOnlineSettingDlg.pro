#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T10:48:03
#
#-------------------------------------------------

TARGET = SignalerOnlineSettingDlg
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../../Common/Utility/ \
               $$PWD/../UIBase/ \
               $$PWD/../BottomButtonsWidget/ \
               $$PWD/../UnitparamtableWidget/ \
               $$PWD/../ScheduleTableWidget/ \
               $$PWD/../TimingplanWidget/ \
               $$PWD/../PhasetableWidget/ \
               $$PWD/../ChanneltableWidget/ \
               $$PWD/../DetectortableWidget/ \
               $$PWD/../TimesectiontableWidget/ \
               $$PWD/../PhasetimingtableWidget/ \
               $$PWD/../PhaseconflicttableWidget

LIBS += -L$$PWD/../../../lib/ -lBottomButtonsWidget \
        -lUIBase \
        -lUnitparamtableWidget -lScheduleTableWidget -lTimingplanWidget -lPhasetableWidget \
        -lChanneltableWidget -lDetectortableWidget -lTimesectiontableWidget -lPhaseconflicttableWidget \
        -lPhasetimingtableWidget \
        -lMDatabase -lUtility

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERONLINESETTINGDLG_LIBRARY

SOURCES += signaleronlinesettingdlg.cpp \
    eventlogdlg.cpp \
    timeipdlg.cpp \
    detectorflowdlg.cpp \
    realtimemonitordlg.cpp

HEADERS += signaleronlinesettingdlg.h\
        signaleronlinesettingdlg_global.h \
    eventlogdlg.h \
    timeipdlg.h \
    detectorflowdlg.h \
    realtimemonitordlg.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
