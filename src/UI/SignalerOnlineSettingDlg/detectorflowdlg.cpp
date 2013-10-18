#include "detectorflowdlg.h"
#include "macrostring.h"
#include "synccommand.h"
#include "mutility.h"

#include <QCalendarWidget>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


DetectorFlowDlg::DetectorFlowDlg(QWidget *parent) :
    QDialog(parent)
{
    curr_detector_id_ = 0;
    sync_cmd_ = SyncCommand::GetInstance();
    handler_ = new DetectorFlowHandler;
    for (int i = 0; i < MAX_DETECTOR; i++)
    {
        detector_flow_list_.append(0);
    }
    InitPage();
    InitSignalSlots();
}

DetectorFlowDlg::~DetectorFlowDlg()
{
    if (handler_ != NULL)
    {
        delete handler_;
        handler_ = NULL;
    }
}

void DetectorFlowDlg::Initialize()
{
    UpdateUI();
    OnReadFlowButtonClicked();
    exec();
}

void DetectorFlowDlg::OnReadFlowButtonClicked()
{
    sync_cmd_->GetDetectorFlowData(this, SLOT(OnCmdParseParam(QByteArray&)));
    this->setEnabled(false);
    int count = detector_tree_->topLevelItemCount();
    QTreeWidgetItem *item = detector_tree_->topLevelItem(count-1);
    detector_tree_->setCurrentItem(item);
}

void DetectorFlowDlg::OnClearFowButtonClicked()
{
    tip_label_->clear();
    sync_cmd_->ClearDetectorFlowInfo();
    this->setEnabled(false);
    QTime t;
    t.start();
    while (t.elapsed() < 2000)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    ResetDetectorFlowInfo();
    UpdateUI();
    OnReadFlowButtonClicked();
    this->setEnabled(true);
    tip_label_->setText("<font color=\"Green\">" + STRING_UI_SIGNALER_DETECTOR_CLEAR_FLOW + STRING_SUCCEEDED + "</font>");
}

void DetectorFlowDlg::OnCalculateButtonClicked()
{
    tip_label_->clear();
    QDateTime start_time = start_time_editor_->dateTime();
    QDateTime end_time = end_time_editor_->dateTime();
    QDateTime utc_time = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss");
    unsigned int start_secs = utc_time.secsTo(start_time);
    unsigned int end_secs = utc_time.secsTo(end_time);
    QString desc = start_time_editor_->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    desc = end_time_editor_->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QList<DetectorFlowInfo> flow_list = handler_->get_detector_flow(curr_detector_id_, start_secs, end_secs);
    UpdateFlowInfoTree(flow_list);
    unsigned int flow_count = 0;
    for (int i = 0; i < flow_list.size(); i++)
    {
        flow_count += flow_list.at(i).detector_data;
    }
    total_flow_lineedit_->setText(QString::number(flow_count));
}

void DetectorFlowDlg::OnDetectorIDTreeDoubleClicked(QTreeWidgetItem *item, int col)
{
    tip_label_->clear();
    if (item == NULL || col < 0)
    {
        return;
    }
    QString str = item->text(0).trimmed();
    if (str.contains("ALL"))
    {
        curr_detector_id_ = 0;
        UpdateFlowInfoTree(handler_->get_detector_flow_list());
    }
    else
    {
        curr_detector_id_ = str.toUInt();
        UpdateFlowInfoTree(handler_->get_detector_flow(curr_detector_id_));
    }
    if (curr_detector_id_ > MAX_DETECTOR)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_DETECTOR_ID_OVERFLOW + QString::number(MAX_DETECTOR) + "!", STRING_OK);
        return;
    }
}

void DetectorFlowDlg::OnDateTimeChanged(const QDateTime &datetime)
{
    QList<DetectorFlowInfo> flow_list;
    QDateTimeEdit *sender_obj = static_cast<QDateTimeEdit *>(sender());
    if (sender_obj == start_time_editor_)
    {
        QDateTime end_datetime = end_time_editor_->dateTime().toUTC();
        unsigned int start_secs = datetime.toTime_t(), end_secs = end_datetime.toTime_t();
        flow_list = handler_->get_detector_flow(curr_detector_id_, start_secs, end_secs);
    }
    else
    {
        QDateTime start_datetime = start_time_editor_->dateTime().toUTC();
        unsigned int start_secs = start_datetime.toTime_t(), end_secs = datetime.toTime_t();
        flow_list = handler_->get_detector_flow(curr_detector_id_, start_secs, end_secs);
    }
    UpdateFlowInfoTree(flow_list);
}

void DetectorFlowDlg::OnCmdParseParam(QByteArray &array)
{
    recv_array_.append(array);
    if (!CheckPackage(recv_array_))
    {
        this->setEnabled(true);
        return;
    }
    QString head(array.left(3));
    QString tail(array.right(3));
    if (head != QString("CYT") || tail != QString("END"))
    {
        this->setEnabled(true);
        return;
    }
    bool status = false;
    unsigned char cmd_id = array.at(3);
    switch (cmd_id)
    {
    case '9':
        status = ParseDetectorDataContent(array);
        if (!status)
        {
            tip_label_->setText("<font color=\"Red\">" + STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR + "</font>");
            QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
        }
        else
        {
            tip_label_->setText("<font color=\"Green\">" + STRING_UI_SGINALER_DETECTOR_READ_FLOW + STRING_SUCCEEDED + "</font>");
            UpdateFlowInfoTree();
        }
        break;
    case 'B':
        status = ParseRealtimeFlowInfoContent(array);
        if (!status)
        {
            QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
        }
        break;
    default:
        break;
    }
    this->setEnabled(true);
}

void DetectorFlowDlg::OnConnectError(const QString &str)
{
    QMessageBox::information(this, STRING_TIP, str, STRING_OK);
    this->setEnabled(true);
}

void DetectorFlowDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_DETECTOR_TITLE);

    detector_tree_ = new QTreeWidget;
    QStringList detector_header;
    detector_header << STRING_UI_SIGNALER_DETECTOR_ID;
    InitTree(detector_tree_, detector_header);
    detector_tree_->setColumnWidth(0, 40);
    InitDetectorTreeContent();

    flow_tree_ = new QTreeWidget;
    QStringList flow_header;
    flow_header << STRING_UI_SIGNALER_DETECTOR_FLOW << STRING_UI_SIGNALER_DETECTOR_DATETIME
                   << STRING_UI_SIGNALER_DETECTOR_NUM << STRING_UI_SIGNALER_DETECTOR_PHASE;
    InitTree(flow_tree_, flow_header);
    flow_tree_->setColumnWidth(0, 80);
    flow_tree_->setColumnWidth(1, 180);
    flow_tree_->setColumnWidth(2, 80);
//    flow_tree_->setMinimumWidth(320);
    start_time_editor_ = new QDateTimeEdit;
//    start_time_editor_->setTimeSpec(Qt::LocalTime);
    end_time_editor_ = new QDateTimeEdit;
//    end_time_editor_->setTimeSpec(Qt::LocalTime);
    SetDateTimeEdit(start_time_editor_);
    start_time_editor_->setDateTime(QDateTime::fromString("1970-01-01 00:00:01", "yyyy-MM-dd hh:mm:ss"));
    SetDateTimeEdit(end_time_editor_);
    end_time_editor_->setDateTime(QDateTime::currentDateTime());
    QLabel *total_flow_label = new QLabel(STRING_UI_SIGNALER_DETECTOR_TOTAL_FLOW + ":");
    total_flow_lineedit_ = new QLineEdit;
    total_flow_lineedit_->setMaximumWidth(80);
    total_flow_lineedit_->setEnabled(false);
    total_flow_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);

    QHBoxLayout *timeedit_hlayout = new QHBoxLayout;
    timeedit_hlayout->addWidget(start_time_editor_);
    timeedit_hlayout->addWidget(new QLabel("--"));
    timeedit_hlayout->addWidget(end_time_editor_);
    timeedit_hlayout->addWidget(total_flow_label);
    timeedit_hlayout->addWidget(total_flow_lineedit_);

    QVBoxLayout *right_vlayout = new QVBoxLayout;
    right_vlayout->addWidget(flow_tree_);
    right_vlayout->addLayout(timeedit_hlayout);

    QHBoxLayout *tree_hlayout = new QHBoxLayout;
    tree_hlayout->addWidget(detector_tree_);
    tree_hlayout->addLayout(right_vlayout);

    tree_hlayout->setStretch(0, 1);
    tree_hlayout->setStretch(1, 4);

    read_flow_button_ = new QPushButton(STRING_UI_SGINALER_DETECTOR_READ_FLOW);
    clear_flow_button_ = new QPushButton(STRING_UI_SIGNALER_DETECTOR_CLEAR_FLOW);
    calculate_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR_FLOW_STATISTIC);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addWidget(read_flow_button_);
    button_hlayout->addWidget(clear_flow_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(calculate_button_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(button_hlayout);
    tip_label_ = new QLabel;
    vlayout->addWidget(tip_label_, 0, Qt::AlignLeft);
    setLayout(vlayout);
}

void DetectorFlowDlg::InitSignalSlots()
{
    connect(read_flow_button_, SIGNAL(clicked()), this, SLOT(OnReadFlowButtonClicked()));
    connect(clear_flow_button_, SIGNAL(clicked()), this, SLOT(OnClearFowButtonClicked()));
    connect(calculate_button_, SIGNAL(clicked()), this, SLOT(OnCalculateButtonClicked()));
    connect(detector_tree_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnDetectorIDTreeDoubleClicked(QTreeWidgetItem*,int)));
//    connect(start_time_editor_, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(OnDateTimeChanged(QDateTime)), Qt::QueuedConnection);
//    connect(end_time_editor_, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(OnDateTimeChanged(QDateTime)), Qt::QueuedConnection);
//    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectError(QString)));
}

void DetectorFlowDlg::UpdateUI()
{
//    UpdateDetectorTree();
    UpdateFlowInfoTree();
    OnCalculateButtonClicked();
}

void DetectorFlowDlg::InitDetectorTreeContent()
{
    for (int i = 0; i < MAX_DETECTOR; i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(detector_tree_);
        item->setText(0, QString::number(i+1));
        item->setTextAlignment(0, Qt::AlignCenter);
        detector_item_list_.append(item);
    }
    detector_tree_->addTopLevelItems(detector_item_list_);
    QTreeWidgetItem *item_all = new QTreeWidgetItem(detector_tree_);
    item_all->setText(0, "ALL");
    item_all->setTextAlignment(0, Qt::AlignCenter);
    detector_tree_->addTopLevelItem(item_all);
}

void DetectorFlowDlg::UpdateDetectorTree()
{
    detector_tree_->clear();
    QList<QTreeWidgetItem *> item_list;
    QList<unsigned char> detector_id_list = handler_->get_detector_id_list();
    for (int i = 0; i < detector_id_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(detector_tree_);
        item->setText(0, QString::number(detector_id_list.at(i)));
        item->setTextAlignment(0, Qt::AlignCenter);
        item_list.append(item);
    }
    QTreeWidgetItem *item_all = new QTreeWidgetItem(detector_tree_);
    item_all->setText(0, "ALL");
    item_all->setTextAlignment(0, Qt::AlignCenter);
    item_list.append(item_all);
    detector_tree_->addTopLevelItems(item_list);
}

void DetectorFlowDlg::UpdateFlowInfoTree()
{
    QList<DetectorFlowInfo> flow_list = handler_->get_detector_flow_list();
    UpdateFlowInfoTree(flow_list);
}

void DetectorFlowDlg::UpdateFlowInfoTree(const QList<DetectorFlowInfo> &flow_list)
{
    flow_tree_->clear();
    QString str;
    QList<QTreeWidgetItem *> item_list;
    for (int i = 0; i < flow_list.size(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(flow_tree_);
        str.sprintf("%d", flow_list.at(i).data_id);
        item->setText(0, str);
        str = MUtility::secondsToDateTime(flow_list.at(i).recv_time);
        item->setText(1, str);
        str.sprintf("%d", flow_list.at(i).detector_data);
        item->setText(2, str);
        str = MUtility::phaseBitsDesc(flow_list.at(i).phase_ids);
        item->setText(3, str);
        for (int j = 0; j < 4; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        item_list.append(item);
    }
    flow_tree_->addTopLevelItems(item_list);
}

void DetectorFlowDlg::SetDetectorFlow(unsigned char detector_id)
{
    if (detector_id == 0 || detector_id > MAX_DETECTOR)
    {
        return;
    }
    detector_flow_list_[detector_id]++;
}

void DetectorFlowDlg::ResetDetectorFlowInfo()
{
    if (detector_array_ != NULL)
    {
        delete [] detector_array_;
        detector_array_ = NULL;
    }
    for (int i = 0; i < detector_flow_list_.size(); i++)
    {
        detector_flow_list_[i] = 0;
    }
    handler_->clear_detector_flow();
}

void DetectorFlowDlg::InitTree(QTreeWidget *tree, const QStringList &header)
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

void DetectorFlowDlg::SetDateTimeEdit(QDateTimeEdit *edit)
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
    QTableView *table = qFindChild<QTableView *>((edit->calendarWidget()));
    QHeaderView *verticalHeader = table->verticalHeader();
    verticalHeader->setResizeMode(QHeaderView::Fixed);
    QHeaderView *h = table->horizontalHeader();
    h->setResizeMode(QHeaderView::Fixed);
    for(int i = 0; i < 7; i++)
    {
        h->resizeSection(i, 30);
    }
}

bool DetectorFlowDlg::CheckPackage(QByteArray &array)
{
    if (array.contains("DETECTDATAER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_DETECTOR_EMPTY, STRING_OK);
        int index = array.indexOf("DETECTDATAER");
        array.remove(index, QString("DETECTDATAER").size()+1);
        return false;
    }
    if (array.contains("DRIVEINFOER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_REQUIRE_DETECTOR_STATUS + STRING_FAILED, STRING_OK);
        int index = array.indexOf("DRIVEINFOER");
        array.remove(index, QString("DRIVEINFOER").size()+1);
        return false;
    }
    return true;
}
// CYT9+数据总长度(4字节)+车辆检测器数据字节流+END
bool DetectorFlowDlg::ParseDetectorDataContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);
    array.remove(0, 4);
    int sz = array.size();
    int data_sz = sizeof(DetectorData_t);
    if (sz % data_sz != 0)
    {
        array.remove(0, sz);
        return false;
    }
    int count = sz / data_sz;
    detector_array_ = new DetectorData_t[count];
    memcpy(detector_array_, array.data(), array.size());
    array.remove(0, array.size());
    // TODO: update ui
    for (int i = 0; i < count; i++)
    {
        handler_->set_detector_flow(detector_array_[i]);
    }
    if (detector_array_ != NULL)
    {
        delete [] detector_array_;
        detector_array_ = NULL;
    }
    return true;
}
// CYTB+检测器编号(1字节)+END
bool DetectorFlowDlg::ParseRealtimeFlowInfoContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);
    unsigned char detector_id = 0;
    if (array.size() != 1)
    {
        array.remove(0, array.size());
        return false;
    }
    memcpy(&detector_id, array.data(), 1);
    array.remove(0, 1);
    array.remove(0, 3);
    SetDetectorFlow(detector_id);

    return true;
}
