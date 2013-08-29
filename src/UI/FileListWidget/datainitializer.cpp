#include "datainitializer.h"
#include "mdatabase.h"


DataInitializer::DataInitializer()
{

}

DataInitializer::~DataInitializer()
{
}

bool DataInitializer::ResetDatabase()
{
    MDatabase *db = MDatabase::GetInstance();
    if (db == NULL)
    {
        return false;
    }

    TSCHeader_t header;             memset(&header, 0x00, sizeof(header));
    Unit_t unit_param;              memset(&unit_param, 0x00, sizeof(unit_param));
    Schedule_t schedule;            memset(&schedule, 0x00, sizeof(schedule));
    TimeSection_t time_section;     memset(&time_section, 0x00, sizeof(time_section));
    Pattern_t timing_plan;          memset(&timing_plan, 0x00, sizeof(timing_plan));
    TimeConfig_t time_config;       memset(&time_config, 0x00, sizeof(time_config));
    Phase_t phase;                  memset(&phase, 0x00, sizeof(phase));
    PhaseError_t phase_err;         memset(&phase_err, 0x00, sizeof(phase_err));
    Channel_t channel;              memset(&channel, 0x00, sizeof(channel));
    ChannelHint_t channel_hint;     memset(&channel_hint, 0x00, sizeof(channel_hint));
    Detector_t detector;            memset(&detector, 0x00, sizeof(detector));


    db->set_tsc_header(header);
    db->set_unit_table(unit_param);
    db->set_schedule_table(schedule);
    db->set_timesection_table(time_section);
    db->set_timing_table(timing_plan);
    db->set_phasetiming_table(time_config);
    db->set_phase_table(phase);
    db->set_phase_conflict_table(phase_err);
    db->set_channel_table(channel);
    db->set_channel_hint_table(channel_hint);
    db->set_detector_table(detector);

    return true;
}
