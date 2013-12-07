#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include "configmanager_global.h"
#include "appsettinginfo.h"

class /*CONFIGMANAGERSHARED_EXPORT*/ ConfigManager
{
public:
    static ConfigManager *getInstance();
    AppSettingInfo &getAppSettingInfo(const QString &file_name);

    void initAppInfo(const QString &file_name, const QString &app_version);

private:
    ConfigManager();
    ~ConfigManager();

private:
    static ConfigManager *instance_;
    AppSettingInfo app_info_;
};

#endif // CONFIGMANAGER_H
