#include "systeminiter.h"
#include "mutility.h"

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
