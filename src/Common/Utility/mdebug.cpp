#include "mdebug.h"
#include <QDir>
#include "mutility.h"

const char* module_g[] =
{
    "Main",
    "Communication",
    "RealTimeMonitorDlg",
    "SignalerOnlineSettingDlg",
    "ConfigManager",
    "SignalerManager",
    "SystemIniter",
    "Utility",
    "XmlHelper",
    "DataCenter",
    "ChanneltableWidget",
    "ConfigoptWidget",
    "DetectortableWidget",
    "FileListWidget",
    "PhaseconflicttableWidget",
    "PhasetableWidget",
    "PhasetimingtableWidget",
    "ScheduleTableWidget",
    "SignalerOnlineSettingDlg",
    "DetectorflowDlg",
    "EventlogDlg",
    "RealTimeMonitorDlg",
    "TimeIPDlg",
    "SignalerStatusWidget",
    "TimesectiontableWidget",
    "TimingplanWidget",
    "TscAboutDlg",
    "TSCLogin",
    "UIBase",
    "UnitparamtableWidget"
};

DLevel level_g[] =
{
    DLevel_Invalid,
    DLevel1,
    DLevel2,
    DLevel3,
    DLevel4,
    DLevel5,
    DLevel6,
    DLevel7,
    DLevel8,
    DLevel9,
    DLevel10,
    DLevel11,
    DLevel12,
    DLevel13,
    DLevel14,
    DLevel15,
    DLevel16
};

MDebugConfig* MDebugConfig::instance_ = NULL;

MDebugConfig::MDebugConfig()
{
    level_config_ = 0;
    type_config_ = 0;
}

MDebugConfig *MDebugConfig::GetInstance()
{
    if(instance_ == NULL)
    {
        instance_ = new MDebugConfig;
    }

    return instance_;
}

void MDebugConfig::DestroyInstance()
{
    if(instance_ != NULL)
    {
        delete instance_;
        instance_ = NULL;
    }
}

bool MDebugConfig::Init(const QString &filepath)
{
    QString filename;
    if(filepath.isEmpty())
    {
        QString str_app_path;
        MUtility::getMainDir(str_app_path);
        filename = str_app_path;
    }
    else
    {
        filename = filepath;
    }

    filename += "/debuglog/";

    QDir dir;
    if(!dir.mkpath(filename))
    {
        return false;
    }

    filename += QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
    filename += ".dbg";
    file_.setFileName(filename);

    return true;
}

void MDebugConfig::DeInit()
{
    if(file_.isOpen())
    {
        file_.close();
    }
}

void MDebugConfig::AddLevel(int level)
{
    if(level < 1 || level > 16)
    {
        return;
    }
    else
    {
        level_config_ |= level_g[level];
    }
}

void MDebugConfig::AddModel(const char *model_name)
{
    if(model_name != NULL && model_name[0] != '\0')
    {
        for(int i = 0; i < DModule_Count;i++)
        {
            if(strcmp(module_g[i],model_name) == 0)
            {
                module_list_.push_back((DModule)i);
            }
        }
    }
}

void MDebugConfig::AddType(DType type)
{
    if(type <= DType_Invalid || type >= DType_Count)
    {
        return;
    }
    else
    {
        if(type == DType_File)
        {
            if(file_.open(QIODevice::WriteOnly))
            {
                type_config_ |= type;
            }
        }
        else
        {
            type_config_ |= type;
        }
    }
}

int MDebugConfig::IsOutput(DModule model, DLevel level = DLevel1)
{
    if(!enable_)
    {
        return DType_Invalid;
    }

    if(model <= DModule_Invalid || model >= DModule_Count)
    {
        return DType_Invalid;
    }

    if(level <= DLevel0 || level >= DLevel_Invalid)
    {
        return DType_Invalid;
    }

    if((level & level_config_) == 0)
    {
        return DType_Invalid;
    }

    return type_config_;
}

QFile *MDebugConfig::GetFile()
{
    return &file_;
}

MDebug::MDebug(DModule model, DLevel level)
    :QDebug(MDebugConfig::GetInstance()->GetFile()),
    debug_(NULL)
{
    MDebugConfig* config = MDebugConfig::GetInstance();

    QList<DModule> mlist = config->module_list();
    QList<DModule>::iterator itr = qFind(mlist.begin(),mlist.end(),model);
    if(itr == mlist.end())
    {
        output_ = DType_Invalid;
    }
    else
    {
        file_ = config->GetFile();
        output_ = config->IsOutput(model,level);

        if((output_ & DType_Terminal) == DType_Terminal)
        {
            debug_ = new QDebug(QtDebugMsg);
        }

        PrintTitle(model);
    }
}

MDebug::MDebug(DModule model)
    : QDebug(MDebugConfig::GetInstance()->GetFile()),
      debug_(NULL)
{
    MDebugConfig *config = MDebugConfig::GetInstance();
    QList<DModule> mlist = config->module_list();
    QList<DModule>::iterator iter = qFind(mlist.begin(), mlist.end(), model);
    if (iter == mlist.end())
    {
        output_ = DType_Invalid;
    }
    else
    {
        file_ = config->GetFile();
        output_ = config->IsOutput(model,DLevel1);
        if ((output_ & DType_Terminal) == DType_Terminal)
        {
            debug_ = new QDebug(QtDebugMsg);
        }
        PrintTitle(model);
    }
}

MDebug::~MDebug()
{
    if((output_ & DType_File) == DType_File)
    {
        if(file_ != NULL && file_->isOpen() && file_->isWritable())
        {
            (QDebug)*this << endl;
        }
    }

    if(debug_ != NULL)
    {
        delete debug_;
        debug_ = NULL;
    }
}

void MDebug::PrintTitle(DModule model)
{
    MDebugConfig* config = MDebugConfig::GetInstance();
    if((output_ & DType_Terminal) == DType_Terminal)
    {
        if(config->timestamp_enable())
        {
            *debug_ << QTime::currentTime().toString("[hh:mm:ss.zzz]").toLatin1().data();
        }

        if(config->module_enable())
        {
            QList<DModule> mlist = config->module_list();
            QList<DModule>::iterator itr = qFind(mlist.begin(),mlist.end(),model);
            if(itr != mlist.end())
            {
                *debug_ << '[' << module_g[*itr] << ']';
            }
        }
    }

    if((output_ & DType_File) == DType_File)
    {
        if(file_ != NULL && file_->isOpen() && file_->isWritable())
        {
            if(config->timestamp_enable())
            {
                (QDebug)*this << QTime::currentTime().toString("[hh:mm:ss.zzz]").toLatin1().data();
            }

            if(config->module_enable())
            {
                QList<DModule> mlist = config->module_list();
                QList<DModule>::iterator itr = qFind(mlist.begin(),mlist.end(),model);
                if(itr != mlist.end())
                {
                    (QDebug)*this << '[' << module_g[*itr] << ']';
                }
            }
        }
    }
}
