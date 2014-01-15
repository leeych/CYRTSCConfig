#include "phaseeditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>

PhaseeditDlg::PhaseeditDlg(QWidget *parent) :
    QDialog(parent)
{
    detector_num_ = 0;
    InitPage();
    InitSignalSlots();
}

void PhaseeditDlg::Initialize(unsigned char phase_id, PhaseHandler *handler)
{
    curr_phase_id_ = phase_id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void PhaseeditDlg::OnOkButtonClicked()
{
    if (ValidateUI() == MinLargerThanMax1)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_PHASE_MIN_LARGER_MAX + QString::number(1) + " !", STRING_OK);
        return;
    }
    if (ValidateUI() == GreenFlashZero)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_PHASE_GREEN_FLASH_ZERO_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == ManGreenClearZero)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_MAN_GREEN_CLEAR_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == ManClearLessGreen)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_MAN_CLEAR_GREEN_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == FixGreenZero)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_FIX_GREEN_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == ManClearLessMinGreen)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_MAN_CLEAR_MIN_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == FixManClearLessFix)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_FIX_MAN_CLEAR_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == ManClearZero)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_MAN_CLEAR_ZERO_TIP, STRING_OK);
        return;
    }
    if (ValidateUI() == DelayLessThanMinGreen)
    {
        QMessageBox::information(this, STRING_WARNING, STRING_UI_PHASE_DELAY_LESS_MINGREEN_TIP, STRING_OK);
        return;
    }

    if (SaveData())
    {
        accept();
        emit updateTreeItemSignal(curr_phase_id_);
    }
}

void PhaseeditDlg::OnCancelButtonClicked()
{
    close();
}

void PhaseeditDlg::InitPage()
{
    setWindowTitle(STRING_UI_PHASE_TABLE + "-" + STRING_UI_EDIT);
    QLabel* phase_id_label = new QLabel(STRING_UI_PHASE_ID);
    phase_id_cmb_ = new QComboBox;
    enable_phase_chk_ = new QCheckBox(STRING_UI_PHASE_ENABLE);
    enable_phase_chk_->setChecked(true);
    enable_phase_chk_->setEnabled(false);

    QLabel* man_green_time_label = new QLabel(STRING_UI_PHASE_MAN_GREEN);
    QLabel* man_clear_time_label = new QLabel(STRING_UI_PHASE_MAN_CLEAR);
    QLabel* min_green_time_label = new QLabel(STRING_UI_PHASE_MIN_GREEN);
    QLabel* delay_green_time_label = new QLabel(STRING_UI_PHASE_UNIT_GREEN);
    QLabel* max1_green_time_label = new QLabel(STRING_UI_PHASE_MAX_GREEN1);
    QLabel* max2_green_time_label = new QLabel(STRING_UI_PHASE_MAX_GREEN2);
    QLabel* fix_green_time_label = new QLabel(STRING_UI_PHASE_FIX_GREEN);
    QLabel* flash_green_time_label = new QLabel(STRING_UI_PHASE_GREEN_FLASH);
    QLabel* phase_mode_label = new QLabel(STRING_UI_PHASE_TYPE);

    man_green_time_spinbox_ = new QSpinBox;
    man_clear_time_spinbox_ = new QSpinBox;
    min_green_time_spinbox_ = new QSpinBox;
    delay_green_time_spinbox_ = new QSpinBox;
    max1_green_time_spinbox_ = new QSpinBox;
    max2_green_time_spinbox_ = new QSpinBox;
    fix_green_time_spinbox_ = new QSpinBox;
    green_flash_time_spinbox_ = new QSpinBox;

    man_green_time_spinbox_->setRange(0, 255);
    man_clear_time_spinbox_->setRange(0, 255);
    min_green_time_spinbox_->setRange(0, 255);
    delay_green_time_spinbox_->setRange(0, 255);
    max1_green_time_spinbox_->setRange(0, 255);
    max2_green_time_spinbox_->setRange(0, 255);
    fix_green_time_spinbox_->setRange(0, 255);
    green_flash_time_spinbox_->setRange(0, 255);

    phase_mode_cmb_ = new QComboBox;

    // additional phases
    phase_mode_cmb_->addItem(STRING_UI_PHASE_FIX);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_DETERMINED);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_ELASTICITY);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_CRUTIAL);

    phase_mode_cmb_->addItem(STRING_UI_PHASE_DELAY);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_WALKMAN);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_BIKE);
    phase_mode_cmb_->addItem(STRING_UI_PHASE_MOTOR);

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(phase_id_label);
    hlayout->addWidget(phase_id_cmb_);
    hlayout->addWidget(enable_phase_chk_);
    hlayout->addStretch(3);

    QGridLayout* grid_layout = new QGridLayout;
    grid_layout->addWidget(man_green_time_label, 0, 0, 1, 1);
    grid_layout->addWidget(man_green_time_spinbox_, 0, 1, 1, 1);
    grid_layout->addWidget(man_clear_time_label, 0, 3, 1, 1);
    grid_layout->addWidget(man_clear_time_spinbox_, 0, 4, 1, 1);
    grid_layout->addWidget(min_green_time_label, 0, 6, 1, 1);
    grid_layout->addWidget(min_green_time_spinbox_, 0, 7, 1, 1);

    grid_layout->addWidget(delay_green_time_label, 1, 0, 1, 1);
    grid_layout->addWidget(delay_green_time_spinbox_, 1, 1, 1, 1);
    grid_layout->addWidget(max1_green_time_label, 1, 3, 1, 1);
    grid_layout->addWidget(max1_green_time_spinbox_, 1, 4, 1, 1);
    grid_layout->addWidget(max2_green_time_label, 1, 6, 1, 1);
    grid_layout->addWidget(max2_green_time_spinbox_, 1, 7, 1, 1);

    grid_layout->addWidget(fix_green_time_label, 2, 0, 1, 1);
    grid_layout->addWidget(fix_green_time_spinbox_, 2, 1, 1, 1);
    grid_layout->addWidget(flash_green_time_label, 2, 3, 1, 1);
    grid_layout->addWidget(green_flash_time_spinbox_, 2, 4, 1, 1);
    grid_layout->addWidget(phase_mode_label, 2, 6, 1, 1);
    grid_layout->addWidget(phase_mode_cmb_, 2, 7, 1, 1);

    QGroupBox* phase_optional_func_grp = new QGroupBox(STRING_UI_PHASE_OPTIONAL);
    man_corss_street_chk_ = new QCheckBox(STRING_UI_PHASE_CROSS_STREET);
    to_be_determined_chk_ = new QCheckBox(STRING_UI_PHASE_TO_BE_DETERMINED);
    man_motor_chk_ = new QCheckBox(STRING_UI_PHASE_MAN_MOTOR_TOGETHER);
    together_chk_ = new QCheckBox(STRING_UI_PHASE_TOGETHER);
    demote_chk_ = new QCheckBox(STRING_UI_PHASE_DEMOTE);
    QLabel* detector_num_label = new QLabel(STRING_UI_PHASE_DETECTOR_NUM);
    detector_num_spinbox_ = new QSpinBox;
	detector_num_spinbox_->setRange(0, 7);

    man_corss_street_chk_->setEnabled(false);
    to_be_determined_chk_->setEnabled(false);
    man_motor_chk_->setEnabled(false);
    together_chk_->setEnabled(false);
    demote_chk_->setEnabled(false);

    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(man_corss_street_chk_);
    fir_hlayout->addWidget(to_be_determined_chk_);
    QHBoxLayout* sec_hlayout = new QHBoxLayout;
    sec_hlayout->addWidget(man_motor_chk_);
    sec_hlayout->addWidget(together_chk_);
    QHBoxLayout* third_hlayout = new QHBoxLayout;
    third_hlayout->addWidget(demote_chk_);
    third_hlayout->addWidget(detector_num_label);
    third_hlayout->addWidget(detector_num_spinbox_);
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addLayout(sec_hlayout);
    vlayout->addLayout(third_hlayout);
    phase_optional_func_grp->setLayout(vlayout);

    QGroupBox* phase_channel_grp = new QGroupBox(STRING_UI_PHASE_CHANNELS);
    QString str;
    for (int i = 1; i <= 32; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        channel_list_.append(chk);
    }

    QGridLayout* glayout = new QGridLayout;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (cnt < channel_list_.size())
            {
                glayout->addWidget(channel_list_.at(cnt), i, j, 1, 1);
                cnt++;
            }
        }
    }
    phase_channel_grp->setLayout(glayout);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);
    QHBoxLayout* bottom_hlayout = new QHBoxLayout;
    bottom_hlayout->addStretch(3);
    bottom_hlayout->addWidget(ok_button_);
    bottom_hlayout->addWidget(cancel_button_);

    QVBoxLayout* whole_vlayout = new QVBoxLayout;
    whole_vlayout->addLayout(hlayout);
    whole_vlayout->addLayout(grid_layout);
    whole_vlayout->addWidget(phase_optional_func_grp);
    whole_vlayout->addWidget(phase_channel_grp);
    whole_vlayout->addLayout(bottom_hlayout);
    setLayout(whole_vlayout);
}

void PhaseeditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
    connect(phase_mode_cmb_, SIGNAL(activated(QString)), this, SLOT(OnPhaseTypeSelected(QString)));
    connect(demote_chk_, SIGNAL(clicked(bool)), detector_num_spinbox_, SLOT(setEnabled(bool)));
}

void PhaseeditDlg::UpdateUI()
{
    ResetUI();
    if (curr_phase_id_ > 0)
    {
        for (int i = 0; i < phase_id_cmb_->count(); i++)
        {
            if (curr_phase_id_ == phase_id_cmb_->itemText(i).toInt())
            {
                phase_id_cmb_->setCurrentIndex(i);
            }
        }
    }
    PhaseParam phase;
    handler_->get_phase(curr_phase_id_, phase);

    man_green_time_spinbox_->setValue(phase.phase_walk_green);
    man_clear_time_spinbox_->setValue(phase.phase_walk_clear);
    min_green_time_spinbox_->setValue(phase.phase_min_green);
    if (phase.phase_green_delay == 0)
    {
        delay_green_time_spinbox_->setValue(3);
    }
    else
    {
        delay_green_time_spinbox_->setValue(phase.phase_green_delay);
    }
    max1_green_time_spinbox_->setValue(phase.phase_max_green1);
    max2_green_time_spinbox_->setValue(phase.phase_max_green2);
    fix_green_time_spinbox_->setValue(phase.phase_fix_green);
    if (phase.phase_green_flash == 0)
    {
        green_flash_time_spinbox_->setValue(4);
    }
    else
    {
        green_flash_time_spinbox_->setValue(phase.phase_green_flash);
    }
    unsigned char spec_func = phase.phase_spec_func;
    spec_func &= 0xFe;
    detector_num_ = spec_func;
    detector_num_ = detector_num_ >> 5;
    UpdatePhaseTypeInfo(phase.phase_type, detector_num_);

    unsigned int channel_ids = phase.phase_channel;
    for (int i = 0; i < channel_list_.size(); i++)
    {
        if ((channel_ids & 0x01) == 0x01)
        {
            channel_list_.at(i)->setChecked(true);
        }
        channel_ids = channel_ids >> 1;
    }
}

void PhaseeditDlg::ResetUI()
{
    phase_id_cmb_->clear();
    QString str;
    QList<PhaseParam> phase_list = handler_->get_phase_list();
    QList<unsigned char> phase_id_list;
    for (unsigned char i = 1; i <= MAX_PHASE_LINE; i++)
    {
        phase_id_list.append(i);
    }
    for (unsigned char i = 1; i <= MAX_PHASE_LINE; i++)
    {
        for (int j = 0; j < phase_list.size(); j++)
        {
            if (!(curr_phase_id_ == phase_list.at(j).phase_id || i != phase_list.at(j).phase_id))
            {
                phase_id_list.removeOne(phase_list.at(j).phase_id);
            }
        }
    }
    for (int i = 0; i < phase_id_list.size(); i++)
    {
        str.sprintf("%d", phase_id_list.at(i));
        phase_id_cmb_->addItem(str);
    }

    for (int i = 0; i < channel_list_.size(); i++)
    {
        channel_list_.at(i)->setChecked(false);
    }
    detector_num_spinbox_->setValue(0);
    detector_num_spinbox_->setEnabled(false);
}

void PhaseeditDlg::UpdatePhaseTypeInfo(unsigned char phase_type, unsigned char detector_num)
{
    if ((phase_type & 0x080) == 0x080)
    {
        phase_mode_cmb_->setCurrentIndex(0);
    }
    else if ((phase_type & 0x040) == 0x040)
    {
        phase_mode_cmb_->setCurrentIndex(1);
        demote_chk_->setEnabled(true);
//        if (detector_num != 0)
//        {
            demote_chk_->setChecked(true);
//        }
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x020) == 0x020)
    {
        phase_mode_cmb_->setCurrentIndex(2);
        demote_chk_->setEnabled(true);
//        if (detector_num != 0)
//        {
            demote_chk_->setChecked(true);
//        }
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x010) == 0x010)
    {
        phase_mode_cmb_->setCurrentIndex(3);
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x08) == 0x08)
    {
        phase_mode_cmb_->setCurrentIndex(4);
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x04) == 0x04)
    {
        phase_mode_cmb_->setCurrentIndex(5);
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x02) == 0x02)
    {
        phase_mode_cmb_->setCurrentIndex(6);
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
    else if ((phase_type & 0x01) == 0x01)
    {
        phase_mode_cmb_->setCurrentIndex(7);
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num);
    }
}

PhaseeditDlg::PhaseErr PhaseeditDlg::ValidateUI()
{
    int min_green = min_green_time_spinbox_->value();
    int max_green1 = max1_green_time_spinbox_->value();
    int delay_time = delay_green_time_spinbox_->value();
    if (delay_time != 0 && delay_time >= min_green)
    {
        return DelayLessThanMinGreen;
    }

    if (min_green > max_green1)
    {
        return MinLargerThanMax1;
    }
    if (green_flash_time_spinbox_->value() == 0)
    {
        return GreenFlashZero;
    }
    if ((phase_mode_cmb_->currentText() == QString(STRING_UI_PHASE_WALKMAN)))
    {
        if ((man_green_time_spinbox_->value() == 0) || (man_clear_time_spinbox_->value() == 0))
        {
            return ManGreenClearZero;
        }
        else if (man_clear_time_spinbox_->value() >= man_green_time_spinbox_->value())
        {
            return ManClearLessGreen;
        }
    }
    if (containsPedestrianChannel())
    {
        if (man_clear_time_spinbox_->value() >= min_green_time_spinbox_->value())
        {
                return ManClearLessMinGreen;
        }
        else if (man_clear_time_spinbox_->value() == 0)
        {
            return ManClearZero;
        }
    }
    if (phase_mode_cmb_->currentText() == QString(STRING_UI_PHASE_FIX))
    {
        if (fix_green_time_spinbox_->value() == 0)
        {
            return FixGreenZero;
        }
        else if (containsPedestrianChannel()
                 && (man_clear_time_spinbox_->value() == 0 || man_clear_time_spinbox_->value() >= fix_green_time_spinbox_->value()))
        {
            return FixManClearLessFix;
        }
    }
    return None;
}

bool PhaseeditDlg::SaveData()
{
    PhaseParam phase;
    phase.phase_id = phase_id_cmb_->currentText().toInt();
    phase.phase_walk_green = man_green_time_spinbox_->value();
    phase.phase_walk_clear = man_clear_time_spinbox_->value();
    phase.phase_min_green = min_green_time_spinbox_->value();
    phase.phase_fix_green = fix_green_time_spinbox_->value();
    phase.phase_green_delay = delay_green_time_spinbox_->value();
    phase.phase_green_flash = green_flash_time_spinbox_->value();
    phase.phase_max_green1 = max1_green_time_spinbox_->value();
    phase.phase_max_green2 = max2_green_time_spinbox_->value();
    phase.phase_channel = get_channels();
    phase.phase_type = handler_->get_phase_type_by_desc(phase_mode_cmb_->currentText().trimmed());
    phase.phase_spec_func = get_spec_func();
    qDebug() << "phase spec_func:" << phase.phase_spec_func;

    if (phase.phase_type == 0x040)      // determined phase
    {
        if (!greenConflictAvoidCheck(phase))
        {
            return false;
        }
    }
    // channels to be determined
    handler_->set_phase(curr_phase_id_, phase);
    curr_phase_id_ = phase.phase_id;
    return true;
}

bool PhaseeditDlg::containsPedestrianChannel()
{
    bool state = false;
    for (int i = 12; i < channel_list_.size(); i++)
    {
        if (channel_list_.at(i)->isChecked())
        {
            state = true;
            break;
        }
    }
    return state;
}

bool PhaseeditDlg::greenConflictAvoidCheck(const PhaseParam &phase_param)
{
    int index = -1;
    unsigned char phase_id = phase_param.phase_id;
    QList<unsigned char> phase_id_list = handler_->get_phase_id_list();
    for (int i = 0; i < phase_id_list.size()-1; i++)
    {
        if (phase_id > phase_id_list.at(i) && phase_id <= phase_id_list.at(i+1))
        {
            index = i;
            break;
        }
        else if (phase_id == phase_id_list.at(i))
        {
            index = i-1;
            break;
        }
    }
    if (index == -1)
    {
        index = phase_id_list.size() - 1;
    }
    PhaseParam param;
    if (!handler_->get_phase(phase_id_list.at(index), param))
    {
        return false;
    }

    qDebug() << "index:" << index << endl
             << "curr_phase_id:" << phase_param.phase_id << "pre_phase_id:" << param.phase_id << endl
             << "curr_phase_channel:" << phase_param.phase_channel << endl
             << "pre_phase_channel:" << param.phase_channel;

    if ((phase_param.phase_id != param.phase_id) && (phase_param.phase_channel & param.phase_channel) != 0x00)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_PHASE_AVOID_GREEN_CONFL_TIP, STRING_OK);
        return false;
    }

    return true;
}

void PhaseeditDlg::OnPhaseTypeSelected( const QString& text )
{
    if (text == STRING_UI_PHASE_ELASTICITY || text == STRING_UI_PHASE_DETERMINED
            || text == STRING_UI_PHASE_MOTOR || text == STRING_UI_PHASE_CRUTIAL
            || text == STRING_UI_PHASE_BIKE || text == STRING_UI_PHASE_WALKMAN
            || text == STRING_UI_PHASE_DELAY)
    {
        demote_chk_->setEnabled(true);
        demote_chk_->setChecked(true);
        detector_num_spinbox_->setEnabled(true);
        detector_num_spinbox_->setValue(detector_num_);
    }
    else
    {
        demote_chk_->setEnabled(false);
        demote_chk_->setChecked(false);
        detector_num_spinbox_->setEnabled(false);
        detector_num_spinbox_->setValue(0);
    }
}

unsigned int PhaseeditDlg::get_channels()
{
	unsigned int channel_ids = 0;
	for (int i = channel_list_.size() - 1; i >= 0; i--)
	{
		channel_ids = channel_ids << 1;
		if (channel_list_.at(i)->isChecked())
		{
			channel_ids |= 0x01;
		}
    }
    qDebug() << "channel_ids:" << channel_ids;
    return channel_ids;
}

unsigned char PhaseeditDlg::get_spec_func()
{
    unsigned char spec = 0x01;
    if (detector_num_spinbox_->isEnabled())
    {
        detector_num_ = detector_num_spinbox_->value();
        spec |= (detector_num_ << 5);
    }
    return spec;
}

PhaseeditDlg::~PhaseeditDlg()
{

}
