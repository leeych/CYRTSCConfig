#include "timeipdlg.h"
#include "macrostring.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QMessageBox>

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
    QDateTime datetime = QDateTime::currentDateTime();
    QString str = datetime.toString("yyyy-MM-dd hh:mm:ss ddd");
    sys_time_text_label_->setText(str);
    sync_time_button_->setEnabled(!str.isEmpty());
}

void TimeIPDlg::OnSyncTimeButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Sync time", STRING_OK);
}

void TimeIPDlg::OnRefreshButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Refresh", STRING_OK);
}

void TimeIPDlg::OnWriteIPButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Set signaler IP", STRING_OK);
}

void TimeIPDlg::closeEvent(QCloseEvent *)
{
    sys_time_text_label_->clear();
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
    grid_layout->addWidget(ip_label, 0, 0, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(ip_lineedit_, 0, 1, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(mask_label, 1, 0, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(mask_lineedit_, 1, 1, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(gateway_label, 2, 0, 1, 1, Qt::AlignLeft);
    grid_layout->addWidget(gateway_lineedit_, 2, 1, 1, 1, Qt::AlignLeft);
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
