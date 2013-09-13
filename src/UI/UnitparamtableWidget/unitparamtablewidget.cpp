#include "unitparamtablewidget.h"
#include "macrostring.h"
#include "mutility.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QRegExp>


UnitparamtableWidget::UnitparamtableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    handler_ = new UnitparamHandler;
    InitPage();
    InitSignalSlots();

    UpdateUI();

	flash_time_spinbox_->setValue(0);
	all_red_time_spinbox_->setValue(0);
}

const QString &UnitparamtableWidget::widget_name()
{
    return widget_name_;
}

void UnitparamtableWidget::OnOkButtonClicked()
{
	if (SaveData())
	{
		handler_->save_data();
	}
}

void UnitparamtableWidget::OnCancelButtonClicked()
{
	UpdateUI();
}

void UnitparamtableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void UnitparamtableWidget::InitPage()
{
    // QLabel* signaler_ip_label = new QLabel(STRING_UI_UNIT_SIGNAL_IP + ":");
    QLabel* server_ip_label = new QLabel(STRING_UI_UNIT_SERVER_IP + ":");
    QLabel* server_port_label = new QLabel(STRING_UI_UNIT_SERVER_PORT + ":");
    QLabel* flash_time_label = new QLabel(STRING_UI_UNIT_FLASH_TIME + ":");
    QLabel* allred_time_label = new QLabel(STRING_UI_UNIT_ALL_RED + ":");
    QLabel* curr_status_label = new QLabel(STRING_UI_UNIT_SIGNALER_STATUS + ":");

/*
    QFont font(STRING_FONT_SONGTI, 11);
	server_ip_label->setFont(font);
	server_port_label->setFont(font);
	flash_time_label->setFont(font);
	allred_time_label->setFont(font);
	curr_status_label->setFont(font);
*/
    // signal_ip_lineedit_ = new QLineEdit;
    server_ip_lineedit_ = new QLineEdit;
    QRegExp ipreg("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    QRegExpValidator *pIpValidator = new QRegExpValidator(ipreg);
    //signal_ip_lineedit_->setValidator(pIpValidator);
    server_ip_lineedit_->setValidator(pIpValidator);
    server_ip_lineedit_->setInputMask("000.000.000.000");
    server_ip_lineedit_->setAlignment(Qt::AlignCenter);

    server_port_lineedit_ = new QLineEdit;
    QIntValidator *int_validator = new QIntValidator(1025, 100000);
    server_port_lineedit_->setValidator(int_validator);
    server_port_lineedit_->setAlignment(Qt::AlignCenter);

    flash_time_spinbox_ = new QSpinBox;
    all_red_time_spinbox_ = new QSpinBox;
	
	flash_time_spinbox_->setMinimumWidth(120);
	all_red_time_spinbox_->setMinimumWidth(120);

    flash_time_spinbox_->setRange(0, 255);
    all_red_time_spinbox_->setRange(0, 255);

    signal_status_cmb_ = new QComboBox;
    signal_status_cmb_->addItem(STRING_UI_UNIT_AUTO_CONTORL);

    QGridLayout* gllayout = new QGridLayout;
    //gllayout->addWidget(signaler_ip_label, 0, 0, 1, 1);
    //gllayout->addWidget(signal_ip_lineedit_, 0, 1, 1, 1);
    gllayout->addWidget(server_ip_label, 1, 0, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(server_ip_lineedit_, 1, 2, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(server_port_label, 2, 0, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(server_port_lineedit_, 2, 2, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(flash_time_label, 3, 0, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(flash_time_spinbox_, 3, 2, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(allred_time_label, 4, 0, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(all_red_time_spinbox_, 4, 2, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(curr_status_label, 5, 0, 1, 1, Qt::AlignCenter);
    gllayout->addWidget(signal_status_cmb_, 5, 2, 1, 1, Qt::AlignCenter);
    QGroupBox* left_group = new QGroupBox(STRING_UI_UNIT_SETTING_GROUP);
    left_group->setLayout(gllayout);

    man_signal_rbt_ = new QRadioButton(STRING_UI_UNIT_MAN_SIGNALER + ":");
    multi_period_timer_signal_rbt_ = new QRadioButton(STRING_UI_UNIT_MULTI_SIGNALER + ":");
    induction_signal_rbt_ = new QRadioButton(STRING_UI_UNIT_INDUCTION_SIGNALER + ":");
    other_signal_rbt_ = new QRadioButton(STRING_UI_UNIT_CENTRALIZE_SIGNALER + ":");

/*
	man_signal_rbt_->setFont(font);
	multi_period_timer_signal_rbt_->setFont(font);
	induction_signal_rbt_->setFont(font);
	other_signal_rbt_->setFont(font);
*/

	other_signal_rbt_->setChecked(true);
    man_signal_rbt_->setEnabled(false);
    multi_period_timer_signal_rbt_->setEnabled(false);
    induction_signal_rbt_->setEnabled(false);

    QLabel* man_schedule_label = new QLabel(STRING_UI_UNIT_MAN_SCHEDULE);
    QLabel* multi_period_schedule_label = new QLabel(STRING_UI_UNIT_MULTI_SCHEDULE);
    QLabel* induction_schedule_label = new QLabel(STRING_UI_UNIT_INDUCTION_SCHEDULE);

/*
	man_schedule_label->setFont(font);
	multi_period_schedule_label->setFont(font);
	induction_schedule_label->setFont(font);
*/

    state1_cmb_ = new QComboBox;
    state2_cmb_ = new QComboBox;
    state3_cmb_ = new QComboBox;
    state4_cmb_ = new QComboBox;

    QHBoxLayout* other_hlayout = new QHBoxLayout;
    other_hlayout->addWidget(state1_cmb_);
    other_hlayout->addWidget(new QLabel(STRING_SYMBOL_ARROW));
    other_hlayout->addWidget(state2_cmb_);
    other_hlayout->addWidget(new QLabel(STRING_SYMBOL_ARROW));
    other_hlayout->addWidget(state3_cmb_);
    other_hlayout->addWidget(new QLabel(STRING_SYMBOL_ARROW));
    other_hlayout->addWidget(state4_cmb_);
    QFrame* frame = new QFrame;
    frame->setLayout(other_hlayout);

    QGridLayout* grlayout = new QGridLayout;
    grlayout->addWidget(man_signal_rbt_, 0, 0, 1, 1);
    grlayout->addWidget(man_schedule_label, 0, 1, 1, 1);
    grlayout->addWidget(multi_period_timer_signal_rbt_, 1, 0, 1, 1);
    grlayout->addWidget(multi_period_schedule_label, 1, 1, 1, 1);
    grlayout->addWidget(induction_signal_rbt_, 2, 0, 1, 1);
    grlayout->addWidget(induction_schedule_label, 2, 1, 1, 1);
    grlayout->addWidget(other_signal_rbt_, 3, 0, 1, 1);
    grlayout->addWidget(frame, 3, 1, 1, 1);

    QList<QComboBox*> cmb_list;
    cmb_list.append(state1_cmb_);
    cmb_list.append(state2_cmb_);
    cmb_list.append(state3_cmb_);
    cmb_list.append(state4_cmb_);
    for (int i = 0; i < cmb_list.size(); i++)
    {
//        cmb_list.at(i)->addItem(STRING_UI_UNIT_INVALID);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_AUTO_CONTORL);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_CLOSE_LIGHTS);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_FLASH);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_ALLRED);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_COORDINATE);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_INDUCTION);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_MAIN_INDUCTION);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_SECOND_INDUCTION);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_SINGLE_OPTIONAL);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_CROSS_STREET);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_COORDINATE_INDUCTION);

//        cmb_list.at(i)->addItem(STRING_UI_UNIT_MASTER_SLAVE);
//        cmb_list.at(i)->addItem(STRING_UI_UNIT_SYSTEM_OPTIONAL);
//        cmb_list.at(i)->addItem(STRING_UI_UNIT_INTERVENTION);

        cmb_list.at(i)->addItem(STRING_UI_UNIT_BUS_FIRST);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_TRAFFIC_CTRL);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_MANUALLY_CTRL);
        cmb_list.at(i)->addItem(STRING_UI_UNIT_SYS_FAILURE_FLASH);
    }

    QGroupBox* right_group = new QGroupBox(STRING_UI_UNIT_FAULT_GROUP);
	/*right_group->setFont(font);*/
    right_group->setLayout(grlayout);

	font_setting_list_.append(right_group);
	font_setting_list_.append(man_schedule_label);
	font_setting_list_.append(multi_period_schedule_label);
	font_setting_list_.append(induction_schedule_label);

	font_setting_list_.append(man_signal_rbt_);
	font_setting_list_.append(multi_period_timer_signal_rbt_);
	font_setting_list_.append(induction_signal_rbt_);
	font_setting_list_.append(other_signal_rbt_);

	font_setting_list_.append(server_ip_label);
	font_setting_list_.append(server_port_label);
	font_setting_list_.append(flash_time_label);
	font_setting_list_.append(allred_time_label);
	font_setting_list_.append(curr_status_label);

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);
    QHBoxLayout* bottom_hlayout = new QHBoxLayout;
    bottom_hlayout->addStretch(2);
    bottom_hlayout->addWidget(ok_button_);
    bottom_hlayout->addStretch(1);
    bottom_hlayout->addWidget(cancel_button_);
    bottom_hlayout->addStretch(2);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(left_group);
    layout->addWidget(right_group);
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addLayout(bottom_hlayout);
    setLayout(vlayout);
    vlayout->setMargin(0);
    vlayout->setSpacing(0);

	QString qss;
	MUtility::getStyleSheetDir(qss);
	setStyleSheet(MUtility::fetchQrc(qss + "mainwindow.qss"));
//    setStyleSheet("background-color: RGB(233,246,254);");
}

void UnitparamtableWidget::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
}

void UnitparamtableWidget::UpdateUI()
{
    Unit_t param;
    memset(&param, 0, sizeof(param));
    handler_->get_unitparam(param);
    server_ip_lineedit_->setEnabled(false);
    server_port_lineedit_->setEnabled(false);
    flash_time_spinbox_->setValue(param.StartUpFlash);
    all_red_time_spinbox_->setValue(param.StartUpRed);
}

bool UnitparamtableWidget::SaveData()
{
    Unit_t param;
    memset(&param, 0x00, sizeof(param));
    param.StartUpFlash = flash_time_spinbox_->value();
    param.StartUpRed = all_red_time_spinbox_->value();
    handler_->set_unitparam(param);

    return true;
}

UnitparamtableWidget::~UnitparamtableWidget()
{
    delete handler_;
}

void UnitparamtableWidget::PageFontSetting( const QFont &font )
{
	for (int i = 0; i < font_setting_list_.size(); i++)
	{
		font_setting_list_.at(i)->setFont(font);
	}
}
