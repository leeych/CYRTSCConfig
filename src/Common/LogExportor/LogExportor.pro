#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T10:30:29
#
#-------------------------------------------------

QT       -= gui

TARGET = LogExportor
TEMPLATE = lib

DEFINES += LOGEXPORTOR_LIBRARY

INCLUDEPATH += $$PWD/../../UI/SignalerOnlineSettingDlg/

DESTDIR = $$PWD/../../../lib/

SOURCES += eventlogexportor.cpp

HEADERS += eventlogexportor.h\
        LogExportor_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE70692F6
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = LogExportor.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
