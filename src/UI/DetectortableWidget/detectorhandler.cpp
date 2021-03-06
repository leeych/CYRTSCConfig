#include "detectorhandler.h"
#include "macrostring.h"

DetectorHandler::DetectorHandler()
{
    db_ = MDatabase::GetInstance();
}

DetectorHandler::~DetectorHandler()
{
}

void DetectorHandler::init_database(void *db_ptr)
{
    db_ = static_cast<MDatabase *>(db_ptr);
}

void DetectorHandler::reset_database()
{
    db_ = MDatabase::GetInstance();
}

void DetectorHandler::init()
{
    detector_list_ = db_->get_detector_table();
	qSort(detector_list_.begin(), detector_list_.end(), detector_less_than);
}

bool DetectorHandler::add_detector(const DetectorParam &detector)
{
    if (!is_detector_exists(detector.detector_id))
    {
        detector_list_.append(detector);
        return true;
    }
    return false;
}

bool DetectorHandler::remove_detector(unsigned char detector_id)
{
    int idx = index_of_detector_list(detector_id);
    if (idx != -1)
    {
        detector_list_.removeAt(idx);
        return true;
    }
    return false;
}

void DetectorHandler::set_detector(unsigned char detector_id, const DetectorParam &detector)
{
    int idx = index_of_detector_list(detector_id);
    if (idx != -1)
    {
		detector_list_[idx].detector_id = detector.detector_id;
		detector_list_[idx].detector_phase_ids = detector.detector_phase_ids;
		detector_list_[idx].detector_delay = detector.detector_delay;
		detector_list_[idx].detector_direction = detector.detector_direction;
		detector_list_[idx].detector_effective_time = detector.detector_effective_time;
		detector_list_[idx].detector_failure_time = detector.detector_failure_time;
		detector_list_[idx].detector_type = detector.detector_type;
		detector_list_[idx].detector_flow = detector.detector_flow;
		detector_list_[idx].detector_occupy = detector.detector_occupy;
		detector_list_[idx].detector_spec_func = detector.detector_spec_func;
        return;
    }
    detector_list_.append(detector);
}

bool DetectorHandler::get_detector(unsigned char detector_id, DetectorParam &detector)
{
    int idx = index_of_detector_list(detector_id);
    if (idx != -1)
    {
		detector = detector_list_.at(idx);
        return true;
    }
    return false;
}

bool DetectorHandler::is_detector_exists(unsigned char detector_id)
{
    return (index_of_detector_list(detector_id) != -1);
}

int DetectorHandler::index_of_detector_list(unsigned char detector_id)
{
    int index = -1;
    for (int i = 0; i < detector_list_.size(); i++)
    {
        // Q_UNLIKELY();
        if (detector_id == detector_list_.at(i).detector_id)
        {
            index = i;
            break;
        }
    }
    return index;
}

QList<DetectorParam> &DetectorHandler::get_detector_list()
{
    return detector_list_;
}

QString DetectorHandler::get_detector_type_desc(unsigned char type)
{
    return detector_type_map_[type];
}

QString DetectorHandler::get_detector_direction_desc(unsigned char direction)
{
    return detector_direction_map_[direction];
}

bool DetectorHandler::save_data()
{
    std::list<DetectorParam> std_detector_list = detector_list_.toStdList();
    std_detector_list.sort();
    detector_list_.clear();
    detector_list_ = detector_list_.fromStdList(std_detector_list);
    db_->set_detector_table(detector_list_);

    return true;
}

bool DetectorHandler::detector_less_than( const DetectorParam &left, const DetectorParam &right )
{
	if (left.detector_id > right.detector_id)
	{
		return false;
	}
	else if (left.detector_phase_ids > right.detector_phase_ids)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
