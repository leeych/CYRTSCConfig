#include "timingeditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

TimingeditDlg::TimingeditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void TimingeditDlg::Initialize(unsigned char id, TimingHandler* handler)
{
    curr_timing_id_ = id;
    handler_ = handler;
    UpdateUI();
    exec();
}

void TimingeditDlg::OnOkButtonClicked()
{
    if (SaveData())
    {
        accept();
        emit updateTableRow(curr_timing_id_);
    }
}

void TimingeditDlg::OnCancelButtonClicked()
{
    close();
}

void TimingeditDlg::OnStageTimingIdSelected(QString str)
{
    unsigned char stage_timing_id = str.toInt();
    unsigned char cycle_time = handler_->get_cycletime_by_stagetiming_id(stage_timing_id);
    cycle_spinbox_->setValue(cycle_time);
}

void TimingeditDlg::InitPage()
{
	setWindowTitle(STRING_UI_TIMING_PLAN + "-" + STRING_UI_EDIT);
    timing_id_cmb_ = new QComboBox;
    phase_timing_id_cmb_ = new QComboBox;
    coor_phase_cmb_ = new QComboBox;
	QString str;
	for (int i = 0; i <= MAX_TIMECONFIG_LINE; i++)
	{
		str.sprintf("%d", i);
		phase_timing_id_cmb_->addItem(str);
	}
	for (int i = 0; i <= 32; i++)
	{
		str.sprintf("%d", i);
		coor_phase_cmb_->addItem(str);
	}

    cycle_spinbox_ = new QSpinBox;
    phase_spinbox_ = new QSpinBox;
    cycle_spinbox_->setRange(0, 255);
    phase_spinbox_->setRange(0, 255);

    QLabel* timing_id_label = new QLabel(STRING_UI_TIMING_ID);
    QLabel* cycle_label = new QLabel(STRING_UI_TIMING_CYCLE);
    QLabel* phase_label = new QLabel(STRING_UI_TIMING_PHASE);
    QLabel* coor_phase_label = new QLabel(STRING_UI_TIMING_COOR_PHASE);
    QLabel* phase_timing_label = new QLabel(STRING_UI_PHASETIMING_ID);

    QGridLayout* grid_layout = new QGridLayout;
    grid_layout->addWidget(timing_id_label, 0, 0, 1, 1);
    grid_layout->addWidget(timing_id_cmb_, 0, 1, 1, 1);
    grid_layout->addWidget(coor_phase_label, 0, 3, 1, 1);
    grid_layout->addWidget(coor_phase_cmb_, 0, 4, 1, 1);
    grid_layout->addWidget(cycle_label, 1, 0, 1, 1);
    grid_layout->addWidget(cycle_spinbox_, 1, 1, 1, 1);
    grid_layout->addWidget(phase_label, 1, 3, 1, 1);
    grid_layout->addWidget(phase_spinbox_, 1, 4, 1, 1);
    grid_layout->addWidget(phase_timing_label, 2, 0, 1, 1);
    grid_layout->addWidget(phase_timing_id_cmb_, 2, 1, 1, 1);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addStretch(3);
    hlayout->addWidget(ok_button_);
    hlayout->addWidget(cancel_button_);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(grid_layout);
    vlayout->addLayout(hlayout);
    setLayout(vlayout);
}

void TimingeditDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));

    connect(phase_timing_id_cmb_, SIGNAL(activated(QString)), this, SLOT(OnStageTimingIdSelected(QString)));
}

void TimingeditDlg::ResetUI()
{
    timing_id_cmb_->clear();
    QList<TimingParam> timing_list = handler_->get_timing_plan_list();
    QList<unsigned char> timing_id_list;
    for (int i = 1; i <= MAX_PATTERN_LINE; i++)
    {
        timing_id_list.append(i);
    }
    for (unsigned char i = 1; i <= MAX_PATTERN_LINE; i++)
    {
        for (int j = 0; j < timing_list.size(); j++)
        {
            if (curr_timing_id_ != timing_list.at(j).timing_id && i == timing_list.at(j).timing_id)
            {
                timing_id_list.removeOne(timing_list.at(j).timing_id);
            }
        }
    }
    QString str;
    for (int i = 0; i < timing_id_list.size(); i++)
    {
        str.sprintf("%d", timing_id_list.at(i));
        timing_id_cmb_->addItem(str);
    }

    cycle_spinbox_->setEnabled(false);
}

void TimingeditDlg::UpdateUI()
{
    ResetUI();
    if (curr_timing_id_ > 0)
    {
        for (int i = 0; i < timing_id_cmb_->count(); i++)
        {
            if (timing_id_cmb_->itemText(i).toInt() == curr_timing_id_)
            {
                timing_id_cmb_->setCurrentIndex(i);
                break;
            }
        }
    }
    TimingParam plan;

    handler_->get_timing_plan(curr_timing_id_, plan);
    phase_timing_id_cmb_->setCurrentIndex(plan.stage_timing_id);
    cycle_spinbox_->setValue(plan.cycle_time);
    phase_spinbox_->setValue(plan.phase_offset);
    coor_phase_cmb_->setCurrentIndex(plan.coor_phase);
}

bool TimingeditDlg::SaveData()
{
    TimingParam plan;
    plan.timing_id = timing_id_cmb_->currentText().toInt();
    plan.cycle_time = cycle_spinbox_->value();
    plan.stage_timing_id = phase_timing_id_cmb_->currentText().toInt();
    plan.phase_offset = phase_spinbox_->value();
    plan.coor_phase = coor_phase_cmb_->currentText().toInt();

    handler_->set_timing_plan(curr_timing_id_, plan);
    curr_timing_id_ = plan.timing_id;

    return true;
}

TimingeditDlg::~TimingeditDlg()
{

}
