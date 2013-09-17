#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T13:57:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = SignalerManager
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../XmlHelper/

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lUtility -lXmlHelper

DESTDIR = $$PWD/../../../lib/

DEFINES += SIGNALERMANAGER_LIBRARY

SOURCES += signalermanager.cpp \
    signalerelement.cpp \
#    signalerparam.cpp

HEADERS += signalermanager.h\
        SignalerManager_global.h \
#    signalerelement.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE850C85C
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SignalerManager.dll
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
