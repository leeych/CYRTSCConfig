#include "eventlogdescriptor.h"
#include "macrostring.h"

EventLogDescriptor *EventLogDescriptor::instance_ = NULL;
const int Max_Channel_Num = 64;

EventLogDescriptor::EventLogDescriptor()
{
    GenSoftwareDesc();

    EventLogTag tag;
    tag.event_type_id = 2;
    tag.log_value = 1;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL + STRING_EVENT_LOG_RESTORE));

    tag.event_type_id = 3;
    tag.log_value = 1;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL));

    tag.event_type_id = 4;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 5;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 6;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 7;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 8;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 9;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 10;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }

    tag.event_type_id = 11;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RG_BRIGHT));
    }

    tag.event_type_id = 12;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT));
    }

    tag.event_type_id = 13;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT));
    }

    tag.event_type_id = 14;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT));
    }

    tag.event_type_id = 15;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT));
    }

    tag.event_type_id = 16;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT));
    }

    tag.event_type_id = 17;
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT));
    }

    tag.event_type_id = 18;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CAN_COMMUNICATION));
    for (int i = 1; i <= 16; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_NORMAL));

        tag.log_value = i + 16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_FAULT));
    }
    tag.log_value = 33;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_MPU + STRING_EVENT_LOG_FAULT));
    tag.log_value = 34;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_MPU + STRING_EVENT_LOG_NORMAL));

    tag.event_type_id = 19;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER_EVENT));
    for (int i = 1; i <= 16; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_NOT_INSTALLED));

        tag.log_value = i + 16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_RUN));

        tag.log_value = i + 2*16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_NORMAL));

        tag.log_value = i + 3*16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_YELLOW_FLASH_FAULT));

        tag.log_value = i + 4*16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_YELLOW_FLASH_SEPERATE + STRING_EVENT_LOG_OFF));

        tag.log_value = i + 5*16;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DRIVER + QString::number(i) + STRING_EVENT_LOG_YELLOW_FLASH_SEPERATE + STRING_EVENT_LOG_CTRL));
    }

    tag.event_type_id = 20;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DETECTOR_FAULT));
    for (int i = 1; i <= 48; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_DETECTOR + QString::number(i) + STRING_EVENT_LOG_FAULT));
    }
}

EventLogDescriptor::~EventLogDescriptor()
{
}

EventLogDescriptor *EventLogDescriptor::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new EventLogDescriptor;
    }
    return instance_;
}

QString EventLogDescriptor::GetLogDesc(unsigned char event_type_id, unsigned int log_value)
{
    EventLogTag tag(event_type_id, log_value);
    QString desc = log_desc_map_.value(tag);
    return desc;
}

void EventLogDescriptor::DisposeDescriptor()
{
    if (instance_ != NULL)
    {
        delete instance_;
        instance_ = NULL;
    }
}


void EventLogDescriptor::GenSoftwareDesc()
{
    EventLogTag tag;
    tag.event_type_id = 1;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_SOFTWARE));
    tag.log_value = 1;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_START));
    tag.log_value = 2;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_SERIAL_OPEN + STRING_SUCCEEDED));
    tag.log_value = 3;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_SERIAL_OPEN +STRING_FAILED));
    tag.log_value = 4;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_SERIAL_CONFIG + STRING_SUCCEEDED));
    tag.log_value = 5;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_SERIAL_CONFIG + STRING_FAILED));
    tag.log_value = 6;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_WATCHDOG_OPEN + STRING_SUCCEEDED));
    tag.log_value = 7;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_WATCHDOG_OPEN + STRING_FAILED));
    tag.log_value = 8;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CONFIG_FILE_OPEN + STRING_SUCCEEDED));
    tag.log_value = 9;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CONFIG_FILE_OPEN + STRING_FAILED));
    tag.log_value = 10;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_NO_TIMESECTION));
    tag.log_value = 11;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_NO_PLAN_RUN));
    tag.log_value = 12;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CTRLMODE_INVALID));
    tag.log_value = 13;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_PHASE_CONFL_STAGE));
    tag.log_value = 14;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_NO_VALID_PHASE));
    tag.log_value = 15;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_WRITE_BACKPLANE_SERIAL + STRING_FAILED));
    tag.log_value = 16;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_WRITE_PANEL_SERIAL + STRING_FAILED));
    tag.log_value = 17;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_INIT + STRING_SUCCEEDED));
    tag.log_value = 18;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_INIT + STRING_FAILED));
    tag.log_value = 19;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_RECV + STRING_SUCCEEDED));
    tag.log_value = 20;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_RECV + STRING_FAILED));
    tag.log_value = 21;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_SEND + STRING_SUCCEEDED));
    tag.log_value = 22;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_SEND + STRING_FAILED));
    tag.log_value = 23;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_LOCAL_SERVER_CONNECT + STRING_FAILED));
    tag.log_value = 24;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_READ_GPS_DATA + STRING_SUCCEEDED));
    tag.log_value = 25;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_READ_GPS_DATA + STRING_FAILED));
}
