#ifndef EVENTLOGHANDLER_H
#define EVENTLOGHANDLER_H

#include <QMap>
#include "eventparam.h"
#include "logparam.h"
#include "tsc.h"


class EventLogHandler
{
public:
    EventLogHandler();
    ~EventLogHandler();

    void init(const EventLog_t &event_log);
    void set_events(unsigned char event_type_id, const EventParam &event);
    bool get_events(unsigned char event_type_id, EventParam &event);
    bool remove_events(unsigned char event_type_id);

    void set_log(unsigned char log_id, const LogParam &loginfo);
    bool get_log(unsigned char log_id, LogParam &loginfo);
    bool remove_log(unsigned char log_id);

private:
    bool is_event_log_valid(const EventLogList_t &loginfo);

private:
    QMap<unsigned char, EventParam> event_map_;
    QMap<unsigned char, LogParam> log_map_;
};

#endif // EVENTLOGHANDLER_H
