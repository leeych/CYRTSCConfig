#ifndef EVENTLOGDESC_H
#define EVENTLOGDESC_H

#include <QMap>
#include <QString>

class EventLogDescriptor
{
public:
    static EventLogDescriptor *GetInstance();
    QString get_log_desc(unsigned char event_type_id, unsigned int log_value);
    QString get_event_type_log_desc(unsigned char event_type_id);
    void DisposeDescriptor();

private:
    EventLogDescriptor();
    ~EventLogDescriptor();
    static EventLogDescriptor *instance_;

    void GenSoftwareDesc();
    void GenLogCaptionDesc();

private:
    class EventLogTag
    {
    public:
        EventLogTag()
        {
            event_type_id = 0;
            log_value = 0;
        }
        EventLogTag(const EventLogTag &rhs)
        {
            this->event_type_id = rhs.event_type_id;
            this->log_value = rhs.log_value;
        }
        EventLogTag(unsigned char event_type_id, unsigned int log_value)
        {
            this->event_type_id = event_type_id;
            this->log_value = log_value;
        }
        EventLogTag& operator=(const EventLogTag &rhs)
        {
            this->event_type_id = rhs.event_type_id;
            this->log_value = rhs.log_value;
            return *this;
        }

        inline bool operator <(const EventLogTag &rhs) const
        {
            if (this->event_type_id < rhs.event_type_id)
            {
                return true;
            }
            if (this->log_value < rhs.log_value)
            {
                return true;
            }
            return false;
        }

    public:
        unsigned char event_type_id;
        unsigned int log_value;
    };

    QMap<EventLogTag, QString> log_desc_map_;
    QMap<unsigned char, QString> log_txt_caption_map_;
};

#endif // EVENTLOGDESC_H
