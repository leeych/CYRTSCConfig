#include "timesectionhandler.h"
#include "macrostring.h"
#include <QDebug>

TimesectionHandler::TimesectionHandler()
{
    db_ = MDatabase::GetInstance();
}

TimesectionHandler::~TimesectionHandler()
{
}

void TimesectionHandler::init()
{
    time_section_event_list_ = db_->get_timesection_table();
    multimap_assign();
}

bool TimesectionHandler::is_timesection_exists(unsigned char time_section_id, unsigned char event_id)
{
    return (index_of_timesection_list(time_section_id, event_id) != -1);
}

bool TimesectionHandler::is_timesection_exists(unsigned char time_section_id)
{
    for (int i = 0; i < time_section_event_list_.size(); i++)
    {
        if (time_section_id == time_section_event_list_.at(i).time_section_id)
        {
            return true;
        }
    }
    return false;
}

int TimesectionHandler::index_of_timesection_list(unsigned char time_section_id, unsigned char event_id)
{
    int index = -1;
    for (int i = 0; i < time_section_event_list_.size(); i++)
    {
        if (time_section_id == time_section_event_list_.at(i).time_section_id
              && event_id == time_section_event_list_.at(i).event_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

bool TimesectionHandler::get_timesection(unsigned char time_section_id, unsigned char event_id, TimeSection &time_section)
{
    int idx = index_of_timesection_list(time_section_id, event_id);
    if (idx != -1)
    {
        time_section.time_section_id = time_section_id;
        time_section.event_id = event_id;
        time_section.start_hour = time_section_event_list_.at(idx).start_hour;
        time_section.start_minute = time_section_event_list_.at(idx).start_minute;
        time_section.aux_func = time_section_event_list_.at(idx).aux_func;
        time_section.ctrl_mode = time_section_event_list_.at(idx).ctrl_mode;
        time_section.pattern_id = time_section_event_list_.at(idx).pattern_id;
        time_section.spec_func = time_section_event_list_.at(idx).spec_func;
        return true;
    }
    return false;
}

void TimesectionHandler::set_timesection(unsigned char pre_section_id, const TimeSection &time_section)
{
	int idx = index_of_timesection_list(pre_section_id, time_section.event_id);
    if (idx == -1)
    {
        time_section_event_list_.append(time_section);
        time_section_map_.insertMulti(time_section.time_section_id, time_section);
        return;
	}
    TimeSection time_section_tmp = time_section_event_list_[idx];
    time_section_map_.remove(time_section.time_section_id, time_section_tmp);
    time_section_map_.insertMulti(time_section.time_section_id, time_section);

	time_section_event_list_[idx] = time_section;
}

bool TimesectionHandler::set_timesection( unsigned char pre_section_id, const QList<TimeSection>& section_list )
{
	if (section_list.isEmpty())
	{
		return false;
	}
	remove_timesection(pre_section_id);
	for (int i = 0; i < section_list.size(); i++)
	{
		set_timesection(section_list.at(i).time_section_id, section_list.at(i));
	}
	return true;
}
bool TimesectionHandler::remove_timesection(unsigned char time_section_id)
{
    QList<TimeSection> time_section_bak_list;
    for (int i = 0; i < time_section_event_list_.size(); i++)
    {
        if (time_section_id == time_section_event_list_.at(i).time_section_id)
        {
            time_section_bak_list.append(time_section_event_list_.at(i));
        }
    }
    for (int i = 0; i < time_section_bak_list.size(); i++)
    {
        time_section_map_.remove(time_section_id, time_section_bak_list.at(i));
        time_section_event_list_.removeAll(time_section_bak_list.at(i));
    }
    return true;
}

bool TimesectionHandler::remove_timesection_event(unsigned char time_section_id, unsigned char event_id)
{
    int idx = index_of_timesection_list(time_section_id, event_id);
    if (idx != -1)
    {
        time_section_map_.remove(time_section_id, time_section_event_list_.at(idx));
        time_section_event_list_.removeAt(idx);
        return true;
    }
    return false;
}

QList<TimeSection> &TimesectionHandler::get_timesection_list()
{
	qSort(time_section_event_list_.begin(), time_section_event_list_.end(), event_id_less_than);
    return time_section_event_list_;
}

bool TimesectionHandler::is_timesection_has_event(unsigned char time_section_id)
{
    return !time_section_map_.values(time_section_id).isEmpty();
}

QList<TimeSection> TimesectionHandler::get_timesection_list(unsigned char timesection_id)
{
    QList<TimeSection> time_section_list = time_section_map_.values(timesection_id);
    qSort(time_section_list.begin(), time_section_list.end(), event_id_less_than);
    return time_section_list;
}

QList<unsigned char> TimesectionHandler::get_timesection_id_list()
{
	std::list<unsigned char> std_list = time_section_map_.keys().toStdList();
	std_list.sort();
	std_list.unique();
	QList<unsigned char> time_section_id_list = QList<unsigned char>::fromStdList(std_list);
	return time_section_id_list;
}

QString TimesectionHandler::get_ctrl_mode_desc(unsigned char ctrl_mode)
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
    case 6:
        str = STRING_UI_UNIT_INDUCTION;
        break;
    case 8:
        str = STRING_UI_UNIT_SINGLE_OPTIONAL;
        break;
    case 11:
        str = STRING_UI_UNIT_MASTER_SLAVE;
        break;
    case 12:
        str = STRING_UI_UNIT_SYSTEM_OPTIONAL;
        break;
    case 13:
        str = STRING_UI_UNIT_INTERVENTION;
        break;
    default:
        break;
    }
    return str;
}

QList<unsigned char> TimesectionHandler::get_timing_id_list()
{
    QList<unsigned char> timing_id_list;
    //for (int i = 0; i < db_->get_pattern_table().FactPatternNum; i++)
    //{
    //    timing_id_list.append(db_->get_pattern_table().PatternList[i].PatternId);
    //}
    //qSort(timing_id_list);
	for (unsigned char i = 1; i <= 32; i++)
	{
		timing_id_list.append(i);
	}
    return timing_id_list;
}

unsigned char TimesectionHandler::get_ctrl_mode_by_desc(const QString &str)
{
    if (str == STRING_UI_UNIT_AUTO_CONTORL)
    {
        return 0;
    }
    else if (str == STRING_UI_UNIT_CLOSE_LIGHTS)
    {
        return 1;
    }
    else if (str == STRING_UI_UNIT_FLASH)
    {
        return 2;
    }
    else if (str == STRING_UI_UNIT_ALLRED)
    {
        return 3;
    }
    else if (str == STRING_UI_UNIT_INDUCTION)
    {
        return 6;
    }
    else if (str == STRING_UI_UNIT_SINGLE_OPTIONAL)
    {
        return 8;
    }
    else if (str == STRING_UI_UNIT_MASTER_SLAVE)
    {
        return 11;
    }
    else if (str == STRING_UI_UNIT_SYSTEM_OPTIONAL)
    {
        return 12;
    }
    else if (str == STRING_UI_UNIT_INTERVENTION)
    {
        return 13;
    }
    return 14;
}

bool TimesectionHandler::event_id_less_than( const TimeSection& pre, const TimeSection& next )
{
	if (pre.time_section_id < next.time_section_id)
	{
		return true;
	}
	else if (pre.time_section_id > next.time_section_id)
	{
		return false;
	}
	else if (pre.event_id <= next.event_id)
	{
		return true;
	}
	else
	{
		return false;
    }
}

void TimesectionHandler::multimap_assign()
{
	time_section_map_.clear();
    for (int i = 0; i < time_section_event_list_.size(); i++)
    {
        time_section_map_.insertMulti(time_section_event_list_.at(i).time_section_id, time_section_event_list_.at(i));
    }
}

bool TimesectionHandler::save_data()
{
	db_->set_timesection_table(time_section_map_);
	return true;
}