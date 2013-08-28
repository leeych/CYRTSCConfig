#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T16:17:34
#
#-------------------------------------------------

TARGET = ScheduleTableWidget
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../../include/ \
               $$PWD/../../DataCenter/MDatabase/ \
               $$PWD/../BottomButtonsWidget

LIBS += -L$$PWD/../../../lib -lMDatabase -lBottomButtonsWidget

DESTDIR = $$PWD/../../../lib

DEFINES += SCHEDULETABLEWIDGET_LIBRARY

SOURCES += scheduletablewidget.cpp \
    scheduledlg.cpp \
    schedulehandler.cpp

HEADERS += scheduletablewidget.h\
        scheduletablewidget_global.h \
    scheduledlg.h \
    schedulehandler.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
