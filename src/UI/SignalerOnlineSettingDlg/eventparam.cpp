#include "eventparam.h"

EventParam::EventParam()
{
    type_id = 0;
    clear_time = 0;
}

EventParam::EventParam(const EventParam &rhs)
{
    this->type_id = rhs.type_id;
    this->event_name = rhs.event_name;
    this->clear_time = rhs.clear_time;  // secsToUtc
}

EventParam &EventParam::operator =(const EventParam &rhs)
{
    this->type_id = rhs.type_id;
    this->event_name = rhs.event_name;
    this->clear_time = rhs.clear_time;
    return *this;
}

bool EventParam::operator ==(const EventParam &rhs)
{
    if (this->type_id != rhs.type_id
            || this->event_name != rhs.event_name
            || this->clear_time != rhs.clear_time)
    {
        return false;
    }
    return true;
}

EventParam::~EventParam()
{
}
