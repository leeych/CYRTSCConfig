#include "eventlogdlg.h"
#include "macrostring.h"
#include "mutility.h"
#include "synccommand.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QCalendarWidget>

#include "bytearraydump.h"

EventLogDlg::EventLogDlg(QWidget *parent) :
    QDialog(parent)
{
    curr_event_type_id_ = 0;
    InitPage();
    InitSignalSlots();
}

EventLogDlg::~EventLogDlg()
{
}

void EventLogDlg::Initialize(const QString &ip, EventLogHandler *handler)
{
    MUtility::getTempDir(file_name_);
    file_name_ += ip + ".edat";
    ip_ = ip;
    handler_ = handler;
    tip_label_->clear();
    UpdateUI();
//    QTreeWidgetItem *item = event_tree_->itemAt(0, 1);
//    event_tree_->setCurrentItem(item);
    OnReadLogButtonClicked();
    exec();
}

void EventLogDlg::OnReadLogButtonClicked()
{
    SyncCommand::GetInstance()->ReadEventLogFile(this, SLOT(OnCmdReadEventLog(QByteArray&)));
}

void EventLogDlg::OnDeleteEventButtonClicked()
{
//    tip_label_->clear();
//    QTreeWidgetItem *item = event_tree_->currentItem();
//    if (item == NULL)
//    {
//        return;
//    }
//    curr_event_type_id_ = handler_->get_event_type_id_by_desc(item->text(0).trimmed());
//    if (curr_event_type_id_ == 0 || curr_event_type_id_ > 20)
//    {
//        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_TYPE_ERR, STRING_OK);
//        return;
//    }
//    QString str = QString::number(curr_event_type_id_);
    SyncCommand::GetInstance()->ClearEventLogs();
    this->setEnabled(false);
    QTime t;
    t.start();
    while (t.elapsed() < 2000)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    this->setEnabled(true);
    SyncCommand::GetInstance()->ReadEventLogFile(this, SLOT(OnCmdReadEventLog(QByteArray&)));
    tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_EVENT_REMOVING + "</font>");
}

void EventLogDlg::OnExportLogButtonClicked()
{
    tip_label_->clear();
    if (handler_->event_log_record_empty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_lOG_EMPTY, STRING_OK);
        return;
    }

    QString dir;
    MUtility::getLogDir(dir);
    QString log_file = QFileDialog::getSaveFileName(this, STRING_UI_SAVEAS, dir + ip_ + ".log" , "Log(*.log);;All File(*.*)");
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
    tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_EVENT_EXPORT_LOG + STRING_SUCCEEDED + "</font>");
}

void EventLogDlg::OnExportReportButtonClicked()
{
    tip_label_->clear();
    if (handler_->event_log_record_empty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_lOG_EMPTY, STRING_OK);
        return;
    }

    QString dir;
    MUtility::getReportDir(dir);
    QString report_file = QFileDialog::getSaveFileName(this, STRING_UI_SAVEAS, dir + ip_ + ".html", "Html(*.html);;All File(*.*)");
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
    tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_EVENT_EXPORT_REPORT + STRING_SUCCEEDED + "</font>");
}

void EventLogDlg::OnEventTypeTreeItemSelected(QTreeWidgetItem *item, int col)
{
    if (item == NULL || col < 0)
    {
        return;
    }
    QString desc = item->text(0).trimmed();
    curr_event_type_id_ = handler_->get_event_type_id_by_desc(desc);
    remove_event_button_->setEnabled(true);
}

void EventLogDlg::OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem *item,int col)
{
    tip_label_->clear();
    if (item == NULL || col < 0)
    {
        return;
    }
    QString desc = item->text(0).trimmed();
    unsigned char event_type_id = handler_->get_event_type_id_by_desc(desc);
    QList<LogParam> log_param_list = handler_->get_event_log_list(event_type_id);
    UpdateEventDetailTree(log_param_list);
}

void EventLogDlg::OnOKButtonClicked()
{
    tip_label_->clear();
    QDateTime start_time = start_datetime_->dateTime();
    QDateTime end_time = end_datetime_->dateTime();
    QDateTime utc_time = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss");
    unsigned int start_secs = utc_time.secsTo(start_time);
    unsigned int end_secs = utc_time.secsTo(end_time);
    QList<LogParam> log_list = handler_->get_event_log_list(start_secs, end_secs);
    UpdateEventDetailTree(log_list);
    count_tip_label_->setText(QString::number(log_list.size()));
}

void EventLogDlg::OnCmdReadEventLog(QByteArray &array)
{
    if (array.isEmpty())
    {
        tip_label_->setText(STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_FAILED);
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_SOCKET_NULL, STRING_OK);
        event_log_array_.clear();
        return;
    }
    event_log_array_.append(array);
    if (event_log_array_.contains("EVENTLOGER"))
    {
        tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_FAILED + "</font>");
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_FAILED, STRING_OK);
        event_log_array_.clear();
        return;
    }

    if (event_log_array_.endsWith("END"))
    {
#if 0
        ByteArrayDump dump;
        QString dir;
        MUtility::getTempDir(dir);
        dump.dumpByteArray(dir + "event_log.dat", event_log_array_);
#endif
        ParseEventLogArray(event_log_array_);
//        SyncCommand::GetInstance()->ReleaseSignalSlots();
        UpdateUI();
        event_log_array_.clear();
        tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_EVENT_READ_LOG + STRING_SUCCEEDED + "</font>");
    }
}

void EventLogDlg::OnCmdClearEventLog(QByteArray &array)
{
    if (array.isEmpty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_SOCKET_NULL, STRING_OK);
        return;
    }
    // TODO: is there any return value left to be determined
    tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_EVENT_LOG_DELETE_LOG + STRING_SUCCEEDED + "</font>");
    SyncCommand::GetInstance()->ReadEventLogFile(this, SLOT(OnCmdReadEventLog(QByteArray&)));
}

void EventLogDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_EVENT_TITLE);
//    event_tree_ = new QTreeWidget;
//    QStringList event_header;
//    event_header << STRING_UI_SIGNALER_EVENT_TYPE << STRING_UI_SIGNALER_EVENT_CLEARTIME;
//    InitTree(event_tree_, event_header);
//    event_tree_->setMaximumWidth(270);
//    event_tree_->setColumnWidth(0, event_tree_->width()*1/3);
//    event_tree_->setColumnWidth(1, event_tree_->width()*2/3);

    event_detail_tree_ = new QTreeWidget;
    QStringList detail_header;
    detail_header /*<< STRING_UI_SIGNALER_EVENT_TYPE_ID << STRING_UI_SIGNALER_EVENT_FLOW_ID*/
                     << STRING_UI_SIGNALER_EVENT_DATETIME << STRING_UI_SIGNALER_EVENT_DESC;
    InitTree(event_detail_tree_, detail_header);
    int width = event_detail_tree_->width();
//    event_detail_tree_->setColumnWidth(0, width * 1/6);
//    event_detail_tree_->setColumnWidth(1, width * 1/6);
    event_detail_tree_->setColumnWidth(0, width * 1/3);
    event_detail_tree_->setColumnWidth(1, width * 2/3);

    count_tip_label_ = new QLabel(this);
    count_tip_label_->setVisible(false);
    QHBoxLayout *tree_hlayout = new QHBoxLayout;
//    tree_hlayout->addWidget(event_tree_);
    tree_hlayout->addWidget(event_detail_tree_);
//    tree_hlayout->addWidget(count_tip_label_);
//    tree_hlayout->setStretch(0, 1);
//    tree_hlayout->setStretch(1, 3);

    start_datetime_ = new QDateTimeEdit;
    setDateTimeEdit(start_datetime_);
    start_datetime_->setDateTime(QDateTime::fromString("1970-01-01 00:00:01", "yyyy-MM-dd hh:mm:ss"));
    end_datetime_ = new QDateTimeEdit;
    setDateTimeEdit(end_datetime_);
    end_datetime_->setDateTime(QDateTime::currentDateTime());

    QLabel *start_tip_label = new QLabel(STRING_START);
    QLabel *end_tip_label = new QLabel("----");
    ok_button_ = new QPushButton(STRING_OK);
    QLabel *count_label = new QLabel(STRING_UI_SIGNALER_EVENT_COUNT + ":");
    count_label->setVisible(false);

    QHBoxLayout *time_hlayout = new QHBoxLayout;
    time_hlayout->addWidget(start_tip_label);
    time_hlayout->addWidget(start_datetime_);
    time_hlayout->addWidget(end_tip_label);
    time_hlayout->addWidget(end_datetime_);
    time_hlayout->addStretch(1);
    time_hlayout->addWidget(ok_button_);
    time_hlayout->addStretch(1);

    QHBoxLayout *count_hlayout = new QHBoxLayout;
    count_hlayout->addWidget(count_label);
    count_hlayout->addWidget(count_tip_label_);
    time_hlayout->addLayout(count_hlayout);

    read_log_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_READ_LOG);
    remove_event_button_ = new QPushButton(STRING_UI_SIGNALER_ALL_EVENT);
    export_log_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_EXPORT_LOG);
    export_report_button_ = new QPushButton(STRING_UI_SIGNALER_EVENT_EXPORT_REPORT);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
//    button_hlayout->addStretch(1);
    button_hlayout->addWidget(read_log_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(remove_event_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(export_log_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(export_report_button_);
//    button_hlayout->addStretch(1);

    tip_label_ = new QLabel(this);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(time_hlayout);
    vlayout->addLayout(button_hlayout);
    vlayout->addWidget(tip_label_);
    setLayout(vlayout);
}

void EventLogDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOKButtonClicked()));
    connect(read_log_button_, SIGNAL(clicked()), this, SLOT(OnReadLogButtonClicked()));
    connect(remove_event_button_, SIGNAL(clicked()), this, SLOT(OnDeleteEventButtonClicked()));
    connect(export_log_button_, SIGNAL(clicked()), this, SLOT(OnExportLogButtonClicked()));
    connect(export_report_button_, SIGNAL(clicked()), this, SLOT(OnExportReportButtonClicked()));
//    connect(event_tree_, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(OnEventTypeTreeItemSelected(QTreeWidgetItem*,int)));
//    connect(event_tree_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem*,int)));

//    connect(read_log_button_, SIGNAL(clicked()), tip_label_, SLOT(clear()));
//    connect(remove_event_button_, SIGNAL(clicked()), tip_label_, SLOT(clear()));
//    connect(export_log_button_, SIGNAL(clicked()), tip_label_, SLOT(clear()));
//    connect(export_report_button_, SIGNAL(clicked()), tip_label_, SLOT(clear()));
}

void EventLogDlg::UpdateEventDetailTree()
{
    event_detail_tree_->clear();
    QList<QTreeWidgetItem *> item_list;
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

void EventLogDlg::UpdateEventDetailTree(const QList<LogParam> &log_param_list)
{
    event_detail_tree_->clear();
    QList<QTreeWidgetItem *> item_list;
    item_list.clear();
    QString desc;
    for (int i = 0; i < log_param_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(event_detail_tree_);
//        desc.sprintf("%d", log_param_list.at(i).event_type_id);
//        item->setText(0, desc);
//        desc.sprintf("%04d", log_param_list.at(i).log_id);
//        item->setText(1, desc);
        desc = handler_->get_datetime_desc(log_param_list.at(i).log_time);
        item->setText(0, desc);
        desc = handler_->get_log_desc(log_param_list.at(i).event_type_id, log_param_list.at(i).log_value);
        item->setText(1, desc);
        item_list.append(item);
    }
    event_detail_tree_->addTopLevelItems(item_list);
}

void EventLogDlg::UpdateUI()
{
    handler_->init_from_file(file_name_);
//    UpdateEventTypeTree();
    QList<LogParam> log_param_list = handler_->get_event_log_list(0);
    UpdateEventDetailTree(log_param_list);
    count_tip_label_->setText(QString::number(log_param_list.size()));
//    QTreeWidgetItem *item = event_tree_->currentItem();
//    if (item == NULL)
//    {
//        remove_event_button_->setEnabled(false);
//        return;
//    }
//    curr_event_type_id_ = handler_->get_event_type_id_by_desc(item->text(0).trimmed());
//    if (curr_event_type_id_ == 0 || curr_event_type_id_ > 20)
//    {
//        remove_event_button_->setEnabled(false);
//    }
}

void EventLogDlg::UpdateEventTypeTree()
{
    event_tree_->clear();
    QList<QTreeWidgetItem *> item_list;
#if 0
    QList<QString> event_desc_list = handler_->get_event_type_desc_list();
#endif
#if 1
    QList<QString> event_desc_list = handler_->get_all_event_type_desc_list();
#endif
    QString str;
    for (int i = 0; i < event_desc_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(event_tree_);
        str = event_desc_list.at(i);
        item->setText(0, event_desc_list.at(i));
        unsigned int id_secs = handler_->get_event_type_id_by_desc(str);
        id_secs = handler_->get_event_type_clear_time(id_secs);
        str = MUtility::secondsToDateTime(id_secs);
        item->setText(1, str);

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

void EventLogDlg::setDateTimeEdit(QDateTimeEdit *edit)
{
    QString dir;
    MUtility::getImageDir(dir);
    edit->setFixedHeight(25);
    edit->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss"));
    edit->setCalendarPopup(true);
    edit->setStyleSheet("QDateTimeEdit::drop-down {"
                        "subcontrol-position: right center;"
                        "width: 15px;"
                        "border-left-style: solid; "
                        "border-top-right-radius: 3px; "
                        "border-bottom-right-radius: 3px;}"
                        "QDateTimeEdit::down-arrow {image: url(" + dir + "arrow.png);}"
                        "border-width: 3px;");
    QCalendarWidget *calendar = new QCalendarWidget;
    QLocale local = QLocale(QLocale::Chinese, QLocale::China);
    calendar->setLocale(local);
    QString qsstyle =
            "QAbstractItemView {background:rgb(255,255,255);selection-background-color: rgb(0, 153, 204);}";
    calendar->setStyleSheet(qsstyle);
    QWidget *calendarNavBar = calendar->findChild<QWidget *>("qt_calendar_navigationbar");
    if (calendarNavBar)
    {
        calendarNavBar->setStyleSheet("background-color:rgb(0, 153, 204);");
    }
    calendar->setHorizontalHeaderFormat(QCalendarWidget::NoHorizontalHeader);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setFixedSize(210,150);
    edit->setCalendarWidget(calendar);
    edit->setAutoFillBackground(true);
}

void EventLogDlg::ParseEventLogArray(QByteArray &byte_arr)
{
    QString head_str(byte_arr.left(4));
    if (head_str != QString("CYT6"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_EVENT_INCORRECT, STRING_OK);
        return;
    }
    // package len considered to validate
    byte_arr.remove(0, 4);
    char char_len[4] = {'\0'};
    char_len[0] = byte_arr.at(0);
    char_len[1] = byte_arr.at(1);
    char_len[2] = byte_arr.at(2);
    char_len[3] = byte_arr.at(3);
    unsigned int len = 0;
    memcpy(&len, char_len, 4);
    len = len-4-3;
    byte_arr.remove(0, 4);
    char cy_header[18] = {'\0'};
    memcpy(cy_header, byte_arr.data(), 18);
    byte_arr.remove(0, 18);
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
