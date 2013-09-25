#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include <QFile>
#include <QFileDialog>

#include "signaleronlinesettingdlg.h"
#include "macrostring.h"
#include "tscparam.h"
#include "filereaderwriter.h"
#include "synccommand.h"
#include "eventloghandler.h"


SignalerOnlineSettingDlg::SignalerOnlineSettingDlg(QWidget *parent)
    : QDialog(parent)
{
    sync_cmd_ = SyncCommand::GetInstance();
    handler_ = new EventLogHandler;
    time_ip_dlg_ = new TimeIPDlg(this);
    flow_dlg_ = new DetectorFlowDlg(this);
    event_log_dlg_ = new EventLogDlg(this);
    monitor_dlg_ = new RealtimeMonitorDlg(this);

    conn_status_ = false;
    is_ver_correct_ = false;
    ver_check_id_ = 0;
    db_ptr_ = NULL;

    InitPage();
    InitSignalSlots();
}

SignalerOnlineSettingDlg::~SignalerOnlineSettingDlg()
{
}

void SignalerOnlineSettingDlg::Initialize(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    UpdateUI();
    exec();
}

void SignalerOnlineSettingDlg::OnConnectButtonClicked()
{
    if (conn_status_)
    {
        sync_cmd_->disconnectFromHost();
        if (ver_check_id_ != 0)
        {
            killTimer(ver_check_id_);
            ver_check_id_ = 0;
        }
    }
    else
    {
        sync_cmd_->connectToHost(ip_, port_);
    }
}

void SignalerOnlineSettingDlg::OnReadButtonClicked()
{
    config_byte_array_.clear();
    sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadConfig(QByteArray&)));
}

void SignalerOnlineSettingDlg::OnUpdateButtonClicked()
{
    // TODO: send file to signaler
    QString file_name("user/config/" + ip_ + ".dat");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, STRING_TIP, STRING_FILE_OPEN + STRING_FAILED, STRING_OK);
        return;
    }
    QByteArray array = file.readAll();
    file.close();
    SyncCommand::GetInstance()->SendConfigData(array, this, SLOT(OnCmdSendConfig(QByteArray&)));
}

void SignalerOnlineSettingDlg::OnSendButtonClicked()
{
    QString file_name = QFileDialog::getOpenFileName(NULL, STRING_OPEN, "./", "Data(*.dat);;All File(*.*)");
    if (file_name.isNull() || file_name.isEmpty())
    {
        return;
    }
    if (!QFile::exists(file_name))
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_FILE_NOT_EXISTS, STRING_OK);
        return;
    }
    // TODO: send file to signaler
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, STRING_TIP, STRING_FILE_OPEN + STRING_FAILED, STRING_OK);
        return;
    }
    QByteArray array = file.readAll();
    file.close();
    SyncCommand::GetInstance()->SendConfigData(array, this, SLOT(OnCmdSendConfig(QByteArray&)));
}

void SignalerOnlineSettingDlg::OnMonitorButtonClicked()
{
    monitor_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnLogButtonClicked()
{
    event_log_dlg_->Initialize(ip_, handler_);
}

void SignalerOnlineSettingDlg::OnFlowButtonClicked()
{
    flow_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnSettingButtonClicked()
{
    time_ip_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnConnectedSlot()
{
    UpdateConnectStatus(true);
    sync_cmd_->ReadTscVersion(this, SLOT(OnCmdGetVerId(QByteArray&)));
//    if (ver_check_id_ == 0)
//    {
//        ver_check_id_ = startTimer(VERSION_CHECK_TIME);
//    }
}

void SignalerOnlineSettingDlg::OnCmdGetVerId(QByteArray &content)
{
    qDebug() << "on cmd ready read";
    char ver[12] = {'\0'};
    memcpy(ver, content.data(), 11);
    if (strcmp(ver, "CYT0V100END") != 0)
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_VERERROR);
        return;
    }

    // reset timer
    is_ver_correct_ = true;
    killTimer(ver_check_id_);
    ver_check_id_ = 0;
    UpdateButtonStatus(true);
    UpdateConnectStatus(true);
    conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_CONNECT);
}

void SignalerOnlineSettingDlg::OnDisconnectedSlot()
{
    UpdateConnectStatus(false);
}

void SignalerOnlineSettingDlg::OnCmdReadConfig(QByteArray &content)
{
    QString file_name = "user/config/" + ip_ + ".dat";
    if (content.isEmpty())
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_READ_FILE_FAILED);
        config_byte_array_.clear();
        QFile::remove(file_name);
        return;
    }
    config_byte_array_.append(content);
    if (!config_byte_array_.right(3).endsWith("END"))
    {
        return;
    }

    bool status = ParseConfigArray(config_byte_array_);
    if (status)
    {
        TSCParam tscparam;
        memcpy(&tscparam, config_byte_array_.data(), config_byte_array_.length());
        FileReaderWriter writer;
        bool status = writer.WriteFile(tscparam, file_name.toStdString().c_str());
        if (!status)
        {
            conn_tip_label_->setText(STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED);
            QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED, STRING_OK);
            QFile::remove(file_name);
        }
    }
    else
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_READ_FILE_SUCCESS);
        UpdateTabPage();
        // TODO: update tab page
    }
    config_byte_array_.clear();
}

void SignalerOnlineSettingDlg::OnCmdSetConfig(QByteArray &content)
{
    if (content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SOCKET_ERROR, STRING_OK);
        return;
    }
    if (content.length() != 8)
    {
        return;
    }
    if (content == "CONFIGYS")
    {
        QString file_name("user/config/" + ip_ + ".dat");
        QFile file(file_name);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, STRING_TIP, STRING_FILE_OPEN + STRING_FAILED, STRING_OK);
            return;
        }
        QByteArray array = file.readAll();
        file.close();

        SyncCommand::GetInstance()->SendConfigData(array.data(), this, SLOT(OnCmdSendConfig(QByteArray&)));
        return;
    }
    if (content == "CONFIGNO")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG_BUSY_WAIT, STRING_OK);
        return;
    }
}

void SignalerOnlineSettingDlg::OnCmdSendConfig(QByteArray &content)
{
    if (content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SOCKET_ERROR, STRING_OK);
        return;
    }
    if (content.length() < 8)
    {
        return;
    }
    if (content == "CONFIGOK")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG + STRING_SUCCEEDED, STRING_OK);
        return;
    }
    if (content == "CONFIGER")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG + STRING_FAILED, STRING_OK);
        return;
    }
}

void SignalerOnlineSettingDlg::timerEvent(QTimerEvent *)
{
    if (ver_check_id_ != 0)
    {
        OnConnectedSlot();
    }
}

void SignalerOnlineSettingDlg::InitPage()
{
    InitTabPage();
    conn_button_ = new QPushButton(STRING_UI_SIGNALER_CONNECT);
    read_button_ = new QPushButton(STRING_UI_SIGNALER_READ_FILE);
    update_button_ = new QPushButton(STRING_UI_SIGNALER_UPDATE_FILE);
    send_button_ = new QPushButton(STRING_UI_SIGNALER_SEND_FILE);
    monitor_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR);
    log_button_ = new QPushButton(STRING_UI_SIGNALER_LOG_READ);
    flow_button_ = new QPushButton(STRING_UI_SIGNALER_FLOW_READ);
    setting_button_ = new QPushButton(STRING_UI_SIGNALER_SETTING);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(conn_button_);
    button_hlayout->addWidget(read_button_);
    button_hlayout->addWidget(update_button_);
    button_hlayout->addWidget(send_button_);
    button_hlayout->addWidget(monitor_button_);
    button_hlayout->addWidget(log_button_);
    button_hlayout->addWidget(flow_button_);
    button_hlayout->addWidget(setting_button_);
    button_hlayout->addStretch(1);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(dialog_tab_);
    vlayout->addLayout(button_hlayout);
    conn_tip_label_ = new QLabel;
    vlayout->addWidget(conn_tip_label_);
    setLayout(vlayout);

    setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint);
}

void SignalerOnlineSettingDlg::InitSignalSlots()
{
    connect(conn_button_, SIGNAL(clicked()), this, SLOT(OnConnectButtonClicked()));
    connect(read_button_, SIGNAL(clicked()), this, SLOT(OnReadButtonClicked()));
    connect(update_button_, SIGNAL(clicked()), this, SLOT(OnUpdateButtonClicked()));
    connect(send_button_, SIGNAL(clicked()), this, SLOT(OnSendButtonClicked()));
    connect(monitor_button_, SIGNAL(clicked()), this, SLOT(OnMonitorButtonClicked()));
    connect(log_button_, SIGNAL(clicked()), this, SLOT(OnLogButtonClicked()));
    connect(flow_button_, SIGNAL(clicked()), this, SLOT(OnFlowButtonClicked()));
    connect(setting_button_, SIGNAL(clicked()), this, SLOT(OnSettingButtonClicked()));

    // tab page slots
    connect(this, SIGNAL(updateTabPageSignal(void *)), unitparam_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), schedule_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), timesection_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), timing_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), stage_timing_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), phase_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), phase_err_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), channel_widget_, SLOT(OnInitDatabase(void*)));
    connect(this, SIGNAL(updateTabPageSignal(void *)), detector_widget_, SLOT(OnInitDatabase(void*)));
}

void SignalerOnlineSettingDlg::InitTabPage()
{
    QFont font(STRING_FONT_SONGTI, 11);
    dialog_tab_ = new MTabWidget(this);
    dialog_tab_->setFont(font);
//    dialog_tab_ = new QTabWidget;
//    dialog_tab_->setFont(font);

    unitparam_widget_ = new UnitparamtableWidget(STRING_UI_UNIT_TABLE);
    schedule_widget_ = new ScheduleTableWidget(STRING_UI_SCHEDULE_PLAN);
    timesection_widget_ = new TimesectiontableWidget(STRING_UI_TIME_SECTION);
    timing_widget_ = new TimingplanWidget(STRING_UI_TIMING_PLAN);
    stage_timing_widget_ = new PhasetimingtableWidget(STRING_UI_PHASE_TIMING);
    phase_widget_ = new PhasetableWidget(STRING_UI_PHASE_TABLE);
    phase_err_widget_ = new PhaseconflicttableWidget(STRING_UI_PHASE_CONFLICT);
    channel_widget_ = new ChanneltableWidget(STRING_UI_CHANNEL);
    detector_widget_ = new DetectortableWidget(STRING_UI_DETECTOR);

    dialog_tab_->addTab(unitparam_widget_, STRING_UI_UNIT_TABLE);
    dialog_tab_->addTab(schedule_widget_, STRING_UI_SCHEDULE_PLAN);
    dialog_tab_->addTab(timesection_widget_, STRING_UI_TIME_SECTION);
    dialog_tab_->addTab(timing_widget_, STRING_UI_TIMING_PLAN);
    dialog_tab_->addTab(stage_timing_widget_, STRING_UI_PHASE_TIMING);
    dialog_tab_->addTab(phase_widget_, STRING_UI_PHASE_TABLE);
    dialog_tab_->addTab(phase_err_widget_, STRING_UI_PHASE_CONFLICT);
    dialog_tab_->addTab(channel_widget_, STRING_UI_CHANNEL);
    dialog_tab_->addTab(detector_widget_, STRING_UI_DETECTOR);
}

void SignalerOnlineSettingDlg::UpdateUI()
{
    setWindowTitle(STRING_UI_SIGNALER_ADVANCED_SETUP);
    UpdateButtonStatus(false);
}

void SignalerOnlineSettingDlg::UpdateConnectStatus(bool status)
{
    conn_status_ = status;
    if (status)
    {
        conn_button_->setText(STRING_UI_SIGNALER_DISCONNECT);
        conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_VERCHECK);
//        UpdateButtonStatus(true);
    }
    else
    {
        conn_button_->setText(STRING_UI_SIGNALER_CONNECT);
        conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_DISCONN);
        UpdateButtonStatus(false);
    }
}

void SignalerOnlineSettingDlg::UpdateButtonStatus(bool enable)
{
    dialog_tab_->setEnabled(enable);
    read_button_->setEnabled(enable);
    update_button_->setEnabled(enable);
    send_button_->setEnabled(enable);
    monitor_button_->setEnabled(enable);
    log_button_->setEnabled(enable);
    flow_button_->setEnabled(enable);
    setting_button_->setEnabled(enable);
}

void SignalerOnlineSettingDlg::UpdateTabPage()
{
    QString file_name("user/config/" + ip_ + ".dat");
    db_ptr_ = new MDatabase;
    FileReaderWriter param_reader;
    param_reader.InitDatabase(db_ptr_);
    if (!param_reader.ReadFile(file_name.toStdString().c_str()))
    {
        return;
    }
    emit updateTabPageSignal((void *)db_ptr_);

    unitparam_widget_->OnUpdateDataSlot();
    unitparam_widget_->UpdateUI();

    schedule_widget_->OnUpdateDataSlot();
    schedule_widget_->UpdateTable();

    timesection_widget_->OnUpdateDataSlot();
    timesection_widget_->UpdateTree();

    timing_widget_->OnUpdateDataSlot();
    timing_widget_->UpdateTable();

    stage_timing_widget_->OnUpdateDataSlot();
    stage_timing_widget_->UpdateTree();

    phase_widget_->OnUpdateDataSlot();
    phase_widget_->UpdateTree();

    phase_err_widget_->OnUpdateDataSlot();
    phase_err_widget_->UpdateTable();

    channel_widget_->OnUpdateDataSlot();
    channel_widget_->UpdateTable();

    detector_widget_->OnUpdateDataSlot();
    detector_widget_->UpdateTable();
}

bool SignalerOnlineSettingDlg::ParseConfigArray(QByteArray &byte_array)
{
    char *header = byte_array.left(4).data();
    char tail[4] = {'\0'};
    memcpy(tail, byte_array.right(3).data(), sizeof(byte_array.right(3).data()));
    if ((strncmp(header, "CTY4", sizeof("CTY4")) != 0) || (strncmp(tail, "END", sizeof("END")) != 0))
    {
        return false;
    }
    byte_array.remove(0, 4);    // remove "CTY4"
    unsigned int len = 0;
    memcpy(&len, byte_array.left(4).data(), 4);
    byte_array.remove(0, 4);    // remove content length
    int index = byte_array.indexOf("END");
    byte_array.remove(index, sizeof("END"));
    return true;
}
