#ifndef LOGPARAM_H
#define LOGPARAM_H

class LogParam
{
public:
    LogParam();
    LogParam(const LogParam &rhs);
    LogParam &operator =(const LogParam &rhs);
    bool operator ==(const LogParam &rhs) const;
    bool operator <(const LogParam &rhs) const;
    ~LogParam();

public:
    unsigned char event_type_id;
    unsigned char log_id;
    unsigned int log_time;
    unsigned int log_value;
};

#endif // LOGPARAM_H
