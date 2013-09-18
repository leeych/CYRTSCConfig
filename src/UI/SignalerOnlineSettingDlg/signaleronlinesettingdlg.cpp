#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include <QFile>
#include <QFileDialog>

#include "signaleronlinesettingdlg.h"
#include "macrostring.h"
#include "tscparam.h"
#include "filereaderwriter.h"


SignalerOnlineSettingDlg::SignalerOnlineSettingDlg(QWidget *parent)
    : QDialog(parent)
{
    conn_status_ = false;
    sync_cmd_ = SyncCommand::GetInstance();
    time_ip_dlg_ = new TimeIPDlg(this);
    flow_dlg_ = new DetectorFlowDlg(this);
    event_log_dlg_ = new EventLogDlg(this);
    monitor_dlg_ = new RealtimeMonitorDlg(this);

    is_ver_correct_ = false;
    ver_check_id_ = 0;

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
    sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadConfig(void *)));
}

void SignalerOnlineSettingDlg::OnUpdateButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Update", STRING_OK);
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
}

void SignalerOnlineSettingDlg::OnMonitorButtonClicked()
{
    monitor_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnLogButtonClicked()
{
    event_log_dlg_->Initialize();
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
    sync_cmd_->ReadTscVersion(this, SLOT(OnCmdGetVerId(void *)));
//    if (ver_check_id_ == 0)
//    {
//        ver_check_id_ = startTimer(VERSION_CHECK_TIME);
//    }
}

void SignalerOnlineSettingDlg::OnCmdGetVerId(void *content)
{
    qDebug() << "on cmd ready read";
    char ver[12] = {'\0'};
    memcpy(ver, content, 11);
    if (strcmp(ver, "CYT0V100END") != 0)
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_VERERROR);
        return;
    }
    conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_CONNECT);

    // reset timer
    is_ver_correct_ = true;
    killTimer(ver_check_id_);
    ver_check_id_ = 0;
    UpdateButtonStatus(true);
}

void SignalerOnlineSettingDlg::OnDisconnectedSlot()
{
    UpdateConnectStatus(false);
}

void SignalerOnlineSettingDlg::OnCmdReadConfig(void *content)
{
    if (content == NULL)
    {
        return;
    }
    char head[4] = {'\0'};
    memcpy(head, content, 4);
    unsigned int len = 0;
    memcpy(&len, content + 4, 4);
    TSCParam tscparam;
    memcpy(&tscparam, content + 8, len);
//    writer.SetTSCParam(tscparam);
    QString file_name = "user/tmp/" + ip_ + ".dat";
    FileReaderWriter writer;
    bool status = writer.WriteFile(tscparam, file_name.toStdString().c_str());
    if (!status)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED, STRING_OK);
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
    }
    else
    {
        conn_button_->setText(STRING_UI_SIGNALER_CONNECT);
        conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_DISCONN);
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
