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
    sync_cmd_ = SyncCommand::GetInstance();
    handler_ = new DetectorFlowHandler;
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
    exec();
}

void DetectorFlowDlg::OnReadFlowButtonClicked()
{
    sync_cmd_->GetDetectorFlowData(this, SLOT(OnCmdParseParam(QByteArray&)));
    read_flow_button_->setEnabled(false);
}

void DetectorFlowDlg::OnClearFowButtonClicked()
{
    sync_cmd_->ClearDetectorFlowInfo();
}

void DetectorFlowDlg::OnOkButtonClicked()
{
    accept();
}

void DetectorFlowDlg::OnDetectorIDTreeDoubleClicked(QTreeWidgetItem *item, int col)
{
    if (item == NULL || col < 0)
    {
        return;
    }
    unsigned char id = item->text(0).trimmed().toUInt();
    if (id > MAX_DETECTOR)
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_DETECTOR_ID_OVERFLOW, STRING_OK);
        return;
    }
    QList<DetectorFlowInfo> flow_list = handler_->get_detector_flow(id);
    UpdateFlowInfoTree(flow_list);
}

void DetectorFlowDlg::OnCmdParseParam(QByteArray &array)
{
    byte_array_.append(array);
    if (!CheckPackage(byte_array_))
    {
        return;
    }
    QString head(array.left(3));
    QString tail(array.right(3));
    if (head != QString("CYT") || tail != QString("END"))
    {
        return;
    }
    bool status = false;
    unsigned char cmd_id = array.at(4);
    switch (cmd_id)
    {
    case '9':
        status = ParseDetectorDataContent(array);
        if (!status)
        {
            QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
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
}

void DetectorFlowDlg::OnConnectError(const QString &str)
{
    QMessageBox::information(this, STRING_TIP, str, STRING_OK);
    return;
}

void DetectorFlowDlg::InitPage()
{
    setWindowTitle(STRING_UI_SIGNALER_DETECTOR_TITLE);

    detector_tree_ = new QTreeWidget;
    QStringList detector_header;
    detector_header << STRING_UI_SIGNALER_DETECTOR_ID;
    InitTree(detector_tree_, detector_header);
//    detector_tree_->setMaximumWidth(100);

    flow_tree_ = new QTreeWidget;
    QStringList flow_header;
    flow_header << STRING_UI_SIGNALER_DETECTOR_FLOW << STRING_UI_SIGNALER_DETECTOR_DATETIME
                   << STRING_UI_SIGNALER_DETECTOR_NUM << STRING_UI_SIGNALER_DETECTOR_PHASE;
    InitTree(flow_tree_, flow_header);
    start_time_editor_ = new QDateTimeEdit;
    end_time_editor_ = new QDateTimeEdit;
    SetDateTimeEdit(start_time_editor_);
    SetDateTimeEdit(end_time_editor_);
    QLabel *total_flow_label = new QLabel(STRING_UI_SIGNALER_DETECTOR_TOTAL_FLOW + ":");
    total_flow_lineedit_ = new QLineEdit;
    total_flow_lineedit_->setContextMenuPolicy(Qt::NoContextMenu);

    QHBoxLayout *timeedit_hlayout = new QHBoxLayout;
    timeedit_hlayout->addWidget(start_time_editor_);
    timeedit_hlayout->addWidget(new QLabel("--"));
    timeedit_hlayout->addWidget(end_time_editor_);
//    timeedit_hlayout->addStretch(1);
    timeedit_hlayout->addWidget(total_flow_label);
    timeedit_hlayout->addWidget(total_flow_lineedit_);

    QVBoxLayout *right_vlayout = new QVBoxLayout;
    right_vlayout->addWidget(flow_tree_);
    right_vlayout->addLayout(timeedit_hlayout);

    QHBoxLayout *tree_hlayout = new QHBoxLayout;
    tree_hlayout->addWidget(detector_tree_);
    tree_hlayout->addLayout(right_vlayout);

    tree_hlayout->setStretch(0, 1);
    tree_hlayout->setStretch(1, 3);

    read_flow_button_ = new QPushButton(STRING_UI_SGINALER_DETECTOR_READ_FLOW);
    clear_flow_button_ = new QPushButton(STRING_UI_SIGNALER_DETECTOR_CLEAR_FLOW);
    ok_button_ = new QPushButton(STRING_OK);

    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addWidget(read_flow_button_);
    button_hlayout->addWidget(clear_flow_button_);
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(ok_button_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(tree_hlayout);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
}

void DetectorFlowDlg::InitSignalSlots()
{
    connect(read_flow_button_, SIGNAL(clicked()), this, SLOT(OnReadFlowButtonClicked()));
    connect(clear_flow_button_, SIGNAL(clicked()), this, SLOT(OnClearFowButtonClicked()));
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOkButtonClicked()));
    connect(detector_tree_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(OnDetectorIDTreeDoubleClicked(QTreeWidgetItem*,int)));
//    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectError(QString)));
}

void DetectorFlowDlg::UpdateUI()
{
    UpdateDetectorTree();
    UpdateFlowInfoTree();
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
        item_list.append(item);
    }
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

        item_list.append(item);
    }
    flow_tree_->addTopLevelItems(item_list);
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
    edit->setFixedHeight(26);
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
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_REQUIRE_DETECTOR_DATA + STRING_FAILED, STRING_OK);
        int index = array.indexOf("DETECTDATAER");
        array.remove(index, QString("DETECTDATAER").size());
        return false;
    }
    if (array == "DRIVEINFOER")
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_REQUIRE_DETECTOR_STATUS + STRING_FAILED, STRING_OK);
        int index = array.indexOf("DRIVEINFOER");
        array.remove(index, QString("DRIVEINFOER").size());
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
    int sz = array.size();
    int data_sz = sizeof(DetectorData_t);
    if (sz % data_sz != 0)
    {
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
    UpdateUI();

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

    return true;
}
