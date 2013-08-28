#ifndef DETECTORHANDLER_H
#define DETECTORHANDLER_H

#include <QList>
#include <QMap>
#include "detectorparam.h"
#include "mdatabase.h"


class DetectorHandler
{
public:
    DetectorHandler();
    ~DetectorHandler();

    void init();

    bool add_detector(const DetectorParam& detector);
    bool remove_detector(unsigned char detector_id);
    void set_detector(unsigned char detector_id, const DetectorParam& detector);
    bool get_detector(unsigned char detector_id, DetectorParam& detector);
    bool is_detector_exists(unsigned char detector_id);
    int index_of_detector_list(unsigned char detector_id);

    QList<DetectorParam>& get_detector_list();
    QString get_detector_type_desc(unsigned char type);
    QString get_detector_direction_desc(unsigned char direction);

    bool save_data();

private:
    MDatabase* db_;
    QList<DetectorParam> detector_list_;
    QMap<int, QString> detector_type_map_;
    QMap<int, QString> detector_direction_map_;
};

#endif // DETECTORHANDLER_H
