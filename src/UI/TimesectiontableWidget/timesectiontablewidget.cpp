#include "timesectiontablewidget.h"
#include "macrostring.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

TimesectiontableWidget::TimesectiontableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    timesection_edit_dlg_ = new TimesectioneditDlg(this);
    handler_ = new TimesectionHandler;
    InitPage();
    InitSignalSlots();
    UpdateTree();
}

const QString &TimesectiontableWidget::widget_name()
{
    return widget_name_;
}

void TimesectiontableWidget::UpdateTree()
{
    section_tree_widget_->clear();
    QString str;
    QList<unsigned char> section_id_list = handler_->get_timesection_id_list();
    int tree_level = section_id_list.size();
//    QList<QTreeWidgetItem* > tree_widgetitem_list;
    QList<TimeSection> section_list;
    for (int i = 0; i < tree_level; i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        str.sprintf("%d", section_id_list.at(i));
        item->setText(0, str);

        for (int j = 0; j < 7; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        section_tree_widget_->addTopLevelItem(item);

        if (!handler_->get_timesection_list(section_id_list.at(i)).isEmpty())
        {
            section_list = handler_->get_timesection_list(section_id_list.at(i));
            for (int k = 0; k < section_list.size(); k++)
            {
                QTreeWidgetItem *item_sub = new QTreeWidgetItem;
                str.sprintf("%d", section_list.at(k).time_section_id);
                item_sub->setText(0, str);
                item_sub->setTextAlignment(0, Qt::AlignCenter);

                str.sprintf("%d", section_list.at(k).event_id);
                item_sub->setText(1, str);
                item_sub->setTextAlignment(1, Qt::AlignCenter);

                str.sprintf("%02d:%02d", section_list.at(k).start_hour, section_list.at(k).start_minute);
                item_sub->setText(2, str);
                item_sub->setTextAlignment(2, Qt::AlignCenter);

//                str = get_ctrl_mode_desc(section_list.at(k).ctrl_mode);
                str = handler_->get_ctrl_mode_desc(section_list.at(k).ctrl_mode);
                item_sub->setText(3, str);
                item_sub->setTextAlignment(3, Qt::AlignCenter);

                str.sprintf("%d", section_list.at(k).pattern_id);
                item_sub->setText(4, str);
                item_sub->setTextAlignment(4, Qt::AlignCenter);

                str.sprintf("%d", section_list.at(k).aux_func);
                item_sub->setText(5, str);
                item_sub->setTextAlignment(5, Qt::AlignCenter);

                str.sprintf("%d", section_list.at(k).spec_func);
                item_sub->setText(6, str);
                item_sub->setTextAlignment(6, Qt::AlignCenter);

                item->addChild(item_sub);
            }
        }
    }
}

void TimesectiontableWidget::OnTreeItemDoubleClicked(QTreeWidgetItem *, int)
{
    if (timesection_edit_dlg_ == NULL)
    {
        return;
    }

    QTreeWidgetItem* item = section_tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    unsigned char section_id = item->text(0).toInt();
    timesection_edit_dlg_->Initialize(section_id, handler_);
}

void TimesectiontableWidget::OnAddActionClicked()
{
    if (section_tree_widget_->topLevelItemCount() >= MAX_TIMESECTION_LINE)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_TIMESECTION_TOP_LIMITED + QString::number(MAX_TIMESECTION_LINE) + " !", STRING_OK);
        return;
    }
    QString str = "-";
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setText(0, str);
    item->setTextAlignment(0, Qt::AlignCenter);
    section_tree_widget_->addTopLevelItem(item);
}

void TimesectiontableWidget::OnEditActionClicked()
{
    QTreeWidgetItem* item = section_tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
    unsigned char time_section_id = item->text(0).toInt();
    timesection_edit_dlg_->Initialize(time_section_id, handler_);
}

void TimesectiontableWidget::OnDeleteActionClicked()
{
	QTreeWidgetItem *item = section_tree_widget_->currentItem();
	if (item == NULL)
	{
		return;
	}

	unsigned char id = item->text(0).toInt();
	unsigned char event_id = item->text(1).toInt();
	int curr_row = section_tree_widget_->indexOfTopLevelItem(item);
	if (curr_row < 0)
	{
		QTreeWidgetItem *item_parent = item->parent();
		if (item_parent == NULL)
		{
			return;
		}
		handler_->remove_timesection_event(id, event_id);
		int index = item_parent->indexOfChild(item);
		item_parent->takeChild(index);
	}
	else
	{
		handler_->remove_timesection(id);
		section_tree_widget_->takeTopLevelItem(curr_row);
	}
}

void TimesectiontableWidget::OnSaveActionClicked()
{
    if (section_tree_widget_->children().size() > 0)
    {
        emit timesectionEditedSignal();
    }
	handler_->save_data();
}

void TimesectiontableWidget::OnUpdateTreeSlot(unsigned char time_section_id)
{
    Q_UNUSED(time_section_id);
    QTreeWidgetItem* item = section_tree_widget_->currentItem();
    if (item == NULL)
    {
        return;
    }
	UpdateTree();
}

void TimesectiontableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void TimesectiontableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void TimesectiontableWidget::OnInitDatabase(void *db_ptr)
{
    handler_->init_database(db_ptr);
}

void TimesectiontableWidget::InitPage()
{
    InitTree();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(section_tree_widget_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void TimesectiontableWidget::InitSignalSlots()
{
    connect(section_tree_widget_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(section_tree_widget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnTreeItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(timesection_edit_dlg_, SIGNAL(updateTreeSignal(unsigned char)), this, SLOT(OnUpdateTreeSlot(unsigned char)));
}

void TimesectiontableWidget::InitTree()
{
    section_tree_widget_ = new QTreeWidget;
    section_tree_widget_->setColumnCount(7);
    QStringList header;
    header << STRING_UI_TIMESECTION_ID << STRING_UI_TIMESECTION_EVENT_ID << STRING_UI_TIMESECTION_TIME << STRING_UI_TIMESECTION_CTRL_MODE
           << STRING_UI_TIMESECTION_TIMING_ID << STRING_UI_TIMESECTION_FUNC_ID << STRING_UI_TIMESECTION_SPECIAL_ID;
    section_tree_widget_->setHeaderLabels(header);
    section_tree_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    section_tree_widget_->setSelectionBehavior(QTreeWidget::SelectRows);
    section_tree_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    section_tree_widget_->setPalette(pal);
    section_tree_widget_->clearFocus();
    section_tree_widget_->setStyleSheet("QHeaderView::section{background-color: rgb(184,219,255); text-align:center;} font:12px;");
    QHeaderView *header_view = section_tree_widget_->header();
    header_view->setDefaultAlignment(Qt::AlignCenter);
	section_tree_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
}

void TimesectiontableWidget::InitContextMenu()
{
    context_menu_ = new QMenu(section_tree_widget_);
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

void TimesectiontableWidget::AddChild(QTreeWidgetItem* item, unsigned char time_section_id)
{
    QTreeWidgetItem* item_sub = NULL;
	item->takeChildren();
    QString str;
    QList<TimeSection> time_section_list = handler_->get_timesection_list(time_section_id);
    for (int i = 0;i < time_section_list.size(); i++)
    {
        item_sub = new QTreeWidgetItem;
        str.sprintf("%d", time_section_list.at(i).time_section_id);
        item_sub->setText(0, str);
        item_sub->setTextAlignment(0, Qt::AlignCenter);

        str.sprintf("%d", time_section_list.at(i).event_id);
        item_sub->setText(1, str);
        item_sub->setTextAlignment(1, Qt::AlignHCenter);

        str.sprintf("%d:%d", time_section_list.at(i).start_hour, time_section_list.at(i).start_minute);
        item_sub->setText(2, str);
        item_sub->setTextAlignment(2, Qt::AlignCenter);

        str = handler_->get_ctrl_mode_desc(time_section_list.at(i).ctrl_mode);
        item_sub->setText(3, str);
        item_sub->setTextAlignment(3, Qt::AlignCenter);

        str.sprintf("%d", time_section_list.at(i).pattern_id);
        item_sub->setText(4, str);
        item_sub->setTextAlignment(4, Qt::AlignCenter);

        str.sprintf("%d", time_section_list.at(i).aux_func);
        item_sub->setText(5, str);
        item_sub->setTextAlignment(5, Qt::AlignCenter);

        str.sprintf("%d", time_section_list.at(i).spec_func);
        item_sub->setText(6, str);
        item_sub->setTextAlignment(6, Qt::AlignCenter);

        item->addChild(item_sub);
    }
	str.sprintf("%d", time_section_id);
	item->setText(0, str);
}

QString TimesectiontableWidget::get_ctrl_mode_desc(unsigned char ctrl_mode)
{
    QString str;
    switch (ctrl_mode)
    {
    case 0:
        str = STRING_UI_UNIT_AUTO_CONTORL;
        break;
    case 1:
        str = STRING_UI_UNIT_CLOSE_LIGHTS;
        break;
    case 2:
        str = STRING_UI_UNIT_FLASH;
        break;
    case 3:
        str = STRING_UI_UNIT_ALLRED;
        break;
    case 4:
        str = STRING_UI_UNIT_COORDINATE;
        break;
    case 5:
        str = STRING_UI_UNIT_INDUCTION;
        break;
    case 6:
        str = STRING_UI_UNIT_MAIN_INDUCTION;
        break;
    case 7:
        str = STRING_UI_UNIT_SECOND_INDUCTION;
        break;
    case 8:
        str = STRING_UI_UNIT_SINGLE_OPTIONAL;
        break;
    case 9:
        str = STRING_UI_UNIT_CROSS_STREET;
        break;
    case 10:
        str = STRING_UI_UNIT_COORDINATE_INDUCTION;
        break;

//    case 11:
//        str = STRING_UI_UNIT_MASTER_SLAVE;
//        break;
//    case 12:
//        str = STRING_UI_UNIT_SYSTEM_OPTIONAL;
//        break;
//    case 13:
//        str = STRING_UI_UNIT_INTERVENTION;
//        break;
    // addtional selections
    case 27:
        str = STRING_UI_UNIT_BUS_FIRST;
        break;
    case 28:
        str = STRING_UI_UNIT_TRAFFIC_CTRL;
        break;
    case 29:
        str = STRING_UI_UNIT_MANUALLY_CTRL;
        break;
    case 30:
        str = STRING_UI_UNIT_SYS_FAILURE_FLASH;
        break;
    default:
        str = "-";
        break;
    }
    return str;
}

TimesectiontableWidget::~TimesectiontableWidget()
{
    if (timesection_edit_dlg_ != NULL)
    {
        delete timesection_edit_dlg_;
        timesection_edit_dlg_ = NULL;
    }
    if (handler_ != NULL)
    {
        delete handler_;
        handler_ = NULL;
    }
}
