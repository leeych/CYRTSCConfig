#include "systeminiter.h"
#include "mutility.h"

#define MDEBUG_BRIEF_ENABLE

#include "mdebug.h"
#include "configmanager.h"

#include <QString>
#include <QDir>

SystemIniter::SystemIniter()
{
}

SystemIniter::DirError SystemIniter::InitUserDir()
{
    QString str;
    MUtility::getUserDir(str);
    DirError res = None;
    QDir file_dir;
    bool status = file_dir.exists(str);
    if (!status)
    {
        if (!file_dir.mkdir(str))
        {
            res = UserDirErr;
        }
        if (!file_dir.mkdir(str + "/config/"))
        {
            res = ConfigDirErr;
        }
        if (!file_dir.mkdir(str + "/xml/"))
        {
            res = XmlDirErr;
        }
        if (!file_dir.mkdir(str + "/log/"))
        {
            res = LogDirErr;
        }
        if (!file_dir.mkdir(str + "/report/"))
        {
            res = ReportDirErr;
        }
        if (!file_dir.mkdir(str + "/monitor/"))
        {
            res = MonitorDirErr;
        }
        if (!file_dir.mkdir(str + "/temp/"))
        {
            res = TempDirErr;
        }
    }
    return res;
}

SystemIniter::DirError SystemIniter::InitDataDir()
{
    QString dir;
    MUtility::getDataDir(dir);
    DirError res = None;
    QDir file_dir;
    bool status = file_dir.exists(dir);
    if (!status)
    {
        if (!file_dir.mkdir(dir))
        {
            res = DataDirErr;
        }
    }
    return res;
}

bool SystemIniter::initLogParams()
{
    ConfigManager *manager = ConfigManager::getInstance();
    LogTracerInfo &loginfo = manager->getLogTracerInfo("logconfig.xml");
    MDebugConfig *config = MDebugConfig::GetInstance();
    config->Init("");
    if (!loginfo.get_debug_enable())
    {
        return false;
    }
    config->set_enable(loginfo.get_debug_enable());
    if (loginfo.get_debug_file_enable())
    {
        config->AddType(DType_File);
    }
    if (loginfo.get_debug_terminal_enable())
    {
        config->AddType(DType_Terminal);
    }

    QList<QString> module_list = loginfo.get_module_name_list();
    for (int i = 0; i < module_list.size(); i++)
    {
        config->AddModel(module_list.at(i).toLatin1().data());
    }

    QList<int> level_list = loginfo.get_level_list();
    for (int i = 0; i < level_list.size(); i++)
    {
        config->AddLevel(level_list.at(i));
    }
    BRIEF_DEBUG(DModule_Main) << "debug start";

    return true;
}
