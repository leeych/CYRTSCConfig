#ifndef EVENTLOGMAP_H
#define EVENTLOGMAP_H

#include <QMap>
#include <QString>
#include "macrostring.h"

class EventLogMap
{
public:
    EventLogMap();
    ~EventLogMap();

private:
    void InitInnerData();
    void InitSoftWareEvent();
    void InitGreenConflict();

private:
};

#endif // EVENTLOGMAP_H
