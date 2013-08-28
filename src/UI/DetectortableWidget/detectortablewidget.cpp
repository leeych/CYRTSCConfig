#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

#include "detectortablewidget.h"
#include "macrostring.h"


DetectortableWidget::DetectortableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    detector_edit_dlg_ = new DetectoreditDlg;
    handler_ = new DetectorHandler;
    InitPage();
    InitSignalSlots();
    UpdateTable();
}

const QString &DetectortableWidget::widget_name()
{
    return widget_name_;
}

void DetectortableWidget::UpdateTable()
{
    detector_table_->clear();
    QString str;
    QList<DetectorParam> detector_list = handler_->get_detector_list();
    int table_row = detector_list.size();
    detector_table_->setRowCount(table_row);
    for (int i = 0; i < table_row; i++)
    {
        QTableWidgetItem* item_id = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_id);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_phase = new QTableWidgetItem;
		str = get_detector_phase_ids_desc(detector_list.at(i).detector_phase_ids);
        item_phase->setText(str);
        item_phase->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 1, item_phase);

        QTableWidgetItem* item_mode = new QTableWidgetItem;
        str = get_detector_type_desc(detector_list.at(i).detector_type);
        item_mode->setText(str);
        item_mode->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 2, item_mode);

        QTableWidgetItem* item_direction = new QTableWidgetItem;
        str = get_detector_direction(detector_list.at(i).detector_direction);
        item_direction->setText(str);
        item_direction->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 3, item_direction);

        // request effective time
        QTableWidgetItem* item_eff_time = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_effective_time);
        item_eff_time->setText(str);
        item_eff_time->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 4, item_eff_time);

        QTableWidgetItem* item_failure = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_failure_time);
        item_failure->setText(str);
        item_failure->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 5, item_failure);

        // to be determined
        QTableWidgetItem* item_option = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_spec_func);
        item_option->setText(str);
        item_option->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 6, item_option);

        QTableWidgetItem* item_flow = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_flow);
        item_flow->setText(str);
        item_flow->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 7, item_flow);

        QTableWidgetItem* item_share = new QTableWidgetItem;
        str.sprintf("%d", detector_list.at(i).detector_occupy);
        item_share->setText(str);
        item_share->setTextAlignment(Qt::AlignCenter);
        detector_table_->setItem(i, 8, item_share);
    }
    InitTableHeader();
}

void DetectortableWidget::OnTableCellDoubleClicked(int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    if (detector_edit_dlg_ == NULL)
    {
        return;
    }
    if (row < 0)
    {
        return;
    }
    unsigned char detector_id = detector_table_->item(row, 0)->text().toInt();
    detector_edit_dlg_->Initialize(detector_id, handler_);
}

void DetectortableWidget::OnAddActionClicked()
{
	int table_row = detector_table_->rowCount();
	if (table_row >= MAX_DETECTOR)
	{
		QMessageBox::information(NULL, STRING_TIP, STRING_UI_DETECTOR_TOP_LIMITED + QString::number(MAX_DETECTOR) + " !", STRING_OK);
		return;
	}
    AddTableRow();
}

void DetectortableWidget::OnEditActionClicked()
{
    if (detector_edit_dlg_ == NULL)
    {
        return;
    }
    int row = detector_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    unsigned char detector_id = detector_table_->item(row, 0)->text().toInt();
    detector_edit_dlg_->Initialize(detector_id, handler_);
}

void DetectortableWidget::OnDeleteActionClicked()
{
    int row = detector_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    unsigned char detector_id = detector_table_->item(row, 0)->text().toInt();
    int index = handler_->index_of_detector_list(detector_id);
    if (index != -1)
    {
        handler_->remove_detector(detector_id);
    }
    detector_table_->removeRow(row);
}

void DetectortableWidget::OnSaveActionClicked()
{
    QMessageBox::information(NULL, STRING_TIP, "Save", STRING_OK);
    return;
}

void DetectortableWidget::OnUpdateTableRowSlot(unsigned char detector_id)
{
    Q_UNUSED(detector_id);
    int row = detector_table_->currentRow();
    if (row < 0)
    {
        return;
    }

	UpdateTable();

    /*DetectorParam detector;
    handler_->get_detector(detector_id, detector);

    QString str;
    str.sprintf("%d", detector.detector_id);
    QTableWidgetItem* item = detector_table_->item(row, 0);
    item->setText(str);

    str = get_detector_phase_ids_desc(detector.detector_phase_ids);
    item = detector_table_->item(row, 1);
    item->setText(str);

    str = get_detector_type_desc(detector.detector_type);
    item = detector_table_->item(row, 2);
    item->setText(str);

    str = get_detector_direction(detector.detector_direction);
    item = detector_table_->item(row, 3);
    item->setText(str);

    str.sprintf("%d", detector.detector_effective_time);
    item = detector_table_->item(row, 4);
    item->setText(str);

    str.sprintf("%d", detector.detector_failure_time);
    item = detector_table_->item(row, 5);
    item->setText(str);

    str.sprintf("%d", detector.detector_spec_func);
    item = detector_table_->item(row, 6);
    item->setText(str);

    str.sprintf("%d", detector.detector_flow);
    item = detector_table_->item(row, 7);
    item->setText(str);

    str.sprintf("%d", detector.detector_occupy);
    item = detector_table_->item(row, 8);
    item->setText(str);*/
}

void DetectortableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void DetectortableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void DetectortableWidget::InitPage()
{
    InitTable();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(detector_table_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void DetectortableWidget::InitSignalSlots()
{
    connect(detector_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(detector_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(detector_edit_dlg_, SIGNAL(updateTableRowSignal(unsigned char)), this, SLOT(OnUpdateTableRowSlot(unsigned char)));
}

void DetectortableWidget::InitTable()
{
    detector_table_ = new QTableWidget;
    detector_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detector_table_->setSelectionBehavior(QTableWidget::SelectRows);
    detector_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    detector_table_->verticalHeader()->setHidden(true);
    detector_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    detector_table_->horizontalHeader()->setClickable(false);
    detector_table_->horizontalHeader()->setStretchLastSection(true);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    detector_table_->setPalette(pal);
    detector_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    detector_table_->clearFocus();
    detector_table_->setShowGrid(true);
	detector_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void DetectortableWidget::InitTableHeader()
{
    detector_table_->setColumnCount(9);
    int width = size().width();
    detector_table_->setColumnWidth(0, width / 9);
    detector_table_->setColumnWidth(1, width / 9);
    detector_table_->setColumnWidth(2, width / 9);
    detector_table_->setColumnWidth(3, width / 9);
    detector_table_->setColumnWidth(4, width / 9);
    detector_table_->setColumnWidth(5, width / 9);
    detector_table_->setColumnWidth(6, width / 9);
    detector_table_->setColumnWidth(7, width / 9);
    detector_table_->setColumnWidth(8, width / 9);

    QStringList headers;
    headers << STRING_UI_DETECTOR_ID << STRING_UI_DETECTOR_PHASE << STRING_UI_DETECTOR_MODE
            << STRING_UI_DETECTOR_DIRECTION << STRING_UI_DETECTOR_EFF_TIME << STRING_UI_DETECTOR_UNEFF_TIME
               << STRING_UI_DETECTOR_OPTION << STRING_UI_DETECTOR_FLOW << STRING_UI_DETECTOR_SHARE;
    detector_table_->setHorizontalHeaderLabels(headers);
}

void DetectortableWidget::InitContextMenu()
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

void DetectortableWidget::AddTableRow()
{
    QString str = "-";
    int table_row = detector_table_->rowCount();
    detector_table_->setRowCount(table_row + 1);

    QTableWidgetItem* item_id = new QTableWidgetItem;
    item_id->setText(str);
    item_id->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 0, item_id);

    QTableWidgetItem* item_phase = new QTableWidgetItem;
    item_phase->setText(str);
    item_phase->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 1, item_phase);

    QTableWidgetItem* item_mode = new QTableWidgetItem;
    item_mode->setText(str);
    item_mode->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 2, item_mode);

    QTableWidgetItem* item_direction = new QTableWidgetItem;
    item_direction->setText(str);
    item_direction->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 3, item_direction);

    QTableWidgetItem* item_eff_time = new QTableWidgetItem;
    item_eff_time->setText(str);
    item_eff_time->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 4, item_eff_time);

    QTableWidgetItem* item_failure = new QTableWidgetItem;
    item_failure->setText(str);
    item_failure->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 5, item_failure);

    QTableWidgetItem* item_option = new QTableWidgetItem;
    item_option->setText(str);
    item_option->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 6, item_option);

    QTableWidgetItem* item_flow = new QTableWidgetItem;
    item_flow->setText(str);
    item_flow->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 7, item_flow);

    QTableWidgetItem* item_share = new QTableWidgetItem;
    item_share->setText(str);
    item_share->setTextAlignment(Qt::AlignCenter);
    detector_table_->setItem(table_row, 8, item_share);
}

QString DetectortableWidget::get_detector_type_desc(unsigned char type)
{
    //return handler_->get_detector_type_desc(type);
	QString str = "-";
	switch (type)
	{
	case 0x080:
		str = STRING_UI_DETECTOR_REQUEST;
		break;
	case 0x040:
		str = STRING_UI_DETECTOR_INDUCTION;
		break;
	case 0x020:
		str = STRING_UI_DETECTOR_TACTICS;
		break;
	case 0x010:
		str = STRING_UI_DETECTOR_STRATEGY;
		break;
	case 0x008:
		str = STRING_UI_DETECTOR_MAN;
		break;
	case 0x004:
		str = STRING_UI_DETECTOR_BUS;
		break;
	case 0x002:
		str = STRING_UI_DETECTOR_BIKE;
		break;
	case 0x001:
		str = STRING_UI_DETECTOR_MOTOR;
		break;
	default:
		break;
	}
	return str;
}

QString DetectortableWidget::get_detector_direction(unsigned char direction)
{
    //return handler_->get_detector_direction_desc(direction);
	QString str = "-";
	switch (direction)
	{
	case 0x080:
		str = STRING_UI_CHANNEL_NORTH_WEST;
		break;
	case 0x040:
		str = STRING_UI_CHANNEL_WEST;
		break;
	case 0x020:
		str = STRING_UI_CHANNEL_SOUTH_WEST;
		break;
	case 0x010:
		str = STRING_UI_CHANNEL_SOUTH;
		break;
	case 0x008:
		str = STRING_UI_CHANNEL_SOUTH_EAST;
		break;
	case 0x004:
		str = STRING_UI_CHANNEL_EAST;
		break;
	case 0x002:
		str = STRING_UI_CHANNEL_NORTH_EAST;
		break;
	case 0x001:
		str = STRING_UI_CHANNEL_NORTH;
		break;
	default:
		break;
	}
	return str;
}

void DetectortableWidget::resizeEvent( QResizeEvent * )
{
	int width = detector_table_->width();
	detector_table_->setColumnWidth(0, width / 9);
	detector_table_->setColumnWidth(1, width / 9);
	detector_table_->setColumnWidth(2, width / 9);
	detector_table_->setColumnWidth(3, width / 9);
	detector_table_->setColumnWidth(4, width / 9);
	detector_table_->setColumnWidth(5, width / 9);
	detector_table_->setColumnWidth(6, width / 9);
	detector_table_->setColumnWidth(7, width / 9);
	detector_table_->setColumnWidth(8, width / 9);
}

QString DetectortableWidget::get_detector_phase_ids_desc( unsigned int phase_ids )
{
	QString str;
    for (size_t i = 1; i <= sizeof(phase_ids) * 8; i++)
	{
		if ((phase_ids & 0x01) == 0x01)
		{
			str += QString::number(i) + "/";
		}
		phase_ids = phase_ids >> 1;
	}
	if (str.isEmpty())
	{
		str = "-";
		return str;
	}
	return str.left(str.size() - 1);
}

DetectortableWidget::~DetectortableWidget()
{
	delete detector_edit_dlg_;
	delete handler_;
}
