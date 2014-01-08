#-------------------------------------------------
#
# Project created by QtCreator 2013-09-29T16:24:05
#
#-------------------------------------------------

QT       -= gui

TARGET = SystemIniter
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../Utility/ \
               $$PWD/../ConfigManager

LIBS += -L$$PWD/../../../lib/ -lUtility -lConfigManager

DESTDIR = $$PWD/../../../lib/

DEFINES += SYSTEMINITER_LIBRARY

SOURCES += systeminiter.cpp

HEADERS += systeminiter.h\
        SystemIniter_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEED25F78
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SystemIniter.dll
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
