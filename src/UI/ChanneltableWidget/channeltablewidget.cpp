#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

#include "channeltablewidget.h"
#include "macrostring.h"

#define WIDGET_CENTER
#include "utilmacros.h"


ChanneltableWidget::ChanneltableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    channel_edit_dlg_ = new ChanneleditDlg(this);
    handler_ = new ChannelHandler;
    InitPage();
    InitSignalSlots();

    UpdateTable();
}

const QString &ChanneltableWidget::widget_name()
{
    return widget_name_;
}

void ChanneltableWidget::UpdateTable()
{
    channel_table_->clearContents();
    QString str;
    QList<ChannelParam> channel_list = handler_->get_channel_list();
    int table_row = channel_list.size();
    channel_table_->setRowCount(table_row);
    for (int i = 0; i < table_row; i++)
    {
        QTableWidgetItem* item_id = new QTableWidgetItem;
        str.sprintf("%d", channel_list.at(i).channel_id);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_status = new QTableWidgetItem;
        str.sprintf("%d", channel_list.at(i).channel_flash);
        item_status->setText(str);
        item_status->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(i, 1, item_status);

        QTableWidgetItem* item_ctrl_mode = new QTableWidgetItem;
        str = handler_->get_channel_type_desc(channel_list.at(i).channel_type);
        item_ctrl_mode->setText(str);
        item_ctrl_mode->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(i, 2, item_ctrl_mode);

        QTableWidgetItem* item_direction = new QTableWidgetItem;
        str = get_channel_direction_desc(channel_list.at(i).channel_direction);
        item_direction->setText(str);
        item_direction->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(i, 3, item_direction);

        QTableWidgetItem* item_lane = new QTableWidgetItem;
        str = get_channel_lane_desc(channel_list.at(i).channel_road);
        item_lane->setText(str);
        item_lane->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(i, 4, item_lane);
    }
//    InitTableHeader();
}

void ChanneltableWidget::OnTableCellDoubleClicked(int row, int col)
{
    Q_UNUSED(col);
    if (channel_edit_dlg_ != NULL)
    {
        if (row < 0)
        {
            return;
        }
        WIDGET_CENTRALIZE(channel_edit_dlg_);
        unsigned char channel_id = channel_table_->item(row, 0)->text().toInt();
        channel_edit_dlg_->Initialize(channel_id, handler_);
    }
}

void ChanneltableWidget::OnAddActionClicked()
{
    int table_row = channel_table_->rowCount();
	if (table_row >= MAX_CHANNEL)
	{
        QMessageBox::information(this, STRING_TIP, STRING_UI_CHANNEL_TOP_LIMITED + QString::number(MAX_CHANNEL) + " !", STRING_OK);
		return;
	}
	QString str;
    channel_table_->setRowCount(table_row + 1);
    for (int i = 0; i < channel_table_->columnCount(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;
        str = "-";
        item->setText(str);
        item->setTextAlignment(Qt::AlignCenter);
        channel_table_->setItem(table_row, i, item);
    }
}

void ChanneltableWidget::OnEditActionClicked()
{
    int row = channel_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    WIDGET_CENTRALIZE(channel_edit_dlg_);
    unsigned char channel_id = channel_table_->item(row, 0)->text().toInt();
    channel_edit_dlg_->Initialize(channel_id, handler_);
}

void ChanneltableWidget::OnDeleteActionClicked()
{
    int row = channel_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    unsigned char channel_id = channel_table_->item(row, 0)->text().toInt();
    if (handler_->is_channel_exists(channel_id))
    {
        handler_->remove_channel(channel_id);
    }
    channel_table_->removeRow(row);
}

void ChanneltableWidget::OnSaveActionClicked()
{
    if (channel_table_->rowCount() > 0)
    {
        emit channelEditedSignal();
    }
	handler_->save_data();
}

void ChanneltableWidget::OnUpdateTableRowSlot(unsigned char channel_id)
{
    Q_UNUSED(channel_id);
    UpdateTable();
}

void ChanneltableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void ChanneltableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void ChanneltableWidget::OnUpdateChannelCtrlsrcSlot()
{
    // TODO: update channel ctrl src field
}

void ChanneltableWidget::OnInitDatabase(void *db_ptr)
{
    handler_->init_database(db_ptr);
}

void ChanneltableWidget::InitPage()
{
    InitTable();
    InitContextMenu();
    buttons_widget_ =  new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(channel_table_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void ChanneltableWidget::InitSignalSlots()
{
    connect(channel_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(channel_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(channel_edit_dlg_, SIGNAL(updateTableRowSignal(unsigned char)), this, SLOT(OnUpdateTableRowSlot(unsigned char)));
}

void ChanneltableWidget::InitTable()
{
    channel_table_ = new QTableWidget;
    channel_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    channel_table_->setSelectionBehavior(QTableWidget::SelectRows);
    channel_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    channel_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    channel_table_->verticalHeader()->setHidden(true);
    channel_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    channel_table_->horizontalHeader()->setClickable(false);
    channel_table_->horizontalHeader()->setStretchLastSection(true);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    channel_table_->setPalette(pal);
	channel_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void ChanneltableWidget::InitTableHeader()
{
    channel_table_->setColumnCount(5);
    int width = size().width();
    channel_table_->setColumnWidth(0, width / 5);
    channel_table_->setColumnWidth(1, width / 5);
    channel_table_->setColumnWidth(2, width / 5);
    channel_table_->setColumnWidth(3, width / 5);
    channel_table_->setColumnWidth(4, width / 5);
    QStringList header;
    header << STRING_UI_CHANNEL_ID << STRING_UI_CHANNEL_STATUS << STRING_UI_CHANNEL_CONTROL_MODE
              << STRING_UI_CHANNEL_DIRECTION << STRING_UI_CHANNEL_LANE;
    channel_table_->setHorizontalHeaderLabels(header);
}

void ChanneltableWidget::InitContextMenu()
{
    context_menu_ = new QMenu;
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

ChanneltableWidget::~ChanneltableWidget()
{
	delete channel_edit_dlg_;
	delete handler_;
}

QString ChanneltableWidget::get_channel_direction_desc( unsigned char direction )
{
	QString str;
	switch (direction)
	{
	case 0:
		str = STRING_UI_CHANNEL_EAST;
		break;
	case 1:
		str = STRING_UI_CHANNEL_SOUTH;
		break;
	case 2:
		str = STRING_UI_CHANNEL_WEST;
		break;
	case 3:
		str = STRING_UI_CHANNEL_NORTH;
		break;
	case 4:
		str = STRING_UI_CHANNEL_SOUTH_EAST;
		break;
	case 5:
		str = STRING_UI_CHANNEL_NORTH_EAST;
		break;
	case 6:
		str = STRING_UI_CHANNEL_SOUTH_WEST;
		break;
	case 7:
		str = STRING_UI_CHANNEL_NORTH_WEST;
		break;
	default:
		break;
	}
	return str;
}

QString ChanneltableWidget::get_channel_lane_desc( unsigned char lane )
{
	QString str;
	switch (lane)
	{
	case 0:
		str = STRING_UI_CHANNEL_LINE;
		break;
	case 1:
		str = STRING_UI_CHANNEL_TURN_LEFT;
		break;
	case 2:
		str = STRING_UI_CHANNEL_TURN_RIGHT;
		break;
	case 3:
		str = STRING_UI_CHANNEL_SIDEWALK;
		break;
	default:
		break;
	}
	return str;
}
