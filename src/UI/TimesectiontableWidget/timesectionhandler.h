#ifndef TIMESECTIONHANDLER_H
#define TIMESECTIONHANDLER_H

#include <QList>
#include <QMultiMap>
#include <QString>
#include "timesection.h"
#include "mdatabase.h"


class TimesectionHandler
{
public:
    TimesectionHandler();
    ~TimesectionHandler();

    void init();

    bool is_timesection_exists(unsigned char time_section_id, unsigned char event_id);
    bool is_timesection_exists(unsigned char time_section_id);
    int index_of_timesection_list(unsigned char time_section_id, unsigned char event_id);
    bool get_timesection(unsigned char time_section_id, unsigned char event_id, TimeSection& time_section);
    
	bool set_timesection(unsigned char pre_section_id, const QList<TimeSection>& section_list);
    bool remove_timesection(unsigned char time_section_id);
    bool remove_timesection_event(unsigned char time_section_id, unsigned char event_id);

    QList<TimeSection> &get_timesection_list();
    bool is_timesection_has_event(unsigned char time_section_id);
    QList<TimeSection> get_timesection_list(unsigned char timesection_id);
    QList<unsigned char> get_timesection_id_list();
	QList<unsigned char> get_timing_id_list();

	QString get_ctrl_mode_desc(unsigned char ctrl_mode);
    unsigned char get_ctrl_mode_by_desc(const QString& str);

	bool save_data();

private:
	void set_timesection(unsigned char time_section_id, const TimeSection& time_section);
	static bool event_id_less_than(const TimeSection& pre, const TimeSection& next);
    void multimap_assign();

private:
    MDatabase* db_;
    QList<TimeSection> time_section_event_list_;
    QMultiMap<unsigned char, TimeSection> time_section_map_;
};

#endif // TIMESECTIONHANDLER_H
