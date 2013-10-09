#include "eventlogdescriptor.h"
#include "macrostring.h"
#include <QDebug>

EventLogDescriptor *EventLogDescriptor::instance_ = NULL;
const int Max_Channel_Num = 64;

EventLogDescriptor::EventLogDescriptor()
{
    GenLogCaptionDesc();
    GenSoftwareDesc();

    EventLogTag tag;
    tag.event_type_id = 2;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL + STRING_EVENT_LOG_RESTORE));
    tag.log_value = 1;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL + STRING_EVENT_LOG_RESTORE));

    tag.event_type_id = 3;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL));
    tag.log_value = 1;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN_CONFL));

    tag.event_type_id = 4;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_BRIGHT_TOGETHER + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_BRIGHT_TOGETHER + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 5;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 6;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 7;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 8;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 9;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 10;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT + STRING_EVENT_LOG_RESTORE));
    }
    tag.event_type_id = 11;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_BRIGHT_TOGETHER));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_BRIGHT_TOGETHER));
    }
    tag.event_type_id = 12;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT));
    }
    tag.event_type_id = 13;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT));
    }
    tag.event_type_id = 14;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT));
    }
    tag.event_type_id = 15;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT));
    }
    tag.event_type_id = 16;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT));
    for (int i = 1; i <= Max_Channel_Num; i++)
    {
        tag.log_value = i;
        log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_CHANNEL + QString::number(i) + STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT));
    }
    tag.event_type_id = 17;
    tag.log_value = 0;
    log_desc_map_.insert(tag, QString(STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT));
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

QList<QString> EventLogDescriptor::get_log_desc_list()
{
    QList<QString> desc_list;
    EventLogTag tag;
    QMap<EventLogTag, QString>::ConstIterator const_iter = log_desc_map_.constBegin();
    while (const_iter != log_desc_map_.constEnd())
    {
        tag = const_iter.key();
        if (tag.log_value == 0)
        {
            desc_list.append(const_iter.value());
        }
        ++const_iter;
    }
    return desc_list;
}

QString EventLogDescriptor::get_log_desc(unsigned char event_type_id, unsigned int log_value)
{
    EventLogTag tag(event_type_id, log_value);
    return log_desc_map_.value(tag);
}

QString EventLogDescriptor::get_event_type_log_desc(unsigned char event_type_id)
{
    QString desc = export_caption_map_.value(event_type_id);
    return desc;
}

QString EventLogDescriptor::get_ctrl_mode_desc(unsigned char ctrl_mode)
{
    QString str;
    switch (ctrl_mode)
    {
    case 0:
        str = STRING_UI_UNIT_AUTO_CONTORL;
        break;
    case 1:
        str = STRING_UI_UNIT_CLOSE_LIGHTS;
        break;
    case 2:
        str = STRING_UI_UNIT_FLASH;
        break;
    case 3:
        str = STRING_UI_UNIT_ALLRED;
        break;
    case 4:
        str = STRING_UI_UNIT_COORDINATE;
        break;
    case 5:
        str = STRING_UI_UNIT_INDUCTION;
        break;
    case 6:
        str = STRING_UI_UNIT_MAIN_INDUCTION;
        break;
    case 7:
        str = STRING_UI_UNIT_SECOND_INDUCTION;
        break;
    case 8:
        str = STRING_UI_UNIT_SINGLE_OPTIONAL;
        break;
    case 9:
        str = STRING_UI_UNIT_CROSS_STREET;
        break;
    case 10:
        str = STRING_UI_UNIT_COORDINATE_INDUCTION;
        break;

//    case 11:
//        str = STRING_UI_UNIT_MASTER_SLAVE;
//        break;
//    case 12:
//        str = STRING_UI_UNIT_SYSTEM_OPTIONAL;
//        break;
//    case 13:
//        str = STRING_UI_UNIT_INTERVENTION;
//        break;
    // addtional selections
    case 27:
        str = STRING_UI_UNIT_BUS_FIRST;
        break;
    case 28:
        str = STRING_UI_UNIT_TRAFFIC_CTRL;
        break;
    case 29:
        str = STRING_UI_UNIT_MANUALLY_CTRL;
        break;
    case 30:
        str = STRING_UI_UNIT_SYS_FAILURE_FLASH;
        break;
    default:
        str = "-";
        break;
    }
    return str;
}

unsigned char EventLogDescriptor::get_event_type_id(const QString &desc)
{
    QMap<EventLogTag, QString>::ConstIterator const_iter = log_desc_map_.constBegin();
    while (const_iter != log_desc_map_.constEnd())
    {
        if (const_iter.value() == desc)
        {
            EventLogTag tag = const_iter.key();
            return tag.event_type_id;
        }
        ++const_iter;
    }
    return 0;
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

void EventLogDescriptor::GenLogCaptionDesc()
{
    export_caption_map_.insert(1, QString(STRING_EXPORT_SOFTWARE_EVENT));
    export_caption_map_.insert(2, QString(STRING_EXPORT_GREEN_CONFLICT_RESTORE));
    export_caption_map_.insert(3, QString(STRING_EXPORT_GREEN_CONFLICT));
    export_caption_map_.insert(4, QString(STRING_EXPORT_RG_BRIGHT_RESTORE));
    export_caption_map_.insert(5, QString(STRING_EXPORT_RED_OFF_BRIGHT_RESTORE));
    export_caption_map_.insert(6, QString(STRING_EXPORT_RED_ERR_BRIGHT_RESTORE));
    export_caption_map_.insert(7, QString(STRING_EXPORT_YELLOW_OFF_BRIGHT_RESTORE));
    export_caption_map_.insert(8, QString(STRING_EXPORT_YELLOW_ERR_BRIGHT_RESTORE));
    export_caption_map_.insert(9, QString(STRING_EXPORT_GREEN_OFF_BRIGHT_RESTORE));
    export_caption_map_.insert(10, QString(STRING_EXPORT_GREEN_ERR_BRIGHT_RESTORE));
    export_caption_map_.insert(11, QString(STRING_EXPORT_RG_BRIGHT));
    export_caption_map_.insert(12, QString(STRING_EXPORT_RED_OFF_BRIGHT));
    export_caption_map_.insert(13, QString(STRING_EXPORT_RED_ERR_BRIGHT));
    export_caption_map_.insert(14, QString(STRING_EXPORT_YELLOW_OFF_BRIGHT));
    export_caption_map_.insert(15, QString(STRING_EXPORT_YELLOW_ERR_BRIGHT));
    export_caption_map_.insert(16, QString(STRING_EXPORT_GREEN_OFF_BRIGHT));
    export_caption_map_.insert(17, QString(STRING_EXPORT_GREEN_ERR_BRIGHT));
    export_caption_map_.insert(18, QString(STRING_EXPORT_CAN_COMMUNICATION));
    export_caption_map_.insert(19, QString(STRING_EXPORT_VEHICLE_FAULT));
    export_caption_map_.insert(20, QString(STRING_EXPORT_DRIVER_EVENT));
}
