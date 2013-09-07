#include "configoptwidget.h"
#include "macrostring.h"

ConfigoptWidget::ConfigoptWidget(QWidget *parent) :
    QWidget(parent)
{
    InitPage();
    InitSignalSlots();
}

void ConfigoptWidget::OnScheduleplanButtonToggled(bool checked)
{
    emit scheduleplanSignal(STRING_UI_SCHEDULE_PLAN);
}

void ConfigoptWidget::OnTimesectionButtonToggled(bool checked)
{
    emit timesectionSignal(STRING_UI_TIME_SECTION);
}

void ConfigoptWidget::OnTimingplanButtonToggled(bool checked)
{
    emit timingplanSignal(STRING_UI_TIMING_PLAN);
}

void ConfigoptWidget::OnPhasetimingButtonToggled(bool checked)
{
    emit phasetimingSignal(STRING_UI_PHASE_TIMING);
}

void ConfigoptWidget::OnPhasetableButtonToggled(bool checked)
{
    emit phasetableSignal(STRING_UI_PHASE_TABLE);
}

void ConfigoptWidget::OnPhaseconflictButtonToggled(bool checked)
{
    emit phaseconflictSignal(STRING_UI_PHASE_CONFLICT);
}

void ConfigoptWidget::OnChannelButtonToggled(bool checked)
{
    emit channelSignal(STRING_UI_CHANNEL);
}

void ConfigoptWidget::OnDetectorButtonToggled(bool checked)
{
    if (checked)
    {
        emit detectorSignal(STRING_UI_DETECTOR);
    }
    else
    {
        ResetButtonStatus();
    }
}

void ConfigoptWidget::OnUnitparamButtonToggled(bool checked)
{
    if (checked)
    {
        emit unitparamSignal(STRING_UI_UNIT_TABLE);
    }
    else
    {
        ResetButtonStatus();
    }
}

void ConfigoptWidget::InitPage()
{
	unit_param_button_ = new QPushButton(STRING_UI_UNIT_TABLE);
    schedule_plan_button_ = new QPushButton(STRING_UI_SCHEDULE_PLAN);
    time_table_button_ = new QPushButton(STRING_UI_TIME_SECTION);
    timing_plan_button_ = new QPushButton(STRING_UI_TIMING_PLAN);
    phase_timing_button_ = new QPushButton(STRING_UI_PHASE_TIMING);
    phase_table_button_ = new QPushButton(STRING_UI_PHASE_TABLE);
    phase_conflict_button_ = new QPushButton(STRING_UI_PHASE_CONFLICT);
    channel_button_ = new QPushButton(STRING_UI_CHANNEL);
    detector_button_ = new QPushButton(STRING_UI_DETECTOR);

	QVBoxLayout* vlayout = new QVBoxLayout;
	vlayout->addWidget(unit_param_button_);
    vlayout->addWidget(schedule_plan_button_);
    vlayout->addWidget(time_table_button_);
    vlayout->addWidget(timing_plan_button_);
    vlayout->addWidget(phase_timing_button_);
    vlayout->addWidget(phase_table_button_);
    vlayout->addWidget(phase_conflict_button_);
    vlayout->addWidget(channel_button_);
    vlayout->addWidget(detector_button_);

    frame_ = new QFrame;
    frame_->setLayout(vlayout);
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(frame_);
	hlayout->setMargin(0);
	hlayout->setSpacing(0);
    setLayout(hlayout);
    QString frame_qss =
       "QFrame{border-width:1px;border-style:solid;border-color:LightSkyBlue;margin-top:1px;}";
    setStyleSheet(frame_qss);
}

void ConfigoptWidget::InitSignalSlots()
{
    connect(unit_param_button_, SIGNAL(toggled(bool)), this, SLOT(OnUnitparamButtonToggled(bool)));
    connect(schedule_plan_button_, SIGNAL(toggled(bool)), this, SLOT(OnScheduleplanButtonToggled(bool)));
    connect(time_table_button_, SIGNAL(toggled(bool)), this, SLOT(OnTimesectionButtonToggled(bool)));
    connect(timing_plan_button_, SIGNAL(toggled(bool)), this, SLOT(OnTimingplanButtonToggled(bool)));
    connect(phase_timing_button_, SIGNAL(toggled(bool)), this, SLOT(OnPhasetimingButtonToggled(bool)));
    connect(phase_table_button_, SIGNAL(toggled(bool)), this, SLOT(OnPhasetableButtonToggled(bool)));
    connect(phase_conflict_button_, SIGNAL(toggled(bool)), this, SLOT(OnPhaseconflictButtonToggled(bool)));
    connect(channel_button_, SIGNAL(toggled(bool)), this, SLOT(OnChannelButtonToggled(bool)));
    connect(detector_button_, SIGNAL(toggled(bool)), this, SLOT(OnDetectorButtonToggled(bool)));
}

void ConfigoptWidget::ResetButtonStatus()
{
    schedule_plan_button_->setChecked(false);
    time_table_button_->setChecked(false);
    timing_plan_button_->setChecked(false);
    phase_timing_button_->setChecked(false);
    phase_table_button_->setChecked(false);
    phase_table_button_->setChecked(false);
    phase_conflict_button_->setChecked(false);
    channel_button_->setChecked(false);
    detector_button_->setChecked(false);
    unit_param_button_->setChecked(false);
}
