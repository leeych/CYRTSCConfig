#include "configmanager.h"
#include "xmlhelper.h"
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

LogTracerInfo &ConfigManager::getLogTracerInfo(const QString &file_name)
{
    initLogInfo(file_name);
    return log_info_;
}

LogTracerInfo &ConfigManager::getLogTracerInfo()
{
    return log_info_;
}

void ConfigManager::releaseInstance()
{
    instance_->~ConfigManager();
}

void ConfigManager::initAppInfo(const QString &file_name, const QString &app_version)
{
    QSettings settings(file_name, QSettings::IniFormat);
    settings.beginGroup("Version");
    settings.setValue("version", app_version);
    settings.endGroup();
}

bool ConfigManager::initLogInfo(const QString &file_name)
{
    XmlHelper *xmlhelper = XmlHelper::GetInstance();
    if (xmlhelper == NULL || !xmlhelper->LoadXmlFile(file_name))
    {
        return false;
    }
    QString node_tag = "debug";
    QDomElement root = xmlhelper->GetXmlNodeParent(node_tag);
    QDomNodeList nodelist = root.childNodes();
    QString tag;
    for (int i = 0; i < nodelist.size(); i++)
    {
        QDomNode child = nodelist.at(i);
        tag = child.nodeName();
        if (tag == "enable")
        {
            log_info_.set_debug_enable(child.toElement().text().toInt() != 0);
            continue;
        }
        if (tag == "file")
        {
            log_info_.set_debug_file_enable(child.toElement().text().toInt() != 0);
            continue;
        }
        if (tag == "terminal")
        {
            log_info_.set_debug_terminal_enable(child.toElement().text().toInt() != 0);
            continue;
        }
        if (tag == "timestamp")
        {
            log_info_.set_debug_timestamp_enable(child.toElement().text().toInt() != 0);
            continue;
        }
        if (tag == "modulename")
        {
            log_info_.set_debug_module_name_enable(child.toElement().text().toInt() != 0);
            continue;
        }
        if (tag == "level")
        {
            initLogLevelInfo(child.toElement().text());
            continue;
        }
        if (tag == "module")
        {
            QDomNodeList modulelist = child.toElement().childNodes();
            for (int j = 0; j < modulelist.size(); j++)
            {
                QDomNode module_child = modulelist.at(j);
                if (module_child.toElement().text().toInt() != 0)
                {
                    log_info_.addModule(module_child.nodeName());
                }
            }
            continue;
        }
    }
//    xmlhelper->releaseInstance();
    return true;
}

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
    delete instance_;
    instance_ = NULL;
}

bool ConfigManager::initLogTracerInfo()
{
    return false;
}

bool ConfigManager::initLogLevelInfo(const QString &node)
{
    if (node.isEmpty())
    {
        return false;
    }
    QString temp;
    int pos = -1, pos_begin = 0;
    while (true)
    {
        pos = node.indexOf(',', pos_begin);
        if (pos != -1)
        {
            temp = node.mid(pos_begin, pos - pos_begin);
            pos_begin = pos + 1;
            if (temp.toInt() != 0)
            {
                log_info_.addLevel(temp.toInt());
            }
        }
        else
        {
            temp = node.mid(pos_begin);
            if (temp.toInt() != 0)
            {
                log_info_.addLevel(temp.toInt());
            }
            break;
        }
    }
    return true;
}
