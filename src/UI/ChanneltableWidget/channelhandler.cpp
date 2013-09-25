#include "channelhandler.h"
#include "macrostring.h"

#include <list>
#include <algorithm>

ChannelHandler::ChannelHandler()
{
    db_ = MDatabase::GetInstance();
}

ChannelHandler::~ChannelHandler()
{
}

void ChannelHandler::init_database(void *db_ptr)
{
    db_ = static_cast<MDatabase *>(db_ptr);
}

void ChannelHandler::reset_database()
{
    db_ = MDatabase::GetInstance();
}

void ChannelHandler::init()
{
    channel_list_ = db_->get_channel_table();
    qSort(channel_list_.begin(), channel_list_.end(), channel_less_than);
}

unsigned char ChannelHandler::get_current_channel_id()
{
    return current_channel_id_;
}

bool ChannelHandler::get_channel(unsigned char channel_id, ChannelParam& channel)
{
    int idx = index_of_channel_list(channel_id);
    if (idx != -1)
    {
		channel = channel_list_.at(idx);
        return true;
    }
    return false;
}

void ChannelHandler::set_channel(unsigned char channel_id, const ChannelParam &channel)
{
	int idx = index_of_channel_list(channel_id);
	if (idx != -1)
	{
		channel_list_[idx] = channel;
		return;
	}
	// add a new record
    channel_list_.append(channel);
}

bool ChannelHandler::is_channel_exists(unsigned char channel_id)
{
    return (index_of_channel_list(channel_id) != -1);
}

int ChannelHandler::index_of_channel_list(unsigned char channel_id)
{
    int index = -1;
    for (int i = 0 ; i < channel_list_.size(); i++)
    {
        if (channel_id == channel_list_.at(i).channel_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

bool ChannelHandler::remove_channel(unsigned char channel_id)
{
    int idx = index_of_channel_list(channel_id);
    if (idx != -1)
    {
        channel_list_.removeAt(idx);
        return true;
    }
    return false;
}

QString ChannelHandler::get_channel_type_desc(unsigned char type)
{
    QString str;
    switch(type)
    {
    case 1:
        str = STRING_UI_CHANNEL_OTHER;
        break;
    case 2:
        str = STRING_UI_CHANNEL_MOTOR;
        break;
    case 3:
        str = STRING_UI_CHANNEL_MAN;
        break;
    case 4:
        str = STRING_UI_CHANNEL_F_PHASE;
        break;
    default:
        break;
    }
    return str;
}

QList<ChannelParam> &ChannelHandler::get_channel_list()
{
    return channel_list_;
}

bool ChannelHandler::save_data()
{
	qSort(channel_list_.begin(), channel_list_.end(), channel_less_than);
    std::list<ChannelParam> std_channel_list = channel_list_.toStdList();
    channel_list_.clear();
    channel_list_ = QList<ChannelParam>::fromStdList(std_channel_list);
    db_->set_channel_table(channel_list_);
    return true;
}

unsigned char ChannelHandler::get_max_channel_id()
{
    unsigned char id = 0;
    for (int i = 0; i < channel_list_.size(); i++)
    {
        if (id < channel_list_.at(i).channel_id)
        {
            id = channel_list_.at(i).channel_id;
        }
    }
    return id;
}

bool ChannelHandler::channel_less_than( const ChannelParam &left, const ChannelParam &right )
{
	if (left.channel_id > right.channel_id)
	{
		return false;
	}
	else if (left.channel_ctrl_src > right.channel_ctrl_src)
	{
		return false;
	}
	else
	{
		return true;
	}
}
