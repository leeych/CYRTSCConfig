#include "timesectioneditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

TimesectioneditDlg::TimesectioneditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void TimesectioneditDlg::Initialize(unsigned char time_section_id, TimesectionHandler *handler)
{
    EnableSettingUI(false);
    curr_section_id_ = time_section_id;
    handler_ = handler;
    time_section_tmp_list_ = handler_->get_timesection_list(curr_section_id_);
    UpdateUI();
    exec();
}

void TimesectioneditDlg::OnOkButtonClicked()
{
    EnableSettingUI(false);
    if (SaveRowData())
    {
        UpdateTree(time_section_tmp_list_);
    }
}

void TimesectioneditDlg::OnCancelButtonClicked()
{
    EnableSettingUI(false);
}

void TimesectioneditDlg::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void TimesectioneditDlg::OnAddActionClicked()
{
    QTreeWidgetItem* item = AddTreeItem();
    event_table_->addTopLevelItem(item);
}

void TimesectioneditDlg::OnEditActionClicked()
{
    QTreeWidgetItem* item = event_table_->currentItem();
    if (item == NULL)
    {
        return;
    }
    OnTreeItemDoubleClicked(item, 0);
}

void TimesectioneditDlg::OnDeleteActionClicked()
{
    QTreeWidgetItem* item = event_table_->currentItem();
    if (item == NULL)
    {
        return;
    }
    unsigned char event_id = item->text(0).toInt();
    int index = event_table_->indexOfTopLevelItem(item);
    if (event_id != 0)
    {
        for (int i = 0; i < time_section_tmp_list_.size(); i++)
        {
            if (time_section_tmp_list_.at(i).event_id && event_id)
            {
                time_section_tmp_list_.removeAt(i);
                break;
            }
        }
    }
    event_table_->takeTopLevelItem(index);
}

void TimesectioneditDlg::OnTreeItemDoubleClicked(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    if (item == NULL)
    {
        return;
    }
    unsigned char event_id = item->text(0).toInt();
	curr_event_id_ = event_id;
    UpdateSettingUI(event_id);
    EnableSettingUI(true);
}

void TimesectioneditDlg::OnTreeCurrentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem*)
{
    if (item == NULL)
    {
        return;
    }
    unsigned char event_id = item->text(0).toInt();
    curr_event_id_ = event_id;
    UpdateSettingUI(event_id);
    EnableSettingUI(false);
}

void TimesectioneditDlg::InitPage()
{
	setWindowTitle(STRING_UI_TIME_SECTION + "-" + STRING_UI_EDIT);
    InitTree();
    InitContextMenu();
    QLabel* timesection_id_label = new QLabel(STRING_UI_TIMESECTION_ID);
    timesection_id_cmb_ = new QComboBox;
    QString str;
    for (int i = 1; i <= 16; i++)
    {
        str.sprintf("%d", i);
        timesection_id_cmb_->addItem(str);
    }
    QHBoxLayout* id_hlayout = new QHBoxLayout;
    id_hlayout->addWidget(timesection_id_label);
    id_hlayout->addWidget(timesection_id_cmb_);
    id_hlayout->addStretch(3);

    section_event_id_cmb_ = new QComboBox;
    start_hour_spinbox_ = new QSpinBox;
    start_minute_spinbox_ = new QSpinBox;
    for (int i = 1; i <= 48; i++)
    {
        str.sprintf("%d", i);
        section_event_id_cmb_->addItem(str);
    }

    start_hour_spinbox_ = new QSpinBox;
    start_minute_spinbox_ = new QSpinBox;
    start_hour_spinbox_->setRange(0, 23);
    start_minute_spinbox_->setRange(0, 59);

    ctrl_mode_cmb_ = new QComboBox;
//    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_INVALID);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_AUTO_CONTORL);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_CLOSE_LIGHTS);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_FLASH);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_ALLRED);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_COORDINATE);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_INDUCTION);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_MAIN_INDUCTION);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_SECOND_INDUCTION);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_SINGLE_OPTIONAL);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_CROSS_STREET);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_COORDINATE_INDUCTION);

//    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_MASTER_SLAVE);
//    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_SYSTEM_OPTIONAL);
//    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_INTERVENTION);

    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_BUS_FIRST);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_TRAFFIC_CTRL);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_MANUALLY_CTRL);
    ctrl_mode_cmb_->addItem(STRING_UI_UNIT_SYS_FAILURE_FLASH);

    timing_id_cmb_ = new QComboBox;
	//for (int i = 1; i <= 32; i++)
	//{
	//	timing_id_cmb_->addItem(QString::number(i));
	//}

    brightness_ctrl_chk_ = new QCheckBox(STRING_UI_TIMESECTION_BRIGHT);
	brightness_ctrl_chk_->setEnabled(false);

    add_button_ = new QPushButton(STRING_UI_ADD);
    edit_button_ = new QPushButton(STRING_UI_EDIT);
    delete_button_ = new QPushButton(STRING_UI_DELETE);

    save_button_ = new QPushButton(STRING_UI_SAVE);
    reset_button_ = new QPushButton(STRING_UI_RESET);

    QHBoxLayout* button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(add_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(edit_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(delete_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(save_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(reset_button_);
    button_hlayout->addStretch(1);

    QVBoxLayout* tree_vlayout = new QVBoxLayout;
    tree_vlayout->addWidget(event_table_);
    tree_vlayout->addLayout(button_hlayout);

    QLabel* timesection_event_id_label = new QLabel(STRING_UI_TIMESECTION_EVENT_ID);
    QLabel* start_time_label = new QLabel(STRING_UI_TIMESECTION_TIME);
    QLabel* ctrl_mode_label = new QLabel(STRING_UI_TIMESECTION_CTRL_MODE);
    QLabel* special_func_label = new QLabel(STRING_UI_TIMESECTION_SPECIAL_FUNC);
    QLabel* timing_id_label = new QLabel(STRING_UI_TIMESECTION_TIMING_ID);
    QLabel* accessibility_label = new QLabel(STRING_UI_TIMESECTION_ACCESSIBILITY);
    func1_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "1");
    func2_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "2");
    func3_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "3");
    func4_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "4");
    func5_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "5");
    func6_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "6");
    func7_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "7");
    bus_chk_ = new QCheckBox(STRING_UI_TIMESECTION_FUNCTION + "8");

    spec_func_chk_list_.append(func1_chk_);
    spec_func_chk_list_.append(func2_chk_);
    spec_func_chk_list_.append(func3_chk_);
    spec_func_chk_list_.append(func4_chk_);
    spec_func_chk_list_.append(func5_chk_);
    spec_func_chk_list_.append(func6_chk_);
    spec_func_chk_list_.append(func7_chk_);
    spec_func_chk_list_.append(bus_chk_);

	for (int i = 0; i < spec_func_chk_list_.size(); i++)
	{
		spec_func_chk_list_.at(i)->setEnabled(false);
	}

    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(timesection_event_id_label);
    fir_hlayout->addWidget(section_event_id_cmb_);
    fir_hlayout->addStretch(1);
    fir_hlayout->addWidget(start_time_label);
    fir_hlayout->addWidget(start_hour_spinbox_);
    fir_hlayout->addWidget(new QLabel(":"));
    fir_hlayout->addWidget(start_minute_spinbox_);
    fir_hlayout->addStretch(1);
    fir_hlayout->addWidget(ok_button_);
    fir_hlayout->addWidget(cancel_button_);

    QHBoxLayout* sec_hlayout = new QHBoxLayout;
    sec_hlayout->addWidget(ctrl_mode_label);
    sec_hlayout->addWidget(ctrl_mode_cmb_);
    sec_hlayout->addStretch(1);
    sec_hlayout->addWidget(timing_id_label);
    sec_hlayout->addWidget(timing_id_cmb_);
    sec_hlayout->addStretch(1);
    sec_hlayout->addWidget(accessibility_label);
    sec_hlayout->addWidget(brightness_ctrl_chk_);

    QHBoxLayout* third_hlayout = new QHBoxLayout;
    third_hlayout->addWidget(special_func_label);
    third_hlayout->addWidget(func1_chk_);
    third_hlayout->addWidget(func2_chk_);
    third_hlayout->addWidget(func3_chk_);
    third_hlayout->addWidget(func4_chk_);
    third_hlayout->addWidget(func5_chk_);
    third_hlayout->addWidget(func6_chk_);
    third_hlayout->addWidget(func7_chk_);
    third_hlayout->addWidget(bus_chk_);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addLayout(sec_hlayout);
    vlayout->addLayout(third_hlayout);
    QGroupBox* event_grp = new QGroupBox(STRING_UI_TIMESECTION_EVENT);
    event_grp->setLayout(vlayout);

    QVBoxLayout* whole_vlayout = new QVBoxLayout;
    whole_vlayout->addLayout(id_hlayout);
    whole_vlayout->addLayout(tree_vlayout);
    whole_vlayout->addWidget(event_grp);
    setLayout(whole_vlayout);
}

void TimesectioneditDlg::InitSignalSlots()
{
    connect(save_button_, SIGNAL(clicked()), this, SLOT(OnSaveButtonClicked()));
    connect(reset_button_, SIGNAL(clicked()), this, SLOT(OnResetButtonClicked()));
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(cancel_button_, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));

    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));

    connect(add_button_, SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(edit_button_, SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(delete_button_, SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));

    connect(event_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(event_table_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnTreeItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(event_table_, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(OnTreeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void TimesectioneditDlg::UpdateUI()
{
    UpdateTree(handler_->get_timesection_list(curr_section_id_));
    unsigned char event_id = 0;
    if (!handler_->get_timesection_list(curr_section_id_).isEmpty())
    {
        event_id = handler_->get_timesection_list(curr_section_id_).at(0).event_id;
    }
    UpdateSettingUI(event_id);
}

void TimesectioneditDlg::UpdateSettingUI(unsigned char event_id)
{
//    if (event_id == 0)
//    {
//        return;
//    }
    timesection_id_cmb_->clear();
    section_event_id_cmb_->clear();
    timing_id_cmb_->clear();
    // time section id
    QList<unsigned char> time_section_id_list;
    for (unsigned char i = 1; i <= MAX_TIMESECTION_LINE; i++)
    {
        time_section_id_list.append(i);
    }
    // time section id  &&  event_id
    QList<unsigned char> event_id_list;
    for (unsigned char i = 1; i <= MAX_EVENT_LINE; i++)
    {
        event_id_list.append(i);
    }

	QList<TimeSection> time_event_list = handler_->get_timesection_list();
    for (int i = 0; i < time_event_list.size(); i++)
    {
		if (curr_section_id_ != time_event_list.at(i).time_section_id)
		{
			time_section_id_list.removeAll(time_event_list.at(i).time_section_id);
		}
		if (event_id != time_event_list.at(i).event_id && curr_section_id_ == time_event_list.at(i).time_section_id)
		{
			event_id_list.removeAll(time_event_list.at(i).event_id);
		}
    }
    int event_len = event_id_list.size();
    QList<unsigned char> event_id_bak_list = event_id_list;
    for (int i = 0; i < event_len; i++)
    {
        for (int j = 0; j < time_section_tmp_list_.size(); j++)
        {
            if (event_id != time_section_tmp_list_.at(j).event_id
                    && event_id_bak_list.at(i) == time_section_tmp_list_.at(j).event_id)
            {
                event_id_list.removeAll(time_section_tmp_list_.at(j).event_id);
            }
        }
    }

    QString str;
    for (int i = 0; i < time_section_id_list.size(); i++)
    {
        str.sprintf("%d", time_section_id_list.at(i));
        timesection_id_cmb_->addItem(str);
    }
    for (int i = 0; i < timesection_id_cmb_->count(); i++)
    {
        if (curr_section_id_ == timesection_id_cmb_->itemText(i).toInt())
        {
            timesection_id_cmb_->setCurrentIndex(i);
            break;
        }
    }
    for (int i = 0; i < event_id_list.size(); i++)
    {
        str.sprintf("%d", event_id_list.at(i));
        section_event_id_cmb_->addItem(str);
    }
    for (int i = 0; i < section_event_id_cmb_->count(); i++)
    {
        if (event_id == section_event_id_cmb_->itemText(i).toInt())
        {
            section_event_id_cmb_->setCurrentIndex(i);
            break;
        }
    }

    // start hour, start minute
    TimeSection section_event;
    for (int i = 0; i < time_section_tmp_list_.size(); i++)
    {
        if (event_id == time_section_tmp_list_.at(i).event_id)
        {
            section_event = time_section_tmp_list_.at(i);
            break;
        }
    }
    start_hour_spinbox_->setValue(section_event.start_hour);
    start_minute_spinbox_->setValue(section_event.start_minute);

    // ctrl mode
    QString ctrl_mode_str = handler_->get_ctrl_mode_desc(section_event.ctrl_mode);
    for (int i = 0; i < ctrl_mode_cmb_->count(); i++)
    {
        if (ctrl_mode_str == ctrl_mode_cmb_->itemText(i))
        {
            ctrl_mode_cmb_->setCurrentIndex(i);
            break;
        }
    }

    // timing id
    QList<unsigned char> timing_id_total_list = handler_->get_timing_id_list();
    for (int i = 0; i < timing_id_total_list.size(); i++)
    {
        str.sprintf("%d", timing_id_total_list.at(i));
        timing_id_cmb_->addItem(str);
    }
    for (int i = 0; i < timing_id_cmb_->count(); i++)
    {
        if (section_event.pattern_id == timing_id_cmb_->itemText(i).toInt())
        {
            timing_id_cmb_->setCurrentIndex(i);
            break;
        }
    }

    // brightness
    if ((section_event.aux_func & 0x04) == 0x04)
    {
        brightness_ctrl_chk_->setChecked(true);
    }

    // special functions
    unsigned char spec_func = section_event.spec_func;
    for (int i = 0; i < spec_func_chk_list_.size(); i++)
    {
        if ((spec_func & 0x01) == 0x01)
        {
            spec_func_chk_list_.at(i)->setChecked(true);
        }
        spec_func = spec_func >> 1;
    }
}

void TimesectioneditDlg::InitTree()
{
    event_table_ = new QTreeWidget;
    event_table_->setColumnCount(6);
    QStringList header;
    header << STRING_UI_TIMESECTION_EVENT_ID << STRING_UI_TIMESECTION_TIME << STRING_UI_TIMESECTION_CTRL_MODE
           << STRING_UI_TIMESECTION_TIMING_ID << STRING_UI_TIMESECTION_FUNC_ID << STRING_UI_TIMESECTION_SPECIAL_ID;
    event_table_->setHeaderLabels(header);
    event_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    event_table_->setSelectionBehavior(QTreeWidget::SelectRows);
    event_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    event_table_->setPalette(pal);
    event_table_->clearFocus();
    event_table_->setStyleSheet("QHeaderView::section{background-color: rgb(184,219,255); text-align:center;} font:12px;");
	QHeaderView *header_view = event_table_->header();
	header_view->setDefaultAlignment(Qt::AlignCenter);
	event_table_->setContextMenuPolicy(Qt::CustomContextMenu);
}

void TimesectioneditDlg::UpdateTree(const QList<TimeSection>& time_section_list)
{
    event_table_->clear();
    QString str;
    int tree_level = time_section_list.size();
    QList<QTreeWidgetItem* > event_list;
    for (int i = 0; i < tree_level; i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        str.sprintf("%d", time_section_list.at(i).event_id);
        item->setText(0, str);

        str.sprintf("%02d:%02d", time_section_list.at(i).start_hour, time_section_list.at(i).start_minute);
        item->setText(1, str);

        str = handler_->get_ctrl_mode_desc(time_section_list.at(i).ctrl_mode);
        item->setText(2, str);

        str.sprintf("%d", time_section_list.at(i).pattern_id);
        item->setText(3, str);

        str.sprintf("%d", time_section_list.at(i).aux_func);
        item->setText(4, str);

        str.sprintf("%d", time_section_list.at(i).spec_func);
        item->setText(5, str);

        for (int j = 0; j < 6; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        event_list.append(item);
    }
    event_table_->addTopLevelItems(event_list);
}

void TimesectioneditDlg::EnableSettingUI(bool enable)
{
    ok_button_->setEnabled(enable);
    cancel_button_->setEnabled(enable);

    section_event_id_cmb_->setEnabled(enable);
    start_hour_spinbox_->setEnabled(enable);
    start_minute_spinbox_->setEnabled(enable);
    ctrl_mode_cmb_->setEnabled(enable);
    timing_id_cmb_->setEnabled(enable);
    //brightness_ctrl_chk_->setEnabled(enable);
    //for (int i = 0;i < spec_func_chk_list_.size(); i++)
    //{
    //    spec_func_chk_list_.at(i)->setEnabled(enable);
    //}
}

void TimesectioneditDlg::InitContextMenu()
{
    context_menu_ = new QMenu(event_table_);
    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);

    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                  "QMenu::item{background-color:transparent;color:#000000}"
                  "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
}

bool TimesectioneditDlg::SaveData()
{
    /*
    QTreeWidgetItem* item = NULL;

    QList<TimeSection> time_section_list;
    for (int i = 0; i < event_table_->topLevelItemCount(); i++)
    {
        item = event_table_->topLevelItem(i);
        TimeSection time_section;
        time_section.time_section_id = timesection_id_cmb_->currentText().toInt();
        time_section.event_id = item->text(0).toInt();
        time_section.start_hour = get_start_hour(item->text(1));
        time_section.start_minute = get_start_minute(item->text(1));
        time_section.ctrl_mode = get_ctrl_mode(item->text(2));
        time_section.pattern_id = item->text(3).toInt();
        time_section.aux_func = item->text(4).toInt();
        time_section.spec_func = item->text(5).toInt();

        time_section_list.append(time_section);
    }

    if (!time_section_list.isEmpty())
    {
        curr_section_id_ = time_section_list.at(0).time_section_id;
        qDebug() << curr_section_id_;
    }
    */

	unsigned char time_section_id = timesection_id_cmb_->currentText().toInt();
	for (int i = 0; i < time_section_tmp_list_.size(); i++)
	{
		time_section_tmp_list_[i].time_section_id = time_section_id;
	}
	if (!handler_->set_timesection(curr_section_id_, time_section_tmp_list_))
	{
		return false;
	}
	if (!time_section_tmp_list_.isEmpty())
	{
		curr_section_id_ = time_section_id;
	}
    return true;
}

bool TimesectioneditDlg::SaveRowData()
{
    TimeSection time_section;
	unsigned char event_id = section_event_id_cmb_->currentText().toInt();
    time_section.time_section_id = timesection_id_cmb_->currentText().toInt();
    time_section.event_id = event_id;
    time_section.pattern_id = timing_id_cmb_->currentText().toInt();
    time_section.ctrl_mode = handler_->get_ctrl_mode_by_desc(ctrl_mode_cmb_->currentText().trimmed());
    time_section.start_hour = start_hour_spinbox_->value();
    time_section.start_minute = start_minute_spinbox_->value();
    time_section.aux_func = get_brightness();
    time_section.spec_func = get_spec_func();

	if (curr_event_id_ != time_section.event_id)
	{
		time_section.event_id = curr_event_id_;
	}
    int idx = index_of_tmp_timesection_list(time_section);
	time_section.event_id = event_id;
    if (idx != -1)
    {
        time_section_tmp_list_[idx] = time_section;
    }
    else
    {
        time_section_tmp_list_.append(time_section);
    }

    return true;
}

QTreeWidgetItem *TimesectioneditDlg::AddTreeItem()
{
    QTreeWidgetItem* item = new QTreeWidgetItem;
    QString str = "-";
    item->setText(0, str);
    item->setText(1, str + ":" + str);
    item->setText(2, str);
    item->setText(3, str);
    item->setText(4, str);
    item->setText(5, str);
    for (int j = 0; j < 6; j++)
    {
        item->setTextAlignment(j, Qt::AlignCenter);
    }
    return item;
}

unsigned char TimesectioneditDlg::get_start_hour(const QString &str)
{
    int idx = str.indexOf(":");
    QString temp = str.left(idx);
    return temp.toInt();
}

unsigned char TimesectioneditDlg::get_start_minute(const QString &str)
{
    int idx = str.indexOf(":");
    QString temp = str.right(str.length() - idx);
    return temp.toInt();
}

unsigned char TimesectioneditDlg::get_ctrl_mode(const QString &str)
{
    if (str == STRING_UI_UNIT_AUTO_CONTORL)
    {
        return 0;
    }
    else if (str == STRING_UI_UNIT_CLOSE_LIGHTS)
    {
        return 1;
    }
    else if (str == STRING_UI_UNIT_FLASH)
    {
        return 2;
    }
    else if (str == STRING_UI_UNIT_ALLRED)
    {
        return 3;
    }
    else if (str == STRING_UI_UNIT_COORDINATE)
    {
        return 4;
    }
    else if (str == STRING_UI_UNIT_INDUCTION)
    {
        return 5;
    }
    else if (str == STRING_UI_UNIT_MAIN_INDUCTION)
    {
        return 6;
    }
    else if (str == STRING_UI_UNIT_SECOND_INDUCTION)
    {
        return 7;
    }
    else if (str == STRING_UI_UNIT_SINGLE_OPTIONAL)
    {
        return 8;
    }
    else if (str == STRING_UI_UNIT_CROSS_STREET)
    {
        return 9;
    }
    else if (str == STRING_UI_UNIT_COORDINATE_INDUCTION)
    {
        return 10;
    }
//    else if (str == STRING_UI_UNIT_MASTER_SLAVE)
//    {
//        return 11;
//    }
//    else if (str == STRING_UI_UNIT_SYSTEM_OPTIONAL)
//    {
//        return 12;
//    }
//    else if (str == STRING_UI_UNIT_INTERVENTION)
//    {
//        return 13;
//    }
//    return 14;
    else if (str == STRING_UI_UNIT_BUS_FIRST)
    {
        return 27;
    }
    else if (str == STRING_UI_UNIT_TRAFFIC_CTRL)
    {
        return 28;
    }
    else if (str == STRING_UI_UNIT_MANUALLY_CTRL)
    {
        return 29;
    }
    else if (str == STRING_UI_UNIT_SYS_FAILURE_FLASH)
    {
        return 30;
    }
    return 31;
}

int TimesectioneditDlg::get_ctrl_mode_current_index(const unsigned char mode)
{
    Q_UNUSED(mode);
    return -1;
}

unsigned char TimesectioneditDlg::get_brightness()
{
    unsigned char bright = 0;
    if (brightness_ctrl_chk_->isChecked())
    {
        bright = 4; // bit3 = 1;
    }
    return bright;
}

unsigned char TimesectioneditDlg::get_spec_func()
{
    unsigned char spec = 0;
    for (int i = spec_func_chk_list_.size() - 1; i >= 0; i--)
    {
        spec = spec << 1;
        if (spec_func_chk_list_.at(i)->isChecked())
        {
            spec |= 0x01;
        }
    }

    return spec;
}

int TimesectioneditDlg::index_of_tmp_timesection_list(const TimeSection &section)
{
    int index = -1;
    if (section.time_section_id == 0 || section.event_id == 0)
    {
        return index;
    }
    for (int i = 0; i < time_section_tmp_list_.size(); i++)
    {
        if (section.time_section_id == time_section_tmp_list_.at(i).time_section_id
                && section.event_id == time_section_tmp_list_.at(i).event_id)
        {
           index = i;
           return index;
        }
    }
    return index;
}

void TimesectioneditDlg::OnSaveButtonClicked()
{
	SaveData();
    accept();
    emit updateTreeSignal(curr_section_id_);
}

void TimesectioneditDlg::OnResetButtonClicked()
{
    time_section_tmp_list_ = handler_->get_timesection_list(curr_section_id_);
    UpdateUI();
}

TimesectioneditDlg::~TimesectioneditDlg()
{

}
