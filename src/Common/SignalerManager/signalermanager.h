#ifndef SIGNALERMANAGER_H
#define SIGNALERMANAGER_H

#include <QMap>
//#include "SignalerManager_global.h"
#include "xmlhelper.h"
#include "signalerelement.h"
#include "signalerparam.h"


class SignalerManager
{
public:
    static SignalerManager *GetInstance()
    {
        if (instance_ == NULL)
        {
            instance_ = new SignalerManager;
        }
        return instance_;
    }

    SignalerMap &GetSignaler(const QString &file_name);
    bool SetSignaler(const SignalerMap& signaler_map, const QString &file_name);

    bool createXmlTest();

private:
    SignalerManager();
    ~SignalerManager();

    static SignalerManager *instance_;

private:
    QString file_name_;
    XmlHelper *xmlhelper_;
    QMap<int, SignalerParam> signaler_map_;
};

#endif // SIGNALERMANAGER_H
