#include "configoptwidget.h"
#include "macrostring.h"

ConfigoptWidget::ConfigoptWidget(QWidget *parent) :
    QWidget(parent)
{
    InitPage();
    InitSignalSlots();
}

void ConfigoptWidget::OnScheduleplanButtonClicked()
{
    emit scheduleplanSignal(STRING_UI_SCHEDULE_PLAN);
}

void ConfigoptWidget::OnTimesectionButtonClicked()
{
    emit timesectionSignal(STRING_UI_TIME_SECTION);
}

void ConfigoptWidget::OnTimingplanButtonClicked()
{
    emit timingplanSignal(STRING_UI_TIMING_PLAN);
}

void ConfigoptWidget::OnPhasetimingButtonClicked()
{
    emit phasetimingSignal(STRING_UI_PHASE_TIMING);
}

void ConfigoptWidget::OnPhasetableButtonClicked()
{
    emit phasetableSignal(STRING_UI_PHASE_TABLE);
}

void ConfigoptWidget::OnPhaseconflictButtonClicked()
{
    emit phaseconflictSignal(STRING_UI_PHASE_CONFLICT);
}

void ConfigoptWidget::OnChannelButtonClicked()
{
    emit channelSignal(STRING_UI_CHANNEL);
}

void ConfigoptWidget::OnDetectorButtonClicked()
{
    emit detectorSignal(STRING_UI_DETECTOR);
}

void ConfigoptWidget::OnUnitparamButtonClicked()
{
    emit unitparamSignal(STRING_UI_UNIT_TABLE);
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
	connect(unit_param_button_, SIGNAL(clicked()), this, SLOT(OnUnitparamButtonClicked()));
    connect(schedule_plan_button_, SIGNAL(clicked()), this, SLOT(OnScheduleplanButtonClicked()));
    connect(time_table_button_, SIGNAL(clicked()), this, SLOT(OnTimesectionButtonClicked()));
    connect(timing_plan_button_, SIGNAL(clicked()), this, SLOT(OnTimingplanButtonClicked()));
    connect(phase_timing_button_, SIGNAL(clicked()), this, SLOT(OnPhasetimingButtonClicked()));
    connect(phase_table_button_, SIGNAL(clicked()), this, SLOT(OnPhasetableButtonClicked()));
    connect(phase_conflict_button_, SIGNAL(clicked()), this, SLOT(OnPhaseconflictButtonClicked()));
    connect(channel_button_, SIGNAL(clicked()), this, SLOT(OnChannelButtonClicked()));
    connect(detector_button_, SIGNAL(clicked()), this, SLOT(OnDetectorButtonClicked()));
}
