#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include "configmanager_global.h"
#include "appsettinginfo.h"
#include "logtracerinfo.h"

class /*CONFIGMANAGERSHARED_EXPORT*/ ConfigManager
{
public:
    static ConfigManager *getInstance();
    AppSettingInfo &getAppSettingInfo(const QString &file_name);
    LogTracerInfo &getLogTracerInfo(const QString &file_name);
    LogTracerInfo &getLogTracerInfo();
    void releaseInstance();

    void initAppInfo(const QString &file_name, const QString &app_version);
    bool initLogInfo(const QString &file_name);


private:
    ConfigManager();
    ~ConfigManager();

    bool initLogTracerInfo();
    bool initLogLevelInfo(const QString &node);
    bool initLogModuleInfo();

private:
    static ConfigManager *instance_;
    AppSettingInfo app_info_;
    LogTracerInfo log_info_;
};

#endif // CONFIGMANAGER_H
