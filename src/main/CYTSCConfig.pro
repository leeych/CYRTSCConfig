#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T13:40:06
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CYRTSCConfig
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include \
               $$PWD/../DataCenter/MDatabase \
               $$PWD/../UI/UIBase \
               $$PWD/../UI/TSCLogin \
               $$PWD/../UI/TscAboutDlg \
               $$PWD/../UI/BottomButtonsWidget \
               $$PWD/../Common/SignalerManager \
               $$PWD/../Common/SystemIniter \
               $$PWD/../Common/Utility \
               $$PWD/../Common/Communication \
               $$PWD/../UI/SignalerOnlineSettingDlg \
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
               $$PWD/../UI/FileListWidget

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../../include/macrostring.h


LIBS += -L$$PWD/../../lib/ -lUIBase -lTscAboutDlg -lUtility -lSystemIniter -lCommunication -lMDatabase \
        -lFileListWidget -lTSCLogin -lBottomButtonsWidget -lConfigoptWidget \
        -lUnitparamtableWidget -lScheduleTableWidget -lTimingplanWidget \
        -lPhasetableWidget -lChanneltableWidget -lDetectortableWidget \
        -lTimesectiontableWidget -lPhaseconflicttableWidget -lPhasetimingtableWidget \
        -lSignalerManager -lSignalerOnlineSettingDlg -lSignalerStatusWidget \


DEFINES += MDATABASE_LIBRARY
DEFINES += UTILITY_LIBRARY
DEFINES += SYSTEMINITER_LIBRARY
DEFINES += COMMUNICATION_LIBRARY
DEFINES += XMLHELPER_LIBRARY
DEFINES += TSCLOGIN_LIBRARY
DEFINES += TSCABOUTDLG_LIBRARY
DEFINES += UIBASE_LIBRARY
DEFINES += FILELISTWIDGET_LIBRARY
DEFINES += CONFIGOPTWIDGET_LIBRARY
DEFINES += BOTTOMBUTTONSWIDGET_LIBRARY

DEFINES += SIGNALERMANAGER_LIBRARY
DEFINES += UNITPARAMTABLEWIDGET_LIBRARY
DEFINES += SCHEDULETABLEWIDGET_LIBRARY
DEFINES += TIMESECTIONTABLEWIDGET_LIBRARY
DEFINES += TIMINGPLANWIDGET_LIBRARY
DEFINES += PHASETIMINGTABLEWIDGET_LIBRARY
DEFINES += PHASETABLEWIDGET_LIBRARY
DEFINES += PHASECONFLICTTABLEWIDGET_LIBRARY
DEFINES += DETECTORTABLEWIDGET_LIBRARY
DEFINES += CHANNELTABLEWIDGET_LIBRARY
DEFINES += SIGNALERONLINESETTINGDLG_LIBRARY
DEFINES += SIGNALERSTATUSWIDGET_LIBRARY


contains(DEFINES,MDATABASE_LIBRARY){
        LIBS += -lMDatabase}
contains(DEFINES,UTILITY_LIBRARY){
        LIBS += -lUtility}
contains(DEFINES,SYSTEMINITER_LIBRARY){
        LIBS += -lSystemIniter}
contains(DEFINES,COMMUNICATION_LIBRARY){
        LIBS += -lCommunication}
contains(DEFINES,XMLHELPER_LIBRARY){
        LIBS += -lXmlHelper}
contains(DEFINES,TSCLOGIN_LIBRARY){
        LIBS += -lTSCLogin}
contains(DEFINES,TSCABOUTDLG_LIBRARY){
        LIBS += -lTscAboutDlg}
contains(DEFINES,UIBASE_LIBRARY){
        LIBS += -lUIBase}
contains(DEFINES,SIGNALERMANAGER_LIBRARY){
        LIBS += -lSignalerManager}
contains(DEFINES,CONFIGOPTWIDGET_LIBRARY){
        LIBS += -lConfigoptWidget}
contains(DEFINES,FILELISTWIDGET_LIBRARY){
        LIBS += -lFileListWidget}
contains(DEFINES,BOTTOMBUTTONSWIDGET_LIBRARY){
        LIBS += -lBottomButtonsWidget}
contains(DEFINES,UNITPARAMTABLEWIDGET_LIBRARY){
        LIBS += -lUnitparamtableWidget}
contains(DEFINES,SCHEDULETABLEWIDGET_LIBRARY){
        LIBS += -lScheduleTableWidget}
contains(DEFINES,TIMESECTIONTABLEWIDGET_LIBRARY){
        LIBS += -lTimesectiontableWidget}
contains(DEFINES,TIMINGPLANWIDGET_LIBRARY){
        LIBS += -lTimingplanWidget}
contains(DEFINES,PHASETIMINGTABLEWIDGET_LIBRARY){
        LIBS += -lPhasetimingtableWidget}
contains(DEFINES,PHASETABLEWIDGET_LIBRARY){
        LIBS += -lPhasetableWidget}
contains(DEFINES,PHASECONFLICTTABLEWIDGET_LIBRARY){
        LIBS += -lPhaseconflicttableWidget}
contains(DEFINES,DETECTORTABLEWIDGET_LIBRARY){
        LIBS += -lDetectortableWidget}
contains(DEFINES,CHANNELTABLEWIDGET_LIBRARY){
        LIBS += -lChanneltableWidget}
contains(DEFINES,SIGNALERONLINESETTINGDLG_LIBRARY){
        LIBS += -lSignalerOnlineSettingDlg}
contains(DEFINES,SIGNALERSTATUSWIDGET_LIBRARY){
        LIBS += -lSignalerStatusWidget}


DESTDIR = $$PWD/../../lib/

TRANSLATIONS = $$PWD/../../resource/language/CYTSC.ts

RESOURCES += \
    ../../resource/Cytsc.qrc

RC_FILE = ../../resource/app.rc
