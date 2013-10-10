#ifndef DETECTORFLOWHANDLER_H
#define DETECTORFLOWHANDLER_H

#include "tsc.h"
#include <QList>

class DetectorFlowInfo
{
public:
    DetectorFlowInfo() :
        data_id(0), detector_id(0),
        detector_data(0), phase_ids(0), recv_time(0)
    { }
    ~DetectorFlowInfo() { }

    DetectorFlowInfo(const DetectorFlowInfo &rhs)
    {
        this->data_id = rhs.data_id;
        this->detector_id = rhs.detector_id;
        this->detector_data = rhs.detector_data;
        this->phase_ids = rhs.phase_ids;
        this->recv_time = rhs.recv_time;
    }

    DetectorFlowInfo& operator =(const DetectorFlowInfo &rhs)
    {
        this->data_id = rhs.data_id;
        this->detector_id = rhs.detector_id;
        this->detector_data = rhs.detector_data;
        this->phase_ids = rhs.phase_ids;
        this->recv_time = rhs.recv_time;
        return *this;
    }

public:
    unsigned short data_id;
    unsigned char detector_id;
    unsigned char detector_data;
    unsigned int phase_ids;
    unsigned int recv_time;
};


class DetectorFlowHandler
{
public:
    DetectorFlowHandler();
    ~DetectorFlowHandler();

    QList<unsigned char> get_detector_id_list();
    QList<DetectorFlowInfo> &get_detector_flow_list();
    QList<DetectorFlowInfo> get_detector_flow(unsigned char detector_id);
    int get_detector_flow_num(unsigned char detector_id);
    void set_detector_flow(const DetectorData_t &item);
    bool remove_detector_flow(unsigned char detector_id, unsigned short data_id);

    int index_of_detector_flow_list(unsigned char detector_id, unsigned short data_id);

private:
    static bool less_than(const DetectorFlowInfo &lhs, const DetectorFlowInfo &rhs);

private:
    DetectorData_t *data_info_;
    QList<DetectorFlowInfo> detector_list_;
};

#endif // DETECTORFLOWHANDLER_H
