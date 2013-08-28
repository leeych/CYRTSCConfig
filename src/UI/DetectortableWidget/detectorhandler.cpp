#include "detectorhandler.h"
#include "macrostring.h"

DetectorHandler::DetectorHandler()
{
    db_ = MDatabase::GetInstance();

    //detector_type_map_.insert(0x080, STRING_UI_DETECTOR_REQUEST);
    //detector_type_map_.insert(0x040, STRING_UI_DETECTOR_INDUCTION);
    //detector_type_map_.insert(0x020, STRING_UI_DETECTOR_TACTICS);
    //detector_type_map_.insert(0x010, STRING_UI_DETECTOR_STRATEGY);
    //detector_type_map_.insert(0x008, STRING_UI_DETECTOR_MAN);
    //detector_type_map_.insert(0x004, STRING_UI_DETECTOR_BUS);
    //detector_type_map_.insert(0x002, STRING_UI_DETECTOR_BIKE);
    //detector_type_map_.insert(0x001, STRING_UI_DETECTOR_MOTOR);

    //detector_direction_map_.insert(0x080, STRING_UI_CHANNEL_NORTH_WEST);
    //detector_direction_map_.insert(0x040, STRING_UI_CHANNEL_WEST);
    //detector_direction_map_.insert(0x020, STRING_UI_CHANNEL_SOUTH_WEST);
    //detector_direction_map_.insert(0x010, STRING_UI_CHANNEL_SOUTH);
    //detector_direction_map_.insert(0x008, STRING_UI_CHANNEL_SOUTH_EAST);
    //detector_direction_map_.insert(0x004, STRING_UI_CHANNEL_EAST);
    //detector_direction_map_.insert(0x002, STRING_UI_CHANNEL_NORTH_EAST);
    //detector_direction_map_.insert(0x001, STRING_UI_CHANNEL_NORTH);
}

DetectorHandler::~DetectorHandler()
{
}

void DetectorHandler::init()
{
    detector_list_ = db_->get_detector_table();
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
		detector_list_[idx] = detector;
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
