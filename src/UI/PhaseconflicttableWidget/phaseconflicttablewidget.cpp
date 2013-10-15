#include <QVBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

#include "phaseconflicttablewidget.h"
#include "macrostring.h"

#define WIDGET_CENTER
#include "utilmacros.h"

PhaseconflicttableWidget::PhaseconflicttableWidget(const QString& name, QWidget* parent)
    : QWidget(parent), widget_name_(name)
{
    phase_edit_dlg_ = new PhaseconflicteditDlg(this);
    handler_ = new PhaseconflictHandler;
    InitPage();
    InitSignalSlots();

    UpdateTable();
}

const QString &PhaseconflicttableWidget::widget_name()
{
    return widget_name_;
}

void PhaseconflicttableWidget::UpdateTable()
{
    QString str;
    int table_row = MAX_PHASE_LINE;
    conflict_table_->setRowCount(table_row);
    for (int i = 0, j = 1; i < table_row; i++, j++)
    {
        QTableWidgetItem* item_id = new QTableWidgetItem;
        str.sprintf("%d", j);
        item_id->setText(str);
        item_id->setTextAlignment(Qt::AlignCenter);
        conflict_table_->setItem(i, 0, item_id);

        QTableWidgetItem* item_confid = new QTableWidgetItem;
        str = get_conflict_desc(j);
        item_confid->setText(str);
        item_confid->setTextAlignment(Qt::AlignCenter);
        conflict_table_->setItem(i, 1, item_confid);
    }
}

void PhaseconflicttableWidget::OnTableCellDoubleClicked(int row, int col)
{
    Q_UNUSED(col);
    if (phase_edit_dlg_ != NULL)
    {
        if (row < 0)
        {
            return;
        }
        WIDGET_CENTRALIZE(phase_edit_dlg_)
        unsigned char phase_id = conflict_table_->item(row, 0)->text().toInt();
        phase_edit_dlg_->Initialize(phase_id, handler_);
    }
}

void PhaseconflicttableWidget::OnUpdateTableRowSlot(unsigned char phase_id)
{
    int row = conflict_table_->currentRow();
    if (row < 0)
    {
        return;
    }
    QString str = get_conflict_desc(phase_id);
    qDebug() << str;
    QTableWidgetItem* item = conflict_table_->item(row, 1);
    item->setText(str);
    //item->setTextAlignment(Qt::AlignCenter);
}

void PhaseconflicttableWidget::OnEditActionClicked()
{
    if (phase_edit_dlg_ != NULL)
    {
        int row = conflict_table_->currentRow();
        if (row < 0)
        {
            return;
        }
        WIDGET_CENTRALIZE(phase_edit_dlg_)
        unsigned char phase_id = conflict_table_->item(row, 0)->text().toInt();
        phase_edit_dlg_->Initialize(phase_id, handler_);
    }
}

void PhaseconflicttableWidget::OnSaveButtonClicked()
{
    if (conflict_table_->rowCount() > 0)
    {
        for (int i = 0; i < conflict_table_->rowCount(); i++)
        {
            if (!conflict_table_->item(i, 1)->text().isEmpty())
            {
                emit phaseErrEditedSignal();
                break;
            }
        }
    }
	handler_->save_data();
}

void PhaseconflicttableWidget::OnCustomContextMenuRequested(QPoint)
{
    if (context_menu_ != NULL)
    {
        context_menu_->exec(QCursor::pos());
    }
}

void PhaseconflicttableWidget::OnUpdateDataSlot()
{
    handler_->init();
}

void PhaseconflicttableWidget::OnInitDatabase(void *db_ptr)
{
    handler_->init_database(db_ptr);
}

void PhaseconflicttableWidget::InitPage()
{
    InitTable();
    InitContextMenu();
    buttons_widget_ = new BottomButtonsWidget;
    buttons_widget_->addButtonPtr()->setEnabled(false);
    buttons_widget_->deleteButtonPtr()->setEnabled(false);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(conflict_table_);
    vlayout->addWidget(buttons_widget_);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);
    setLayout(vlayout);
}

void PhaseconflicttableWidget::InitSignalSlots()
{
    connect(conflict_table_, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    connect(conflict_table_, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(OnTableCellDoubleClicked(int,int)));
    connect(phase_edit_dlg_, SIGNAL(updateTableRowSignal(unsigned char)), this, SLOT(OnUpdateTableRowSlot(unsigned char)));
    connect(edit_action_, SIGNAL(triggered()), this, SLOT(OnEditActionClicked()));
    connect(buttons_widget_->saveButtonPtr(), SIGNAL(clicked()), this, SLOT(OnSaveButtonClicked()));
    connect(buttons_widget_->editButtonPtr(), SIGNAL(clicked()), this, SLOT(OnEditActionClicked()));
}

void PhaseconflicttableWidget::InitTable()
{
    conflict_table_ = new QTableWidget;
    conflict_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    conflict_table_->setSelectionBehavior(QTableWidget::SelectRows);
    conflict_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    conflict_table_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    conflict_table_->verticalHeader()->setHidden(true);
    conflict_table_->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255)}");
    conflict_table_->horizontalHeader()->setClickable(false);
    conflict_table_->horizontalHeader()->setStretchLastSection(true);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    conflict_table_->setPalette(pal);
    conflict_table_->setShowGrid(false);
	conflict_table_->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTableHeader();
}

void PhaseconflicttableWidget::InitTableHeader()
{
    int width = size().width();
    conflict_table_->setColumnCount(2);
    conflict_table_->setColumnWidth(0, width / 3);
    conflict_table_->setColumnWidth(1, width * 2/3);
    QStringList header;
    header << STRING_UI_PHASE_CONFLICT_ID << STRING_UI_PHASE_CONFLICT_CONID;
    conflict_table_->setHorizontalHeaderLabels(header);
}

void PhaseconflicttableWidget::InitContextMenu()
{
    context_menu_ = new QMenu(conflict_table_);
    edit_action_ = new QAction(STRING_UI_EDIT, context_menu_);
    context_menu_->addAction(edit_action_);
    QString menu_qss = "QMenu{background-color: rgb(255,255,255);border: 1px solid rgb(164,209,251)}"
                       "QMenu::item{background-color:transparent;color:#000000}"
                       "QMenu::item:selected{background-color:rgb(164,209,251);color:rgb(255,101,3)}";
    context_menu_->setStyleSheet(menu_qss);
}

QString PhaseconflicttableWidget::get_conflict_desc(unsigned char phase_id)
{
    PhaseConflictParam phase_err;
    if (!handler_->get_phase_conflict(phase_id, phase_err))
    {
        return QString();
    }
    QList<unsigned char> conflict_id_list;
    unsigned int conflict_ids = phase_err.conflict_phase_id;
    for (int i = 32; i > 0; i--)
    {
        if ((conflict_ids & 0x080000000) == 0x080000000)
        {
            conflict_id_list.append(i);
        }
        conflict_ids = conflict_ids << 1;
    }
    QString str, temp;
    for (int i = conflict_id_list.size() - 1; i >= 0; i--)
    {
        temp.sprintf("%d", conflict_id_list.at(i));
        str += temp + "/";
    }
    return str.left(str.lastIndexOf("/"));
}

PhaseconflicttableWidget::~PhaseconflicttableWidget()
{
	delete phase_edit_dlg_;
	delete handler_;
}
