#ifndef PHASECONFLICTHANDLER_H
#define PHASECONFLICTHANDLER_H

#include "mdatabase.h"
#include <QList>

class PhaseconflictHandler
{
public:
    PhaseconflictHandler();
    ~PhaseconflictHandler();

    void init();

    void set_phase_conflict(unsigned char phase_id, unsigned int conflict_ids);
    bool get_phase_conflict(unsigned char phase_id, PhaseConflictParam &phase_conflict);
    int index_of_phase_conflict_list(unsigned char phase_id);
    bool is_phase_exists(unsigned char phase_id);
    QList<PhaseConflictParam>& get_phase_conflict_list();

    bool save_data();

private:
	static bool phase_conflict_less_than(const PhaseConflictParam &left, const PhaseConflictParam &right);

private:
    MDatabase* db_;
    QList<PhaseConflictParam> phase_conflict_list_;
};

#endif // PHASECONFLICTHANDLER_H
