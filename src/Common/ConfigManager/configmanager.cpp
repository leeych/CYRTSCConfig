#include "configmanager.h"
#include <QSettings>

ConfigManager* ConfigManager::instance_ = NULL;

ConfigManager *ConfigManager::getInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new ConfigManager;
    }
    return instance_;
}

AppSettingInfo &ConfigManager::getAppSettingInfo(const QString &file_name)
{
    QSettings settings(file_name, QSettings::IniFormat);
    settings.beginGroup("Version");
    app_info_.soft_version_ = settings.value("version").toString();
    settings.endGroup();
    return app_info_;
}

void ConfigManager::initAppInfo(const QString &file_name, const QString &app_version)
{
    QSettings settings(file_name, QSettings::IniFormat);
    settings.beginGroup("Version");
    settings.setValue("version", app_version);
    settings.endGroup();
}

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{

}
