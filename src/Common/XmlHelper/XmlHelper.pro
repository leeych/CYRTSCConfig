#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T13:36:50
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = XmlHelper
TEMPLATE = lib

DESTDIR = $$PWD/../../../lib/

DEFINES += XMLHELPER_LIBRARY

SOURCES += xmlhelper.cpp

HEADERS += xmlhelper.h\
        XmlHelper_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7C48BD4
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = XmlHelper.dll
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
