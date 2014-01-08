#include "timeipdlg.h"
#include "macrostring.h"
#include "synccommand.h"
#include "command.h"
#include "mutility.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIODevice>
#include <QRegExpValidator>

#define STOP_TIMER
#ifdef STOP_TIMER
#define STOP_CMD_TIMER(timer) \
    if (timer->isActive()){ timer->stop();}
#else
#define STOP_CMD_TIMER(timer)
#endif

TimeIPDlg::TimeIPDlg(QWidget *parent) :
    QDialog(parent)
{
    timer_id_ = 0;
    cmd_timer_ = new QTimer(this);
    curr_cmd_ = NoneCmd;
    has_disconnected_ = false;
    port_ = 0;
    InitPage();
    InitSignalSlots();
}

TimeIPDlg::~TimeIPDlg()
{
    if (timer_id_ != 0)
    {
        killTimer(timer_id_);
        timer_id_ = 0;
    }
    if (cmd_timer_ != NULL)
    {
        delete cmd_timer_;
        cmd_timer_ = NULL;
    }
}

void TimeIPDlg::Initialize()
{
    UpdateUI();
    exec();
}

void TimeIPDlg::OnReadSystimeButtonClicked()
{
    SyncCommand::GetInstance()->ReadSignalerTime(this, SLOT(OnCmdReadTscTime(QByteArray&)));
}

void TimeIPDlg::OnSyncTimeButtonClicked()
{
    QDateTime datetime = QDateTime::currentDateTime();
    unsigned int seconds = datetime.toTime_t();
    seconds += 60*60*8;
    SyncCommand::GetInstance()->SyncSignalerTime(seconds, this, SLOT(OnCmdSyncSignalerTime(QByteArray&)));
    sync_time_button_->setEnabled(false);
}

void TimeIPDlg::OnReadIPButtonClicked()
{
    read_network_button_->setEnabled(false);
    SyncCommand::GetInstance()->ReadSignalerNetworkInfo(this, SLOT(OnCmdReadNetworkingInfo(QByteArray&)));
    EnableButtonExcept(false, NULL);
    curr_cmd_ = ReadNetwork;
    cmd_timer_->start(31000);
    read_network_button_->setEnabled(true);
}

void TimeIPDlg::OnWriteIPButtonClicked()
{
    QStringList param_list;
    QString ip = ip_lineedit_->text().trimmed();
    QString netmask = mask_lineedit_->text().trimmed();
    QString gateway = gateway_lineedit_->text().trimmed();
    if (!MUtility::checkIPString(ip))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_TIME_NETWORK_IP_ERROR, STRING_OK);
        return;
    }
    else if (!MUtility::checkIPString(netmask))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_TIME_NETWORK_NETMASK_ERROR, STRING_OK);
        return;
    }
    else if (!MUtility::checkIPString(gateway))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_TIME_NETWORK_GATEWAY_ERROR, STRING_OK);
        return;
    }

    int ret = QMessageBox::question(this, STRING_TIP, STRING_UI_SIGNALER_WRITE_IP_REQUEST_TIP, STRING_OK, STRING_NO);
    if (ret != 0)
    {
        return;
    }
    param_list << "0" << Trimmed(gateway) << Trimmed(ip) << Trimmed(netmask);
    SyncCommand::GetInstance()->ConfigNetwork(param_list, this, SLOT(OnCmdWriteIPAddress(QByteArray&)));
    emit networkSettingSignal(true);
    SyncCommand::GetInstance()->disconnectFromHost();
    has_disconnected_ = true;
    this->setEnabled(false);
    QTime t;
    t.start();
    while (t.elapsed() < 1000)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    port_ = SyncCommand::GetInstance()->getSocketPort();
    SyncCommand::GetInstance()->connectToHost(ip, port_);
    curr_cmd_ = WriteNetwork;
    cmd_timer_->start(31000);
    this->setEnabled(true);
}

void TimeIPDlg::OnCmdTimerTimeoutSlot()
{
    switch (curr_cmd_)
    {
    case ReadNetwork:
        break;
    case WriteNetwork:
        break;
    case NoneCmd:
        break;
    default:
        break;
    }
    QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SERVER_NOT_REPLY, STRING_OK);
    curr_cmd_ = NoneCmd;
    cmd_timer_->stop();
}

void TimeIPDlg::OnCmdReadTscTime(QByteArray &array_content)
{
	QString head(array_content.left(4));
    if (head != QString("CYT7"))
    {
        return;
    }
	array_content.remove(0, 4);
    unsigned int sec = 0;
    memcpy(&sec, array_content.data(), 4);
    if (sec >= 60 * 60 * 8)     // east 8 time-zoon
    {
        sec -= 60 * 60 * 8;
    }
    signaler_time_ = QDateTime::fromTime_t(sec).toLocalTime();
    sys_time_text_label_->setText(signaler_time_.toString("yyyy-MM-dd hh:mm:ss ddd"));
    if (timer_id_ == 0)
    {
        timer_id_ = startTimer(1000);
    }
    EnableButtonExcept(true, NULL);
}

void TimeIPDlg::OnCmdReadNetworkingInfo(QByteArray &content)
{
    STOP_CMD_TIMER(cmd_timer_)
    QString network(content.data());
    if (network.left(4) != QString("CYT8") || network.right(3) != QString("END"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_PACKAGE_INCORRECT, STRING_OK);
        return;
    }

    QString gateway = "DefaultGateway=\"";
    QString ipaddress = "IPAddress=\"";
    QString netmask = "SubnetMask=\"";
    QString end = "END";
    int gateway_idx = network.indexOf(gateway);
    gateway_idx += gateway.size();
    int ip_idx = network.indexOf(ipaddress);
    gateway = network.mid(gateway_idx, ip_idx - gateway_idx - 2);

    ip_idx += ipaddress.size();
    int netmask_idx = network.indexOf(netmask);
    ipaddress = network.mid(ip_idx, netmask_idx - ip_idx - 2);

    netmask_idx += netmask.size();
    int end_idx = network.indexOf(end);
    netmask = network.mid(netmask_idx, end_idx - netmask_idx - 2);

    gateway_lineedit_->setText(gateway);
    ip_lineedit_->setText(ipaddress);
    mask_lineedit_->setText(netmask);

    EnableButtonExcept(true, NULL);
}

void TimeIPDlg::OnCmdSyncSignalerTime(QByteArray &array_content)
{
    sync_time_button_->setEnabled(true);
    if (array_content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SYNC_TIME_NULL + STRING_FAILED, STRING_OK);
        return;
    }
    if (array_content.contains("TIMECFGOK"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SYNC_TIME + STRING_SUCCEEDED, STRING_OK);
        return;
    }
    if (array_content.contains("TIMECFGER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SYNC_TIME + STRING_FAILED, STRING_OK);
        return;
    }
}

void TimeIPDlg::OnCmdWriteIPAddress(QByteArray &array_content)
{
    STOP_CMD_TIMER(cmd_timer_)

    if (array_content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_NETWORK_CFG_NULL, STRING_OK);
    }
    else if (array_content.contains("NETSETOK"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_NETWORK + STRING_SUCCEEDED, STRING_OK);
    }
    else if (array_content.contains("NETSETER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_NETWORK + STRING_FAILED, STRING_OK);
    }
    disconnect(SyncCommand::GetInstance(), SIGNAL(connectedSignal()), this, SLOT(OnConnectEstablish()));
}

void TimeIPDlg::OnConnectEstablish()
{
    STOP_CMD_TIMER(cmd_timer_);
    if (has_disconnected_)
    {
        SyncCommand::GetInstance()->InitParseHandler(this, SLOT(OnCmdWriteIPAddress(QByteArray&)));
    }
}

void TimeIPDlg::OnConnectError(QAbstractSocket::SocketError)
{
    QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_TIP_DISCONN, STRING_OK);
}

void TimeIPDlg::closeEvent(QCloseEvent *)
{
    sys_time_text_label_->clear();
    ip_lineedit_->clear();
    mask_lineedit_->clear();
    gateway_lineedit_->clear();
    if (timer_id_ != 0)
    {
        killTimer(timer_id_);
        timer_id_ = 0;
    }
    cmd_timer_->stop();
}

void TimeIPDlg::timerEvent(QTimerEvent *)
{
    if (timer_id_ != 0)
    {
        signaler_time_ = signaler_time_.addSecs(1);
        sys_time_text_label_->setText(signaler_time_.toString("yyyy-MM-dd hh:mm:ss ddd"));
    }
}

void TimeIPDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_TIME_NETWORK_TITLE);
    read_sys_time_button_ = new QPushButton(STRING_UI_SIGNALER_READ_SYSTIME);
    sync_time_button_ = new QPushButton(STRING_UI_SIGNALER_SYNC_TIME);
    read_network_button_ = new QPushButton(STRING_UI_SIGNALER_READ_NETWORK);
    write_ip_button_ = new QPushButton(STRING_UI_SIGNALER_SET_NETWORK);

    QGroupBox *time_grp = new QGroupBox(STRING_UI_SIGNALER_TIME_GRP);
    QLabel *sys_time_label = new QLabel(STRING_UI_SIGNALER_SYS_TIME + ":");
    sys_time_text_label_ = new QLabel;
    QHBoxLayout *time_hlayout = new QHBoxLayout;
    time_hlayout->addWidget(sys_time_label);
    time_hlayout->addWidget(sys_time_text_label_);
    time_hlayout->addStretch(1);
    time_hlayout->addWidget(read_sys_time_button_);
    time_hlayout->addWidget(sync_time_button_);
    time_grp->setLayout(time_hlayout);

    QGroupBox *network_grp = new QGroupBox(STRING_UI_SIGNALER_NETWORK_GRP);
    QLabel *ip_label = new QLabel(STRING_IP + ":");
    ip_lineedit_ = new QLineEdit(this);
//    QRegExp reg("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
//    QRegExpValidator *validator = new QRegExpValidator(reg);
//    ip_lineedit_->setValidator(validator);
    ip_lineedit_->setInputMask("000.000.000.000");

    QLabel *mask_label = new QLabel(STRING_MASK + ":");
    mask_lineedit_ = new QLineEdit;
    mask_lineedit_->setInputMask("000.000.000.000");
    QLabel *gateway_label = new QLabel(STRING_GATEWAY + ":");
    gateway_lineedit_ = new QLineEdit;
    gateway_lineedit_->setInputMask("000.000.000.000");

    mask_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    gateway_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);

    QGridLayout *grid_layout = new QGridLayout;
    grid_layout->addWidget(ip_label, 0, 0, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(ip_lineedit_, 0, 1, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(mask_label, 1, 0, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(mask_lineedit_, 1, 1, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(gateway_label, 2, 0, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(gateway_lineedit_, 2, 1, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(read_network_button_, 3, 0, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(write_ip_button_, 3, 1, 1, 1, Qt::AlignCenter);
    network_grp->setLayout(grid_layout);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(time_grp);
    vlayout->addWidget(network_grp);
    vlayout->setStretch(0, 1);
    vlayout->setStretch(1, 2);
    setLayout(vlayout);

    setWindowFlags(this->windowFlags() & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint);
}

void TimeIPDlg::InitSignalSlots()
{
    connect(read_sys_time_button_, SIGNAL(clicked()), this, SLOT(OnReadSystimeButtonClicked()));
    connect(sync_time_button_, SIGNAL(clicked()), this, SLOT(OnSyncTimeButtonClicked()));
    connect(read_network_button_, SIGNAL(clicked()), this, SLOT(OnReadIPButtonClicked()));
    connect(write_ip_button_, SIGNAL(clicked()), this, SLOT(OnWriteIPButtonClicked()));
    connect(cmd_timer_, SIGNAL(timeout()), this, SLOT(OnCmdTimerTimeoutSlot()));
//    connect(SyncCommand::GetInstance(), SIGNAL(connectedSignal()), this, SLOT(OnConnectEstablish()));
}

void TimeIPDlg::UpdateUI()
{
    sync_time_button_->setEnabled(false);
}

void TimeIPDlg::EnableButtonExcept(bool enable, QPushButton *btn_ptr)
{
    if (button_list_.isEmpty())
    {
        button_list_.append(read_sys_time_button_);
        button_list_.append(sync_time_button_);
        button_list_.append(read_network_button_);
        button_list_.append(write_ip_button_);
    }
    for (int i = 0; i < button_list_.size(); i++)
    {
        if (button_list_.at(i) != btn_ptr)
        {
            button_list_.at(i)->setEnabled(enable);
        }
    }
}

QString TimeIPDlg::Trimmed(QString &str)
{
    int index = str.indexOf(" ");
    if (index < 0)
    {
        return str;
    }
    return Trimmed(str);
}
