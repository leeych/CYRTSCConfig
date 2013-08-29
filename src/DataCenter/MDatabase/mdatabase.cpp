#include "mdatabase.h"
#include <list>
#include <algorithm>


//MDatabase *MDatabase::GetInstance()
//{
//    if (instance_ != NULL)
//    {
//        instance_ = new MDatabase;
//    }
//    return instance_;
//}

//void MDatabase::DestroyInstance()
//{
//    delete instance_;
//    instance_ = NULL;
//}

void MDatabase::set_tsc_header(const TSCHeader_t &header)
{
    memcpy(&tsc_header_, &header, sizeof(tsc_header_));
}

void MDatabase::set_unit_table(const Unit_t& ut)
{
    memcpy(&unit_table_, &ut, sizeof(unit_table_));
}

void MDatabase::set_schedule_table(const QList<ScheduleParam> &sched_list)
{
    sched_table_.FactScheduleNum = sched_list.size();
    for (int i = 0; i < sched_list.size(); i++)
    {
        sched_table_.ScheduleList[i].ScheduleId = sched_list.at(i).schedule_id;
        sched_table_.ScheduleList[i].ScheduleMonth = sched_list.at(i).schedule_month;
        sched_table_.ScheduleList[i].ScheduleWeek = sched_list.at(i).schedule_week;
        sched_table_.ScheduleList[i].ScheduleDay = sched_list.at(i).schedule_day;
        sched_table_.ScheduleList[i].TimeSectionId = sched_list.at(i).time_section_id;
    }
}

void MDatabase::set_schedule_table(const Schedule_t &schedule)
{
    memcpy(&sched_table_, &schedule, sizeof(schedule));
}

void MDatabase::set_timesection_table(const QMultiMap<unsigned char, TimeSection> &time_section_map)
{
    QList<unsigned char> map_key_list = time_section_map.keys();
    QList<TimeSection> map_value_list;
    for (int i = 0, j = 0; i < map_key_list.size(); i++)
    {
        map_value_list = time_section_map.values(map_key_list.at(i));
        for (j = 0; j < map_value_list.size(); j++)
        {
            timesection_table_.TimeSectionList[i][j].TimeSectionId = map_value_list.at(i).time_section_id;
            timesection_table_.TimeSectionList[i][j].EventId = map_value_list.at(i).event_id;
            timesection_table_.TimeSectionList[i][j].StartHour = map_value_list.at(i).start_hour;
            timesection_table_.TimeSectionList[i][j].StartMinute = map_value_list.at(i).start_minute;
            timesection_table_.TimeSectionList[i][j].ControlMode = map_value_list.at(i).ctrl_mode;
            timesection_table_.TimeSectionList[i][j].PatternId = map_value_list.at(i).pattern_id;
            timesection_table_.TimeSectionList[i][j].AuxFunc = map_value_list.at(i).aux_func;
            timesection_table_.TimeSectionList[i][j].SpecFunc = map_value_list.at(i).spec_func;
        }
    }
}

void MDatabase::set_timesection_table(const TimeSection_t &timesection)
{
    memcpy(&timesection_table_, &timesection, sizeof(timesection));
}

void MDatabase::set_timing_table(const QList<TimingParam> &tp_list)
{
    pattern_table_.FactPatternNum = tp_list.size();
    for (int i = 0; i < tp_list.size(); i++)
    {
        pattern_table_.PatternList[i].PatternId = tp_list.at(i).timing_id;
        pattern_table_.PatternList[i].CycleTime = tp_list.at(i).cycle_time;
        pattern_table_.PatternList[i].PhaseOffset = tp_list.at(i).phase_offset;
        pattern_table_.PatternList[i].CoordPhase = tp_list.at(i).coor_phase;
        pattern_table_.PatternList[i].TimeConfigId = tp_list.at(i).stage_timing_id;
        pattern_table_.PatternList[i].CycleTime = cycle_time_map_[tp_list.at(i).stage_timing_id];
    }
}

void MDatabase::set_timing_table(const Pattern_t &plan)
{
    memcpy(&pattern_table_, &plan, sizeof(plan));
}

void MDatabase::set_phasetiming_table(const QMultiMap<unsigned char, PhaseTiming> &stage_timing_plan)
{
    QList<unsigned char> map_key_list = stage_timing_plan.keys();
    QList<PhaseTiming> map_value_list;
    unsigned short cycle_time = 0;
    unsigned char idx = 0;
    for (int i = 0; i < map_key_list.size(); i++)
    {
        map_value_list = stage_timing_plan.values(map_key_list.at(i));
        cycle_time = 0;
        for (int j = 0; j < map_value_list.size(); j++)
        {
            timeconfig_table_.TimeConfigList[i][j].TimeConfigId = map_value_list.at(j).phase_timing_id;
            timeconfig_table_.TimeConfigList[i][j].StageId = map_value_list.at(j).stage_id;
            timeconfig_table_.TimeConfigList[i][j].PhaseId = map_value_list.at(j).phase_id;     // single phase
            timeconfig_table_.TimeConfigList[i][j].GreenTime = map_value_list.at(j).green_time;
            timeconfig_table_.TimeConfigList[i][j].RedTime = map_value_list.at(j).red_time;
            timeconfig_table_.TimeConfigList[i][j].YellowTime = map_value_list.at(j).yellow_time;
            timeconfig_table_.TimeConfigList[i][j].SpecFunc = map_value_list.at(j).spec_func;
            timeconfig_table_.TimeConfigList[i][j].SpecFunc |= (map_value_list.at(j).delay_time << 1);

            cycle_time += (map_value_list.at(j).green_time + map_value_list.at(j).yellow_time + map_value_list.at(j).red_time);
            idx = j;
        }
        cycle_time_map_.insert(timeconfig_table_.TimeConfigList[i][idx].TimeConfigId, cycle_time);
    }
}

void MDatabase::set_phasetiming_table(const TimeConfig_t &timeconfig)
{
    memcpy(&timeconfig_table_, &timeconfig, sizeof(timeconfig));
}

void MDatabase::set_phase_table(const QList<PhaseParam> &phase_list)
{
    phase_table_.FactPhaseNum = phase_list.size();
    QList<unsigned char> channel_id_list;
    for (int i = 0; i < phase_list.size(); i++)
    {
        phase_table_.PhaseList[i].PhaseId = phase_list.at(i).phase_id;
        phase_table_.PhaseList[i].PhaseWalkGreen = phase_list.at(i).phase_walk_green;
        phase_table_.PhaseList[i].PhaseWalkClear = phase_list.at(i).phase_walk_clear;
        phase_table_.PhaseList[i].PhaseMinGreen = phase_list.at(i).phase_min_green;
        phase_table_.PhaseList[i].PhaseMaxGreen1 = phase_list.at(i).phase_max_green1;
        phase_table_.PhaseList[i].PhaseMaxGreen2 = phase_list.at(i).phase_max_green2;
        phase_table_.PhaseList[i].PhaseFixGreen = phase_list.at(i).phase_fix_green;
        phase_table_.PhaseList[i].PhaseGreenFlash = phase_list.at(i).phase_green_flash;
        phase_table_.PhaseList[i].PhaseGreenDelay = phase_list.at(i).phase_green_delay;
        phase_table_.PhaseList[i].PhaseType = phase_list.at(i).phase_type;
        phase_table_.PhaseList[i].PhaseSpecFunc = phase_list.at(i).phase_spec_func;
        phase_table_.PhaseList[i].PhaseReserved = phase_list.at(i).phase_reserved;
        channel_id_list = get_id_list_by_bits_op(phase_list.at(i).phase_channel);
        for (int i = 0; i < channel_id_list.size(); i++)
        {
            channel_phase_map_.insertMulti(channel_id_list.at(i), phase_list.at(i).phase_id);
        }
    }
}

void MDatabase::set_phase_table(const Phase_t &phase)
{
    memcpy(&phase_table_, &phase, sizeof(phase));
}

void MDatabase::set_channel_table(const QList<ChannelParam> &channel_list)
{
    int index = 0, i = 0;
    QList<unsigned char> phase_id_list;
    for (i = 0; i < channel_list.size(); i++)
    {
        if (channel_phase_map_.isEmpty())
        {
            channel_table_.ChannelList[index].ChannelCtrlSrc = 0;
            channel_table_.ChannelList[index].ChannelId = channel_list.at(i).channel_id;
            channel_table_.ChannelList[index].ChannelFlash = channel_list.at(i).channel_flash;
            channel_table_.ChannelList[index].ChannelType = channel_list.at(i).channel_type;
            index++;
        }
        else
		{
			phase_id_list = channel_phase_map_.values(channel_list.at(i).channel_id);
            for (int j = 0; j < phase_id_list.size(); j++)
            {
                channel_table_.ChannelList[index].ChannelCtrlSrc = phase_id_list.at(j);
                channel_table_.ChannelList[index].ChannelId = channel_list.at(i).channel_id;
                channel_table_.ChannelList[index].ChannelFlash = channel_list.at(i).channel_flash;
                channel_table_.ChannelList[index].ChannelType = channel_list.at(i).channel_type;
                index++;
            }
        }

        channel_hint_table_.ChannelHintList[i].ChannelId = channel_list.at(i).channel_id;
        channel_hint_table_.ChannelHintList[i].ChannelDirect = channel_list.at(i).channel_direction;
        channel_hint_table_.ChannelHintList[i].ChannelRoad = channel_list.at(i).channel_road;
    }
    channel_table_.FactChannelNum = index;
    channel_hint_table_.FactChannelHintNum = i;
}

void MDatabase::set_channel_table(const Channel_t &channel)
{
    memcpy(&channel_table_, &channel, sizeof(channel));
}

void MDatabase::set_channel_hint_table(const ChannelHint_t &channel_hint)
{
    memcpy(&channel_hint_table_, &channel_hint, sizeof(channel_hint));
}

void MDatabase::set_phase_conflict_table(const QList<PhaseConflictParam> &conflict_list)
{
    phase_conflict_table_.FactPhaseErrorNum = conflict_list.size();
    for (int i = 0; i < conflict_list.size(); i++)
    {
        phase_conflict_table_.PhaseErrorList[i].PhaseId = conflict_list.at(i).phase_id;
        phase_conflict_table_.PhaseErrorList[i].PhaseConflict = conflict_list.at(i).conflict_phase_id;
    }
}

void MDatabase::set_phase_conflict_table(const PhaseError_t &phase_err)
{
    memcpy(&phase_conflict_table_, &phase_err, sizeof(phase_err));
}

void MDatabase::set_detector_table(const QList<DetectorParam> &detector_list)
{
    QList<unsigned char> phase_id_list;
    int index = 0;
    for (int i = 0; i < detector_list.size(); i++)
    {
        phase_id_list = get_id_list_by_bits_op(detector_list.at(i).detector_phase_ids);
        if (phase_id_list.size() == 0)
        {
            detector_table_.DetectorList[index].DetectorId = detector_list.at(i).detector_id;
            detector_table_.DetectorList[index].DetectorPhase = detector_list.at(i).detector_phase_ids;
            detector_table_.DetectorList[index].DetectorType = detector_list.at(i).detector_type;
            detector_table_.DetectorList[index].DetectorDirect = detector_list.at(i).detector_direction;
            detector_table_.DetectorList[index].DetectorDelay = detector_list.at(i).detector_delay;
            detector_table_.DetectorList[index].DetectorFlow = detector_list.at(i).detector_flow;
            detector_table_.DetectorList[index].DetectorOccupy = detector_list.at(i).detector_occupy;
            // request effective time && request failure time left to be done
            detector_table_.DetectorList[index].DetectorSpecFunc = (detector_list.at(i).detector_failure_time << 2);
            detector_table_.DetectorList[index].DetectorSpecFunc |= detector_list.at(i).detector_spec_func;
            index++;
            continue;
        }
        for (int j = 0; j < phase_id_list.size(); j++)
        {
            detector_table_.DetectorList[index].DetectorId = detector_list.at(i).detector_id;
            detector_table_.DetectorList[index].DetectorPhase = phase_id_list.at(j);
            detector_table_.DetectorList[index].DetectorType = detector_list.at(i).detector_type;
            detector_table_.DetectorList[index].DetectorDirect = detector_list.at(i).detector_direction;
            detector_table_.DetectorList[index].DetectorDelay = detector_list.at(i).detector_delay;
            detector_table_.DetectorList[index].DetectorFlow = detector_list.at(i).detector_flow;
            detector_table_.DetectorList[index].DetectorOccupy = detector_list.at(i).detector_occupy;
            // request effective time && request failure time left to be done
            detector_table_.DetectorList[index].DetectorSpecFunc = (detector_list.at(i).detector_failure_time << 2);
            detector_table_.DetectorList[index].DetectorSpecFunc |= detector_list.at(i).detector_spec_func;
            index++;
        }
    }
    detector_table_.FactDetectorNum = index;
}

void MDatabase::set_detector_table(const Detector_t &detector)
{
    memcpy(&detector_table_, &detector, sizeof(detector));
}

TSCHeader_t &MDatabase::get_tsc_header()
{
    return tsc_header_;
}

Unit_t &MDatabase::get_unit_table()
{
    return unit_table_;
}

Schedule_t &MDatabase::get_schedule()
{
    return sched_table_;
}

TimeSection_t &MDatabase::get_timesection()
{
    return timesection_table_;
}

Pattern_t &MDatabase::get_timingplan()
{
    return pattern_table_;
}

TimeConfig_t &MDatabase::get_timeconfig()
{
    return timeconfig_table_;
}

Phase_t &MDatabase::get_phase()
{
    return phase_table_;
}

PhaseError_t &MDatabase::get_phase_err()
{
    return phase_conflict_table_;
}

Channel_t &MDatabase::get_channel()
{
    return channel_table_;
}

ChannelHint_t &MDatabase::get_channel_hint()
{
    return channel_hint_table_;
}

Detector_t &MDatabase::get_detector()
{
    return detector_table_;
}

QList<ScheduleParam> MDatabase::get_schedule_table()
{
    QList<ScheduleParam> sched_list;
    for (int i = 0; i < sched_table_.FactScheduleNum; i++)
    {
        ScheduleParam sched;
        sched.schedule_id = sched_table_.ScheduleList[i].ScheduleId;
        sched.schedule_month = sched_table_.ScheduleList[i].ScheduleMonth;
        sched.schedule_week = sched_table_.ScheduleList[i].ScheduleWeek;
        sched.schedule_day = sched_table_.ScheduleList[i].ScheduleDay;
        sched.time_section_id = sched_table_.ScheduleList[i].TimeSectionId;

        sched_list.push_back(sched);
    }
    return sched_list;
}

QList<TimeSection> MDatabase::get_timesection_table()
{
    QList<TimeSection> time_section_list;
    for (int i = 0; i < timesection_table_.FactTimeSectionNum; i++)
    {
        for (int j = 0; j < timesection_table_.FactEventNum; j++)
        {
            TimeSection section;
            section.time_section_id = timesection_table_.TimeSectionList[i][j].TimeSectionId;
            section.event_id = timesection_table_.TimeSectionList[i][j].EventId;
            section.start_hour = timesection_table_.TimeSectionList[i][j].StartHour;
            section.start_minute = timesection_table_.TimeSectionList[i][j].StartMinute;
            section.ctrl_mode = timesection_table_.TimeSectionList[i][j].ControlMode;
            section.pattern_id = timesection_table_.TimeSectionList[i][j].PatternId;
            section.aux_func = timesection_table_.TimeSectionList[i][j].AuxFunc;
            section.spec_func = timesection_table_.TimeSectionList[i][j].SpecFunc;

            time_section_list.append(section);
        }
    }
    return time_section_list;
}

QList<TimingParam> MDatabase::get_timing_table()
{
    QList<TimingParam> timing_list;
    for (int i = 0; i < pattern_table_.FactPatternNum; i++)
    {
        TimingParam param;
        param.timing_id = pattern_table_.PatternList[i].PatternId;
        param.cycle_time = pattern_table_.PatternList[i].CycleTime;
        param.coor_phase = pattern_table_.PatternList[i].CoordPhase;
        param.phase_offset = pattern_table_.PatternList[i].PhaseOffset;
        param.stage_timing_id = pattern_table_.PatternList[i].TimeConfigId;

        timing_list.append(param);
    }
    return timing_list;
}

QList<PhaseTiming> MDatabase::get_timeconfig_table()
{
	unsigned char spec_func = 0;
    QList<PhaseTiming> phase_timing_list;
    for (int i = 0; i < timeconfig_table_.FactTimeConfigNum; i++)
    {
        for (int j = 0; j < timeconfig_table_.FactStageNum; j++)
        {
            PhaseTiming phase_timing;
            phase_timing.phase_timing_id = timeconfig_table_.TimeConfigList[i][j].TimeConfigId;
            phase_timing.phase_id = timeconfig_table_.TimeConfigList[i][j].PhaseId;
            phase_timing.stage_id = timeconfig_table_.TimeConfigList[i][j].StageId;
            phase_timing.green_time = timeconfig_table_.TimeConfigList[i][j].GreenTime;
            phase_timing.yellow_time = timeconfig_table_.TimeConfigList[i][j].YellowTime;
            phase_timing.red_time = timeconfig_table_.TimeConfigList[i][j].RedTime;
			spec_func = (timeconfig_table_.TimeConfigList[i][j].SpecFunc & 0x0FE);
			spec_func = (spec_func >> 1);
			phase_timing.delay_time = spec_func;
            phase_timing.spec_func = (timeconfig_table_.TimeConfigList[i][j].SpecFunc & 0x01);

            phase_timing_list.append(phase_timing);
        }
    }
    return phase_timing_list;
}

QList<PhaseParam> MDatabase::get_phase_table()
{
    QList<PhaseParam> phase_list;
    QList<unsigned char> channel_id_list;
	init_channel_ctrl_src_phase();
    for (int i = 0; i < phase_table_.FactPhaseNum; i++)
    {
        PhaseParam phase;
        phase.phase_id = phase_table_.PhaseList[i].PhaseId;
        phase.phase_walk_green = phase_table_.PhaseList[i].PhaseWalkGreen;
        phase.phase_walk_clear = phase_table_.PhaseList[i].PhaseWalkClear;
        phase.phase_min_green = phase_table_.PhaseList[i].PhaseMinGreen;
        phase.phase_fix_green = phase_table_.PhaseList[i].PhaseFixGreen;
        phase.phase_green_delay = phase_table_.PhaseList[i].PhaseGreenDelay;
        phase.phase_green_flash = phase_table_.PhaseList[i].PhaseGreenFlash;
        phase.phase_max_green1 = phase_table_.PhaseList[i].PhaseMaxGreen1;
        phase.phase_max_green2 = phase_table_.PhaseList[i].PhaseMaxGreen2;
        phase.phase_type = phase_table_.PhaseList[i].PhaseType;
        phase.phase_spec_func = phase_table_.PhaseList[i].PhaseSpecFunc;
        phase.phase_reserved = phase_table_.PhaseList[i].PhaseReserved;
        //phase.phase_channel = ...
        channel_id_list = channel_phase_read_map_.keys(phase.phase_id);
        if (channel_id_list.isEmpty())
        {
            phase.phase_channel = 0;
        }
        else
        {
            unsigned int phase_channel = 0;
            for (int j = 0; j < channel_id_list.size(); j++)
            {
                phase_channel |= (0x01 << (channel_id_list.at(j) - 1));
            }
            phase.phase_channel = phase_channel;
        }
        phase_list.append(phase);
    }
    return phase_list;
}

QList<ChannelParam> MDatabase::get_channel_table()
{
    QList<ChannelParam> channel_list;
    for (int i = 0; i < channel_table_.FactChannelNum; i++)
    {
        ChannelParam channel;
        channel.channel_id = channel_table_.ChannelList[i].ChannelId;
        channel.channel_flash = channel_table_.ChannelList[i].ChannelFlash;
        channel.channel_type = channel_table_.ChannelList[i].ChannelType;
        channel.channel_direction = channel_hint_table_.ChannelHintList[i].ChannelDirect;
        channel.channel_road = channel_hint_table_.ChannelHintList[i].ChannelRoad;

        channel_list.append(channel);
    }
    std::list<ChannelParam> std_channel_list = channel_list.toStdList();
    std_channel_list.sort();
    std::list<ChannelParam>::iterator iter = unique(std_channel_list.begin(), std_channel_list.end());
    std_channel_list.erase(iter, std_channel_list.end());
    channel_list.clear();
    channel_list = QList<ChannelParam>::fromStdList(std_channel_list);

    return channel_list;
}

QList<DetectorParam> MDatabase::get_detector_table()
{
    QList<DetectorParam> detector_list;
    for (int i = 0; i < detector_table_.FactDetectorNum; i++)
    {
        DetectorParam detector;
        detector.detector_id = detector_table_.DetectorList[i].DetectorId;
//        detector.detector_phase_ids = detector_table_.DetectorList[i].DetectorPhase;
        detector.detector_type = detector_table_.DetectorList[i].DetectorType;
        detector.detector_direction = detector_table_.DetectorList[i].DetectorDirect;
        detector.detector_delay = detector_table_.DetectorList[i].DetectorDelay;
        detector.detector_spec_func = (detector_table_.DetectorList[i].DetectorSpecFunc & 0x02);
        detector.detector_flow = detector_table_.DetectorList[i].DetectorFlow;
        detector.detector_occupy = detector_table_.DetectorList[i].DetectorOccupy;
		detector.detector_effective_time = detector_table_.DetectorList[i].DetectorDelay;
		detector.detector_failure_time = detector_table_.DetectorList[i].DetectorSpecFunc >> 2;
        // request failure time

        detector_list.append(detector);
    }
	qSort(detector_list.begin(), detector_list.end(), detector_less_than);
    std::list<DetectorParam> std_detector_list = detector_list.toStdList();
	std_detector_list.unique();
    detector_list.clear();
	detector_list = QList<DetectorParam>::fromStdList(std_detector_list);

    int index = -1;
    DetectorParam detector_tmp;
    for (int i = 0; i < detector_table_.FactDetectorNum; i++)
    {
        detector_tmp.detector_id = detector_table_.DetectorList[i].DetectorId;
//        detector_tmp.detector_phase_ids = detector_table_.DetectorList[i].DetectorPhase;
        detector_tmp.detector_type = detector_table_.DetectorList[i].DetectorType;
        detector_tmp.detector_direction = detector_table_.DetectorList[i].DetectorDirect;
        detector_tmp.detector_delay = detector_table_.DetectorList[i].DetectorDelay;
        detector_tmp.detector_spec_func = detector_table_.DetectorList[i].DetectorSpecFunc;
        detector_tmp.detector_flow = detector_table_.DetectorList[i].DetectorFlow;
        detector_tmp.detector_occupy = detector_table_.DetectorList[i].DetectorOccupy;
        index = detector_list.indexOf(detector_tmp);
        if (index != -1)
        {
            detector_list[index].detector_phase_ids |= (0x01 << (detector_table_.DetectorList[i].DetectorPhase - 1));
        }
    }

    return detector_list;
}

QList<PhaseConflictParam> MDatabase::get_phase_conflict_table()
{
    QList<PhaseConflictParam> conflict_list;
    for (int i = 0; i < phase_conflict_table_.FactPhaseErrorNum; i++)
    {
        PhaseConflictParam conflict;
        conflict.phase_id = phase_conflict_table_.PhaseErrorList[i].PhaseId;
        conflict.conflict_phase_id = phase_conflict_table_.PhaseErrorList[i].PhaseConflict;
        conflict_list.append(conflict);
    }
    return conflict_list;
}

MDatabase::MDatabase()
{
    memset(&tsc_header_, 0x00, sizeof(tsc_header_));
    memset(&sched_table_, 0x00, sizeof(sched_table_));
    memset(&unit_table_, 0x00, sizeof(unit_table_));
    memset(&timesection_table_, 0x00, sizeof(timesection_table_));
    memset(&pattern_table_, 0x00, sizeof(pattern_table_));
    memset(&timeconfig_table_, 0x00, sizeof(timeconfig_table_));
    memset(&phase_table_, 0x00, sizeof(phase_table_));
    memset(&phase_conflict_table_, 0x00, sizeof(phase_conflict_table_));
    memset(&channel_table_, 0x00, sizeof(channel_table_));
    memset(&channel_hint_table_, 0x00, sizeof(channel_hint_table_));
    memset(&detector_table_, 0x00, sizeof(detector_table_));
}

MDatabase::~MDatabase()
{
}

QList<unsigned char> MDatabase::get_id_list_by_bits_op(unsigned int phase_ids)
{
    QList<unsigned char> phase_id_list;
    for (unsigned char i = 1; i <= 32; i++)
    {
        if ((phase_ids & 0x01) == 0x01)
        {
            phase_id_list.append(i);
        }
        phase_ids = phase_ids >> 1;
    }
    return phase_id_list;
}

bool MDatabase::detector_less_than( const DetectorParam &left, const DetectorParam &right )
{
	if (left.detector_id > right.detector_id)
	{
		return false;
	}
	else if (left.detector_phase_ids > right.detector_phase_ids)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

void MDatabase::init_channel_ctrl_src_phase()
{
	channel_phase_read_map_.clear();
	for (int i = 0; i < channel_table_.FactChannelNum; i++)
	{
		channel_phase_read_map_.insertMulti(channel_table_.ChannelList[i].ChannelId, channel_table_.ChannelList[i].ChannelCtrlSrc);
	}
}