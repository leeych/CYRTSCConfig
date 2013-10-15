#include "detectorflowhandler.h"

DetectorFlowHandler::DetectorFlowHandler()
{
}

DetectorFlowHandler::~DetectorFlowHandler()
{
}

QList<unsigned char> DetectorFlowHandler::get_detector_id_list()
{
    QList<unsigned char> detector_id_list;
    unsigned char id = 0;
    for (int i = 0; i < detector_list_.size(); i++)
    {
        id = detector_list_.at(i).detector_id;
        if (!detector_id_list.contains(id))
        {
            detector_id_list.append(id);
        }
    }
    qSort(detector_id_list);
    return detector_id_list;
}

QList<DetectorFlowInfo> &DetectorFlowHandler::get_detector_flow_list()
{
    qSort(detector_list_.begin(), detector_list_.end(), less_than);
    return detector_list_;
}

QList<DetectorFlowInfo> DetectorFlowHandler::get_detector_flow(unsigned char detector_id)
{
    QList<DetectorFlowInfo> flow_list;
    QList<DetectorFlowInfo>::ConstIterator iter = detector_list_.constBegin();
    while (iter != detector_list_.constEnd())
    {
        if ((*iter).detector_id == detector_id)
        {
            flow_list.append(*iter);
        }
        ++iter;
    }
    return flow_list;
}

QList<DetectorFlowInfo> DetectorFlowHandler::get_detector_flow(unsigned char detector_id,
                                                               unsigned int start_secs, unsigned int end_secs)
{
    QList<DetectorFlowInfo> bak_list, flow_list;
    if (detector_id == 0)
    {
        flow_list = get_detector_flow_list();
    }
    else
    {
        flow_list = get_detector_flow(detector_id);
    }

    unsigned int secs = 0;
    for (int i = 0; i < flow_list.size(); i++)
    {
        secs = flow_list.at(i).recv_time;
        if (secs >= start_secs && secs <= end_secs)
        {
            bak_list.append(flow_list.at(i));
        }
    }
    return bak_list;
}

int DetectorFlowHandler::get_detector_flow_num(unsigned char detector_id)
{
    int flow_count = 0;
    QList<DetectorFlowInfo> flow_list = get_detector_flow(detector_id);
    for (int i = 0; i < flow_list.size(); i++)
    {
        if (flow_list.at(i).detector_data != 0)
        {
            flow_count += flow_list.at(i).detector_data;
        }
    }
    return flow_count;
}

void DetectorFlowHandler::set_detector_flow(const DetectorData_t &item)
{
    DetectorFlowInfo flow_info;
    flow_info.data_id = item.DataId;
    flow_info.detector_id = item.DetectorId;
    flow_info.detector_data = item.DetectorData;
    flow_info.phase_ids = item.PhaseId;
    flow_info.recv_time = item.RecvTime;

    int idx = index_of_detector_flow_list(item.DetectorId, item.DataId);
    if (idx != -1)
    {
        detector_list_[idx] = flow_info;
        return;
    }
    detector_list_.append(flow_info);
}

bool DetectorFlowHandler::remove_detector_flow(unsigned char detector_id, unsigned short data_id)
{
    int idx = index_of_detector_flow_list(detector_id, data_id);
    if (idx != -1)
    {
        detector_list_.removeAt(idx);
        return true;
    }
    return false;
}

void DetectorFlowHandler::clear_detector_flow()
{
    detector_list_.clear();
}

int DetectorFlowHandler::index_of_detector_flow_list(unsigned char detector_id, unsigned short data_id)
{
    int idx = -1;
    for (int i = 0; i < detector_list_.size(); i++)
    {
        if (detector_list_.at(i).detector_id == detector_id
                && detector_list_.at(i).data_id == data_id)
        {
            idx = i;
            return idx;
        }
    }
    return idx;
}

bool DetectorFlowHandler::less_than(const DetectorFlowInfo &lhs, const DetectorFlowInfo &rhs)
{
    if (lhs.detector_id < rhs.detector_id)
    {
        return true;
    }
    else if (lhs.data_id < rhs.data_id)
    {
        return true;
    }
    return false;
}
