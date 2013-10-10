#include "detectorflowhandler.h"

DetectorFlowHandler::DetectorFlowHandler()
{
}

DetectorFlowHandler::~DetectorFlowHandler()
{
}

QList<DetectorFlowInfo> &DetectorFlowHandler::get_detector_flow_list()
{
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

void DetectorFlowHandler::set_detector_flow(const DetectorData_t &item)
{
    DetectorFlowInfo flow_info;
    flow_info.data_id = item.DataId;
    flow_info.detector_id = item.DetectorId;
    flow_info.detector_data = item.DetectorData;
    flow_info.phase_id = item.PhaseId;
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
