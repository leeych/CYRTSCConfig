#include <memory>
#include "schedulehandler.h"

ScheduleHandler::ScheduleHandler()
{
    db_ = MDatabase::GetInstance();
}

ScheduleHandler::~ScheduleHandler()
{
}

void ScheduleHandler::init()
{
    schedule_list_ = db_->get_schedule_table();
	qSort(schedule_list_.begin(), schedule_list_.end(), schedule_less_than);
}

unsigned char ScheduleHandler::GetMaxScheduleId()
{
    unsigned char max_id = 0;
    for (int i = 0; i < schedule_list_.size(); i++)
    {
        if (max_id < schedule_list_.at(i).schedule_id)
        {
            max_id = schedule_list_.at(i).schedule_id;
        }
    }
    return max_id;
}

void ScheduleHandler::set_schedule_count(int row)
{
    schedule_num_ = row;
}

int ScheduleHandler::get_schedule_count()
{
    return schedule_num_;
}

void ScheduleHandler::set_schedule(unsigned char sched_id,
                                   unsigned short month, unsigned char week, unsigned int day,
                                   unsigned char timesectionid)
{
    for (int i = 0; i < schedule_list_.size(); i++)
    {
        if (schedule_list_.at(i).schedule_id == sched_id)
        {
            schedule_list_[i].schedule_month = month;
            schedule_list_[i].schedule_week = week;
            schedule_list_[i].schedule_day = day;
            schedule_list_[i].time_section_id = timesectionid;
            return;
        }
    }
    ScheduleParam schedule;
    schedule.schedule_day = sched_id;
    schedule.schedule_month = month;
    schedule.schedule_week = week;
    schedule.schedule_day = day;
    schedule.time_section_id = timesectionid;
    schedule_list_.append(schedule);
}

void ScheduleHandler::set_schedule(unsigned char sched_id, const ScheduleParam &schedule)
{
    int idx = index_of_schedule_list(sched_id);
    if (idx != -1)
    {
        schedule_list_[idx].schedule_id = schedule.schedule_id;
        schedule_list_[idx].schedule_month = schedule.schedule_month;
        schedule_list_[idx].schedule_week = schedule.schedule_week;
        schedule_list_[idx].schedule_day = schedule.schedule_day;
        schedule_list_[idx].time_section_id = schedule.time_section_id;
        return;
    }

    schedule_list_.append(schedule);
}

bool ScheduleHandler::get_schedule(unsigned char sched_id, ScheduleParam &schedule)
{
    int idx = index_of_schedule_list(sched_id);
    if (idx != -1)
    {
        schedule = schedule_list_[idx];
        return true;
    }
    return false;
}

bool ScheduleHandler::is_schedule_exists(unsigned char sched_id)
{
    return (index_of_schedule_list(sched_id) != -1);
}

int ScheduleHandler::index_of_schedule_list(unsigned char sched_id)
{
    int index = -1;
    for (int i = 0; i < schedule_list_.size(); i++)
    {
        if (sched_id == schedule_list_.at(i).schedule_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

QList<ScheduleParam> &ScheduleHandler::get_schedule_list()
{
    return schedule_list_;
}

bool ScheduleHandler::save_data()
{
    std::list<ScheduleParam> std_list = schedule_list_.toStdList();
    std_list.sort();
    schedule_list_.clear();
    schedule_list_ = QList<ScheduleParam>::fromStdList(std_list);
    db_->set_schedule_table(schedule_list_);
    return true;
}

bool ScheduleHandler::remove_schedule( const unsigned char sched_id )
{
	int idx = index_of_schedule_list(sched_id);
	if (idx != -1)
	{
		schedule_list_.removeAt(idx);
		return true;
	}
	return false;
}

bool ScheduleHandler::schedule_less_than( const ScheduleParam &left, const ScheduleParam &right )
{
	if (left.schedule_id > right.schedule_id)
	{
		return false;
	}
	else if (left.time_section_id > right.time_section_id)
	{
		return false;
	}
	else
	{
		return true;
	}
}