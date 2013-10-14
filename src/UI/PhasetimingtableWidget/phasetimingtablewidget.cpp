#include "phasetimingtablewidget.h"
#include "macrostring.h"
#include "phasetimingeditdlg.h"
#include "bottombuttonswidget.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

PhasetimingtableWidget::PhasetimingtableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    phase_timing_edit_dlg_ = new PhasetimingeditDlg(this);
    handler_ = new PhasetimingHandler;
    InitPage();
    InitSignalSlots();
    UpdateTree();
}

const QString &PhasetimingtableWidget::widget_name()
{
    return widget_name_;
}

void PhasetimingtableWidget::UpdateTree()
{
    tree_widget_->clear();
    QString str;
    QList<unsigned char> plan_id_list = handler_->get_phasetiming_id_list();
    int tree_level = plan_id_list.size();
    QList<QTreeWidgetItem* > tree_widgetitem_list;
    QList<PhaseTiming> plan_list;
    for (int i = 0; i < tree_level; i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        str.sprintf("%d", plan_id_list.at(i));
        item->setText(0, str);
        for (int j = 0; j < tree_widget_->columnCount(); j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        if (!handler_->get_phasetiming_list(plan_id_list.at(i)).isEmpty())
        {
            plan_list = handler_->get_phasetiming_list(plan_id_list.at(i));
            for (int k = 0; k < plan_list.size(); k++)
            {
                QTreeWidgetItem *item_sub = new QTreeWidgetItem;
                str.sprintf("%d", plan_list.at(k).phase_timing_id);
                item_sub->setText(0, str);
                item_sub->setTextAlignment(0, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).stage_id);
                item_sub->setText(1, str);
                item_sub->setTextAlignment(1, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).phase_id);
                item_sub->setText(2, str);
                item_sub->setTextAlignment(2, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).green_time);
                item_sub->setText(3, str);
                item_sub->setTextAlignment(3, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).yellow_time);
                item_sub->setText(4, str);
                item_sub->setTextAlignment(4, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).red_time);
                item_sub->setText(5, str);
                item_sub->setTextAlignment(5, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).delay_time);
                item_sub->setText(6, str);
                item_sub->setTextAlignment(6, Qt::AlignCenter);

                str.sprintf("%d", plan_list.at(k).spec_func);
                item_sub->setText(7, str);
                item_sub->setTextAlignment(7, Qt::AlignCenter);

                item->addChild(item_sub);
            }
        }
        tree_widgetitem_list.append(item);
    }
    tree_widget_->addTopLevelItems(tree_widgetitem_list);
}

void PhasetimingtableWidget::OnAddActionClicked()
{
    if (tree_widget_->topLevelItemCount() >= MAX_TIMECONFIG_LINE)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_PHASE_TIMING_TOP_LIMITED + QString::number(MAX_TIMECONFIG_LINE) + " !" , STRING_OK);
        return;
    }
    QString str("-");
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, str);
    item->setTextAlignment(0, Qt::AlignCenter);
    tree_widget_->addTopLevelItem(item);
}

void PhasetimingtableWidget::OnEditActionClicked()
{
    QTreeWidgetItem *item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    unsigned char id =item->text(0).toInt();
    phase_timing_edit_dlg_->Initialize(id, handler_);
}

void PhasetimingtableWidget::OnDeleteActionClicked()
{
    QTreeWidgetItem *item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }

	unsigned char id = item->text(0).toInt();
	unsigned char stage_id = item->text(1).toInt();
	int curr_row = tree_widget_->indexOfTopLevelItem(item);
	if (curr_row < 0)
	{
		QTreeWidgetItem *item_parent = item->parent();
		if (item_parent == NULL)
		{
			return;
		}
		handler_->remove_phasetiming_stage(id, stage_id);
		int index = item_parent->indexOfChild(item);
		item_parent->takeChild(index);
	}
	else
	{
		handler_->remove_phasetiming(id);
		tree_widget_->takeTopLevelItem(curr_row);
	}
}

void PhasetimingtableWidget::OnSaveActionClicked()
{
    if (tree_widget_->children().size() > 0)
    {
        emit stageTimingEditedSignal();
    }
	handler_->save_data();
    emit updateTimingCycleSignal();
}

void PhasetimingtableWidget::OnTreeItemDoubleClicked(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    if (item == NULL)
    {
        return;
    }
    unsigned char timing_id = item->text(0).toInt();
    phase_timing_edit_dlg_->Initialize(timing_id, handler_);
}

void PhasetimingtableWidget::InitPage()
{
    InitTree();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget(this);
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(tree_widget_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void PhasetimingtableWidget::InitSignalSlots()
{
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(tree_widget_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(tree_widget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnTreeItemDoubleClicked(QTreeWidgetItem*,int)));
	connect(phase_timing_edit_dlg_, SIGNAL(updatePhasetimingSignal(unsigned char)), this, SLOT(OnUpdateTreeSlot(unsigned char)));
}

void PhasetimingtableWidget::InitTree()
{
    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setColumnCount(7);
    QStringList header;
    header << STRING_UI_PHASETIMING_ID << STRING_UI_PHASE_TIMING_STAGE << STRING_UI_PHASETIMING_RELEASE_ID << STRING_UI_PHASETIMING_GREEN_TIME << STRING_UI_PHASETIMING_YELLOW_TIME
              << STRING_UI_PHASETIMING_RED_TIME << STRING_UI_PHASETIMING_DELAY_TIME << STRING_UI_PHASETIMING_OPTION;
    tree_widget_->setHeaderLabels(header);
    tree_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree_widget_->setSelectionBehavior(QTreeWidget::SelectRows);
    tree_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    tree_widget_->setPalette(pal);
//    tree_widget_->setStyleSheet("font:9px;border:0px;");
    tree_widget_->clearFocus();
    tree_widget_->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
	QHeaderView* header_view = tree_widget_->header();
	header_view->setDefaultAlignment(Qt::AlignCenter);
	tree_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
}

void PhasetimingtableWidget::InitContextMenu()
{
    context_menu_ = new QMenu(tree_widget_);
    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);
    save_action_ = new QAction(STRING_UI_SAVE, context_menu_);

    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    context_menu_->addAction(save_action_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                       "QMenu::item{background-color:transparent;color:#000000}"
                       "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
}

void PhasetimingtableWidget::OnUpdateTreeSlot(unsigned char phase_timing_id)
{
    Q_UNUSED(phase_timing_id);
    UpdateTree();
    /*
	QTreeWidgetItem *item = tree_widget_->currentItem();
	if (item == NULL)
	{
		return;
	}
	unsigned char stage_id = item->text(1).toInt();
	if (stage_id == 0)
	{
		AddChild(item, phase_timing_id);
	}
	else
	{
		QTreeWidgetItem *item_parent = item->parent();
		AddChild(item_parent, phase_timing_id);
    }
    */
}

void PhasetimingtableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void PhasetimingtableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void PhasetimingtableWidget::OnInitDatabase(void *db_ptr)
{
    handler_->init_database(db_ptr);
}

void PhasetimingtableWidget::AddChild(QTreeWidgetItem *item, unsigned char phase_timing_id)
{
	QTreeWidgetItem *item_sub = NULL;
	item->takeChildren();
	QString str;
	QList<PhaseTiming> phase_timing_list = handler_->get_phasetiming_list(phase_timing_id);
	for (int i = 0; i < phase_timing_list.size(); i++)
	{
		item_sub = new QTreeWidgetItem;
		str.sprintf("%d", phase_timing_list.at(i).phase_timing_id);
		item_sub->setText(0, str);
		item_sub->setTextAlignment(0, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).stage_id);
		item_sub->setText(1, str);
		item_sub->setTextAlignment(1, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).phase_id);
		item_sub->setText(2, str);
		item_sub->setTextAlignment(2, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).green_time);
		item_sub->setText(3, str);
		item_sub->setTextAlignment(3, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).yellow_time);
		item_sub->setText(4, str);
		item_sub->setTextAlignment(4, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).red_time);
		item_sub->setText(5, str);
		item_sub->setTextAlignment(5, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).delay_time);
		item_sub->setText(6, str);
		item_sub->setTextAlignment(6, Qt::AlignCenter);

		str.sprintf("%d", phase_timing_list.at(i).spec_func);
		item_sub->setText(7, str);
		item_sub->setTextAlignment(7, Qt::AlignCenter);

		item->addChild(item_sub);
	}
	str.sprintf("%d", phase_timing_id);
	item->setText(0, str);
}

PhasetimingtableWidget::~PhasetimingtableWidget()
{
	delete phase_timing_edit_dlg_;
	delete handler_;
}
