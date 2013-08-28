#include "channeleditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

ChanneleditDlg::ChanneleditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void ChanneleditDlg::Initialize(unsigned char channel_id, ChannelHandler *handler)
{
    curr_channel_id_ = channel_id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void ChanneleditDlg::SetHandler(ChannelHandler *handler)
{
    handler_ = handler;
}

void ChanneleditDlg::OnOkButtonClicked()
{
    if (SaveData())
    {
        accept();
        emit updateTableRowSignal(curr_channel_id_);
    }
}

void ChanneleditDlg::OnCancelButtonClicked()
{
    close();
}

void ChanneleditDlg::InitPage()
{
	setWindowTitle(STRING_UI_CHANNEL + "-" + STRING_UI_EDIT);
    QLabel* channel_id_label = new QLabel(STRING_UI_CHANNEL_ID);
    QLabel* channel_ctrl_mode_label = new QLabel(STRING_UI_CHANNEL_CONTROL_MODE);
    QLabel* channel_lane_direction_label = new QLabel(STRING_UI_CHANNEL_LANE_DIRECTION);
//    QLabel* channel_status_label = new QLabel(STRING_UI_CHANNEL_STATUS);

    channel_id_cmb_ = new QComboBox;
    ctrl_mode_cmb_ =new QComboBox;
    direction_cmb_ = new QComboBox;
    lane_mode_cmb_ = new QComboBox;

    QString str;
    for (int i = 1; i <= 32; i++)
    {
        str.sprintf("%d", i);
        channel_id_cmb_->addItem(str);
    }
    ctrl_mode_cmb_->addItem(STRING_UI_CHANNEL_OTHER);
    ctrl_mode_cmb_->addItem(STRING_UI_CHANNEL_MOTOR);
    ctrl_mode_cmb_->addItem(STRING_UI_CHANNEL_MAN);
    ctrl_mode_cmb_->addItem(STRING_UI_CHANNEL_F_PHASE);

    direction_cmb_->addItem(STRING_UI_CHANNEL_EAST);
    direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH);
    direction_cmb_->addItem(STRING_UI_CHANNEL_WEST);
    direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH);
    direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH_EAST);
    direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH_EAST);
    direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH_WEST);
    direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH_WEST);

    lane_mode_cmb_->addItem(STRING_UI_CHANNEL_LINE);
    lane_mode_cmb_->addItem(STRING_UI_CHANNEL_TURN_LEFT);
    lane_mode_cmb_->addItem(STRING_UI_CHANNEL_TURN_RIGHT);
    lane_mode_cmb_->addItem(STRING_UI_CHANNEL_SIDEWALK);

    yellow_flash_chk_ = new QCheckBox(STRING_UI_CHANNEL_YELLOW_FLASH);
    red_flash_chk_ = new QCheckBox(STRING_UI_CHANNEL_RED_FLASH);
    alter_flash_chk_ = new QCheckBox(STRING_UI_CHANNEL_ALTER);

	yellow_flash_chk_->setEnabled(false);
	red_flash_chk_->setEnabled(false);
	alter_flash_chk_->setEnabled(false);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(channel_id_label);
    fir_hlayout->addWidget(channel_id_cmb_);
//    fir_hlayout->addStretch(1);
    fir_hlayout->addWidget(channel_ctrl_mode_label);
    fir_hlayout->addWidget(ctrl_mode_cmb_);

    QHBoxLayout* sec_hlayout = new QHBoxLayout;
    sec_hlayout->addWidget(channel_lane_direction_label);
    sec_hlayout->addWidget(direction_cmb_);
    sec_hlayout->addWidget(lane_mode_cmb_);

    QGroupBox* grp = new QGroupBox(STRING_UI_CHANNEL_STATUS);
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(yellow_flash_chk_);
    hlayout->addWidget(red_flash_chk_);
    hlayout->addWidget(alter_flash_chk_);
    grp->setLayout(hlayout);

    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(3);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(cancel_button_);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addLayout(sec_hlayout);
    vlayout->addWidget(grp);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void ChanneleditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
}

void ChanneleditDlg::UpdateUI()
{
    ResetUI();
    for (int i = 0; i < channel_id_cmb_->count(); i++)
    {
        if (curr_channel_id_ == channel_id_cmb_->itemText(i).toInt())
        {
            channel_id_cmb_->setCurrentIndex(i);
        }
    }
    ChannelParam channel;
    if (!handler_->get_channel(handler_->get_current_channel_id(), channel))
    {
        return;
    }
    switch (channel.channel_type)
    {
    case 1:
        ctrl_mode_cmb_->setCurrentIndex(0);
        break;
    case 2:
        ctrl_mode_cmb_->setCurrentIndex(1);
        break;
    case 3:
        ctrl_mode_cmb_->setCurrentIndex(2);
        break;
    case 4:
        ctrl_mode_cmb_->setCurrentIndex(3);
        break;
    default:
        break;
    }
}

void ChanneleditDlg::ResetUI()
{
    channel_id_cmb_->clear();
    QList<unsigned char> channel_id_list;
    QList<ChannelParam> channel_list = handler_->get_channel_list();
    for (unsigned char i = 1; i <= MAX_CHANNEL; i++)
    {
        channel_id_list.append(i);
    }
    for (unsigned char i = 1; i <= MAX_CHANNEL; i++)
    {
        for (int j = 0; j < channel_list.size(); j++)
        {
            if (curr_channel_id_ != channel_list.at(j).channel_id && i == channel_list.at(j).channel_id)
            {
                channel_id_list.removeOne(channel_list.at(j).channel_id);
            }
        }
    }
    QString str;
    for (int i = 0; i < channel_id_list.size(); i++)
    {
        str.sprintf("%d", channel_id_list.at(i));
        channel_id_cmb_->addItem(str);
    }
    yellow_flash_chk_->setChecked(false);
    red_flash_chk_->setChecked(false);
    alter_flash_chk_->setChecked(false);
}

bool ChanneleditDlg::SaveData()
{
    ChannelParam channel;
    channel.channel_id = channel_id_cmb_->currentText().toInt();
    channel.channel_flash = SaveChannelFlash();
    channel.channel_type = SaveChannelType();
	channel.channel_direction = get_channel_direction();
	channel.channel_road = get_channel_lane();

	handler_->set_channel(curr_channel_id_, channel);
	curr_channel_id_ = channel.channel_id;

    return true;
}

unsigned char ChanneleditDlg::SaveChannelType()
{
	unsigned char ctrl_type = 0;
    if (ctrl_mode_cmb_->currentText() == STRING_UI_CHANNEL_OTHER)
    {
        ctrl_type = 1;
    }
    else if (ctrl_mode_cmb_->currentText() == STRING_UI_CHANNEL_MOTOR)
    {
        ctrl_type = 2;
    }
    else if (ctrl_mode_cmb_->currentText() == STRING_UI_CHANNEL_MAN)
    {
        ctrl_type = 3;
    }
    else if (ctrl_mode_cmb_->currentText() == STRING_UI_CHANNEL_F_PHASE)
    {
        ctrl_type = 4;
    }
	return ctrl_type;
}

unsigned char ChanneleditDlg::SaveChannelFlash()
{
	unsigned char flash_mode = 0;
    if (alter_flash_chk_->isChecked())
    {
		flash_mode |= 0x08;
    }
	else if (red_flash_chk_->isChecked())
	{
		flash_mode |= 0x04;
	}
	else if (yellow_flash_chk_->isChecked())
	{
		flash_mode |= 0x02;
	}
	return flash_mode;
}

ChanneleditDlg::~ChanneleditDlg()
{

}

unsigned char ChanneleditDlg::get_channel_direction()
{
	unsigned char index = direction_cmb_->currentIndex();	
	return index;
}

unsigned char ChanneleditDlg::get_channel_lane()
{
	unsigned char index = lane_mode_cmb_->currentIndex();
	return index;
}