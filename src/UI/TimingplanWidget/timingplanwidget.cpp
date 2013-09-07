#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>

#include "timingplanwidget.h"
#include "macrostring.h"

TimingplanWidget::TimingplanWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    timing_edit_dlg_ = new TimingeditDlg(this);
    handler_ = new TimingHandler;
    InitPage();
    InitSignalSlots();

    UpdateTable();
}

const QString &TimingplanWidget::widget_name()
{
    return widget_name_;
}

void TimingplanWidget::UpdateTable()
{
    timing_table_->clear();
    QList<TimingParam> timing_list = handler_->get_timing_plan_list();
    int table_row = timing_list.size();
    timing_table_->setRowCount(table_row);
    QString str;
    for (int i = 0; i < table_row; i++)
    {
        QTableWidgetItem* item_id = new QTableWidgetItem;
        str.sprintf("%d", timing_list.at(i).timing_id);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_cycle = new QTableWidgetItem;
        str.sprintf("%d", timing_list.at(i).cycle_time);
        item_cycle->setText(str);
        item_cycle->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(i, 1, item_cycle);

        QTableWidgetItem* item_phase = new QTableWidgetItem;
        str.sprintf("%d", timing_list.at(i).phase_offset);
        item_phase->setText(str);
        item_phase->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(i, 2, item_phase);

        QTableWidgetItem* item_coor_phase = new QTableWidgetItem;
        str.sprintf("%d", timing_list.at(i).coor_phase);
        item_coor_phase->setText(str);
        item_coor_phase->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(i, 3, item_coor_phase);

        QTableWidgetItem* item_phase_timing = new QTableWidgetItem;
        str.sprintf("%d", timing_list.at(i).stage_timing_id);
        item_phase_timing->setText(str);
        item_phase_timing->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(i, 4, item_phase_timing);
    }
    InitTableHeader();
}

void TimingplanWidget::OnAddActionClicked()
{
    int table_row = timing_table_->rowCount();
	if (table_row >= MAX_PATTERN_LINE)
	{
        QMessageBox::information(this, STRING_TIP, STRING_UI_TIMING_TOP_LIMITED + QString::number(MAX_PATTERN_LINE) + " !", STRING_OK);
		return;
	}

    timing_table_->setRowCount(table_row + 1);
    QString str;
    for (int i = 0; i < timing_table_->columnCount(); i++)
    {
        str = "-";
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(str);
        item->setTextAlignment(Qt::AlignCenter);
        timing_table_->setItem(table_row, i, item);
    }
}

void TimingplanWidget::OnEditActionClicked()
{
    if (timing_edit_dlg_ != NULL)
    {
        int row = timing_table_->currentRow();
        if (row < 0)
        {
            return;
        }
        unsigned char id = timing_table_->item(row, 0)->text().toInt();
        timing_edit_dlg_->Initialize(id, handler_);
    }
}

void TimingplanWidget::OnDeleteActionClicked()
{
    int row = timing_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    unsigned char timing_id = timing_table_->item(row, 0)->text().toInt();
    if (handler_->is_timing_plan_exists(timing_id))
    {
        handler_->remove_timing_plan(timing_id);
    }
    timing_table_->removeRow(row);
}

void TimingplanWidget::OnSaveActionClicked()
{
	handler_->save_data();
}

void TimingplanWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void TimingplanWidget::OnTableCellDoubleClicked(int row, int col)
{
	Q_UNUSED(col);
	if (timing_edit_dlg_ == NULL || row < 0)
	{
		return;
	}
    unsigned char timing_id = timing_table_->item(row, 0)->text().toInt();
    timing_edit_dlg_->Initialize(timing_id, handler_);
}

void TimingplanWidget::OnUpdateTableRowSlot(unsigned char id)
{
    TimingParam plan;
    if (!handler_->get_timing_plan(id, plan))
    {
        QMessageBox::information(this, STRING_TIP, "Do not exists this plan", STRING_OK);
        return;
    }
	UpdateTable();
}

void TimingplanWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void TimingplanWidget::OnUpdateTimingCycleSlot()
{
    // TODO: update cycle time from mdatabase
    handler_->update_cycle_time();
    UpdateCycletime();
}

void TimingplanWidget::InitPage()
{
    InitTable();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(timing_table_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void TimingplanWidget::InitSignalSlots()
{
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(timing_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));

    connect(timing_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(timing_edit_dlg_, SIGNAL(updateTableRow(unsigned char)), this, SLOT(OnUpdateTableRowSlot(unsigned char)));
}

void TimingplanWidget::InitTable()
{
    timing_table_ = new QTableWidget;
    timing_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    timing_table_->setSelectionBehavior(QTableWidget::SelectRows);
    timing_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    timing_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    timing_table_->horizontalHeader()->setClickable(false);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    timing_table_->setPalette(pal);
    timing_table_->verticalHeader()->setHidden(true);
    timing_table_->clearFocus();
    timing_table_->setShowGrid(true);
    timing_table_->horizontalHeader()->setStretchLastSection(true);
	timing_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void TimingplanWidget::InitTableHeader()
{
    timing_table_->setColumnCount(5);
    timing_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    int width = size().width();
    timing_table_->setColumnWidth(0, width / 5);
    timing_table_->setColumnWidth(1, width / 5);
    timing_table_->setColumnWidth(2, width / 5);
    timing_table_->setColumnWidth(3, width / 5);
    timing_table_->setColumnWidth(4, width / 5);
    QStringList headers;
    headers << STRING_UI_TIMING_ID << STRING_UI_TIMING_CYCLE << STRING_UI_TIMING_PHASE
            << STRING_UI_TIMING_COOR_PHASE << STRING_UI_PHASE_TIMING_ID;
    timing_table_->setHorizontalHeaderLabels(headers);
}

void TimingplanWidget::InitContextMenu()
{
    context_menu_ = new QMenu(timing_table_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                           "QMenu::item{background-color:transparent;color:#000000}"
                           "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);

    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);
    save_action_ = new QAction(STRING_UI_SAVE, context_menu_);

    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    context_menu_->addAction(save_action_);
}

void TimingplanWidget::UpdateCycletime()
{
    QTableWidgetItem *item_stage_id = NULL;
    QTableWidgetItem *item_cycle_time = NULL;
    unsigned char stage_id = 0;
    unsigned char cycle_time = 0;

    int table_row = timing_table_->rowCount();
    for (int i = 0; i < table_row; i++)
    {
        item_stage_id = timing_table_->item(i, 4);
        stage_id = item_stage_id->text().toInt();
        cycle_time = handler_->get_cycletime_by_stagetiming_id(stage_id);
        item_cycle_time = timing_table_->item(i, 1);
        item_cycle_time->setText(QString::number(cycle_time));
    }
}

void TimingplanWidget::resizeEvent(QResizeEvent *)
{
    int width = size().width();
    timing_table_->setColumnWidth(0, width / 5);
    timing_table_->setColumnWidth(1, width / 5);
    timing_table_->setColumnWidth(2, width / 5);
    timing_table_->setColumnWidth(3, width / 5);
    timing_table_->setColumnWidth(4, width / 5);
}

TimingplanWidget::~TimingplanWidget()
{
	delete timing_edit_dlg_;
	delete handler_;
}
