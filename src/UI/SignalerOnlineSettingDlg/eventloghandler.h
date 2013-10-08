#ifndef EVENTLOGHANDLER_H
#define EVENTLOGHANDLER_H

#include <QMap>
#include <QList>
#include "eventparam.h"
#include "logparam.h"
#include "tsc.h"

class EventLogDescriptor;

class EventLog
{
public:
    EventLog()
    {
//        length = 0;
//        memset(header, 0x00, sizeof(header));
        memset(&event_type_info, 0x00, sizeof(event_type_info));
        memset(&log_info, 0x00, sizeof(log_info));
    }
    EventLog(const EventLog &rhs)
    {
//        this->length = rhs.length;
        memcpy(&event_type_info, &rhs.event_type_info, sizeof(rhs.event_type_info));
        memcpy(&log_info, &rhs.log_info, sizeof(rhs.log_info));
    }

//    unsigned int length;
//    char header[18];
    EventClass_t event_type_info;
    EventLog_t log_info;
};

class EventLogHandler
{
public:
    EventLogHandler();
    ~EventLogHandler();

    void init(const EventLog_t &event_log);
    void init_from_file(const QString &file_name);
    void set_events(unsigned char event_type_id, const EventParam &event);
    bool get_events(unsigned char event_type_id, EventParam &event);
    bool remove_events(unsigned char event_type_id);

    void set_log(const LogParam &loginfo);
    bool get_log(unsigned char event_type_id, unsigned int log_value, LogParam &loginfo);
    bool remove_log(unsigned char event_type_id, unsigned int log_value);
//    QList<EventParam> get_event_type_list();
    QList<QString> get_all_event_type_desc_list();
    QList<QString> get_event_type_desc_list();
    QList<LogParam> get_event_log_list(unsigned char event_type_id);

    QString get_log_desc(unsigned char event_type_id, unsigned int log_value);
    QString get_datetime_desc(unsigned int seconds);
    unsigned char get_event_type_id_by_desc(const QString &desc);

    bool export_event_log(const QString &file_name);
    bool export_report(const QString &file_name);

private:
    typedef QMap<unsigned int, LogParam> LogParamMap;   // qmap<logvalue, logparam>
    typedef QMap<unsigned char, LogParamMap>::iterator EventLogIter;

    bool is_event_log_valid(const EventLogList_t &loginfo);
    bool is_event_log_exists(unsigned char event_type_id, unsigned int log_value);
    EventLogIter index_of_event_log(unsigned char event_type_id, unsigned int log_value);

private:
    QMap<unsigned char, EventParam> event_map_;
    QMap<unsigned char, LogParam> log_map_;
    QMap<unsigned char, LogParamMap> event_log_map_;

    EventLogDescriptor *descriptor_;
};

#endif // EVENTLOGHANDLER_H
