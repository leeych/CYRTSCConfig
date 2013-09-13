#include "eventlogdlg.h"
#include "macrostring.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

#include <QMessageBox>

EventLogDlg::EventLogDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

EventLogDlg::~EventLogDlg()
{
}

void EventLogDlg::Initialize()
{
    UpdateUI();
    exec();
}

void EventLogDlg::OnReadLogButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Read log", STRING_OK);
}

void EventLogDlg::OnRemoveEventButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Remove event", STRING_OK);
}

void EventLogDlg::OnExportLogButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Export event log", STRING_OK);
}

void EventLogDlg::OnExportReportButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Export report", STRING_OK);
}

void EventLogDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_EVENT_TITLE);
    event_tree_ = new QTreeWidget;
    QStringList event_header;
    event_header << STRING_UI_SIGNALER_EVENT_TYPE << STRING_UI_SIGNALER_EVENT_CLEARTIME;
    InitTree(event_tree_, event_header);
    event_tree_->setMaximumWidth(240);

    event_detail_tree_ = new QTreeWidget;
    QStringList detail_header;
    detail_header << STRING_UI_SIGNALER_EVENT_TYPE_ID << STRING_UI_SIGNALER_EVENT_FLOW_ID
                     << STRING_UI_SIGNALER_EVENT_DATETIME << STRING_UI_SIGNALER_EVENT_DESC;
    InitTree(event_detail_tree_, detail_header);

    QHBoxLayout *tree_hlayout = new QHBoxLayout;
    tree_hlayout->addWidget(event_tree_);
    tree_hlayout->addWidget(event_detail_tree_);
    tree_hlayout->setStretch(0, 1);
    tree_hlayout->setStretch(1, 3);

    read_log_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_READ_LOG);
    remove_event_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_REMOVE_EVENT);
    export_log_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_EXPORT_LOG);
    export_report_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_EXPORT_REPORT);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(read_log_button_);
    button_hlayout->addWidget(remove_event_button_);
    button_hlayout->addWidget(export_log_button_);
    button_hlayout->addWidget(export_report_button_);
    button_hlayout->addStretch(1);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void EventLogDlg::InitSignalSlots()
{
    connect(read_log_button_, SIGNAL(clicked()), this, SLOT(OnReadLogButtonClicked()));
    connect(remove_event_button_, SIGNAL(clicked()), this, SLOT(OnRemoveEventButtonClicked()));
    connect(export_log_button_, SIGNAL(clicked()), this, SLOT(OnExportLogButtonClicked()));
    connect(export_report_button_, SIGNAL(clicked()), this, SLOT(OnExportReportButtonClicked()));
}

void EventLogDlg::UpdateUI()
{
}

void EventLogDlg::InitTree(QTreeWidget *tree, const QStringList &header)
{
    int col_count = header.size();
    tree->setColumnCount(col_count);
    tree->setHeaderLabels(header);

    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionBehavior(QTreeWidget::SelectRows);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(233, 246, 254));
    tree->setPalette(pal);
    tree->clearFocus();
    tree->setStyleSheet("QHeaderView::section{background-color: rgb(184, 219, 255); text-align:center;}");
    QHeaderView *header_view = tree->header();
    header_view->setDefaultAlignment(Qt::AlignCenter);
}
