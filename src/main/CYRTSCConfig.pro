#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T13:40:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CYRTSCConfig
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include \
               $$PWD/../DataCenter\MDatabase \
               $$PWD/../UI/UIBase \
               $$PWD/../UI/TSCLogin \
               $$PWD/../UI/BottomButtonsWidget \
               $$PWD/../UI/SignalerStatusWidget \
               $$PWD/../UI/ConfigoptWidget \
               $$PWD/../UI/FileListWidget \
               $$PWD/../UI/ScheduleTableWidget \
               $$PWD/../UI/TimingplanWidget \
               $$PWD/../UI/PhasetableWidget \
               $$PWD/../UI/ChanneltableWidget \
               $$PWD/../UI/DetectortableWidget \
               $$PWD/../UI/TimesectiontableWidget \
               $$PWD/../UI/PhasetimingtableWidget \
               $$PWD/../UI/PhaseconflicttableWidget \
               $$PWD/../UI/UnitparamtableWidget \
               $$PWD/../Common/Utility \
#               $$PWD/../Common/FileReaderWriter

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../../include/macrostring.h

LIBS += -L$$PWD/../../lib -lTSCLogin -lBottomButtonsWidget -lSignalerStatusWidget -lConfigoptWidget -lFileListWidget \
        -lScheduleTableWidget -lTimingplanWidget -lPhasetableWidget -lChanneltableWidget \
        -lDetectortableWidget -lTimesectiontableWidget -lPhaseconflicttableWidget \
        -lPhasetimingtableWidget -lUnitparamtableWidget -lUIBase \
        -lMDatabase \
        -lUtility

DESTDIR = $$PWD/../../lib/

TRANSLATIONS = $$PWD/../../resource/CYTSC.ts
