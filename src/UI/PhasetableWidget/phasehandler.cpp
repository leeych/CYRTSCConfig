#include "phasehandler.h"
#include <QDebug>

PhaseHandler::PhaseHandler()
{
    db_ = MDatabase::GetInstance();
}

PhaseHandler::~PhaseHandler()
{
}

void PhaseHandler::init()
{
    phase_list_ = db_->get_phase_table();
}

void PhaseHandler::set_phase(unsigned char phase_id, const PhaseParam &phase)
{
    // modify phase by phase_id
	int idx = index_of_phase_list(phase_id);
    if (idx != -1)
    {
		phase_list_[idx] = phase;
        return;
    }

    // add a new phase
    phase_list_.append(phase);
}

bool PhaseHandler::get_phase(unsigned char phase_id, PhaseParam &phase)
{
    int idx = index_of_phase_list(phase_id);
    if (idx != -1)
    {
		phase = phase_list_.at(idx);
        return true;
    }
    return false;
}

bool PhaseHandler::is_phase_exists(unsigned char phase_id)
{
    return (index_of_phase_list(phase_id) != -1);
}

int PhaseHandler::index_of_phase_list(unsigned char phase_id)
{
    int index = -1;
    for (int i = 0; i < phase_list_.size(); i++)
    {
        if (phase_id == phase_list_.at(i).phase_id)
        {
            index = i;
            qDebug() << "index: " << index;
            return index;
        }
    }
    return index;
}

bool PhaseHandler::add_phase(const PhaseParam &phase)
{
    if (is_phase_exists(phase.phase_id))
    {
        return false;
    }
    phase_list_.append(phase);
    return true;
}

bool PhaseHandler::remove_phase(unsigned char phase_id)
{
    if (int idx = index_of_phase_list(phase_id) != -1)
    {
        phase_list_.removeAt(idx);
        return true;
    }
    return false;
}

QList<PhaseParam *> PhaseHandler::get_phase_ptr_list()
{
    QList<PhaseParam *> ptr_list;
    for (int i = 0; i < phase_list_.size(); i++)
    {
        PhaseParam* phase_ptr = &(phase_list_[i]);
        ptr_list.append(phase_ptr);
    }
    return ptr_list;
}

QList<PhaseParam> &PhaseHandler::get_phase_list()
{
    return phase_list_;
}

unsigned char PhaseHandler::get_max_phase_id()
{
    unsigned char id = 0;
    for (int i = 0; i < phase_list_.size(); i++)
    {
        if (id < phase_list_.at(i).phase_id)
        {
            id = phase_list_.at(i).phase_id;
        }
    }
    return id;
}

void PhaseHandler::dump_list()
{
    qDebug() << "phase_list size:" << phase_list_.size();
    for (int i = 0; i < phase_list_.size(); i++)
    {
        qDebug() << phase_list_.at(i).phase_id << phase_list_.at(i).phase_walk_green
                    << phase_list_.at(i).phase_walk_clear << phase_list_.at(i).phase_min_green
                       << phase_list_.at(i).phase_green_delay << phase_list_.at(i).phase_green_flash
                          << phase_list_.at(i).phase_max_green1 << phase_list_.at(i).phase_max_green2
                          << phase_list_.at(i).phase_fix_green << phase_list_.at(i).phase_type
						  << phase_list_.at(i).phase_reserved;
    }
}

unsigned char PhaseHandler::get_phase_channel_id( unsigned char phase_id )
{
    QList<ChannelParam> channels = db_->get_channel_table();
	unsigned char channel_id = 0;

    for (int i = 0; i < channels.size(); i++)
    {
        if (phase_id == channels.at(i).channel_ctrl_src)
        {
            channel_id = channels.at(i).channel_id;
            break;
        }
    }
	return channel_id;
}

QString PhaseHandler::get_phase_ctrled_channels_desc( unsigned int channel_ids )
{
	QString str;
	for (int i = 1; i <= 32; i++)
	{
		if ((channel_ids & 0x01) == 0x01)
		{
			str += QString::number(i) + "/";
		}
		channel_ids = channel_ids >> 1;
	}
    return str.left(str.size() - 1);
}

bool PhaseHandler::save_data()
{
    std::list<PhaseParam> std_list = phase_list_.toStdList();
    std_list.sort();
    phase_list_.clear();
    phase_list_ = QList<PhaseParam>::fromStdList(std_list);
    db_->set_phase_table(phase_list_);
    return true;
}
