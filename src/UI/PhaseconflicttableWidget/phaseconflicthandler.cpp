#include "phaseconflicthandler.h"

PhaseconflictHandler::PhaseconflictHandler()
{
    db_ = MDatabase::GetInstance();
}

PhaseconflictHandler::~PhaseconflictHandler()
{
}

void PhaseconflictHandler::init_database(void *db_ptr)
{
    db_ = static_cast<MDatabase *>(db_ptr);
}

void PhaseconflictHandler::reset_database()
{
    db_ = MDatabase::GetInstance();
}

void PhaseconflictHandler::init()
{
    phase_conflict_list_ = db_->get_phase_conflict_table();
	qSort(phase_conflict_list_.begin(), phase_conflict_list_.end(), phase_conflict_less_than);
}

void PhaseconflictHandler::set_phase_conflict(unsigned char phase_id, unsigned int conflict_ids)
{
    if (phase_id <= 0)
    {
        return;
    }
    int idx = index_of_phase_conflict_list(phase_id);
    if (idx != -1)
    {
        phase_conflict_list_[idx].phase_id = phase_id;
        phase_conflict_list_[idx].conflict_phase_id = conflict_ids;
		return;
    }
    PhaseConflictParam phase_err;
    phase_err.conflict_phase_id = conflict_ids;
    phase_err.phase_id = phase_id;
    phase_conflict_list_.append(phase_err);
}

bool PhaseconflictHandler::get_phase_conflict(unsigned char phase_id, PhaseConflictParam &phase_conflict)
{
    int idx = index_of_phase_conflict_list(phase_id);
    if(idx != -1)
    {
        phase_conflict = phase_conflict_list_[idx];
        return true;
    }
    return false;
}

int PhaseconflictHandler::index_of_phase_conflict_list(unsigned char phase_id)
{
    int index = -1;
    for (int i = 0; i < phase_conflict_list_.size(); i++)
    {
        if (phase_id == phase_conflict_list_.at(i).phase_id)
        {
            index = i;
            break;
        }
    }
    return index;
}

bool PhaseconflictHandler::is_phase_exists(unsigned char phase_id)
{
    return (index_of_phase_conflict_list(phase_id) != -1);
}

QList<PhaseConflictParam> &PhaseconflictHandler::get_phase_conflict_list()
{
    return phase_conflict_list_;
}

bool PhaseconflictHandler::save_data()
{
    std::list<PhaseConflictParam> std_list = phase_conflict_list_.toStdList();
    std_list.sort();
    phase_conflict_list_.clear();
    phase_conflict_list_ = QList<PhaseConflictParam>::fromStdList(std_list);
    db_->set_phase_conflict_table(phase_conflict_list_);
    return true;
}

bool PhaseconflictHandler::phase_conflict_less_than( const PhaseConflictParam &left, const PhaseConflictParam &right )
{
	if (left.phase_id > right.phase_id)
	{
		return false;
	}
	else if (left.conflict_phase_id > right.conflict_phase_id)
	{
		return false;
	}
	else
	{
		return true;
	}
}
