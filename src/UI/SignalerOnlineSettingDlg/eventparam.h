#ifndef EVENTPARAM_H
#define EVENTPARAM_H

#include <QString>

class EventParam
{
public:
    EventParam();
    EventParam(const EventParam &rhs);
    EventParam& operator =(const EventParam &rhs);
    bool operator ==(const EventParam &rhs);
    ~EventParam();

public:
    unsigned char type_id;
    QString event_name;
    unsigned int clear_time;
};

#endif // EVENTPARAM_H
