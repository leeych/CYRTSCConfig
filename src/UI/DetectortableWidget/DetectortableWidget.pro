#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T08:59:37
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = DetectortableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib/ -lMDatabase -lBottomButtonsWidget

DESTDIR = $$PWD/../../../lib/

DEFINES += DETECTORTABLEWIDGET_LIBRARY

SOURCES += detectortablewidget.cpp \
    detectoreditdlg.cpp \
    detectorhandler.cpp

HEADERS += detectortablewidget.h\
        detectortablewidget_global.h \
    detectoreditdlg.h \
    detectorhandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
