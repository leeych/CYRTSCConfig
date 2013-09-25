#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>

#include "scheduletablewidget.h"
#include "macrostring.h"


ScheduleTableWidget::ScheduleTableWidget(const QString &name, QWidget *parent)
    : QWidget(parent), name_(name)
{
    schedule_dlg_ = new ScheduleDlg(this);
    handler_ = new ScheduleHandler;
    InitPage();
    InitSignalSlots();
    UpdateTable();
}

ScheduleTableWidget::~ScheduleTableWidget()
{
    delete schedule_dlg_;
    delete handler_;
}

const QString &ScheduleTableWidget::widget_name()
{
    return name_;
}

void ScheduleTableWidget::UpdateTable()
{
    QList<ScheduleParam> schedule_list = handler_->get_schedule_list();
    int table_row = schedule_list.size();
    schedule_table_->setRowCount(table_row);
    QString str;
    for (int i = 0; i < schedule_table_->rowCount(); i++)
    {
        QTableWidgetItem* item_id = new QTableWidgetItem;
        str.sprintf("%d", schedule_list.at(i).schedule_id);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_month = new QTableWidgetItem;
        str = GetMonthDisp(schedule_list.at(i).schedule_month);
        item_month->setText(str);
        item_month->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(i, 1, item_month);

        QTableWidgetItem* item_weekday = new QTableWidgetItem;
        str = GetWeekDisp(schedule_list.at(i).schedule_week);
        item_weekday->setText(str);
        item_weekday->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(i, 2, item_weekday);

        QTableWidgetItem* item_day = new QTableWidgetItem;
        str = GetDayDisp(schedule_list.at(i).schedule_day);
        item_day->setText(str);
        item_day->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(i, 3, item_day);

        QTableWidgetItem* item_timesection = new QTableWidgetItem;
        str.sprintf("%d", schedule_list.at(i).time_section_id);
        item_timesection->setText(str);
        item_timesection->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(i, 4, item_timesection);
    }
}

void ScheduleTableWidget::OnAddActionClicked()
{
    int table_row = schedule_table_->rowCount();
	if (table_row >= MAX_SCHEDULE_LINE)
	{
        QMessageBox::information(this, STRING_TIP, STRING_UI_SCHEDULE_TOP_LIMITED + QString::number(MAX_SCHEDULE_LINE) + " !", STRING_OK);
		return;
	}
    schedule_table_->setRowCount(table_row + 1);

    for (int i = 0; i < schedule_table_->columnCount(); i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;
        QString str;
        str = "-";
        item->setText(str);
        item->setTextAlignment(Qt::AlignCenter);
        schedule_table_->setItem(table_row, i, item);
    }
}

void ScheduleTableWidget::OnEditActionClicked()
{
    if (schedule_dlg_ == NULL)
    {
        return;
    }

    int row = schedule_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    unsigned char sched_id = schedule_table_->item(row, 0)->text().toInt();
    schedule_dlg_->Initialize(sched_id, handler_);
}

void ScheduleTableWidget::OnDeleteActionClicked()
{
    int row = schedule_table_->currentRow();
    if (row < 0)
    {
        return;
    }
	unsigned char sched_id = schedule_table_->item(row, 0)->text().toInt();
	if (handler_->is_schedule_exists(sched_id))
	{
		handler_->remove_schedule(sched_id);
	}
    schedule_table_->removeRow(row);
}

void ScheduleTableWidget::OnSaveActionClicked()
{
	handler_->save_data();
}

void ScheduleTableWidget::OnTableCellDoubleClicked(int row, int col)
{
    Q_UNUSED(row);
    Q_UNUSED(col);
    OnEditActionClicked();
}

void ScheduleTableWidget::OnUpdateTableSignal(unsigned char sched_id)
{
    Q_UNUSED(sched_id);
    UpdateTable();
}

void ScheduleTableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void ScheduleTableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void ScheduleTableWidget::OnInitDatabase(void *db_ptr)
{
    handler_->init_database(db_ptr);
}

void ScheduleTableWidget::InitPage()
{
    InitContextMenu();
    InitTable();
    buttons_widget_ = new BottomButtonsWidget;
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(schedule_table_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void ScheduleTableWidget::InitSignalSlots()
{
    connect(add_action_, SIGNAL(triggered()), this, SLOT(OnAddActionClicked()));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(delete_action_, SIGNAL(triggered()), this, SLOT(OnDeleteActionClicked()));
    connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSaveActionClicked()));

    connect(buttons_widget_->addButtonPtr(), SIGNAL(clicked()), this, SLOT(OnAddActionClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->deleteButtonPtr(), SIGNAL(clicked()), this, SLOT(OnDeleteActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveActionClicked()));

    connect(schedule_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(schedule_dlg_, SIGNAL(updateTableSignal(unsigned char)), this, SLOT(OnUpdateTableSignal(unsigned char)));
    connect(schedule_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
}

void ScheduleTableWidget::InitTable()
{
    schedule_table_ = new QTableWidget;
    schedule_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    schedule_table_->setSelectionBehavior(QTableWidget::SelectRows);
    schedule_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    schedule_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    schedule_table_->horizontalHeader()->setClickable(false);

    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    schedule_table_->setPalette(pal);
//    schedule_table_->setStyleSheet("font:9px; text-align:center;");
    schedule_table_->verticalHeader()->setHidden(true);
    schedule_table_->clearFocus();
//    schedule_table_->setStyleSheet("border:0px;");
    schedule_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void ScheduleTableWidget::InitTableHeader()
{
    schedule_table_->setColumnCount(5);
    schedule_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    schedule_table_->setColumnWidth(0, 60);
    schedule_table_->setColumnWidth(1, 140);
    schedule_table_->setColumnWidth(2, 150);
    schedule_table_->setColumnWidth(3, 100);
    schedule_table_->setColumnWidth(4, 50);
    QStringList headers;
    headers << STRING_UI_SCHEDULE_ID << STRING_UI_SCHEDULE_MONTH << STRING_UI_SCHEDULE_WEEKDAY
            << STRING_UI_SCHEDULE_DAY << STRING_UI_TIME_SECTION;
    schedule_table_->setHorizontalHeaderLabels(headers);
    schedule_table_->setShowGrid(true);
    schedule_table_->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{background-color: rgb(184, 219, 255)}");
    schedule_table_->horizontalHeader()->setClickable(false);
    schedule_table_->horizontalHeader()->setStretchLastSection(true);
}

void ScheduleTableWidget::InitContextMenu()
{
    context_menu_ = new QMenu(this);
    add_action_ = new QAction(STRING_UI_ADD, context_menu_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    delete_action_ = new QAction(STRING_UI_DELETE, context_menu_);
    save_action_ = new QAction(STRING_UI_SAVE, context_menu_);

    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                           "QMenu::item{background-color:transparent;color:#000000}"
                           "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
    context_menu_->addAction(add_action_);
    context_menu_->addAction(edit_action_);
    context_menu_->addAction(delete_action_);
    context_menu_->addAction(save_action_);
}

QString ScheduleTableWidget::GetMonthDisp(unsigned short month)
{
    QString str;
    QString tmp;
    for (int i = 1; i <= 12; i++)
    {
        month = (month >> 1);
        if ((month & 0x01) == 0x01)
        {
            tmp += (str.sprintf("%d", i) + "/");
        }
    }
    return tmp.left(tmp.size() - 1);
}

QString ScheduleTableWidget::GetWeekDisp(unsigned char week)
{
    QString str;
    QString tmp;
	QList<int> weekday_list;
	unsigned char week_bak = week;
	week_bak = (week_bak >> 1);
    for (int i = 1; i <= 6; i++)
    {
        week_bak = (week_bak >> 1);
        if ((week_bak & 0x01) == 0x01)
        {
            tmp += (str.sprintf("%d", i) + "/");
        }
    }
	if ((week & 0x02) == 0x02)
	{
		tmp += (QString::number(7) + "/");
	}
    return tmp.left(tmp.size() - 1);
}

QString ScheduleTableWidget::GetDayDisp(unsigned int day)
{
    QString str, tmp;
    for (int i = 1; i <= 31; i++)
    {
        day = (day >> 1);
        if ((day & 0x01) == 0x01)
        {
            tmp += (str.sprintf("%d", i) + "/");
        }
    }
    return tmp.left(tmp.size() - 1);
}

void ScheduleTableWidget::resizeEvent(QResizeEvent *)
{
    int width = size().width();
    schedule_table_->setColumnWidth(0, width / 10);
    schedule_table_->setColumnWidth(1, width / 4);
    schedule_table_->setColumnWidth(2, width / 5);
    schedule_table_->setColumnWidth(3, width / 4);
//    schedule_table_->setColumnWidth(4, width / 10);
}
