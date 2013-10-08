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
    if (checked)
    {
        emit scheduleplanSignal(STRING_UI_SCHEDULE_PLAN);
        ResetButtonStatus(schedule_plan_button_);
    }
}

void ConfigoptWidget::OnTimesectionButtonToggled(bool checked)
{
    if (checked)
    {
        emit timesectionSignal(STRING_UI_TIME_SECTION);
        ResetButtonStatus(time_table_button_);
    }
}

void ConfigoptWidget::OnTimingplanButtonToggled(bool checked)
{
    if (checked)
    {
        emit timingplanSignal(STRING_UI_TIMING_PLAN);
        ResetButtonStatus(timing_plan_button_);
    }
}

void ConfigoptWidget::OnPhasetimingButtonToggled(bool checked)
{
    if (checked)
    {
        emit phasetimingSignal(STRING_UI_PHASE_TIMING);
        ResetButtonStatus(phase_timing_button_);
    }
}

void ConfigoptWidget::OnPhasetableButtonToggled(bool checked)
{
    if (checked)
    {
        emit phasetableSignal(STRING_UI_PHASE_TABLE);
        ResetButtonStatus(phase_table_button_);
    }
}

void ConfigoptWidget::OnPhaseconflictButtonToggled(bool checked)
{
    if (checked)
    {
        emit phaseconflictSignal(STRING_UI_PHASE_CONFLICT);
        ResetButtonStatus(phase_conflict_button_);
    }
}

void ConfigoptWidget::OnDetectorButtonToggled(bool checked)
{
    if (checked)
    {
        emit detectorSignal(STRING_UI_DETECTOR);
        ResetButtonStatus(detector_button_);
    }
}

void ConfigoptWidget::OnUnitparamButtonToggled(bool checked)
{
    if (checked)
    {
        emit unitparamSignal(STRING_UI_UNIT_TABLE);
        ResetButtonStatus(unit_param_button_);
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

    unit_param_button_->setCheckable(true);
    schedule_plan_button_->setCheckable(true);
    time_table_button_->setCheckable(true);
    timing_plan_button_->setCheckable(true);
    phase_timing_button_->setCheckable(true);
    phase_table_button_->setCheckable(true);
    phase_conflict_button_->setCheckable(true);
    channel_button_->setCheckable(true);
    detector_button_->setCheckable(true);

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
    //connect(channel_button_, SIGNAL(clicked()), this, SLOT(OnChannelButtonToggled));
    connect(channel_button_, SIGNAL(toggled(bool)), this, SLOT(OnChannelButtonToggled(bool)));
    connect(detector_button_, SIGNAL(toggled(bool)), this, SLOT(OnDetectorButtonToggled(bool)));

    button_list_.append(unit_param_button_);
    button_list_.append(schedule_plan_button_);
    button_list_.append(time_table_button_);
    button_list_.append(timing_plan_button_);
    button_list_.append(phase_timing_button_);
    button_list_.append(phase_table_button_);
    button_list_.append(phase_conflict_button_);
    button_list_.append(channel_button_);
    button_list_.append(detector_button_);
}

void ConfigoptWidget::ResetButtonStatus(const QPushButton *self_btn)
{
    for (int i = 0; i < button_list_.size(); i++)
    {
        if (button_list_.at(i) != self_btn)
        {
            button_list_.at(i)->setChecked(false);
        }
    }
}

void ConfigoptWidget::OnChannelButtonToggled(bool checked)
{
    if (checked)
    {
        emit channelSignal(STRING_UI_CHANNEL);
        ResetButtonStatus(channel_button_);
    }
}
