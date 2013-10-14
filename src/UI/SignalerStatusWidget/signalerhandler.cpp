#include "signalerhandler.h"
#include "mutility.h"

SignalerHandler::SignalerHandler()
{
    QString dir;
    MUtility::getMainDir(dir);
    file_name_ = dir + "/user/xml/signaler.xml";
    manager_ = NULL;
}

SignalerHandler::~SignalerHandler()
{
}

void SignalerHandler::init()
{
//    SignalerMap &signaler_map = db_->get_signaler_map();
    manager_ = SignalerManager::GetInstance();
    //manager_->createXmlTest();
    if (manager_ == NULL)
    {
        return;
    }

    signaler_map_ =  manager_->GetSignaler(file_name_);
}

void SignalerHandler::set_signaler(const SignalerParam &signaler)
{
    if (signaler_map_.contains(signaler.signaler_id))
    {
        signaler_map_[signaler.signaler_id] = signaler;
        return;
    }
    signaler_map_.insert(signaler.signaler_id, signaler);
}

bool SignalerHandler::get_signaler(int signaler_id, SignalerParam &signaler)
{
    if (signaler_map_.contains(signaler_id))
    {
        signaler = signaler_map_[signaler_id];
        return true;
    }
    return false;
}

bool SignalerHandler::remove_signaler(int signaler_id)
{
    if (signaler_map_.contains(signaler_id))
    {
        signaler_map_.remove(signaler_id);
        return true;
    }
    return false;
}

bool SignalerHandler::save_data()
{
//    db_->set_signaler(signaler_map_);
    bool status = manager_->SetSignaler(signaler_map_, file_name_);
    return status;
}

QList<SignalerParam> SignalerHandler::get_signaler_list()
{
    QList<SignalerParam> signaler_list = signaler_map_.values();
    qSort(signaler_list.begin(), signaler_list.end(), signaler_less_than);
    return signaler_list;
}

void SignalerHandler::set_signaler_list(const QList<SignalerParam> &signaler_list)
{
    signaler_map_.clear();
    QList<SignalerParam>::const_iterator iter = signaler_list.constBegin();
    while (iter != signaler_list.constEnd())
    {
        signaler_map_.insert((*iter).signaler_id, *iter);
        ++iter;
    }
}

bool SignalerHandler::signaler_less_than(const SignalerParam &left, const SignalerParam &right)
{
    if (left.signaler_id > right.signaler_id)
    {
        return false;
    }
    return true;
}

int SignalerHandler::get_max_signaler_id()
{
	SignalerMap::ConstIterator iter = signaler_map_.constEnd();
	--iter;
	int max_id = iter.key();
    return max_id;
}

bool SignalerHandler::set_signaler_status(int id, SignalerParam::SignalerStatus status)
{
    if (!signaler_map_.contains(id))
    {
        return false;
    }
    signaler_map_[id].signaler_status = status;
    return true;
}
