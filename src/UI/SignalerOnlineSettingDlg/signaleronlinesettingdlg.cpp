#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "signaleronlinesettingdlg.h"
#include "macrostring.h"


SignalerOnlineSettingDlg::SignalerOnlineSettingDlg(const QString &dialog_name, QWidget *parent)
    : QDialog(parent), dialog_title_(dialog_name)
{
    InitPage();
    InitSignalSlots();
}

SignalerOnlineSettingDlg::~SignalerOnlineSettingDlg()
{
}

void SignalerOnlineSettingDlg::Initialize()
{
    UpdateUI();
    exec();
}

void SignalerOnlineSettingDlg::OnConnectButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Connect", STRING_OK);
}

void SignalerOnlineSettingDlg::OnReadButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Read", STRING_OK);
}

void SignalerOnlineSettingDlg::OnUpdateButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Update", STRING_OK);
}

void SignalerOnlineSettingDlg::OnSendButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Send", STRING_OK);
}

void SignalerOnlineSettingDlg::OnMonitorButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Monitor", STRING_OK);
}

void SignalerOnlineSettingDlg::OnLogButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Log", STRING_OK);
}

void SignalerOnlineSettingDlg::OnFlowButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Flow", STRING_OK);
}

void SignalerOnlineSettingDlg::OnSettingButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Setting", STRING_OK);
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
    dialog_tab_ = new MTabWidget;

    unitparam_widget_ = new unitparam_widget_;
    schedule_widget_ = new ScheduleTableWidget;
    timesection_widget_ = new TimesectiontableWidget;
    timing_widget_ = new TimingplanWidget;
    stage_timing_widget_ = new PhasetimingtableWidget;
    phase_widget_ = new PhasetableWidget;
    phase_err_widget_ = new PhaseconflicttableWidget;
    channel_widget_ = new ChanneltableWidget;
    detector_widget_ = new DetectortableWidget;

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
    setWindowTitle(dialog_title_);
}
