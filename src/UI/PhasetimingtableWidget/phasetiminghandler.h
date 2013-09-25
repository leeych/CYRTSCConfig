#ifndef PHASETIMINGHANDLER_H
#define PHASETIMINGHANDLER_H

#include "phasetiming.h"
#include "mdatabase.h"
#include <QList>
#include <QMultiMap>
#include <QString>


class PhasetimingHandler
{
public:
    PhasetimingHandler();
    ~PhasetimingHandler();

    void init_database(void *db_ptr);
    void reset_database();

    void init();

    bool is_phasetiming_exists(unsigned char phase_timing_id, unsigned char stage_id);
    int index_of_phasetiming_list(unsigned char phase_timing_id, unsigned char stage_id);
    bool get_phasetiming(unsigned char phase_timing_id, unsigned char stage_id, PhaseTiming& plan);
	bool set_phasetiming(unsigned char pre_phase_timing_id, const QList<PhaseTiming>& plan_list);
    bool remove_phasetiming(unsigned char phase_timing_id);
    bool remove_phasetiming_stage(unsigned char phase_timing_id, unsigned char stage_id);

    QList<unsigned char> get_phasetiming_id_list();
    QList<unsigned char> get_phasetiming_stage_id_list();
    QList<PhaseTiming> get_phasetiming_list();
    QList<PhaseTiming> get_phasetiming_list(unsigned char phase_timing_id);
	QString get_release_phase_ids_desc(unsigned int phase_ids);
	unsigned char get_start_delay_time_desc(unsigned char spec_func);

    bool save_data();

private:
	unsigned char get_max_map_key();
	void set_phasetiming(unsigned char phase_timing_id, const PhaseTiming& plan);
	static bool stage_less_than(const PhaseTiming& pre, const PhaseTiming& next);
    void multimap_assign();

private:
    MDatabase* db_;
    QList<PhaseTiming> timing_list_;
    QMultiMap<unsigned char, PhaseTiming> timing_multi_map_;
};

#endif // PHASETIMINGHANDLER_H
