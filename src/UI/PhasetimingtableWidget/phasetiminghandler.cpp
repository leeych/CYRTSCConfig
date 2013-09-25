#include "phasetiminghandler.h"
#include <list>

PhasetimingHandler::PhasetimingHandler()
{
    db_ = MDatabase::GetInstance();
}

PhasetimingHandler::~PhasetimingHandler()
{
}

void PhasetimingHandler::init_database(void *db_ptr)
{
    db_ = static_cast<MDatabase *>(db_ptr);
}

void PhasetimingHandler::reset_database()
{
    db_ = MDatabase::GetInstance();
}

void PhasetimingHandler::init()
{
    timing_list_ = db_->get_timeconfig_table();
	qSort(timing_list_.begin(), timing_list_.end(), stage_less_than);
    multimap_assign();
}

bool PhasetimingHandler::is_phasetiming_exists(unsigned char phase_timing_id, unsigned char stage_id)
{
    return (index_of_phasetiming_list(phase_timing_id, stage_id) != -1);
}

int PhasetimingHandler::index_of_phasetiming_list(unsigned char phase_timing_id, unsigned char stage_id)
{
    int index = -1;
    for (int i = 0;i < timing_list_.size(); i++)
    {
        if (phase_timing_id == timing_list_.at(i).phase_timing_id
                && stage_id == timing_list_.at(i).stage_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

void PhasetimingHandler::set_phasetiming(unsigned char phase_timing_id, const PhaseTiming &plan)
{
    int idx = index_of_phasetiming_list(phase_timing_id, plan.stage_id);
    if (idx == -1)
    {
        timing_list_.append(plan);
        timing_multi_map_.insertMulti(plan.phase_timing_id, plan);
        return;
    }
    PhaseTiming timing_tmp = timing_list_[idx];
    timing_multi_map_.remove(timing_tmp.phase_timing_id, timing_tmp);
    timing_multi_map_.insertMulti(plan.phase_timing_id, plan);

	timing_list_[idx] = plan;
}

bool PhasetimingHandler::set_phasetiming( unsigned char pre_phase_timing_id, const QList<PhaseTiming>& plan_list )
{
	if (plan_list.isEmpty())
	{
		return false;
	}
	remove_phasetiming(pre_phase_timing_id);
	for (int i = 0; i < plan_list.size(); i++)
	{
		set_phasetiming(plan_list.at(i).phase_timing_id, plan_list.at(i));
	}
	return true;
}

bool PhasetimingHandler::get_phasetiming(unsigned char phase_timing_id, unsigned char stage_id, PhaseTiming &plan)
{
    int idx = index_of_phasetiming_list(phase_timing_id, stage_id);
    if (idx != -1)
    {
        plan.phase_timing_id = phase_timing_id;
        plan.stage_id = stage_id;
        plan.phase_id = timing_list_.at(idx).phase_id;
        plan.green_time = timing_list_.at(idx).green_time;
        plan.yellow_time = timing_list_.at(idx).yellow_time;
        plan.red_time = timing_list_.at(idx).red_time;
        plan.spec_func = timing_list_.at(idx).spec_func;
        return true;
    }
    return false;
}

bool PhasetimingHandler::remove_phasetiming(unsigned char phase_timing_id)
{
    QList<PhaseTiming> phase_timing_bak_list;
    for (int i = 0; i < timing_list_.size(); i++)
    {
        if (phase_timing_id == timing_list_.at(i).phase_timing_id)
        {
            phase_timing_bak_list.append(timing_list_.at(i));
        }
    }
    for (int i = 0; i < phase_timing_bak_list.size(); i++)
    {
        timing_multi_map_.remove(phase_timing_id, phase_timing_bak_list.at(i));
        timing_list_.removeAll(phase_timing_bak_list.at(i));
    }
    return true;
}

bool PhasetimingHandler::remove_phasetiming_stage(unsigned char phase_timing_id, unsigned char stage_id)
{
    int idx = index_of_phasetiming_list(phase_timing_id, stage_id);
    if (idx != -1)
    {
        timing_multi_map_.remove(phase_timing_id, timing_list_.at(idx));
        timing_list_.removeAt(idx);
        return true;
    }
    return false;
}

QList<unsigned char> PhasetimingHandler::get_phasetiming_id_list()
{
	std::list<unsigned char> std_list = timing_multi_map_.keys().toStdList();
	std_list.sort();
	std_list.unique();
	QList<unsigned char> stage_timing_id_list = QList<unsigned char>::fromStdList(std_list);
	return stage_timing_id_list;
}

QList<PhaseTiming> PhasetimingHandler::get_phasetiming_list()
{
	qSort(timing_list_.begin(), timing_list_.end(), stage_less_than);
    return timing_list_;
}

QList<PhaseTiming> PhasetimingHandler::get_phasetiming_list(unsigned char phase_timing_id)
{
//    QList<PhaseTiming> temp_list;
	QList<PhaseTiming> temp_list = timing_multi_map_.values(phase_timing_id);
	qSort(temp_list.begin(), temp_list.end(), stage_less_than);
	return temp_list;
}

unsigned char PhasetimingHandler::get_max_map_key()
{
	return 0;
}

QString PhasetimingHandler::get_release_phase_ids_desc( unsigned int phase_ids )
{
	QString str;
    for (size_t i = 1; i <= sizeof(phase_ids) * 8; i++)
	{
		if ((phase_ids & 0x01) == 0x01)
		{
			str += QString::number(i) + "/";
		}
		phase_ids = phase_ids >> 1;
	}
    return str.left(str.size() - 1);
}

bool PhasetimingHandler::save_data()
{
//    std::list<PhaseTiming> std_list = timing_list_.toStdList();
//    std_list.sort();
//    timing_list_.clear();
//    timing_list_ = QList<PhaseTiming>::fromStdList(std_list);
//    multimap_assign();
    db_->set_phasetiming_table(timing_multi_map_);
    return true;
}

bool PhasetimingHandler::stage_less_than( const PhaseTiming& pre, const PhaseTiming& next )
{
	if (pre.phase_timing_id < next.phase_timing_id)
	{
		return true;
	}
	else if (pre.phase_timing_id > next.phase_timing_id)
	{
		return false;
	}
	else if (pre.stage_id <= next.stage_id)
	{
		return true;
	}
	else
	{
		return false;
    }
}

void PhasetimingHandler::multimap_assign()
{
	timing_multi_map_.clear();
    for (int i = 0; i < timing_list_.size(); i++)
    {
        timing_multi_map_.insertMulti(timing_list_.at(i).phase_timing_id, timing_list_.at(i));
    }
}

unsigned char PhasetimingHandler::get_start_delay_time_desc( unsigned char spec_func )
{
	spec_func = spec_func & 0x0F0;
	spec_func = spec_func >> 1;
	return spec_func;
}
