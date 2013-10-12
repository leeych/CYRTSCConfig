#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include "phasetablewidget.h"
#include "macrostring.h"
#include "bottombuttonswidget.h"
#include "phaseeditdlg.h"

PhasetableWidget::PhasetableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    phase_edit_dlg_ = new PhaseeditDlg(this);
    handler_ = new PhaseHandler;
    InitPage();
    InitSignalSlots();
    UpdateTree();
}

const QString &PhasetableWidget::widget_name()
{
    return widget_name_;
}

void PhasetableWidget::UpdateTree()
{
    tree_widget_->clear();
    QString str;
    QList<PhaseParam> phase_list = handler_->get_phase_list();
    int tree_level = phase_list.size();
    QList<QTreeWidgetItem *> tree_widget_item_list;
    for (int i = 0; i < tree_level; i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        str.sprintf("%d", phase_list.at(i).phase_id);
        item->setText(0, str);
        str.sprintf("%d", phase_list.at(i).phase_walk_green);
        item->setText(1, str);
        str.sprintf("%d", phase_list.at(i).phase_walk_clear);
        item->setText(2, str);
        str.sprintf("%d", phase_list.at(i).phase_min_green);
        item->setText(3, str);
        str.sprintf("%d", phase_list.at(i).phase_green_delay);
        item->setText(4, str);
        str.sprintf("%d", phase_list.at(i).phase_max_green1);
        item->setText(5, str);
        str.sprintf("%d", phase_list.at(i).phase_max_green2);
        item->setText(6, str);
        str.sprintf("%d", phase_list.at(i).phase_fix_green);
        item->setText(7, str);
        str.sprintf("%d", phase_list.at(i).phase_green_flash);
        item->setText(8, str);
//        str = get_phase_type_desc(phase_list.at(i).phase_type);
        str = handler_->get_phase_type_desc(phase_list.at(i).phase_type);
        item->setText(9, str);
        str.sprintf("%d", phase_list.at(i).phase_spec_func);
        item->setText(10, str);
        str.sprintf("%d", phase_list.at(i).phase_reserved);
        item->setText(11, str);
        str = get_phase_ctrled_channels_desc(phase_list.at(i).phase_channel);
        item->setText(12, str);

        for (int j = 0; j < 13; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        tree_widget_item_list.append(item);
    }
    tree_widget_->addTopLevelItems(tree_widget_item_list);
}

void PhasetableWidget::OnTreeItemDoubleClicked(QTreeWidgetItem *item, int col)
{
    Q_UNUSED(col);
    if (phase_edit_dlg_ != NULL)
    {
        unsigned char id = item->text(0).toInt();
        phase_edit_dlg_->Initialize(id, handler_);
    }
}

void PhasetableWidget::OnAddActionClicked()
{
    /*
    if (!handler_->addNewPhase())
    {
        QMessageBox::information(this, STRING_TIP, "Already to limited.", STRING_OK);
        return;
    }
    UpdateTree();
    */

	int row_cnt = tree_widget_->topLevelItemCount();
	if (row_cnt >= MAX_CHANNEL)
	{
        QMessageBox::information(this, STRING_TIP, STRING_UI_PHASE_TOP_LIMITED + QString::number(MAX_CHANNEL) + " !", STRING_OK);
		return;
	}
//	UpdateTree();

    QTreeWidgetItem* item = new QTreeWidgetItem;
    QString str = "-";
    for (int j = 0; j < tree_widget_->columnCount(); j++)
    {
		item->setText(j, str);
        item->setTextAlignment(j, Qt::AlignCenter);
    }
    tree_widget_->addTopLevelItem(item);
}

void PhasetableWidget::OnEditActionClicked()
{
	if (phase_edit_dlg_ == NULL)
	{
		return;
	}
    QTreeWidgetItem* item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    Q_ASSERT(item != NULL);
    unsigned char phase_id = item->text(0).toInt();
    phase_edit_dlg_->Initialize(phase_id, handler_);
}

void PhasetableWidget::OnDeleteActionClicked()
{
    QTreeWidgetItem* item = tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }

    int curr_row = tree_widget_->indexOfTopLevelItem(item);
    unsigned char phase_id = item->text(0).toInt();
    if (handler_->is_phase_exists(phase_id))
    {
        handler_->remove_phase(phase_id);
    }
    tree_widget_->takeTopLevelItem(curr_row);
}

void PhasetableWidget::OnSaveActionClicked()
{
    if (tree_widget_->children().size() > 0)
    {
        emit phaseEditedSignal();
    }
	handler_->save_data();
    emit updateChannelCtrlsrcSignal();
}

void PhasetableWidget::OnUpdateTreeItemSlot(unsigned char id)
{
    PhaseParam phase;
    handler_->get_phase(id, phase);
    QTreeWidgetItem* item = tree_widget_->currentItem();
    QString str;
    str.sprintf("%d", phase.phase_id);
    item->setText(0, str);
    str.sprintf("%d", phase.phase_walk_green);
    item->setText(1, str);
    str.sprintf("%d", phase.phase_walk_clear);
    item->setText(2, str);
    str.sprintf("%d", phase.phase_min_green);
    item->setText(3, str);
    str.sprintf("%d", phase.phase_green_delay);
    item->setText(4, str);
    str.sprintf("%d", phase.phase_max_green1);
    item->setText(5, str);
    str.sprintf("%d", phase.phase_max_green2);
    item->setText(6, str);
    str.sprintf("%d", phase.phase_fix_green);
    item->setText(7, str);
    str.sprintf("%d", phase.phase_green_flash);
    item->setText(8, str);
//	str = get_phase_type_desc(phase.phase_type);
    str = handler_->get_phase_type_desc(phase.phase_type);
	item->setText(9, str);
    str.sprintf("%d", phase.phase_spec_func);
    item->setText(10, str);
    str.sprintf("%d", phase.phase_reserved);
	item->setText(11, str);
	str = get_phase_ctrled_channels_desc(phase.phase_channel);
    item->setText(12, str);
}

void PhasetableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void PhasetableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void PhasetableWidget::OnInitDatabase(void *ptr)
{
    handler_->init_database(ptr);
}

void PhasetableWidget::InitPage()
{
    InitTree();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(tree_widget_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void PhasetableWidget::InitSignalSlots()
{
    connect(tree_widget_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(tree_widget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnTreeItemDoubleClicked(QTreeWidgetItem*,int)));
//    connect(tree_widget_->header(), SIGNAL(clicked(QModelIndex)), tree_widget_, SLOT(sortByColumn(int)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));
    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(phase_edit_dlg_, SIGNAL(updateTreeItemSignal(unsigned char)), this, SLOT(OnUpdateTreeItemSlot(unsigned char)));
}

void PhasetableWidget::InitTree()
{
    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setColumnCount(13);
    QStringList header;
    header << STRING_UI_PHASE_ID << STRING_UI_PHASE_MAN_GREEN << STRING_UI_PHASE_MAN_CLEAR
              << STRING_UI_PHASE_MIN_GREEN << STRING_UI_PHASE_UNIT_GREEN << STRING_UI_PHASE_MAX_GREEN1
                 << STRING_UI_PHASE_MAX_GREEN2 << STRING_UI_PHASE_FIX_GREEN << STRING_UI_PHASE_GREEN_FLASH
                    << STRING_UI_PHASE_TYPE << STRING_UI_PHASE_OPTION_FUNC << STRING_UI_PHASE_EXTENSION
                       << STRING_UI_PHASE_CHANNELS;
    tree_widget_->setHeaderLabels(header);

    tree_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree_widget_->setSelectionBehavior(QTreeWidget::SelectRows);
    tree_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    tree_widget_->setPalette(pal);
    tree_widget_->clearFocus();    
    tree_widget_->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255); text-align:center;}");
	QHeaderView *header_view = tree_widget_->header();
	header_view->setDefaultAlignment(Qt::AlignCenter);
    header_view->setClickable(true);
    header_view->setSortIndicator(0, Qt::AscendingOrder);
    header_view->setSortIndicatorShown(true);
	tree_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
}

void PhasetableWidget::InitContextMenu()
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

QString PhasetableWidget::get_phase_type_desc( unsigned char phase_type )
{
    QString str = handler_->get_phase_type_desc(phase_type);
	return str;
}

QString PhasetableWidget::get_phase_ctrled_channels_desc( unsigned int channel_ids )
{
	QString str;
	for (int i = 1; i <= 32; i++)
	{
		if ((channel_ids & 0x01) == 0x01)
		{
			str += QString::number(i) + "/";
		}
		channel_ids = channel_ids >> 1;
	}
	if (str.isEmpty())
	{
		return "-";
	}
	return str.left(str.size() - 1);
}

PhasetableWidget::~PhasetableWidget()
{
	delete phase_edit_dlg_;
	delete handler_;
}
