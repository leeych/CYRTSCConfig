#ifndef SIGNALERHANDLER_H
#define SIGNALERHANDLER_H

#include "signalerparam.h"
//#include "mdatabase.h"
#include "signalermanager.h"
#include <QList>

class SignalerHandler
{
public:
    SignalerHandler();
    ~SignalerHandler();

    void init();
    void set_signaler(const SignalerParam &signaler);
    bool get_signaler(int signaler_id, SignalerParam &signaler);
    bool remove_signaler(int signaler_id);
    bool save_data();

	int get_max_signaler_id();
    bool set_signaler_status(int id, SignalerParam::SignalerStatus);

    QList<SignalerParam> get_signaler_list();
    void set_signaler_list(const QList<SignalerParam> &signaler_list);

private:
    static bool signaler_less_than(const SignalerParam &left, const SignalerParam &right);

private:
//    MDatabase *db_;
//    XmlHelper *xmlhelper_;
    QString file_name_;
    SignalerManager *manager_;
    SignalerMap signaler_map_;
};

#endif // SIGNALERHANDLER_H
