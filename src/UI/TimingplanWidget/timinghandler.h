#ifndef TIMINGHANDLER_H
#define TIMINGHANDLER_H

#include <QList>
#include "timingparam.h"
#include "mdatabase.h"

class TimingHandler
{
public:
    TimingHandler();
    ~TimingHandler();

    void init();

    bool is_timing_plan_exists(unsigned char timing_id);
    void set_timing_plan(unsigned char timing_id, const TimingParam& plan);
    bool get_timing_plan(unsigned char timing_id, TimingParam& plan);
    int index_of_timing_plan_list(unsigned char timing_id);
    bool add_timing_plan(const TimingParam& plan);
    bool remove_timing_plan(unsigned char timing_id);

    QList<TimingParam> &get_timing_plan_list();
	QList<unsigned char> get_phasetiming_id_list();

    bool save_data();

private:
    unsigned char get_max_timing_id();

private:
    MDatabase* db_;
    QList<TimingParam> timing_list_;
};

#endif // TIMINGHANDLER_H
