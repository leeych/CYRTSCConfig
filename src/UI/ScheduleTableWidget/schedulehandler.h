#ifndef SCHEDULEHANDLER_H
#define SCHEDULEHANDLER_H

#include <QList>
#include "mdatabase.h"

class ScheduleHandler
{
public:
    ScheduleHandler();
    ~ScheduleHandler();

    void init();

    unsigned char GetMaxScheduleId();
    bool AddSchedule();

    void set_schedule_count(int row);
    int get_schedule_count();
    void set_schedule(unsigned char sche_id,
                      unsigned short month, unsigned char week, unsigned int day,
                      unsigned char timesectionid);
    void set_schedule(unsigned char sched_id, const ScheduleParam& schedule);
    bool get_schedule(unsigned char sched_id, ScheduleParam& schedule);

    bool add_schedule(const ScheduleParam& schedule);
    bool is_schedule_exists(unsigned char sched_id);
    int index_of_schedule_list(unsigned char sched_id);
	bool remove_schedule(const unsigned char sched_id);
    QList<ScheduleParam>& get_schedule_list();

    bool save_data();

private:

private:
    MDatabase* db_;
    unsigned char schedule_num_;
    QList<ScheduleParam> schedule_list_;
};

#endif // SCHEDULEHANDLER_H
