#-------------------------------------------------
#
# Project created by QtCreator 2013-08-12T10:25:01
#
#-------------------------------------------------

QT       -= gui

TARGET = MDatabase
TEMPLATE = lib
CONFIG += c++11

DEFINES += MDATABASE_LIBRARY

INCLUDEPATH += $$PWD/../../../include/

DESTDIR += $$PWD/../../../lib

SOURCES += mdatabase.cpp \
    phaseparam.cpp \
    channelparam.cpp \
    detectorparam.cpp \
    phasetiming.cpp \
    timesection.cpp \
    timingparam.cpp \
    scheduleparam.cpp \
    phaseconflictparam.cpp \
    tscparam.cpp \
    signalerparam.cpp

HEADERS += mdatabase_global.h \
    mdatabase.h \
    phaseparam.h \
    channelparam.h \
    detectorparam.h \
    phasetiming.h \
    timesection.h \
    timingparam.h \
    scheduleparam.h \
    phaseconflictparam.h \
    tscparam.h \
    signalerparam.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
