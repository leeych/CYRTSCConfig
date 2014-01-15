#ifndef EVENTLOGDESC_H
#define EVENTLOGDESC_H

#include <map>
#include <string>
#include <QMap>
#include <QString>

class EventLogDescriptor
{
public:
    static EventLogDescriptor *GetInstance();
    QList<QString> get_log_desc_list();
    QString get_log_desc(unsigned char event_type_id, unsigned int log_value);
    QString get_event_type_log_desc(unsigned char event_type_id);
    QString get_ctrl_mode_desc(unsigned char ctrl_mode);
    unsigned char get_event_type_id(const QString &desc);
    void DisposeDescriptor();

    std::string get_log_desc_std(unsigned char event_type_id, unsigned int log_value);
    std::string get_event_type_log_desc_std(unsigned char event_type_id);
    std::string get_ctrl_mode_desc_std(unsigned char ctrl_mode);

    EventLogDescriptor();
    ~EventLogDescriptor();

private:
    static EventLogDescriptor *instance_;

    void GenSoftwareDesc();
    void GenLogCaptionDesc();

//    void tranform();

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

        inline bool operator ==(const EventLogTag &rhs) const
        {
            if (this->event_type_id != rhs.event_type_id
                    || this->log_value != rhs.log_value)
            {
                return false;
            }
            return true;
        }

    public:
        unsigned char event_type_id;
        unsigned int log_value;
    };

//    QMap<EventLogTag, QString> log_desc_map_;
    QMap<QString, QString> log_desc_map_;

    QMap<unsigned int, QString> log_desc_map_ex_;
    QMap<unsigned char, QString> export_caption_map_;
    std::map<EventLogTag, std::string> log_desc_stdmap_;
    std::map<unsigned char, std::string> export_caption_stdmap_;
};

#endif // EVENTLOGDESC_H
