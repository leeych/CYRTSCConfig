#ifndef CHANNELHANDLER_H
#define CHANNELHANDLER_H

#include "channelparam.h"
#include "mdatabase.h"
#include <QList>


class ChannelHandler
{
public:
    ChannelHandler();
    ~ChannelHandler();

    void init_database(void *db_ptr);
    void reset_database();

    void init();
    unsigned char get_current_channel_id();
    bool get_channel(unsigned char channel_id, ChannelParam&);
    void set_channel(unsigned char channel_id, const ChannelParam& channel);

    bool is_channel_exists(unsigned char channel_id);
    int index_of_channel_list(unsigned char channel_id);
    bool remove_channel(unsigned char channel_id);

    QString get_channel_type_desc(unsigned char type);
    QList<ChannelParam>& get_channel_list();

    bool save_data();

private:
    unsigned char get_max_channel_id();
	static bool channel_less_than(const ChannelParam &left, const ChannelParam &right);

private:
    unsigned char current_channel_id_;
    MDatabase* db_;
    QList<ChannelParam> channel_list_;
};

#endif // CHANNELHANDLER_H
