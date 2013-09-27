#include "eventlogdlg.h"
#include "macrostring.h"
#include "synccommand.h"
#include "eventloghandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

#include <QFile>
#include <QFileDialog>

EventLogDlg::EventLogDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

EventLogDlg::~EventLogDlg()
{
}

void EventLogDlg::Initialize(const QString &ip, EventLogHandler *handler)
{
    file_name_ = "user/tmp/" + ip + ".edat";
    ip_ = ip;
    handler_ = handler;
    tip_label_->clear();
    UpdateUI();
    exec();
}

void EventLogDlg::OnReadLogButtonClicked()
{
    SyncCommand::GetInstance()->ReadEventLogFile(this, SLOT(OnCmdReadEventLog(QByteArray&)));
}

void EventLogDlg::OnDeleteEventButtonClicked()
{
    QMessageBox::information(this, STRING_TIP, "Remove event", STRING_OK);
    SyncCommand::GetInstance()->DeleteEventLog("1 20130924", this, SLOT(OnCmdDeleteEventLog(QByteArray&)));
}

void EventLogDlg::OnExportLogButtonClicked()
{
    QString log_file = QFileDialog::getSaveFileName(this, STRING_UI_SAVEAS, "./user/log/" + ip_ + ".log" , "Log(*.log);;All File(*.*)");
    if (log_file.isNull() || log_file.isEmpty())
    {
        return;
    }
    bool status = handler_->export_event_log(log_file);
    if (!status)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_LOG_EXPORT_LOG_FAILED, STRING_OK);
        return;
    }
    tip_label_->setText(STRING_UI_SIGNALER_EVENT_EXPORT_LOG + STRING_SUCCEEDED);
}

void EventLogDlg::OnExportReportButtonClicked()
{
    QString report_file = QFileDialog::getSaveFileName(this, STRING_UI_SAVEAS, "./user/report/" + ip_ + ".html", "Html(*.html,*.htm);;All File(*.*)");
    if (report_file.isNull() || report_file.isEmpty())
    {
        return;
    }
    bool status = handler_->export_report(report_file);
    if (!status)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_LOG_EXPORT_HTML_FAILED, STRING_OK);
        return;
    }
    tip_label_->setText(STRING_UI_SIGNALER_EVENT_EXPORT_REPORT + STRING_SUCCEEDED);
}

void EventLogDlg::OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem *item,int col)
{
    if (item == NULL || col < 0)
    {
        return;
    }
    UpdateEventDetailTree();
}

void EventLogDlg::OnCmdReadEventLog(QByteArray &array)
{
    if (array.isEmpty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_SOCKET_NULL, STRING_OK);
        event_log_array_.clear();
        tip_label_->setText(STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_FAILED);
        return;
    }
    event_log_array_.append(array);
    if (event_log_array_.contains("EVENTLOGER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_FAILED, STRING_OK);
        event_log_array_.clear();
        tip_label_->setText(STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_FAILED);
        return;
    }

    if (event_log_array_.endsWith("END"))
    {
        ParseEventLogArray(event_log_array_);
        event_log_array_.clear();
        tip_label_->setText(STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_SUCCEEDED);
        UpdateUI();
    }
}

void EventLogDlg::OnCmdDeleteEventLog(QByteArray &array)
{
    if (array.isEmpty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_SOCKET_NULL, STRING_OK);
        return;
    }
    // TODO: is there any return value left to be determined
    tip_label_->setText(STRING_UI_SIGNALER_EVENT_LOG_DELETE_LOG + STRING_SUCCEEDED);
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
//    button_hlayout->addStretch(1);
    button_hlayout->addWidget(read_log_button_);
    button_hlayout->addWidget(remove_event_button_);
    button_hlayout->addWidget(export_log_button_);
    button_hlayout->addWidget(export_report_button_);
//    button_hlayout->addStretch(1);

    tip_label_ = new QLabel(this);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(button_hlayout);
    vlayout->addWidget(tip_label_);
    setLayout(vlayout);
}

void EventLogDlg::InitSignalSlots()
{
    connect(read_log_button_, SIGNAL(clicked()), this, SLOT(OnReadLogButtonClicked()));
    connect(remove_event_button_, SIGNAL(clicked()), this, SLOT(OnDeleteEventButtonClicked()));
    connect(export_log_button_, SIGNAL(clicked()), this, SLOT(OnExportLogButtonClicked()));
    connect(export_report_button_, SIGNAL(clicked()), this, SLOT(OnExportReportButtonClicked()));
    connect(event_tree_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem*,int)));
}

void EventLogDlg::UpdateEventDetailTree()
{
    QList<QTreeWidgetItem *> item_list;
    event_detail_tree_->clear();
    item_list.clear();
    QList<LogParam> log_param_list = handler_->get_event_log_list(0);
    QString desc;
    for (int i = 0; i < log_param_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(event_detail_tree_);
        desc.sprintf("%d", log_param_list.at(i).event_type_id);
        item->setText(0, desc);
        desc.sprintf("%04d", log_param_list.at(i).log_id);
        item->setText(1, desc);
        desc = handler_->get_datetime_desc(log_param_list.at(i).log_time);
        item->setText(2, desc);
        desc = handler_->get_log_desc(log_param_list.at(i).event_type_id, log_param_list.at(i).log_value);
        item->setText(3, desc);
        item_list.append(item);
    }
    event_detail_tree_->addTopLevelItems(item_list);
}

void EventLogDlg::UpdateUI()
{
    handler_->init_from_file(file_name_);
    UpdateEventTypeTree();
    UpdateEventDetailTree();
}

void EventLogDlg::UpdateEventTypeTree()
{
    event_tree_->clear();
    QList<QTreeWidgetItem *> item_list;
    QList<QString> event_desc_list = handler_->get_event_type_desc_list();
    for (int i = 0; i < event_desc_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(event_tree_);
        item->setText(0, event_desc_list.at(i));
        item_list.append(item);
    }
    event_tree_->addTopLevelItems(item_list);
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

void EventLogDlg::ParseEventLogArray(QByteArray &byte_arr)
{
    char head[4] = {'\0'};
    char const *content = byte_arr.data();
    memcpy(head, content, 4);
    QString head_str(head);
//    if (strncmp(head, "CYT6", 4) != 0)
    if (head_str != QString("CYT6"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_INCORRECT, STRING_OK);
        return;
    }
    // package len considered to validate
    unsigned int len = 0;
    memcpy(&len, content + 4, 4);
    len = len-4-3;
    char cy_header[18] = {'\0'};
    memcpy(cy_header, content + 8, sizeof(cy_header));
    byte_arr.remove(0, 4+4+18);
    int index = byte_arr.indexOf("END");
    byte_arr.remove(index, 3);

    QFile file(file_name_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    file.write(byte_arr);
    file.close();
}
