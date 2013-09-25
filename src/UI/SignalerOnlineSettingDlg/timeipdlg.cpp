#include "timeipdlg.h"
#include "macrostring.h"
#include "synccommand.h"
#include "command.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QIODevice>


TimeIPDlg::TimeIPDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

TimeIPDlg::~TimeIPDlg()
{
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
    SyncCommand::GetInstance()->SyncSignalerTime(seconds, this, SLOT(OnCmdSyncSignalerTime(QByteArray&)));
}

void TimeIPDlg::OnRefreshButtonClicked()
{
    SyncCommand::GetInstance()->ReadSignalerNetworkInfo(this, SLOT(OnCmdReadNetworkingInfo(QByteArray&)));
    EnableButtonExcept(false, NULL);
}

void TimeIPDlg::OnWriteIPButtonClicked()
{
    QStringList param_list;
    QString ip = ip_lineedit_->text().trimmed();
    QString netmask = mask_lineedit_->text().trimmed();
    QString gateway = gateway_lineedit_->text().trimmed();
    param_list << "0" << Trimmed(gateway) << Trimmed(ip) << Trimmed(netmask);
    SyncCommand::GetInstance()->ConfigNetwork(param_list, this, SLOT(OnCmdWriteIPAddress(QByteArray&)));
}

void TimeIPDlg::OnCmdReadTscTime(QByteArray &array_content)
{
    char head[4] = {'\0'};
    char *content = array_content.data();
    memcpy(head, content, 4);
    if (strcmp(head, "CYT7") != 0)
    {
        return;
    }
    unsigned int sec = 0;
    memcpy(&sec, content + 4, 4);
    if (sec >= 60 * 60 * 8)     // east 8 time-zoon
    {
        sec -= 60 * 60 * 8;
    }
    QDateTime datetime = QDateTime::fromTime_t(sec).toLocalTime();
    sys_time_text_label_->setText(datetime.toString("yyyy-MM-dd hh:mm:ss ddd"));

    EnableButtonExcept(true, NULL);
}

void TimeIPDlg::OnCmdReadNetworkingInfo(QByteArray &content)
{
    QString network(content.data());
    if (network.left(4) != QString("CYT8") || network.right(3) != QString("END"))
    {
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
    if (array_content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_SYNC_TIME_NULL + STRING_FAILED, STRING_OK);
        return;
    }
    if (array_content == "TIMECFGOK")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SYNC_TIME + STRING_SUCCEEDED, STRING_OK);
        return;
    }
    if (array_content == "TIMECFGER")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SYNC_TIME + STRING_FAILED, STRING_OK);
        return;
    }
}

void TimeIPDlg::OnCmdWriteIPAddress(QByteArray &array_content)
{
    if (array_content.isEmpty())
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_NETWORK_CFG_NULL, STRING_OK);
        return;
    }
    if (array_content == "NETSETOK")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_NETWORK + STRING_SUCCEEDED, STRING_OK);
        return;
    }
    if (array_content == "NETSETER")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_NETWORK + STRING_FAILED, STRING_OK);
        return;
    }
}

void TimeIPDlg::OnConnectEstablish()
{
}

void TimeIPDlg::OnConnectError(QAbstractSocket::SocketError)
{
    qDebug() << "socket error";
}

void TimeIPDlg::closeEvent(QCloseEvent *)
{
    sys_time_text_label_->clear();
    ip_lineedit_->clear();
    mask_lineedit_->clear();
    gateway_lineedit_->clear();
}

void TimeIPDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_TIME_NETWORK_TITLE);
    read_sys_time_button_ = new QPushButton(STRING_UI_SIGNALER_READ_SYSTIME);
    sync_time_button_ = new QPushButton(STRING_UI_SIGNALER_SYNC_TIME);
    refresh_button_ = new QPushButton(STRING_UI_SIGNALER_REFRESH);
    write_ip_button_ = new QPushButton(STRING_UI_SIGNALER_WRITE_IP);

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
    ip_lineedit_ = new QLineEdit;
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
    grid_layout->addWidget(refresh_button_, 3, 0, 1, 1, Qt::AlignCenter);
    grid_layout->addWidget(write_ip_button_, 3, 1, 1, 1, Qt::AlignCenter);
    network_grp->setLayout(grid_layout);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(time_grp);
    vlayout->addWidget(network_grp);
    setLayout(vlayout);

    setWindowFlags(this->windowFlags() & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint);
}

void TimeIPDlg::InitSignalSlots()
{
    connect(read_sys_time_button_, SIGNAL(clicked()), this, SLOT(OnReadSystimeButtonClicked()));
    connect(sync_time_button_, SIGNAL(clicked()), this, SLOT(OnSyncTimeButtonClicked()));
    connect(refresh_button_, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));
    connect(write_ip_button_, SIGNAL(clicked()), this, SLOT(OnWriteIPButtonClicked()));
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
        button_list_.append(refresh_button_);
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
