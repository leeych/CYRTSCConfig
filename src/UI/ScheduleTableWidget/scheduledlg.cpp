#include "scheduledlg.h"
#include "macrostring.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QMessageBox>

ScheduleDlg::ScheduleDlg(QWidget *parent) :
    QDialog(parent), handler_(NULL)
{
    InitPage();
    InitSignalSlots();
}

void ScheduleDlg::Initialize( unsigned char id, ScheduleHandler* handler)
{
    handler_ = handler;
    id_ = id;
    ScheduleParam schedule;
    if (handler_->get_schedule(id, schedule))
    {
        month_ = schedule.schedule_month;
        week_ = schedule.schedule_week;
        day_ = schedule.schedule_day;
    }
    UpdateUI(id);
    exec();
}

void ScheduleDlg::OnOkButtonClicked()
{
    if (SaveData())
    {
        accept();
        emit updateTableSignal(id_);
    }
}

void ScheduleDlg::OnCancelButtonClicked()
{
    close();
}

void ScheduleDlg::OnMonthAllRadioButtonChecked(bool flag)
{
    if (!flag)
    {
        return;
    }
    for (int i = 0; i < month_list_.size(); i++)
    {
        month_list_.at(i)->setChecked(true);
    }
}

void ScheduleDlg::OnMonthNoneRadioButtonChecked(bool flag)
{
    if (flag)
    {
        for (int i = 0; i < month_list_.size(); i++)
        {
            month_list_.at(i)->setChecked(false);
        }
    }
}

void ScheduleDlg::OnWeekdayAllRadioButtonChecked(bool state)
{
    if (state)
    {
        for (int i = 0; i < weekday_list_.size(); i++)
        {
            weekday_list_.at(i)->setChecked(true);
        }
    }
}

void ScheduleDlg::OnWeekdayNoneRadioButtonChecked(bool state)
{
    if (state)
    {
        for (int i = 0; i < weekday_list_.size(); i++)
        {
            weekday_list_.at(i)->setChecked(false);
        }
    }
}

void ScheduleDlg::OnDayAllRadioButtonChecked(bool state)
{
    if (state)
    {
        for (int i = 0; i < day_list_.size(); i++)
        {
            day_list_.at(i)->setChecked(true);
        }
    }
}

void ScheduleDlg::OnDayNoneRadioButtonChecked(bool state)
{
    if (state)
    {
        for (int i = 0; i < day_list_.size(); i++)
        {
            day_list_.at(i)->setChecked(false);
        }
    }
}

void ScheduleDlg::InitPage()
{
	setWindowTitle(STRING_UI_SCHEDULE_PLAN + "-" + STRING_UI_EDIT);
    QLabel* available_schedule_id_label = new QLabel(STRING_UI_SCHE_DLG_AVAILABLE + ":");
    available_schedule_id_cmb_ = new QComboBox;
    available_schedule_id_cmb_->setMinimumWidth(40);
    QString str;
//    for (int i = 1; i <= 40; i++)
//    {
//        available_schedule_id_cmb_->addItem(str.sprintf("%d", i));
//    }

    QLabel* timesection_id_label = new QLabel(STRING_UI_SCHE_DLG_TIMESECTION);
    timesection_id_cmb_ = new QComboBox;
    timesection_id_cmb_->setMinimumWidth(40);
    for (int i = 0; i <= 16; i++)
    {
        timesection_id_cmb_->addItem(str.sprintf("%d", i));
    }

    QHBoxLayout* id_hlayout = new QHBoxLayout;
    id_hlayout->addWidget(available_schedule_id_label);
    id_hlayout->addWidget(available_schedule_id_cmb_);
    id_hlayout->addStretch(1);
    id_hlayout->addWidget(timesection_id_label);
    id_hlayout->addWidget(timesection_id_cmb_);
    QGroupBox* id_grp = new QGroupBox;
    id_grp->setLayout(id_hlayout);

    QLabel* month_schedule_label = new QLabel(STRING_UI_SCHE_DLG_MONTH + ":");
    month_all_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_ALL);
    month_none_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_NONE);
	month_hidden_rbt_ = new QRadioButton;

	QButtonGroup *month_btn_grp = new QButtonGroup;
	month_btn_grp->addButton(month_all_rbt_, 1);
	month_btn_grp->addButton(month_none_rbt_, 2);
	month_btn_grp->addButton(month_hidden_rbt_, 3);
	month_hidden_rbt_->setChecked(true);

    for (int i = 1; i <= 12; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        chk->setChecked(false);
        month_list_.append(chk);
    }
    QHBoxLayout* rbt_hlayout = new QHBoxLayout;
    rbt_hlayout->addWidget(month_schedule_label);
    rbt_hlayout->addWidget(month_all_rbt_);
    rbt_hlayout->addWidget(month_none_rbt_);
	rbt_hlayout->addWidget(month_hidden_rbt_);
	month_hidden_rbt_->setVisible(false);
    rbt_hlayout->addStretch(3);
    QHBoxLayout* month_hlayout = new QHBoxLayout;
    for (int i = 0; i < month_list_.size(); i++)
    {
        month_hlayout->addWidget(month_list_.at(i));
    }
    QVBoxLayout* month_vlayout = new QVBoxLayout;
    month_vlayout->addLayout(rbt_hlayout);
    month_vlayout->addLayout(month_hlayout);
    QGroupBox* month_grp = new QGroupBox;
    month_grp->setLayout(month_vlayout);

    QLabel* weekday_schedule_label = new QLabel(STRING_UI_SCHE_DLG_WEEKDAY + ":");
    weekday_all_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_ALL);
    weekday_none_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_NONE);
	weekday_hidden_rbt_ = new QRadioButton;

	QButtonGroup *weekday_btn_grp = new QButtonGroup;
	weekday_btn_grp->addButton(weekday_all_rbt_, 1);
	weekday_btn_grp->addButton(weekday_none_rbt_, 2);
	weekday_btn_grp->addButton(weekday_hidden_rbt_, 3);
	weekday_hidden_rbt_->setChecked(true);

    mon_chk_ = new QCheckBox(STRING_UI_MONDAY);
    thue_chk_ = new QCheckBox(STRING_UI_THUESDAY);
    wed_chk_ = new QCheckBox(STRING_UI_WEDNESDAY);
    thur_chk_ = new QCheckBox(STRING_UI_THURSDAY);
    fri_chk_ = new QCheckBox(STRING_UI_FRIDAY);
    sat_chk_ = new QCheckBox(STRING_UI_SATURDAY);
    sun_chk_ = new QCheckBox(STRING_UI_SUNDAY);
    weekday_list_.append(mon_chk_);
    weekday_list_.append(thue_chk_);
    weekday_list_.append(wed_chk_);
    weekday_list_.append(thur_chk_);
    weekday_list_.append(fri_chk_);
    weekday_list_.append(sat_chk_);
    weekday_list_.append(sun_chk_);

    QHBoxLayout* weekrbt_hlayout = new QHBoxLayout;
    weekrbt_hlayout->addWidget(weekday_schedule_label);
    weekrbt_hlayout->addWidget(weekday_all_rbt_);
    weekrbt_hlayout->addWidget(weekday_none_rbt_);
	weekrbt_hlayout->addWidget(weekday_hidden_rbt_);
	weekday_hidden_rbt_->setVisible(false);
    weekrbt_hlayout->addStretch(3);
    QHBoxLayout* weekday_hlayout = new QHBoxLayout;
    for (int i = 0; i < weekday_list_.size(); i++)
    {
        weekday_hlayout->addWidget(weekday_list_.at(i));
    }
    weekday_hlayout->addStretch(2);
    QVBoxLayout* weekday_vlayout = new QVBoxLayout;
    weekday_vlayout->addLayout(weekrbt_hlayout);
    weekday_vlayout->addLayout(weekday_hlayout);
    QGroupBox* weekday_grp = new QGroupBox;
    weekday_grp->setLayout(weekday_vlayout);

    QLabel* day_schedule_label = new QLabel(STRING_UI_SCHE_DLG_DAY);
    day_all_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_ALL);
    day_none_rbt_ = new QRadioButton(STRING_UI_SCHE_DLG_NONE);
	day_hidden_rbt_ = new QRadioButton;

	QButtonGroup *day_btn_grp = new QButtonGroup;
	day_btn_grp->addButton(day_all_rbt_, 1);
	day_btn_grp->addButton(day_none_rbt_, 2);
	day_btn_grp->addButton(day_hidden_rbt_, 3);
	day_hidden_rbt_->setChecked(true);

    for (int i = 1; i <= 31; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        chk->setChecked(false);
        day_list_.append(chk);
    }

    QHBoxLayout* dayrbt_hlayout = new QHBoxLayout;
    dayrbt_hlayout->addWidget(day_schedule_label);
    dayrbt_hlayout->addWidget(day_all_rbt_);
    dayrbt_hlayout->addWidget(day_none_rbt_);
	dayrbt_hlayout->addWidget(day_hidden_rbt_);
	day_hidden_rbt_->setVisible(false);
    dayrbt_hlayout->addStretch(3);

    QGridLayout* glayout = new QGridLayout;
    int cnt = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (cnt != day_list_.size())
            {
                glayout->addWidget(day_list_.at(cnt), i, j, 1, 1);
                cnt++;
            }
            else
            {
                break;
            }
        }
    }

    QVBoxLayout* day_grp_vlayout = new QVBoxLayout;
    day_grp_vlayout->addLayout(dayrbt_hlayout);
    day_grp_vlayout->addLayout(glayout);
    QGroupBox* day_grp = new QGroupBox;
    day_grp->setLayout(day_grp_vlayout);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);
    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(3);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(cancel_button_);

    QVBoxLayout* whole_vlayout = new QVBoxLayout;
    whole_vlayout->addWidget(id_grp);
    whole_vlayout->addWidget(month_grp);
    whole_vlayout->addWidget(weekday_grp);
    whole_vlayout->addWidget(day_grp);
    whole_vlayout->addLayout(button_hlayout);
    setLayout(whole_vlayout);
    QString qss =  "QGroupBox {border-width:1px;border-style:solid;border-color:LightSkyBlue;margin-top: 1ex;}"
            "QGroupBox::title{subcontrol-origin:margin;subcontrol-position:top left;left:10px;margin-left:0px;padding:0px}"
            "QComboBox{background-color: rgb(233,246,254);border:1px solid rgb(128,171,224);border-radius:3px;}"
            "QComboBox QAbstractItemView{selection-background-color:rgb(128,171,224);}"
            "QComboBox::item{background-color: transparent;color: #000000;}"
            "QComboBox::item:selected{background-color: #83c8f9;}"
            "QWidget{background-color:#eaf6fe;}"
            "QPushButton{border:1px solid rgb(164,209,251);border-radius:3px;min-height:25px;min-width:80px;"
            "background-color:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 rgb(221,238,254),stop:1 rgb(197,226,255));}"
            "QPushButton:pressed{background-color:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 rgb(254,103,0),stop:1 rgb(254,103,0));}";
    setStyleSheet(qss);
}

void ScheduleDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));

    connect(month_all_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnMonthAllRadioButtonChecked(bool)));
    connect(month_none_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnMonthNoneRadioButtonChecked(bool)));
    connect(weekday_all_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnWeekdayAllRadioButtonChecked(bool)));
    connect(weekday_none_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnWeekdayNoneRadioButtonChecked(bool)));
    connect(day_all_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnDayAllRadioButtonChecked(bool)));
    connect(day_none_rbt_, SIGNAL(clicked(bool)), this, SLOT(OnDayNoneRadioButtonChecked(bool)));
}

void ScheduleDlg::UpdateUI(unsigned char sched_id)
{
    ResetUI();
    ScheduleParam schedule;
    handler_->get_schedule(sched_id, schedule);
    for (int i = 0; i < available_schedule_id_cmb_->count(); i++)
    {
        if (id_ == available_schedule_id_cmb_->itemText(i).toInt())
        {
            available_schedule_id_cmb_->setCurrentIndex(i);
            break;
        }
    }
    timesection_id_cmb_->setCurrentIndex(schedule.time_section_id);

    unsigned short month = schedule.schedule_month;
	int month_cnt = 0;
    for (int j = 0; j < month_list_.size(); j++)
    {
        month = (month >> 1);
        if ((month & 0x01) == 0x01)
        {
            month_list_.at(j)->setChecked(true);
			month_cnt++;
        }
    }
	if (month_cnt == month_list_.size())
	{
		month_all_rbt_->setChecked(true);
	}

    unsigned char week = schedule.schedule_week;
	int weekday_cnt = 0;
    for (int j = 0; j < weekday_list_.size(); j++)
    {
        week = (week >> 1);
        if ((week & 0x01) == 0x01)
        {
            weekday_list_.at(j)->setChecked(true);
			weekday_cnt++;
        }
    }
	if (weekday_cnt == weekday_list_.size())
	{
		weekday_all_rbt_->setChecked(true);
	}

    unsigned int day = schedule.schedule_day;
	int day_cnt = 0;
    for (int j = 0; j < day_list_.size(); j++)
    {
        day = (day >> 1);
        if ((day & 0x01) == 0x01)
        {
            day_list_.at(j)->setChecked(true);
			day_cnt++;
        }
    }
	if (day_cnt == day_list_.size())
	{
		day_all_rbt_->setChecked(true);
	}
}

void ScheduleDlg::ResetUI()
{
    available_schedule_id_cmb_->clear();
    QList<unsigned char> sched_id_list;
    for (unsigned char i = 1; i <= 40; i++)
    {
        sched_id_list.append(i);
    }
    QList<ScheduleParam> schedule_list = handler_->get_schedule_list();
    for (unsigned char i = 1; i <= 40; i++)
    {
        for (int j = 0; j < schedule_list.size(); j++)
        {
            if (id_ != schedule_list.at(j).schedule_id && i == schedule_list.at(j).schedule_id)
            {
                sched_id_list.removeOne(schedule_list.at(j).schedule_id);
            }
        }
    }
    QString str;
    for (int i = 0; i < sched_id_list.size(); i++)
    {
        str.sprintf("%d", sched_id_list.at(i));
        available_schedule_id_cmb_->addItem(str);
    }

	month_all_rbt_->setChecked(false);
	month_none_rbt_->setChecked(true);
	weekday_all_rbt_->setChecked(false);
	weekday_none_rbt_->setChecked(true);
	day_all_rbt_->setChecked(false);
	day_none_rbt_->setChecked(true);
    for (int i = 0; i < month_list_.size(); i++)
    {
        month_list_.at(i)->setChecked(false);
    }
    for (int i = 0; i < weekday_list_.size(); i++)
    {
        weekday_list_.at(i)->setChecked(false);
    }
    for (int i = 0; i < day_list_.size(); i++)
    {
        day_list_.at(i)->setChecked(false);
    }
}

bool ScheduleDlg::SaveData()
{
    SaveMonth();
    SaveWeek();
    SaveDay();

    ScheduleParam schedule;
    schedule.schedule_id = available_schedule_id_cmb_->currentText().toInt();
    schedule.time_section_id = timesection_id_cmb_->currentText().toInt();
    schedule.schedule_month = month_;
    schedule.schedule_week = week_;
    schedule.schedule_day = day_;
    handler_->set_schedule(id_, schedule);
    return true;
}

void ScheduleDlg::SaveMonth()
{
    unsigned short month = 0;
    for (int i = month_list_.size() - 1; i >= 0 ; i--)
    {
        if (month_list_.at(i)->isChecked())
        {
            month = (month | 0x01);
        }
        month = (month << 1);
    }
    month_ = month;
}

void ScheduleDlg::SaveWeek()
{
    unsigned char week = 0;
    for (int i = weekday_list_.size() - 1; i >= 0; i--)
    {
        if (weekday_list_.at(i)->isChecked())
        {
            week = (week | 0x01);
        }
        week = (week << 1);
    }
    week_ = week;
}

void ScheduleDlg::SaveDay()
{
    unsigned int day = 0;
    for (int i = day_list_.size() - 1; i >= 0; i--)
    {
        if (day_list_.at(i)->isChecked())
        {
            day = (day | 0x01);
        }
        day = (day << 1);
    }
    day_ = day;
}

ScheduleDlg::~ScheduleDlg()
{

}
