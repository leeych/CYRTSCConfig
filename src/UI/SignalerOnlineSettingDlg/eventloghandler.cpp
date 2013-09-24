#include "eventloghandler.h"
#include "eventlogdescriptor.h"
#include "macrostring.h"

#include <QDateTime>
#include <QStringList>
#include <QFile>

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
    if (event_log.FactEventLogNum == 0)
    {
        return;
    }

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

void EventLogHandler::init_from_file(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray array = file.readAll();
//    EventLog_t *loginfo = (*(static_cast<EventLog*>(array.data()))).log_info;
    EventLog_t loginfo;
    memcpy(&loginfo, array.data(), sizeof(loginfo));
    init(loginfo);
    file.close();
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
    QList<EventParam> event_list;
    return event_list;
}

QList<QString> EventLogHandler::get_event_type_desc_list()
{
    QString str;
    QList<QString> event_type_desc_list;
    QList<LogParam> log_list;
    EventLogIter iter = event_log_map_.begin();
    while (iter != event_log_map_.end())
    {
        LogParamMap log_map = iter.value();
        log_list = log_map.values();
        qSort(log_list);
        for (int i = 0; i < log_list.size(); i++)
        {
            str = descriptor_->GetLogDesc(iter.key(), log_list.at(i).log_value);
            event_type_desc_list.append(str);
        }
        ++iter;
    }
    return event_type_desc_list;
}

QList<LogParam> EventLogHandler::get_event_log_list(unsigned char event_type_id)
{
    QList<LogParam> log_list;
    if (event_type_id == 0)     // return all of the log_param
    {
        EventLogIter iter = event_log_map_.begin();
        while (iter != event_log_map_.end())
        {
            QMap<unsigned int, LogParam>::iterator itr = iter.value().begin();
            while (itr != iter.value().end())
            {
                log_list += itr.value();
                ++itr;
            }
            ++iter;
        }
        qSort(log_list);
        return log_list;
    }

    LogParamMap log_map = event_log_map_.value(event_type_id);
    log_list = log_map.values();
    qSort(log_list);
    return log_list;
}

QString EventLogHandler::get_log_desc(unsigned char event_type_id, unsigned int log_value)
{
    return descriptor_->GetLogDesc(event_type_id, log_value);
}

QString EventLogHandler::get_datetime_desc(unsigned int seconds)
{
    QString desc;
    if (seconds >= 60 * 60 * 8)
    {
        seconds -= 60 * 60 * 8;
    }
    QDateTime datetime = QDateTime::fromTime_t(seconds);
    desc = datetime.toString("yyyy-MM-dd hh:mm:ss ddd");
    return desc;
}

bool EventLogHandler::export_event_log(const QString &file_name)
{
    if (event_log_map_.isEmpty())
    {
        return false;
    }

    QStringList header;
    header << STRING_UI_SIGNALER_EVENT_FLOW_ID << "    "
           << STRING_UI_SIGNALER_EVENT_DATETIME << "    "
           << STRING_UI_SIGNALER_EVENT_DESC << "\n";
    QString line("%1    %2    %3 \n");
    QString content, caption;
    QMap<unsigned int, LogParam> log_map;
    EventLogIter iter = event_log_map_.begin();
    while (iter != event_log_map_.end())
    {
        log_map = iter.value();
        caption = descriptor_->GetEventTypeDesc(iter.key());
        content += caption + "\n";
        for (int i = 0; i < header.count(); i++)
        {
            content.append(header.at(i));
        }
        QMap<unsigned int, LogParam>::iterator log_itr = log_map.begin();
        while (log_itr != log_map.end())
        {
            content += line.arg(log_itr.value().log_id).arg(this->get_datetime_desc(log_itr.value().log_time)).arg(this->get_log_desc(iter.key(), log_itr.key()));
            ++log_itr;
        }
        ++iter;
    }

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.close();
        return false;
    }
    file.write(content.toUtf8());
    file.close();

    return true;
}

bool EventLogHandler::export_report(const QString &file_name)
{
    if (event_log_map_.isEmpty())
    {
        return false;
    }
    int fidx = file_name.lastIndexOf("/");
    int bidx = file_name.lastIndexOf(".");
    QString ip = file_name.mid(fidx, bidx - fidx + 1);
    QString declare_str = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\">"
            "<head>"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"
            "<title>" + ip + "</title>"
            "</head>";
    QString content = declare_str;
    content += "<body>";

    QString table_str = "<table cellspacing=0 cellpadding=4 border=1 valign=middle>";
    QString header = "<tr><th align=left valign=middle><font size=4 >" + STRING_UI_SIGNALER_EVENT_TYPE + "</th>"
            "<th align=left valign=middle><font size=4 >" + STRING_UI_SIGNALER_EVENT_FLOW_ID + "</th>"
            "<th align=left valign=middle><font size=4 >" + STRING_UI_SIGNALER_EVENT_DATETIME + "</th>"
            "<th align=left valign=middle><font size=4 >" + STRING_UI_SIGNALER_EVENT_DESC + "</th>"
			"</tr>";

    QString caption_str;
    QString tbody_str;
    LogParamMap log_map;
    EventLogIter iter = event_log_map_.begin();
    QString str;
    while (iter != event_log_map_.end())
    {
        caption_str = "<tr><td>" + descriptor_->GetEventTypeDesc(iter.key()) + "</td></tr>";
        log_map = iter.value();
        LogParamMap::iterator log_itr = log_map.begin();
        while (log_itr != log_map.end())
        {
            tbody_str +=
                    "<tr>"
                    "<td>" + QString::number(log_itr.value().event_type_id) + "</td>"
                    "<td>" + str.sprintf("%04d", log_itr.value().log_id) + "</td>"
                    "<td>" + this->get_datetime_desc(log_itr.value().log_time) + "</td>"
                    "<td>" + this->get_log_desc(iter.key(), log_itr.value().log_value) + "</td>"
                    "</tr>";
            ++log_itr;
        }
        table_str += caption_str + header + tbody_str;
        tbody_str = "";

        ++iter;
    }
    content += table_str + "</body></html>";

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.close();
        return false;
    }
    file.write(content.toUtf8());
    file.close();

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
