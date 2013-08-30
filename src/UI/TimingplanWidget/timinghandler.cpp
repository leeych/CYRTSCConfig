#include "timinghandler.h"

TimingHandler::TimingHandler()
{
    db_ = MDatabase::GetInstance();
}

TimingHandler::~TimingHandler()
{
}

void TimingHandler::init()
{
    timing_list_ = db_->get_timing_table();
    for (int i = 0; i < timing_list_.size(); i++)
    {
        stage_timing_cycle_map_.insert(timing_list_.at(i).stage_timing_id, timing_list_.at(i).cycle_time);
    }
}

bool TimingHandler::is_timing_plan_exists(unsigned char timing_id)
{
    return (index_of_timing_plan_list(timing_id) != -1);
}

void TimingHandler::set_timing_plan(unsigned char timing_id, const TimingParam &plan)
{
    // modify plan
    int idx = index_of_timing_plan_list(timing_id);
    if (idx != -1)
    {
        memcpy(&(timing_list_[idx]), &plan, sizeof(plan));
        return;
    }

    // add new plan
    timing_list_.append(plan);
}

bool TimingHandler::get_timing_plan(unsigned char timing_id, TimingParam &plan)
{
    int idx = index_of_timing_plan_list(timing_id);
    if (idx != -1)
    {
        memcpy(&plan, &(timing_list_[idx]), sizeof(plan));
        return true;
    }
    return false;
}

int TimingHandler::index_of_timing_plan_list(unsigned char timing_id)
{
    int index = -1;
    for (int i = 0; i < timing_list_.size(); i++)
    {
        if (timing_id == timing_list_.at(i).timing_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

bool TimingHandler::add_timing_plan(const TimingParam &plan)
{
    if (is_timing_plan_exists(plan.timing_id))
    {
        return false;
    }
    timing_list_.append(plan);
    return true;
}

bool TimingHandler::remove_timing_plan(unsigned char timing_id)
{
    int idx = index_of_timing_plan_list(timing_id);
    if (idx != -1)
    {
        timing_list_.removeAt(idx);
        return true;
    }
    return false;
}

QList<TimingParam> &TimingHandler::get_timing_plan_list()
{
    return timing_list_;
}

QList<unsigned char> TimingHandler::get_phasetiming_id_list()
{
    QList<unsigned char> plan_id_list;
    QList<PhaseTiming> plan_list = db_->get_timeconfig_table();
    for (int i = 0; i < plan_list.size(); i++)
    {
        plan_id_list.append(plan_list.at(i).phase_timing_id);
    }
    return plan_id_list;
}

bool TimingHandler::save_data()
{
    std::list<TimingParam> std_list = timing_list_.toStdList();
    std_list.sort();
    timing_list_.clear();
    timing_list_ = QList<TimingParam>::fromStdList(std_list);
    db_->set_timing_table(timing_list_);
    return true;
}

void TimingHandler::update_cycle_time()
{
    cycle_time_assign();
    for (int i = 0; i < timing_list_.size(); i++)
    {
        if (stage_timing_cycle_map_.contains(timing_list_.at(i).stage_timing_id))
        {
            timing_list_[i].cycle_time = stage_timing_cycle_map_.value(timing_list_.at(i).stage_timing_id);
        }
    }
}

unsigned char TimingHandler::get_cycletime_by_stagetiming_id(unsigned char stage_timing_id)
{
    if (stage_timing_cycle_map_.contains(stage_timing_id))
    {
        return stage_timing_cycle_map_.value(stage_timing_id);
    }
    return 0;
}

void TimingHandler::cycle_time_assign()
{
    stage_timing_cycle_map_.clear();
    unsigned char cycle_time = 0;
    QList<PhaseTiming> plan_list = db_->get_timeconfig_table();
    for (int i = 0; i < plan_list.size(); i++)
    {
        cycle_time = plan_list.at(i).green_time + plan_list.at(i).yellow_time + plan_list.at(i).red_time;
		if (stage_timing_cycle_map_.contains(plan_list.at(i).phase_timing_id))
		{
			stage_timing_cycle_map_[plan_list.at(i).phase_timing_id] += cycle_time;
		}
        else
		{
			stage_timing_cycle_map_.insert(plan_list.at(i).phase_timing_id, cycle_time);
		}
    }
}
