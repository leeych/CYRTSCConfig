#include "eventloghandler.h"
#include "eventlogdescriptor.h"

EventLogHandler::EventLogHandler()
{
    descriptor_ = EventLogDescriptor::GetInstance();
}

EventLogHandler::~EventLogHandler()
{
    descriptor_->DisposeDescriptor();
}

void EventLogHandler::init(const EventLog_t &event_log)
{
    int count = 0;
    LogParam logparam;
    LogParamMap log_map;
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
                log_map.insert(logparam.log_value, logparam);
                count++;
            }
            else
            {
                event_log_map_.insert(event_log.EventLogList[j + i * MAX_EVENTLOG].EventClassId, log_map);
                log_map.clear();
                break;
            }
        }
    }
}

void EventLogHandler::set_log(const LogParam &loginfo)
{
    LogParamMap log_map = event_log_map_.value(loginfo.event_type_id);
    if (log_map.contains(loginfo.log_value))
    {
        log_map.remove(loginfo.log_value);
        log_map.insert(loginfo.log_value, loginfo);
        event_log_map_[loginfo.event_type_id] = log_map;
        return;
    }
    log_map.insert(loginfo.log_id, loginfo);
}

bool EventLogHandler::get_log(unsigned char event_type_id, unsigned int log_value, LogParam &loginfo)
{
    if (!is_event_log_exists(event_type_id, log_value))
    {
        return false;
    }
    LogParamMap log_map = event_log_map_.value(log_value);
    loginfo = log_map.value(log_value);

    return true;
}

bool EventLogHandler::remove_log(unsigned char event_type_id, unsigned int log_value)
{
    if (!is_event_log_exists(event_type_id, log_value))
    {
        return false;
    }
    LogParamMap log_map = event_log_map_[event_type_id];
    log_map.remove(log_value);
    event_log_map_.remove(event_type_id);
    event_log_map_.insert(event_type_id, log_map);

    return true;
}

QList<EventParam> EventLogHandler::get_event_type_list()
{
    return QList<EventParam> event_list;
}

QList<LogParam> EventLogHandler::get_event_log_list()
{
    return QList<LogParam> log_list;
}

QString EventLogHandler::get_desc(unsigned char event_type_id, unsigned int log_value)
{
    return descriptor_->GetLogDesc(event_type_id, log_value);
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

bool EventLogHandler::is_event_log_exists(unsigned char event_type_id, unsigned int log_value)
{
    if (!event_log_map_.contains(event_type_id))
    {
        return false;
    }

    LogParamMap log_map = event_log_map_.value(event_type_id);
    LogParamMap::iterator event_log_iter = log_map.begin();
    while (event_log_iter != log_map.end())
    {
        if (log_map.contains(log_value))
        {
            return true;
        }
        ++event_log_iter;
    }
    return false;
}

EventLogHandler::EventLogIter EventLogHandler::index_of_event_log(unsigned char event_type_id, unsigned int log_value)
{
    if (!is_event_log_exists(event_type_id, log_value))
    {
        return event_log_map_.end();
    }
    return event_log_map_.find(event_type_id);
}
