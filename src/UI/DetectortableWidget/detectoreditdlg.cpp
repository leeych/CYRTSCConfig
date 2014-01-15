#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>

#include "detectoreditdlg.h"
#include "macrostring.h"


DetectoreditDlg::DetectoreditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void DetectoreditDlg::Initialize(unsigned char id, DetectorHandler* handler)
{
    curr_detector_id_ = id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void DetectoreditDlg::OnOkButtonClicked()
{
    if (SaveData())
    {
        accept();
        emit updateTableRowSignal(curr_detector_id_);
    }
}

void DetectoreditDlg::OnCancelButtonClicked()
{
    close();
}

void DetectoreditDlg::InitPage()
{
	setWindowTitle(STRING_UI_DETECTOR + "-" + STRING_UI_EDIT);
    QLabel* detector_id_label = new QLabel(STRING_UI_DETECTOR_ID);
    detector_id_cmb_ = new QComboBox;
    QHBoxLayout* id_hlayout = new QHBoxLayout;
    id_hlayout->addWidget(detector_id_label);
    id_hlayout->addWidget(detector_id_cmb_);
    id_hlayout->addStretch(3);

    QLabel* detector_mode_label = new QLabel(STRING_UI_DETECTOR_MODE);
    QLabel* detector_direction_label = new QLabel(STRING_UI_DETECTOR_DIRECTION);
    QLabel* detector_eff_time_label = new QLabel(STRING_UI_DETECTOR_EFF_TIME + "(s)");
    QLabel* detector_uneff_time_label = new QLabel(STRING_UI_DETECTOR_UNEFF_TIME + "(min)");
//    QLabel* detector_optional_param_label = new QLabel(STRING_UI_DETECTOR_OPTION);
    QLabel* detector_flow_label = new QLabel(STRING_UI_DETECTOR_KEY_LANE_FLOW);
    QLabel* detector_share_label = new QLabel(STRING_UI_DETECTOR_SHARE);
    model_chk_ = new QCheckBox(STRING_UI_DETECTOR_MODEL);
    main_lane_chk_ = new QCheckBox(STRING_UI_DETECTOR_KEY_LANE);

    QGroupBox* phase_grp = new QGroupBox(STRING_UI_DETECTOR_PHASE);
    QString str;
    for (int i = 1; i <= 32; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        phase_chk_list_.append(chk);
    }
    QGridLayout* grid_layout = new QGridLayout;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (cnt < phase_chk_list_.size())
            {
                grid_layout->addWidget(phase_chk_list_.at(cnt), i, j, 1, 1);
                cnt++;
            }
        }
    }
    phase_grp->setLayout(grid_layout);

    detector_mode_cmb_ = new QComboBox;
    detector_direction_cmb_ = new QComboBox;
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_REQUEST);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_INDUCTION);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_TACTICS);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_STRATEGY);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_MAN);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_BUS);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_BIKE);
    detector_mode_cmb_->addItem(STRING_UI_DETECTOR_MOTOR);

    //detector_type_map_.insert(0x080, 0);
    //detector_type_map_.insert(0x040, 1);
    //detector_type_map_.insert(0x020, 2);
    //detector_type_map_.insert(0x010, 3);
    //detector_type_map_.insert(0x008, 4);
    //detector_type_map_.insert(0x004, 5);
    //detector_type_map_.insert(0x002, 6);
    //detector_type_map_.insert(0x001, 7);

    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_EAST);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_WEST);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH_EAST);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH_EAST);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_SOUTH_WEST);
    detector_direction_cmb_->addItem(STRING_UI_CHANNEL_NORTH_WEST);

    //detector_direction_map_.insert(0x080, 7);
    //detector_direction_map_.insert(0x040, 2);
    //detector_direction_map_.insert(0x020, 6);
    //detector_direction_map_.insert(0x010, 1);
    //detector_direction_map_.insert(0x008, 4);
    //detector_direction_map_.insert(0x004, 0);
    //detector_direction_map_.insert(0x002, 5);
    //detector_direction_map_.insert(0x001, 3);

    eff_time_spinbox_ = new QSpinBox;
    failure_time_spinbox_ = new QSpinBox;
    eff_time_spinbox_->setRange(0, 255);
    failure_time_spinbox_->setRange(0, 255);

    flow_spinbox_ = new QSpinBox;
    share_spinbox_ = new QSpinBox;
    flow_spinbox_->setRange(0, 65535);
	flow_spinbox_->setEnabled(false);
    share_spinbox_->setRange(0, 200);

    flow_spinbox_->setMinimumWidth(40);
    share_spinbox_->setMinimumWidth(40);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QGroupBox* detector_grp = new QGroupBox;
    QGridLayout* glayout = new QGridLayout;
    glayout->addWidget(detector_mode_label, 0, 0, 1, 1);
    glayout->addWidget(detector_mode_cmb_, 0, 1, 1, 1);
    glayout->addWidget(detector_direction_label, 0, 3, 1, 1);
    glayout->addWidget(detector_direction_cmb_, 0, 4, 1, 1);
    glayout->addWidget(detector_eff_time_label, 1, 0, 1, 1);
    glayout->addWidget(eff_time_spinbox_, 1, 1, 1, 1);
    glayout->addWidget(detector_uneff_time_label, 1, 3, 1, 1);
    glayout->addWidget(failure_time_spinbox_, 1, 4, 1, 1);
    detector_grp->setLayout(glayout);

    QGroupBox* optional_grp = new QGroupBox(STRING_UI_DETECTOR_OPTION);
    QHBoxLayout* opt_hlayout = new QHBoxLayout;
    opt_hlayout->addWidget(model_chk_);
	model_chk_->setEnabled(false);
	main_lane_chk_->setEnabled(true);
    opt_hlayout->addWidget(main_lane_chk_);
    opt_hlayout->addWidget(detector_flow_label);
    opt_hlayout->addWidget(flow_spinbox_);
    optional_grp->setLayout(opt_hlayout);

    QHBoxLayout* share_hlayout = new QHBoxLayout;
    share_hlayout->addWidget(detector_share_label);
    share_hlayout->addWidget(share_spinbox_);
    share_hlayout->addStretch(1);

    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(2);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(cancel_button_);

    QVBoxLayout* whole_vlayout = new QVBoxLayout;
    whole_vlayout->addLayout(id_hlayout);
    whole_vlayout->addWidget(phase_grp);
	whole_vlayout->addWidget(detector_grp);
    whole_vlayout->addWidget(optional_grp);
    whole_vlayout->addLayout(share_hlayout);
    whole_vlayout->addLayout(button_hlayout);
    setLayout(whole_vlayout);
}

void DetectoreditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
	connect(main_lane_chk_, SIGNAL(toggled(bool)), this, SLOT(OnMainLaneChecked(bool)));
}

void DetectoreditDlg::UpdateUI()
{
    ResetUI();
    if (curr_detector_id_ > 0)
    {
        for (int i = 0; i < detector_id_cmb_->count(); i++)
        {
            if (detector_id_cmb_->itemText(i).toInt() == curr_detector_id_)
            {
                detector_id_cmb_->setCurrentIndex(i);
            }
        }
    }
    // requested phase left to be done.
    DetectorParam detector;
    handler_->get_detector(curr_detector_id_, detector);

	eff_time_spinbox_->setValue(detector.detector_effective_time);
	failure_time_spinbox_->setValue(detector.detector_failure_time);
	flow_spinbox_->setValue(detector.detector_flow);
	share_spinbox_->setValue(detector.detector_occupy);
    update_detector_type(detector.detector_type);
    update_detector_direction(detector.detector_direction);
	update_detector_phase_ids(detector.detector_phase_ids);
	update_detector_spec_func(detector.detector_spec_func);
}

void DetectoreditDlg::ResetUI()
{
    detector_id_cmb_->clear();
    QList<DetectorParam> detector_list = handler_->get_detector_list();
    QList<unsigned char> detector_id_list;
    for (unsigned char i = 1; i <= 56; i++)
    {
        detector_id_list.append(i);
    }
    for (int i = 1; i <= MAX_DETECTOR; i++)
    {
        for (int j = 0; j < detector_list.size(); j++)
        {
            if (curr_detector_id_ != detector_list.at(j).detector_id && i == detector_list.at(j).detector_id)
            {
                detector_id_list.removeOne(detector_list.at(j).detector_id);
            }
        }
    }
    qDebug() << detector_id_list.size();
    QString str;
    for (int i = 0; i < detector_id_list.size(); i++)
    {
        str.sprintf("%d", detector_id_list.at(i));
        detector_id_cmb_->addItem(str);
    }

    for (int i = 0; i < phase_chk_list_.size(); i++)
    {
        phase_chk_list_.at(i)->setChecked(false);
    }
}

bool DetectoreditDlg::SaveData()
{
    DetectorParam detector;
    detector.detector_id = detector_id_cmb_->currentText().toInt();
    detector.detector_direction = get_detector_direction();
    detector.detector_type = get_detector_type();
	detector.detector_effective_time = eff_time_spinbox_->value();
	detector.detector_failure_time = failure_time_spinbox_->value();
    detector.detector_flow = flow_spinbox_->value();
    detector.detector_occupy = share_spinbox_->value();
	detector.detector_phase_ids = get_detector_phase_ids();
	detector.detector_spec_func = get_detector_spec_func();
	detector.detector_delay = eff_time_spinbox_->value();

    // other fields left to be done
    curr_detector_id_ = detector.detector_id;
    handler_->set_detector(curr_detector_id_, detector);

    return true;
}

void DetectoreditDlg::update_detector_type(unsigned char detector_type)
{
    //detector_mode_cmb_->setCurrentIndex(detector_type_map_[detector_type]);
    switch (detector_type)
    {
    case 0x080:
        detector_mode_cmb_->setCurrentIndex(0);
        break;
    case 0x040:
        detector_mode_cmb_->setCurrentIndex(1);
        break;
    case 0x020:
        detector_mode_cmb_->setCurrentIndex(2);
        break;
    case 0x010:
        detector_mode_cmb_->setCurrentIndex(3);
        break;
    case 0x008:
        detector_mode_cmb_->setCurrentIndex(4);
        break;
    case 0x004:
        detector_mode_cmb_->setCurrentIndex(5);
        break;
    case 0x002:
        detector_mode_cmb_->setCurrentIndex(6);
        break;
    case 0x001:
        detector_mode_cmb_->setCurrentIndex(7);
        break;
    default:
//        detector_mode_cmb_->setCurrentIndex(8);
        break;
    }
}

unsigned char DetectoreditDlg::get_detector_type()
{
	QString text = detector_mode_cmb_->currentText();
	if (text == STRING_UI_DETECTOR_REQUEST)
	{
		return 0x080;
	}
	else if (text == STRING_UI_DETECTOR_INDUCTION)
	{
		return 0x040;
	}
	else if (text == STRING_UI_DETECTOR_TACTICS)
	{
		return 0x020;
	}
	else if (text == STRING_UI_DETECTOR_STRATEGY)
	{
		return 0x010;
	}
	else if (text == STRING_UI_DETECTOR_MAN)
	{
		return 0x008;
	}
	else if (text == STRING_UI_DETECTOR_BUS)
	{
		return 0x004;
	}
	else if (text == STRING_UI_DETECTOR_BIKE)
	{
		return 0x002;
	}
	else if (text == STRING_UI_DETECTOR_MOTOR)
	{
		return 0x001;
	}
	return 0;
}

void DetectoreditDlg::update_detector_direction(unsigned char detector_direction)
{
    //detector_direction_cmb_->setCurrentIndex(detector_direction_map_[detector_direction]);
    switch (detector_direction)
    {
    case 0x080:
        detector_direction_cmb_->setCurrentIndex(7);
        break;
    case 0x040:
        detector_direction_cmb_->setCurrentIndex(2);
        break;
    case 0x020:
        detector_direction_cmb_->setCurrentIndex(6);
        break;
    case 0x010:
        detector_direction_cmb_->setCurrentIndex(1);
        break;
    case 0x008:
        detector_direction_cmb_->setCurrentIndex(4);
        break;
    case 0x004:
        detector_direction_cmb_->setCurrentIndex(0);
        break;
    case 0x002:
        detector_direction_cmb_->setCurrentIndex(5);
        break;
    case 0x001:
        detector_direction_cmb_->setCurrentIndex(3);
        break;
    default:
        break;
    }
}

unsigned char DetectoreditDlg::get_detector_direction()
{
	QString text = detector_direction_cmb_->currentText();
	if (text == STRING_UI_CHANNEL_NORTH_WEST)
	{
		return 0x080;
	}
	else if (text == STRING_UI_CHANNEL_WEST)
	{
		return 0x040;
	}
	else if (text == STRING_UI_CHANNEL_SOUTH_WEST)
	{
		return 0x020;
	}
	else if (text == STRING_UI_CHANNEL_SOUTH)
	{
		return 0x010;
	}
	else if (text == STRING_UI_CHANNEL_SOUTH_EAST)
	{
		return 0x008;
	}
	else if (text == STRING_UI_CHANNEL_EAST)
	{
		return 0x004;
	}
	else if (text == STRING_UI_CHANNEL_NORTH_EAST)
	{
		return 0x002;
	}
	else if (text == STRING_UI_CHANNEL_NORTH)
	{
		return 0x001;
	}
	return 0;
}

void DetectoreditDlg::update_detector_phase_ids( unsigned int phase_ids )
{
    for (size_t i = 0; i < sizeof(phase_ids) * 8; i++)
	{
		if ((phase_ids & 0x01) == 0x01)
		{
			phase_chk_list_.at(i)->setChecked(true);
		}
		phase_ids = phase_ids >> 1;
	}
}

unsigned int DetectoreditDlg::get_detector_phase_ids()
{
	unsigned int phase_ids = 0;
	for (int i = phase_chk_list_.size() - 1; i >= 0; i--)
	{
		phase_ids = phase_ids << 1;
		if (phase_chk_list_.at(i)->isChecked())
		{
			phase_ids |= 0x01;
		}
	}
	return phase_ids;
}

void DetectoreditDlg::update_detector_spec_func( unsigned char spec_func )
{
	main_lane_chk_->setChecked((spec_func & 0x02) == 0x02);
}

unsigned char DetectoreditDlg::get_detector_spec_func()
{
	unsigned char spec_func = 0;
	if (main_lane_chk_->isChecked())
	{
		spec_func |= 0x02;
	}
	return spec_func;
}

DetectoreditDlg::~DetectoreditDlg()
{

}

void DetectoreditDlg::OnMainLaneChecked( bool status )
{
	flow_spinbox_->setEnabled(status);
}
