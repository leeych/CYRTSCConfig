#ifndef __MDEBUG_H__
#define __MDEBUG_H__

#include <QDebug>
#include <QFile>
#include <QDateTime>

#define MDEBUG_ENABLE 1

typedef enum DModule_tag
{
    DModule_Invalid = -1,
    DModule_Main = 0,
    DModule_Communication = 1,
    DModule_RealTimeMonitor = 2,
    DModule_Advanced = 3,
    DModule_Utility,
    DModule_Login,
    DModule_UIBase,
    DModule_Count
}DModule;

typedef enum DLevel_tag
{
    DLevel0  = 0x00000000, //invalid level
    DLevel1  = 0x00000001,
    DLevel2  = 0x00000002,
    DLevel3  = 0x00000004,
    DLevel4  = 0x00000008,
    DLevel5  = 0x00000010,
    DLevel6  = 0x00000020,
    DLevel7  = 0x00000040,
    DLevel8  = 0x00000080,
    DLevel9  = 0x00000100,
    DLevel10 = 0x00000200,
    DLevel11 = 0x00000400,
    DLevel12 = 0x00000800,
    DLevel13 = 0x00001000,
    DLevel14 = 0x00002000,
    DLevel15 = 0x00004000,
    DLevel16 = 0x00008000,
    DLevel_Invalid = 0x00010000  //if current max level is 16
}DLevel;

typedef enum DType_tag
{
    DType_Invalid  = 0x0000,
    DType_Terminal = 0x0001,
    DType_File     = 0x0002,
    DType_Count    = 0x1000
}DType;

class MDebugConfig
{
public:
    MDebugConfig();
    static MDebugConfig* GetInstance();
    void DestroyInstance();

    bool Init(const QString& filepath);
    void DeInit();
    void AddLevel(int level);
    void AddModel(const char* model_name);
    void AddType(DType type);

    int IsOutput(DModule model,DLevel level);
    QFile* GetFile();
    bool module_enable(){ return module_enable_; }
    bool timestamp_enable(){ return timestamp_enable_; }
    bool terminal_enable(){ return terminal_enable_; }
    QList<DModule> module_list() { return module_list_; }

    void set_enable(bool enable){ enable_ = enable; }
    void set_file_enable(bool enable) { file_enable_ = enable; }
    void set_module_enable(bool enable){ module_enable_ = enable; }
    void set_timestamp_enable(bool enable){ timestamp_enable_ = enable; }
    void set_terminal_enable(bool enable){ terminal_enable_ = enable; }

private:
    QFile file_;
    int level_config_;
    int type_config_;
    static MDebugConfig* instance_;
    bool enable_;
    bool file_enable_;
    bool module_enable_;
    bool timestamp_enable_;
    bool terminal_enable_;
    QList<DModule> module_list_;
};

class MDebug : public QDebug
{
public:
    MDebug(DModule model, DLevel level);
    MDebug(DModule model);
    ~MDebug();

    template<typename T>
    MDebug& operator<<(const T& value)
    {
        if((output_ & DType_Terminal) == DType_Terminal)
        {
            *debug_ << value;
        }

        if((output_ & DType_File) == DType_File)
        {
            if(file_ != NULL && file_->isOpen() && file_->isWritable())
            {
                (QDebug)*this << value;
            }
        }

        return *this;
    }

private:
    void PrintTitle(DModule model);
private:
    int output_;
    QFile* file_;
    QDebug* debug_;
};

class MNoDebug
{
public:
    MNoDebug(){}
    ~MNoDebug(){}

    template<typename T>
    MNoDebug& operator<<(const T& value)
    {
        return *this;
    }
};

#ifdef MDEBUG_ENABLE
inline MDebug GetMDebug(DModule model, DLevel level){return MDebug(model,level);}
#define MDEBUG(model,level) GetMDebug(model,level)
#else
inline MNoDebug GetMDebug(){return MNoDebug();}
#define MDEBUG(model,level) GetMDebug()
#endif

#ifdef MDEBUG_BRIEF_ENABLE
inline MDebug GetMDebug(DModule model) { return MDebug(model, DLevel1); }
#define BRIEF_DEBUG(model) GetMDebug(model)
#else
inline MNoDebug GetMDebug() { return MNoDebug(); }
#define BRIEF_DEBUG(model) GetMDebug()
#endif

#endif
