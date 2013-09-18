#include "eventloghandler.h"

EventLogHandler::EventLogHandler()
{
}

EventLogHandler::~EventLogHandler()
{
}

void EventLogHandler::init(const EventLog_t &event_log)
{
    int count = 0;
    LogParam logparam;
    for (int i = 0; i < MAX_EVENTCLASS_LINE; i++)
    {
        for (int j = 0; j < MAX_EVENTLOG; j++)
        {
            if (is_event_log_valid(event_log.EventLogList[j + i * MAX_EVENTLOG]))
            {
                logparam.event_type_id = event_log.EventLogList[j + i * MAX_EVENTLOG].EventClassId;
                logparam.log_id = event_log.EventLogList[j + i * MAX_EVENTLOG].EventLogId;
                logparam.log_time = event_log.EventLogList[j + i * MAX_EVENTLOG].EventLogTime;
                logparam.log_value = event_log.EventLogList[j + i * MAX_EVENTLOG].EventLogValue;
                log_map_.insert(logparam.log_id, logparam);
                count++;
            }
            else
            {
                break;
            }
        }
    }
}

void EventLogHandler::set_log(unsigned char log_id, const LogParam &loginfo)
{
    if (log_map_.contains(log_id))
    {
        log_map_[log_id] = loginfo;
        return;
    }
    log_map_.insert(log_id, loginfo);
}

bool EventLogHandler::get_log(unsigned char log_id, LogParam &loginfo)
{
    if (!log_map_.contains(log_id))
    {
        return false;
    }
    loginfo = log_map_.value(log_id);
    return true;
}

bool EventLogHandler::remove_log(unsigned char log_id)
{
    if (!log_map_.contains(log_id))
    {
        return false;
    }
    log_map_.remove(log_id);
    return true;
}

bool EventLogHandler::is_event_log_valid(const EventLogList_t &loginfo)
{
    if (loginfo.EventClassId == 0
            || loginfo.EventLogId == 0
            || loginfo.EventLogTime == 0
            || loginfo.EventLogValue == 0)
    {
        return false;
    }
    return true;
}
