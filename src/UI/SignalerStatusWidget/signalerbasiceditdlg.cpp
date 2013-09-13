#include "signalerbasiceditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

SignalerbasiceditDlg::SignalerbasiceditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void SignalerbasiceditDlg::Initialize(int signaler_id, SignalerHandler *handler)
{
    curr_signaler_id_ = signaler_id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void SignalerbasiceditDlg::InitPage()
{
	setWindowTitle(STRING_UI_SIGNALER_BASIC_INFO);
	id_label_caption_ = new QLabel(STRING_UI_SIGNALER_ID + ":");
    id_label_ = new QLabel;

    QLabel* name_label = new QLabel(STRING_UI_SIGNALER_NAME);
    QLabel* ip_label = new QLabel(STRING_UI_SIGNALER_IP);
    QLabel* port_label = new QLabel(STRING_UI_SIGNALER_PORT);
    QLabel* pos_label = new QLabel(STRING_UI_SIGNALER_POSITION);
    QLabel* branch_label = new QLabel(STRING_UI_SIGNALER_BRANCH);
    QLabel* mode_label = new QLabel(STRING_UI_SIGNALER_MODE);
    QLabel* version_label = new QLabel(STRING_UI_SIGNALER_VERSION);
    QLabel* mark_label = new QLabel(STRING_UI_SIGNALER_MARKUP);

    name_lineedit_ = new QLineEdit;
    ip_lineedit_ = new QLineEdit;
	ip_lineedit_->setInputMask("000.000.000.000");
    port_lineedit_ = new QLineEdit;
    pos_lineedit_ = new QLineEdit;
    branch_lineedit_ = new QLineEdit;
    mode_lineedit_ = new QLineEdit;
    version_lineedit_ = new QLineEdit;
    remark_lineedit_ = new QLineEdit;

    name_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    ip_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    port_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    pos_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    branch_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    mode_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    version_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);
    remark_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QGroupBox* set_grp = new QGroupBox(STRING_UI_SIGNALER_PARAM_SET);
    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(id_label_caption_);
    fir_hlayout->addWidget(id_label_);
    fir_hlayout->addStretch(3);

    QGridLayout* glayout = new QGridLayout;
    glayout->addWidget(name_label, 0, 0, 1, 1);
    glayout->addWidget(name_lineedit_, 0, 1, 1, 3);
    glayout->addWidget(ip_label, 1, 0, 1, 1);
    glayout->addWidget(ip_lineedit_, 1, 1, 1, 1);
    glayout->addWidget(port_label, 1, 2, 1, 1);
    glayout->addWidget(port_lineedit_, 1, 3, 1, 1);
    glayout->addWidget(pos_label, 2, 0, 1, 1);
    glayout->addWidget(pos_lineedit_, 2, 1, 1, 3);
    glayout->addWidget(branch_label, 3, 0, 1, 1);
    glayout->addWidget(branch_lineedit_, 3, 1, 1, 1);
    glayout->addWidget(mode_label, 4, 0, 1, 1);
    glayout->addWidget(mode_lineedit_, 4, 1, 1, 1);
    glayout->addWidget(version_label, 5, 0, 1, 1);
    glayout->addWidget(version_lineedit_, 5, 1, 1, 3);
    glayout->addWidget(mark_label, 6, 0, 1, 1);
    glayout->addWidget(remark_lineedit_, 6, 1, 1, 3);
    set_grp->setLayout(glayout);

    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(2);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(cancel_button_);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addWidget(set_grp);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void SignalerbasiceditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
}

void SignalerbasiceditDlg::UpdateUI()
{
	SignalerParam signaler;
	if (!handler_->get_signaler(curr_signaler_id_, signaler))
	{
		id_label_caption_->setVisible(false);
		id_label_->setVisible(false);
	}
	else
	{
		id_label_caption_->setVisible(true);
		id_label_->setVisible(true);
	}

	id_label_->setText(QString::number(signaler.signaler_id));
	name_lineedit_->setText(signaler.signaler_name);
	ip_lineedit_->setText(signaler.signaler_ip);
	port_lineedit_->setText(QString::number(signaler.signaler_port));
	pos_lineedit_->setText(signaler.location_desc);
	branch_lineedit_->setText(signaler.branch_road);
	version_lineedit_->setText(signaler.software_version);
	mode_lineedit_->setText(signaler.signaler_mode);
    remark_lineedit_->setText(signaler.remark);
}

SignalerbasiceditDlg::~SignalerbasiceditDlg()
{

}

void SignalerbasiceditDlg::OnOkButtonClicked()
{
	if (SaveData())
	{
		accept();
		emit updateSignalerSignal(curr_signaler_id_);
	}
}

void SignalerbasiceditDlg::OnCancelButtonClicked()
{
    reject();
}

bool SignalerbasiceditDlg::SaveData()
{
	SignalerParam signaler;
	if (id_label_->text().toUInt() == 0)
	{
		signaler.signaler_id = handler_->get_max_signaler_id() + 1;
	}
	else
	{
		signaler.signaler_id = id_label_->text().toUInt();
	}
	signaler.signaler_port = pos_lineedit_->text().trimmed().toUInt();
	signaler.signaler_ip = ip_lineedit_->text().trimmed();
	signaler.signaler_port = port_lineedit_->text().toUInt();
	signaler.signaler_name = QString(name_lineedit_->text().toUtf8());
	signaler.signaler_mode = QString(mode_lineedit_->text().toUtf8());
	signaler.software_version = QString(version_lineedit_->text().toUtf8());
	signaler.branch_road = QString(branch_lineedit_->text().toUtf8());
	signaler.location_desc = QString(pos_lineedit_->text().toUtf8());
    signaler.remark = QString(remark_lineedit_->text().toUtf8());
	signaler.signaler_status = SignalerParam::Offline;
	handler_->set_signaler(signaler);
	return true;
}
