#include "phasetimingeditdlg.h"
#include "macrostring.h"
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QGridLayout>
#include <QMessageBox>

PhasetimingeditDlg::PhasetimingeditDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

void PhasetimingeditDlg::Initialize(unsigned char phase_timing_id, PhasetimingHandler* handler)
{
	EnableSettingUI(false);
    curr_timing_id_ = phase_timing_id;
    handler_ = handler;
	phase_timing_tmp_list_ = handler_->get_phasetiming_list(curr_timing_id_);
    UpdateUI();
    exec();
}

void PhasetimingeditDlg::OnSaveButtonClicked()
{
	SaveData();
    accept();
    emit updatePhasetimingSignal(curr_timing_id_);
}

void PhasetimingeditDlg::OnResetButtonClicked()
{
	phase_timing_tmp_list_ = handler_->get_phasetiming_list(curr_timing_id_);
    UpdateUI();
}

void PhasetimingeditDlg::OnOkButtonClicked()
{
    if (ValidateUI() == GreenLessYellow)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_PHASE_TIMING_GREEN_LT_YELLOW, STRING_OK);
        return;
    }

    EnableSettingUI(false);
    if (SaveRowData())
    {
        UpdateTree(phase_timing_tmp_list_);
    }
}

void PhasetimingeditDlg::OnCancelButtonClicked()
{
    EnableSettingUI(false);
}

void PhasetimingeditDlg::OnAddActionClicked()
{
    QTreeWidgetItem* item = AddTreeTopLevelItem();
    tree_widget_->addTopLevelItem(item);
}

void PhasetimingeditDlg::OnEditActionClicked()
{
    QTreeWidgetItem *item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    OnTreeItemDoubleClicked(item, 0);
}

void PhasetimingeditDlg::OnDeleteActionClicked()
{
    QTreeWidgetItem *item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    unsigned char stage_id = item->text(0).toInt();
    int index = tree_widget_->indexOfTopLevelItem(item);
    if (stage_id != 0)
    {
        for (int i = 0; i < phase_timing_tmp_list_.size(); i++)
        {
            if (phase_timing_tmp_list_.at(i).stage_id == stage_id)
            {
                phase_timing_tmp_list_.removeAt(i);
                break;
            }
        }
    }
    tree_widget_->takeTopLevelItem(index);
}

void PhasetimingeditDlg::OnTreeItemDoubleClicked(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    if (item == NULL)
    {
        return;
    }
    unsigned char stage_id = item->text(0).toInt();
    UpdateSettingUI(stage_id);
    EnableSettingUI(true);
}

void PhasetimingeditDlg::OnTreeCurrentItemChanged(QTreeWidgetItem *curr_item, QTreeWidgetItem *pre_item)
{
    Q_UNUSED(pre_item);
    if (curr_item == NULL)
    {
        return;
    }
    unsigned char stage_id = curr_item->text(0).toInt();
	curr_stage_id_ = stage_id;
    UpdateSettingUI(stage_id);
    EnableSettingUI(false);
}

void PhasetimingeditDlg::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void PhasetimingeditDlg::InitPage()
{
	setWindowTitle(STRING_UI_PHASE_TIMING + "-" + STRING_UI_EDIT);
    InitTree();
    InitContextMenu();

    QLabel* phase_timing_id_label = new QLabel(STRING_UI_PHASE_TIMING_ID);
    phase_timing_id_cmb_ = new QComboBox;

    add_button_ = new QPushButton(STRING_UI_ADD);
    edit_button_ = new QPushButton(STRING_UI_EDIT);
    delete_button_ = new QPushButton(STRING_UI_DELETE);

    save_button_ = new QPushButton(STRING_UI_SAVE);
    reset_button_ = new QPushButton(STRING_UI_RESET);
    ok_button_ = new QPushButton(STRING_OK);
    cancel_button_ = new QPushButton(STRING_CANCEL);

    QHBoxLayout* fir_hlayout = new QHBoxLayout;
    fir_hlayout->addWidget(phase_timing_id_label);
    fir_hlayout->addWidget(phase_timing_id_cmb_);
    fir_hlayout->addStretch(3);
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

    QGroupBox* setting_grp = new QGroupBox(STRING_UI_SETTING);
    QLabel* phase_id_label = new QLabel(STRING_UI_PHASE_TIMING_STAGE);
    phase_id_cmb_ = new QComboBox;
    QLabel* opt_param_label = new QLabel(STRING_UI_PHASE_TIMING_OPT);
    induction_id_chk_ = new QCheckBox(STRING_UI_PHASE_TIMING_INDUCTION);
    induction_id_chk_->setEnabled(false);

    QString str;
    for (int i = 1; i <=32; i++)
    {
        str.sprintf("%d", i);
        QCheckBox* chk = new QCheckBox(str);
        release_phase_id_list_.append(chk);
    }

    QLabel* green_time_label = new QLabel(STRING_UI_PHASE_TIMING_GREEN);
    QLabel* yellow_time_label = new QLabel(STRING_UI_PHASE_TIMING_YELLOW);
    QLabel* red_time_label = new QLabel(STRING_UI_PHASE_TIMING_RED);
    QLabel* delay_time_label = new QLabel(STRING_UI_PHASE_TIMING_DELAY);
    green_time_spinbox_ = new QSpinBox;
    yellow_time_spinbox_ = new QSpinBox;
    red_time_spinbox_ = new QSpinBox;
    delay_time_spinbox_ = new QSpinBox;

    green_time_spinbox_->setRange(3, 255);
    red_time_spinbox_->setRange(0, 255);
    yellow_time_spinbox_->setRange(3, 255);
	delay_time_spinbox_->setRange(0, 10);

    QHBoxLayout* phase_hlayout = new QHBoxLayout;
    phase_hlayout->addWidget(phase_id_label);
    phase_hlayout->addWidget(phase_id_cmb_);
    //phase_hlayout->addStretch(1);
    //phase_hlayout->addWidget(opt_param_label);
    //phase_hlayout->addWidget(induction_id_chk_);
    phase_hlayout->addStretch(1);
    phase_hlayout->addWidget(ok_button_);
    phase_hlayout->addWidget(cancel_button_);

    QGroupBox* release_grp = new QGroupBox(/*STRING_UI_PHASE_TIMING_RELEASE*/);
 /*   QGridLayout* glayout = new QGridLayout;
    int cnt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (cnt < release_phase_id_list_.size())
            {
                glayout->addWidget(release_phase_id_list_.at(cnt), i, j, 1, 1);
                cnt++;
            }
        }
    }*/
	release_phase_id_cmb_ = new QComboBox;
	for (unsigned char i = 1; i <= 32; i++)
	{
		release_phase_id_cmb_->addItem(QString::number(i));
	}
	QHBoxLayout *release_hlayout = new QHBoxLayout;
	release_hlayout->addWidget(new QLabel(STRING_UI_PHASE_TIMING_RELEASE + ":"));
	release_hlayout->addWidget(release_phase_id_cmb_);
	release_hlayout->addStretch(1);
	release_hlayout->addWidget(opt_param_label);
	release_hlayout->addWidget(induction_id_chk_);
    release_grp->setLayout(release_hlayout);

    QHBoxLayout* other_hlayout = new QHBoxLayout;
    other_hlayout->addWidget(green_time_label);
    other_hlayout->addWidget(green_time_spinbox_);
    other_hlayout->addWidget(yellow_time_label);
    other_hlayout->addWidget(yellow_time_spinbox_);
    other_hlayout->addWidget(red_time_label);
    other_hlayout->addWidget(red_time_spinbox_);
    other_hlayout->addWidget(delay_time_label);
    other_hlayout->addWidget(delay_time_spinbox_);

    QVBoxLayout* grp_vlayout = new QVBoxLayout;
    grp_vlayout->addLayout(phase_hlayout);
    grp_vlayout->addWidget(release_grp);
    grp_vlayout->addLayout(other_hlayout);
    setting_grp->setLayout(grp_vlayout);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addLayout(fir_hlayout);
    vlayout->addWidget(tree_widget_);
    vlayout->addLayout(button_hlayout);
    vlayout->addWidget(setting_grp);
    setLayout(vlayout);
}

void PhasetimingeditDlg::InitSignalSlots()
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

    connect(tree_widget_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(tree_widget_, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(OnTreeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(tree_widget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnTreeItemDoubleClicked(QTreeWidgetItem*,int)));
}

void PhasetimingeditDlg::InitTree()
{
    tree_widget_ = new QTreeWidget;
    tree_widget_->setColumnCount(7);
    QStringList header;
    header << STRING_UI_PHASE_TIMING_STAGE << STRING_UI_PHASE_TIMING_RELEASE << STRING_UI_PHASE_TIMING_GREEN << STRING_UI_PHASE_TIMING_YELLOW
           << STRING_UI_PHASE_TIMING_RED << STRING_UI_PHASE_TIMING_DELAY << STRING_UI_PHASE_TIMING_OPT;
    tree_widget_->setHeaderLabels(header);
    tree_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree_widget_->setSelectionBehavior(QTreeWidget::SelectRows);
    tree_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    tree_widget_->setPalette(pal);
    tree_widget_->clearFocus();
    tree_widget_->setStyleSheet("QHeaderView::section{background-color: rgb(184,219,255); text-align:center;} font:12px;");
	QHeaderView* header_view = tree_widget_->header();
	header_view->setDefaultAlignment(Qt::AlignCenter);
	tree_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
}

void PhasetimingeditDlg::UpdateTree(const QList<PhaseTiming>& phase_timing_list)
{
	tree_widget_->clear();
    QString str;
    int tree_level = phase_timing_list.size();
    QList<QTreeWidgetItem* > tree_widgetitem_list;
    for (int i = 0; i < tree_level; i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        str.sprintf("%d", phase_timing_list.at(i).stage_id);
        item->setText(0, str);
        str.sprintf("%d", phase_timing_list.at(i).phase_id);/*handler_->get_release_phase_ids_desc(phase_timing_list.at(i).phase_id);*/
        item->setText(1, str);
        str.sprintf("%d", phase_timing_list.at(i).green_time);
        item->setText(2, str);
        str.sprintf("%d", phase_timing_list.at(i).yellow_time);
        item->setText(3, str);
        str.sprintf("%d", phase_timing_list.at(i).red_time);
        item->setText(4, str);
		// start delay time
        str.sprintf("%d", phase_timing_list.at(i).delay_time);
        item->setText(5, str);
        str.sprintf("%d", phase_timing_list.at(i).spec_func);
        item->setText(6, str);

        for (int j = 0; j < 7; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }

        tree_widgetitem_list.append(item);
    }
    tree_widget_->addTopLevelItems(tree_widgetitem_list);
}

void PhasetimingeditDlg::InitContextMenu()
{
    context_menu_ = new QMenu(tree_widget_);
    add_action_ =  new QAction(STRING_UI_ADD, context_menu_);
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

QTreeWidgetItem *PhasetimingeditDlg::AddTreeTopLevelItem()
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    QString str("-");
    for (int i = 0; i < tree_widget_->columnCount(); i++)
    {
        item->setText(i, str);
        item->setTextAlignment(i, Qt::AlignCenter);
    }
    return item;
}

void PhasetimingeditDlg::UpdateUI()
{
    UpdateTree(phase_timing_tmp_list_);
    unsigned char stage_id = 0;
    if (!handler_->get_phasetiming_list(curr_timing_id_).isEmpty())
    {
        stage_id = handler_->get_phasetiming_list(curr_timing_id_).at(0).stage_id;
    }
	curr_stage_id_ = stage_id;
    UpdateSettingUI(stage_id);
}

void PhasetimingeditDlg::UpdateSettingUI(unsigned char stage_id)
{
    phase_timing_id_cmb_->clear();
    phase_id_cmb_->clear();
    // phase timing id
    QList<unsigned char> phase_timing_id_list;
    for (unsigned char i = 1; i <= 16; i++)
    {
        phase_timing_id_list.append(i);
    }

	QList<unsigned char> stage_id_list;
	for (unsigned char i = 1; i <=32; i++)
	{
		stage_id_list.append(i);
	}

    QList<PhaseTiming> stage_timing_list = handler_->get_phasetiming_list();
    for (int i = 0; i < stage_timing_list.size(); i++)
    {
		if (curr_timing_id_ != stage_timing_list.at(i).phase_timing_id)
		{
			phase_timing_id_list.removeAll(stage_timing_list.at(i).phase_timing_id);
		}
		if (stage_id != stage_timing_list.at(i).stage_id && curr_timing_id_ == stage_timing_list.at(i).phase_timing_id)
		{
			stage_id_list.removeAll(stage_timing_list.at(i).stage_id);
		}
    }

	QString str;
    for (int i = 0; i < phase_timing_id_list.size(); i++)
    {
        str.sprintf("%d", phase_timing_id_list.at(i));
        phase_timing_id_cmb_->addItem(str);
    }
    for (int i = 0; i < phase_timing_id_cmb_->count(); i++)
    {
        if (curr_timing_id_ == phase_timing_id_cmb_->itemText(i).toInt())
        {
            phase_timing_id_cmb_->setCurrentIndex(i);
            break;
        }
    }

    // stage id
	/*
	for (int i = 0; i < phase_timing_tmp_list_.size(); i++)
	{
		stage_id_list.removeAll(phase_timing_tmp_list_.at(i).stage_id);
	}
	*/
	QList<unsigned char> stage_id_bak_list = stage_id_list;
	for (int i = 0; i < stage_id_bak_list.size(); i++)
	{
		for (int j = 0; j < phase_timing_tmp_list_.size(); j++)
		{
			if (stage_id != phase_timing_tmp_list_.at(j).stage_id
				&& stage_id_bak_list.at(i) == phase_timing_tmp_list_.at(j).stage_id)
			{
				stage_id_list.removeAll(phase_timing_tmp_list_.at(j).stage_id);
			}
		}
	}
    for (int i = 0; i < stage_id_list.size(); i++)
    {
        str.sprintf("%d", stage_id_list.at(i));
        phase_id_cmb_->addItem(str);
//        if (stage_id == stage_id_list.at(i))
//        {
//            phase_id_cmb_->setCurrentIndex(i);
//        }
    }
    for (int i = 0; i < phase_id_cmb_->count(); i++)
    {
        if (stage_id == phase_id_cmb_->itemText(i).toInt())
        {
            phase_id_cmb_->setCurrentIndex(i);
            break;
        }
    }

    // stage optional param
    PhaseTiming plan;
	for (int i = 0; i < phase_timing_tmp_list_.size(); i++)
	{
		if (phase_timing_tmp_list_.at(i).stage_id == stage_id)
		{
			plan = phase_timing_tmp_list_.at(i);
			break;
		}
	}

	induction_id_chk_->setChecked((plan.spec_func & 0x01) == 0x01);
    /*unsigned char release_phase = plan.phase_id;
    for (int i = 0; i < release_phase_id_list_.size(); i++)
    {
		release_phase_id_list_.at(i)->setChecked(false);
        if (release_phase == (i + 1))
        {
            release_phase_id_list_.at(i)->setChecked(true);
        }
    }*/
	//release_phase_id_cmb_->setCurrentIndex(32);
	for (int i = 0; i < release_phase_id_cmb_->count(); i++)
	{
		if (plan.phase_id == release_phase_id_cmb_->itemText(i).toInt())
		{
			release_phase_id_cmb_->setCurrentIndex(i);
			break;
		}
	}

    // green yellow red time
    green_time_spinbox_->setValue(plan.green_time);
    yellow_time_spinbox_->setValue(plan.yellow_time);
    red_time_spinbox_->setValue(plan.red_time);
    delay_time_spinbox_->setValue(plan.delay_time);
}

void PhasetimingeditDlg::EnableSettingUI(bool enable)
{
    ok_button_->setEnabled(enable);
    cancel_button_->setEnabled(enable);

//    induction_id_chk_->setEnabled(enable);
    phase_id_cmb_->setEnabled(enable);
    green_time_spinbox_->setEnabled(enable);
    yellow_time_spinbox_->setEnabled(enable);
    red_time_spinbox_->setEnabled(enable);
    delay_time_spinbox_->setEnabled(enable);
	release_phase_id_cmb_->setEnabled(enable);

    //for (int i = 0; i < release_phase_id_list_.size(); i++)
    //{
    //    release_phase_id_list_.at(i)->setEnabled(enable);
    //}
}

PhasetimingeditDlg::TimingErr PhasetimingeditDlg::ValidateUI()
{
    if (green_time_spinbox_->value() < yellow_time_spinbox_->value())
    {
        return GreenLessYellow;
    }
    return None;
}

bool PhasetimingeditDlg::SaveData()
{
	unsigned char phase_timing_id = phase_timing_id_cmb_->currentText().toInt();
	for (int i = 0; i < phase_timing_tmp_list_.size(); i++)
	{
		phase_timing_tmp_list_[i].phase_timing_id = phase_timing_id;
	}
	if (!handler_->set_phasetiming(curr_timing_id_, phase_timing_tmp_list_))
	{
		return false;
	}
	if (!phase_timing_tmp_list_.isEmpty())
	{
		curr_timing_id_ = phase_timing_id;
	}
    return true;
}

bool PhasetimingeditDlg::SaveRowData()
{
    PhaseTiming plan;
	unsigned char stage_id = phase_id_cmb_->currentText().toInt();
    plan.phase_timing_id = phase_timing_id_cmb_->currentText().toInt();
    plan.stage_id = stage_id;
    plan.phase_id = release_phase_id_cmb_->currentText().toInt(); /*get_phase_ids();*/
    plan.green_time = green_time_spinbox_->value();
    plan.yellow_time = yellow_time_spinbox_->value();
    plan.red_time = red_time_spinbox_->value();
    plan.spec_func = get_spec_func();
	plan.delay_time = delay_time_spinbox_->value();
	if (plan.stage_id != curr_stage_id_)
	{
		plan.stage_id = curr_stage_id_;
	}
    int idx = index_of_phasetiming_tmp_list(plan);
	plan.stage_id = stage_id;
    if (idx != -1)
    {
        phase_timing_tmp_list_[idx] = plan;
    }
    else
    {
        phase_timing_tmp_list_.append(plan);
    }
    return true;
}

unsigned int PhasetimingeditDlg::get_phase_ids()
{
    unsigned char phase_ids = 0;
    for (int i = release_phase_id_list_.size() - 1; i >= 0; i--)
    {
		phase_ids = phase_ids << 1;
        if (release_phase_id_list_.at(i)->isChecked())
        {
            phase_ids |= 0x01;
        }
    }
    return phase_ids;
}

unsigned char PhasetimingeditDlg::get_spec_func()
{
    unsigned char spec_func = 0;
    if (induction_id_chk_->isChecked())
    {
        spec_func = 0x01;
    }
    return spec_func;
}

int PhasetimingeditDlg::index_of_phasetiming_tmp_list(const PhaseTiming &plan)
{
    int index = -1;
    if (plan.phase_timing_id == 0 || plan.stage_id == 0)
    {
        return index;
    }

    for (int i = 0; i < phase_timing_tmp_list_.size(); i++)
    {
        if (plan.phase_timing_id == phase_timing_tmp_list_.at(i).phase_timing_id
                && plan.stage_id == phase_timing_tmp_list_.at(i).stage_id)
        {
            index = i;
            return index;
        }
    }
    return index;
}

PhasetimingeditDlg::~PhasetimingeditDlg()
{
}
