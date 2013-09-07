#include "signalerhandler.h"

SignalerHandler::SignalerHandler()
{
    db_ = MDatabase::GetInstance();
}

SignalerHandler::~SignalerHandler()
{
}

void SignalerHandler::init()
{
    SignalerMap &signaler_map = db_->get_signaler_map();
    SignalerMap::iterator iter = signaler_map.begin();
    while (iter != signaler_map.end())
    {
        signaler_map_.insert(iter.key(), iter.value());
        ++iter;
    }
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
    db_->set_signaler(signaler_map_);
    return true;
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
    QList<SignalerParam>::iterator iter = signaler_list.begin();
    while (iter != signaler_list.end())
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
