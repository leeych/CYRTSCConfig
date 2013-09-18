#include "logparam.h"

LogParam::LogParam()
{
    event_type_id = 0;
    log_id = 0;
    log_time = 0;
    log_value = 0;
}

LogParam::LogParam(const LogParam &rhs)
{
    this->event_type_id = rhs.event_type_id;
    this->log_id = rhs.log_id;
    this->log_time = rhs.log_time;
    this->log_value = rhs.log_value;
}

LogParam &LogParam::operator =(const LogParam &rhs)
{
    this->event_type_id = rhs.event_type_id;
    this->log_id = rhs.log_id;
    this->log_time = rhs.log_time;
    this->log_value = rhs.log_value;

    return *this;
}

bool LogParam::operator ==(const LogParam &rhs)
{
    if (this->event_type_id != rhs.event_type_id
            || this->log_id != rhs.log_id
            || this->log_time != rhs.log_time
            || this->log_value != rhs.log_value)
    {
        return false;
    }
    return true;
}

LogParam::~LogParam()
{
}
