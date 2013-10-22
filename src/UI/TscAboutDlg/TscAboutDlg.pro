#-------------------------------------------------
#
# Project created by QtCreator 2013-10-11T14:56:27
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = TscAboutDlg
TEMPLATE = lib

DEFINES += TSCABOUTDLG_LIBRARY

INCLUDEPATH += $$PWD/../../../include/

LIBS += -L$$PWD/../../../lib/ -lUtility

DESTDIR = $$PWD/../../../lib/

SOURCES += tscaboutdlg.cpp

HEADERS += tscaboutdlg.h\
        TscAboutDlg_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE9812E98
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = TscAboutDlg.dll
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
