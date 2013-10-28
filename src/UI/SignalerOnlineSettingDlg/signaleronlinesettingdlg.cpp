#include "signaleronlinesettingdlg.h"
#include "macrostring.h"
#include "mutility.h"
#include "tscparam.h"
#include "filereaderwriter.h"
#include "synccommand.h"
#include "eventloghandler.h"

#include "mtabwidget.h"
#include "unitparamtablewidget.h"
#include "scheduletablewidget.h"
#include "timesectiontablewidget.h"
#include "timingplanwidget.h"
#include "phasetimingtablewidget.h"
#include "phasetablewidget.h"
#include "phaseconflicttablewidget.h"
#include "channeltablewidget.h"
#include "detectortablewidget.h"

#include "timeipdlg.h"
#include "detectorflowdlg.h"
#include "eventlogdlg.h"
#include "realtimemonitordlg.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>

#define CMD_WAIT_TIME   31000


SignalerOnlineSettingDlg::SignalerOnlineSettingDlg(QWidget *parent)
    : QDialog(parent)
{
    sync_cmd_ = SyncCommand::GetInstance();
    handler_ = new EventLogHandler;
    time_ip_dlg_ = new TimeIPDlg(this);
    flow_dlg_ = new DetectorFlowDlg(this);
    event_log_dlg_ = new EventLogDlg(this);
    monitor_dlg_ = new RealtimeMonitorDlg(this);

    cmd_timer_ = new QTimer(this);
    conn_timer_ = new QTimer(this);
    curr_cmd_ = NoneCmd;
    is_edit_ip_ = false;

    conn_status_ = false;
    is_cfg_read_ = false;
    is_ver_correct_ = false;
//    cmd_timer_id_ = 0;
    ver_check_id_ = 0;
    ui_lock_id_ = 0;
    db_ptr_ = new MDatabase;
    desktop_ = QApplication::desktop();

    InitPage();
    InitSignalSlots();
}

SignalerOnlineSettingDlg::~SignalerOnlineSettingDlg()
{
    if (handler_ != NULL)
    {
        delete handler_;
        handler_ = NULL;
    }
    if (db_ptr_ != NULL)
    {
        delete db_ptr_;
        db_ptr_ = NULL;
    }
    if (sync_cmd_ != NULL)
    {
        sync_cmd_->DistroyInstance();
    }
    if (cmd_timer_ != NULL)
    {
        delete cmd_timer_;
        cmd_timer_ = NULL;
    }
}

void SignalerOnlineSettingDlg::Initialize(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    MUtility::getConfigDir(cfg_file_);
    cfg_file_ += ip_ + ".dat";
    UpdateUI();
    exec();
}

void SignalerOnlineSettingDlg::OnConnectButtonClicked()
{
    EnableNetworkErrorTip(true);
    conn_tip_label_->clear();
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
//        conn_button_->setEnabled(false);
//        sync_cmd_->syncConnectToHost(ip_, port_);

        EnableNetworkErrorTip(false);
        sync_cmd_->connectToHost(ip_, port_);
        conn_timer_->start(SOCKET_WAIT_MS);
        conn_tip_label_->setText(STRING_UI_SIGNALER_CONNECTING_TIP);
        conn_button_->setEnabled(false);
    }
}

void SignalerOnlineSettingDlg::OnReadButtonClicked()
{
    config_byte_array_.clear();
    conn_tip_label_->setText(STRING_UI_SIGNALER_READING_CONFIG);
    sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadConfig(QByteArray&)));
    UpdateButtonStatus(false);
    curr_cmd_ = ReadConfig;
    cmd_timer_->start(CMD_WAIT_TIME);
}

void SignalerOnlineSettingDlg::OnUpdateButtonClicked()
{
    if (!SaveTempConfigFile())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED, STRING_OK);
        return;
    }
    conn_tip_label_->clear();
    conn_tip_label_->setText(STRING_REQUEST_SEND_CFG);
    tmp_file_ = cfg_file_;
    SyncCommand::GetInstance()->SetConfiguration(this, SLOT(OnCmdSetConfig(QByteArray&)));
    UpdateButtonStatus(false);
    ui_lock_id_ = startTimer(READ_WAIT_TIME);
}

void SignalerOnlineSettingDlg::OnSendButtonClicked()
{
    conn_tip_label_->clear();
    QString file_name = QFileDialog::getOpenFileName(this, STRING_OPEN, "./", "Data(*.dat);;All File(*.*)");
    if (file_name.isNull() || file_name.isEmpty())
    {
        return;
    }
    if (!QFile::exists(file_name))
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_FILE_NOT_EXISTS, STRING_OK);
        return;
    }
    conn_tip_label_->setText(STRING_SOCKET_SEND_CONFIG);
    tmp_file_ = file_name;
//    OnUpdateButtonClicked();
    conn_tip_label_->setText(STRING_REQUEST_SEND_CFG);
    SyncCommand::GetInstance()->SetConfiguration(this, SLOT(OnCmdSetConfig(QByteArray&)));
    UpdateButtonStatus(false);
    ui_lock_id_ = startTimer(READ_WAIT_TIME);
}

void SignalerOnlineSettingDlg::OnMonitorButtonClicked()
{
    conn_tip_label_->clear();
    monitor_dlg_->move((desktop_->width()-monitor_dlg_->size().width())/2, (desktop_->height()-monitor_dlg_->size().height())/2);
    monitor_dlg_->Initialize(ip_);
}

void SignalerOnlineSettingDlg::OnLogButtonClicked()
{
    conn_tip_label_->clear();
    event_log_dlg_->resize(700, 450);
    event_log_dlg_->move((desktop_->width()-event_log_dlg_->size().width())/2, (desktop_->height()-event_log_dlg_->size().height())/2);
    event_log_dlg_->Initialize(ip_, handler_);
}

void SignalerOnlineSettingDlg::OnFlowButtonClicked()
{
    conn_tip_label_->clear();
    flow_dlg_->resize(640, 420);
    flow_dlg_->move((desktop_->width()-flow_dlg_->width())/2, (desktop_->height()-flow_dlg_->height())/2);
    flow_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnSettingButtonClicked()
{
    conn_tip_label_->clear();
    time_ip_dlg_->resize(420, 260);
    time_ip_dlg_->move((desktop_->width()-time_ip_dlg_->width())/2, (desktop_->height()-time_ip_dlg_->height())/2);
    time_ip_dlg_->Initialize();
}

void SignalerOnlineSettingDlg::OnSaveAsbuttonClicked()
{
    if (!SaveTabPageData())
    {
        return;
    }
    QString file_name = QFileDialog::getSaveFileName(this, STRING_UI_SAVEAS, "data/cy_tsc.dat", "Data(*.dat);;All File(*.*)");
    if (file_name.isNull() || file_name.isEmpty())
    {
        return;
    }
    FileReaderWriter writer;
    writer.InitDatabase(db_ptr_);
    bool status = writer.WriteFile(file_name.toStdString().c_str());
    if (!status)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SAVE_FAILED, STRING_OK);
    }
    else
    {
        conn_tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_SAVE_SUCCESS + "</font>");
    }
}

void SignalerOnlineSettingDlg::OnMoreButtonToggled(bool toggled)
{
    more_widget_->setHidden(!toggled);
}

void SignalerOnlineSettingDlg::OnNetworkSettingSlot(bool flag)
{
    is_edit_ip_ = flag;
}

void SignalerOnlineSettingDlg::OnConnectedSlot()
{
    UpdateConnectStatus(true);
    sync_cmd_->ReadTscVersion(this, SLOT(OnCmdGetVerId(QByteArray&)));
    conn_button_->setEnabled(true);
    EnableDialogs(true);
    if (ver_check_id_ == 0)
    {
        ver_check_id_ = startTimer(VERSION_CHECK_TIME);
    }
}

void SignalerOnlineSettingDlg::OnCmdGetVerId(QByteArray &content)
{
    char ver[12] = {'\0'};
    memcpy(ver, content.data(), 11);
    if (strcmp(ver, "CYT0V100END") != 0)
    {
        conn_tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_TIP_VERERROR + "</font>");
        return;
    }

    // reset timer
    is_ver_correct_ = true;
    killTimer(ver_check_id_);
    ver_check_id_ = 0;
    UpdateButtonStatus(true);
    UpdateConnectStatus(true);
    conn_tip_label_->setText(STRING_UI_SIGNALER_TIP_CONNECT);
	more_button_->setChecked(true);
    sync_cmd_->ReleaseSignalSlots();
    if (is_edit_ip_)
    {
        emit versionCheckedSignal();
        is_edit_ip_ = false;
    }
}

void SignalerOnlineSettingDlg::OnDisconnectedSlot()
{
    UpdateConnectStatus(false);
    more_button_->setChecked(false);
}

void SignalerOnlineSettingDlg::OnConnectErrorSlot(QString err)
{
    QMessageBox::information(this, STRING_TIP, err, STRING_OK);
    conn_button_->setEnabled(true);
    // disable other dialog
    EnableDialogs(false);
}

void SignalerOnlineSettingDlg::OnConnTimerTimeoutSlot()
{
    sync_cmd_->closeConnection();
    conn_timer_->stop();
    EnableNetworkErrorTip(true);
    if (!sync_cmd_->isConnectionValid())
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_NETWORK_OUTOFF);
        // TODO: update
        conn_button_->setEnabled(true);
    }
}

void SignalerOnlineSettingDlg::OnCmdTimerTimeoutSlot()
{
    switch (curr_cmd_)
    {
    case Connect:
        ;
    case GetVersion:
        ;
    case ReadConfig:
        ;
    case ReqSetConfig:
        ;
    case SendConfig:
        ;
    case NoneCmd:
        conn_tip_label_->setText(STRING_UI_SIGNALER_SERVER_NOT_REPLY);
        UpdateButtonStatus(true);
        break;
    default:
        break;
    }
    curr_cmd_ = NoneCmd;
    cmd_timer_->stop();
}

void SignalerOnlineSettingDlg::OnCmdReadConfig(QByteArray &content)
{
    curr_cmd_ = NoneCmd;
    cmd_timer_->stop();
    if (content.isEmpty())
    {
        conn_tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_READ_FILE_FAILED + "</font>");
        config_byte_array_.clear();
        QFile::remove(cfg_file_);
        UpdateButtonStatus(true);
        return;
    }
    config_byte_array_.append(content);
    if (!config_byte_array_.right(3).endsWith("END"))
    {
        return;
    }

    bool status = ParseConfigArray(config_byte_array_);
    conn_tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_READ_FILE_SUCCESS + "</font>");
    if (status)
    {
        QFile file(cfg_file_);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QMessageBox::warning(this, STRING_WARNING, STRING_UI_TEMP_FILE + STRING_UI_OPEN_FILE + STRING_FAILED, STRING_OK);
            conn_tip_label_->setText(STRING_UI_TEMP_FILE + STRING_UI_OPEN_FILE + STRING_FAILED);
            config_byte_array_.clear();
            UpdateButtonStatus(true);
            return;
        }
        qint64 sz = file.write(config_byte_array_);
        if (sz != config_byte_array_.size())
        {
            conn_tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED + "</font>");
            QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED, STRING_OK);
            QFile::remove(cfg_file_);
        }
        else
        {
            dialog_tab_->setEnabled(true);
            is_cfg_read_ = true;
            UpdateButtonStatus(true);
            UpdateTabPage();
        }
        UpdateButtonStatus(true);
        file.close();
    }
    else
    {
        conn_tip_label_->setText(STRING_UI_SIGNALER_PARSE_FILE_CONFIG + STRING_FAILED);
    }
    config_byte_array_.clear();
}

void SignalerOnlineSettingDlg::OnCmdSetConfig(QByteArray &content)
{
    if (content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SOCKET_ERROR, STRING_OK);
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
        conn_tip_label_->clear();
        return;
    }
    if (content.contains("CONFIGYS"))
    {
        QFile file(tmp_file_);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, STRING_TIP, STRING_FILE_OPEN + STRING_FAILED, STRING_OK);
            killTimer(ui_lock_id_);
            ui_lock_id_ = 0;
            UpdateButtonStatus(true);
            conn_tip_label_->clear();
            return;
        }
        QByteArray array = file.readAll();
        unsigned sz = array.size() + 11;
        char temp[4] = {'\0'};
        memcpy(temp, &sz, 4);
        array.insert(0, "CYT4");
        array.insert(4, temp, 4);
        array.append("END");
        file.close();
        conn_tip_label_->setText(STRING_SOCKET_SEND_CONFIG);
        SyncCommand::GetInstance()->SendConfigData(array, this, SLOT(OnCmdSendConfig(QByteArray&)));
        return;
    }
    if (content.contains("CONFIGNO"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG_BUSY_WAIT, STRING_OK);
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
        conn_tip_label_->clear();
        return;
    }
    else
    {
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
        conn_tip_label_->clear();
        return;
    }
}

void SignalerOnlineSettingDlg::OnCmdSendConfig(QByteArray &content)
{
    if (content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SOCKET_ERROR, STRING_OK);
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
        conn_tip_label_->clear();
        return;
    }
    if (content.length() < 8)
    {
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
//        conn_tip_label_->clear();
        conn_tip_label_->setText(STRING_PACKAGE_LEN_ERROR);
        return;
    }
    if (content.contains("CONFIGOK"))
    {
        conn_tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_CONFIG + STRING_SUCCEEDED + "</font>");
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG + STRING_SUCCEEDED, STRING_OK);
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
        UpdateTabPage();
    }
    else if (content.contains("CONFIGER"))
    {
        conn_tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_CONFIG + STRING_FAILED + "</font>");
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_CONFIG + STRING_FAILED, STRING_OK);
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
        UpdateButtonStatus(true);
    }
}

void SignalerOnlineSettingDlg::closeEvent(QCloseEvent *)
{
    conn_tip_label_->clear();
    sync_cmd_->disconnectFromHost();
    cmd_timer_->stop();
}

void SignalerOnlineSettingDlg::timerEvent(QTimerEvent *)
{
    if (ver_check_id_ != 0)
    {
        OnConnectedSlot();
    }
    if (ui_lock_id_ != 0)
    {
        QMessageBox::information(this, STRING_TIP, STRING_SOCKET_TIMEOUT, STRING_OK);
        UpdateButtonStatus(true);
        conn_tip_label_->clear();
        killTimer(ui_lock_id_);
        ui_lock_id_ = 0;
    }
}

void SignalerOnlineSettingDlg::resizeEvent(QResizeEvent *)
{
    emit sizeChangedSignal();
}

void SignalerOnlineSettingDlg::keyPressEvent(QKeyEvent *key)
{
    if (key->key() == Qt::Key_Escape)
    {
        conn_tip_label_->clear();
        sync_cmd_->disconnectFromHost();
        cmd_timer_->stop();
    }
    QDialog::keyPressEvent(key);
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
    saveas_button_ = new QPushButton(STRING_UI_SIGNALER_SAVEAS);

    more_button_ = new QPushButton(STRING_DOWN_ARROW);
    more_button_->setCheckable(true);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addWidget(conn_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(read_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(update_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(saveas_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(more_button_);

    more_widget_ = new QWidget(this);
    QHBoxLayout *more_hlayout = new QHBoxLayout;
    more_hlayout->addWidget(send_button_);
    more_hlayout->addStretch(1);
    more_hlayout->addWidget(monitor_button_);
    more_hlayout->addStretch(1);
    more_hlayout->addWidget(setting_button_);
    more_hlayout->addStretch(1);
    more_hlayout->addWidget(log_button_);
    more_hlayout->addStretch(1);
    more_hlayout->addWidget(flow_button_);
    more_hlayout->setSpacing(0);
    more_hlayout->setMargin(0);

    more_widget_->setLayout(more_hlayout);
    more_widget_->hide();

    QVBoxLayout *button_vlayout = new QVBoxLayout;
    button_vlayout->addLayout(button_hlayout);
    button_vlayout->addWidget(more_widget_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(dialog_tab_);
    vlayout->addLayout(button_vlayout);
    conn_tip_label_ = new QLabel(this);
    vlayout->addWidget(conn_tip_label_);
    vlayout->setSizeConstraint(QLayout::SetFixedSize);
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
    connect(this, SIGNAL(versionCheckedSignal()), time_ip_dlg_, SLOT(OnConnectEstablish()));
    connect(time_ip_dlg_, SIGNAL(networkSettingSignal(bool)), this, SLOT(OnNetworkSettingSlot(bool)));

    connect(saveas_button_, SIGNAL(clicked()), this, SLOT(OnSaveAsbuttonClicked()));
    connect(more_button_, SIGNAL(toggled(bool)), this, SLOT(OnMoreButtonToggled(bool)));

//    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), conn_tip_label_, SLOT(setText(QString)));
//    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectErrorSlot(QString)));
    connect(cmd_timer_, SIGNAL(timeout()), this, SLOT(OnCmdTimerTimeoutSlot()));
    connect(conn_timer_, SIGNAL(timeout()), this, SLOT(OnConnTimerTimeoutSlot()));

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

    connect(this, SIGNAL(sizeChangedSignal()), schedule_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), timesection_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), timing_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), stage_timing_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), phase_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), phase_err_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), channel_widget_, SLOT(updateSlot()));
    connect(this, SIGNAL(sizeChangedSignal()), detector_widget_, SLOT(updateSlot()));
}

void SignalerOnlineSettingDlg::InitTabPage()
{
    QFont font;
    font.setPixelSize(13);
    dialog_tab_ = new MTabWidget(this);
    dialog_tab_->setTabBarFont(font);

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

void SignalerOnlineSettingDlg::EnableNetworkErrorTip(bool enable)
{
    if (enable)
    {
        connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), conn_tip_label_, SLOT(setText(QString)));
        connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectErrorSlot(QString)));
    }
    else
    {
        disconnect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), conn_tip_label_, SLOT(setText(QString)));
        disconnect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectErrorSlot(QString)));
    }
}

void SignalerOnlineSettingDlg::UpdateUI()
{
    setWindowTitle(STRING_UI_SIGNALER_ADVANCED_SETUP);
    UpdateButtonStatus(false);
    update_button_->setEnabled(is_cfg_read_);
//    UpdateButtonStatus(true);
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
//    dialog_tab_->setEnabled(enable);
    read_button_->setEnabled(enable);
    if (!is_cfg_read_)
    {
        update_button_->setEnabled(false);
    }
    else
    {
        update_button_->setEnabled(enable);
    }

    send_button_->setEnabled(enable);
    monitor_button_->setEnabled(enable);
    log_button_->setEnabled(enable);
    flow_button_->setEnabled(enable);
    setting_button_->setEnabled(enable);
    saveas_button_->setEnabled(enable);
//    more_button_->setEnabled(enable);
}

void SignalerOnlineSettingDlg::UpdateTabPage()
{
    FileReaderWriter param_reader;
    param_reader.InitDatabase(db_ptr_);
    if (!param_reader.ReadFile(cfg_file_.toStdString().c_str()))
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

void SignalerOnlineSettingDlg::EnableDialogs(bool enable)
{
    flow_dlg_->setEnabled(enable);
    event_log_dlg_->setEnabled(enable);
    monitor_dlg_->setEnabled(enable);
    time_ip_dlg_->setEnabled(enable);
}

bool SignalerOnlineSettingDlg::ParseConfigArray(QByteArray &byte_array)
{
    QString header(byte_array.left(4));
    QString tail(byte_array.right(3));
    if (header != QString("CYT4") || tail != QString("END"))
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

bool SignalerOnlineSettingDlg::SaveTabPageData()
{
    bool status = unitparam_widget_->OnOkButtonClicked();
    if (!status)
    {
        return false;
    }
    schedule_widget_->OnSaveActionClicked();
    timesection_widget_->OnSaveActionClicked();
    stage_timing_widget_->OnSaveActionClicked();
    timing_widget_->OnSaveActionClicked();
    phase_err_widget_->OnSaveButtonClicked();
    phase_widget_->OnSaveActionClicked();
    channel_widget_->OnSaveActionClicked();
    detector_widget_->OnSaveActionClicked();
    return true;
}

bool SignalerOnlineSettingDlg::SaveTempConfigFile()
{
    if (!SaveTabPageData())
    {
//        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED, STRING_OK);
        return false;
    }
    FileReaderWriter writer;
    writer.InitDatabase(db_ptr_);
    bool status = writer.WriteFile(cfg_file_.toStdString().c_str());
    if (!status)
    {
//        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SAVE_FAILED, STRING_OK);
        return false;
    }
    return true;
}
