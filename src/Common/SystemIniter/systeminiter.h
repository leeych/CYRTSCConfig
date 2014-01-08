#ifndef SYSTEMINITER_H
#define SYSTEMINITER_H

#include "SystemIniter_global.h"

class SYSTEMINITERSHARED_EXPORT SystemIniter
{
public:
    SystemIniter();

    enum DirError
    {
        None = 0,
        UserDirErr,
        ConfigDirErr,
        XmlDirErr,
        LogDirErr,
        ReportDirErr,
        MonitorDirErr,
        TempDirErr,

        DataDirErr
    };

    DirError InitUserDir();
    DirError InitDataDir();

    bool initLogParams();
};

#endif // SYSTEMINITER_H
