#include "realtimemonitordlg.h"
#include "macrostring.h"
#include "mutility.h"
#include "synccommand.h"
#include "filereaderwriter.h"
#include "eventlogdescriptor.h"
#include "detectorflowhandler.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>

#define RESET_FLAG

#ifdef RESET_FLAG
#define RESET_LIGHT(exp) \
    if(!is_first_light_){exp;}
#else
#define RESET_LIGHT(exp) \
    exp
#endif


RealtimeMonitorDlg::RealtimeMonitorDlg(QWidget *parent) :
    QDialog(parent)
{
    sync_cmd_ = SyncCommand::GetInstance();
    signaler_timer_ = new QTimer(this);
    count_down_timer_ = new QTimer(this);
    is_inited_ = false;
    is_first_light_ = true;
    ui_timer_id_ = 0;
    is_uitimer_started_ = false;
    total_stage_count_ = 0;
	curr_stage_id_ = 0;
    count_down_seconds_ = 0;
    count_down_light_ = 0;
    handler_ = new DetectorFlowHandler;

    InitPage();
    InitSignalSlots();
    InitCtrlModeDesc();
    InitFaultDesc();
	ResetChannelColor();

}

RealtimeMonitorDlg::~RealtimeMonitorDlg()
{
    QPointF *ptr = NULL;
    for (int i = 0; i < channel_point_list_.size(); i++)
    {
        ptr = channel_point_list_.at(i);
        delete ptr;
        channel_point_list_[i] = NULL;
    }
    if (handler_ != NULL)
    {
        delete handler_;
        handler_ = NULL;
    }
}

void RealtimeMonitorDlg::Initialize(const QString &ip)
{
    ip_ = ip;
    MUtility::getUserDir(cfg_file_);
    cfg_file_ += "/monitor/" + ip_ + ".mdat";
    sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadSignalerConfigFile(QByteArray&)));
    setWindowTitle(ip_ + "-" + STRING_UI_SIGNALER_MONITOR);
    UpdateUI();
    CloseAllLights();
	ResetChannelColor();
    count_down_timer_->start(1000);
    exec();
}

void RealtimeMonitorDlg::OnSignalerRecordButtonToggled(bool checked)
{
    if (checked)
    {
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_SIGNALER_RECORD, signaler_tree_);
        ResetButtonStatus(signaler_record_button_);
    }
    tree_grp_->setHidden(!checked);
}

void RealtimeMonitorDlg::OnLightStatusButtonToggled(bool checked)
{
    if (checked)
    {
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_LIGHT_STATUS, light_tree_);
        tree_grp_->setHidden(false);
        ResetButtonStatus(light_status_button_);
    }
    tree_grp_->setHidden(!checked);
}

void RealtimeMonitorDlg::OnDriverButtonToggled(bool checked)
{
    if (checked)
    {
        sync_cmd_->GetDriverBoardInfo();
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_DRIVER_STATUS, driver_tree_);
        ResetButtonStatus(driver_button_);
    }
    tree_grp_->setHidden(!checked);
}

void RealtimeMonitorDlg::OnDetectorButtonToggled(bool checked)
{
    if (checked)
    {
        sync_cmd_->GetDetectorFlowData();
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_DETECTOR_STATUS, detector_tree_);
        ResetButtonStatus(detector_button_);
    }
    tree_grp_->setHidden(!checked);
}

void RealtimeMonitorDlg::OnConnectError(QString str)
{
    QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_CONNECT_ERROR + ":\n" + str, STRING_OK);
}

void RealtimeMonitorDlg::OnSignalerTimeTimerOutSlot()
{
    date_time_ = date_time_.addSecs(1);
    signaler_time_label_->setText(date_time_.toString("yyyy-MM-dd hh:mm:ss"));
    second_count_++;
    if (second_count_ % 3 == 0)
    {
        UpdateScheduleInfo();
        second_count_ = 0;
    }
}

void RealtimeMonitorDlg::OnCountDownTimerOutSlot()
{
    QString str;
    switch (count_down_light_)
    {
    case Red:
        if (count_down_seconds_ != 0)
        {
            --count_down_seconds_;
        }
        str.sprintf("00-00-%02d", count_down_seconds_);
        break;
    case Yellow:
        if (count_down_seconds_ != 0)
        {
            --count_down_seconds_;
        }
        str.sprintf("00-%02d-00", count_down_seconds_);
        break;
    case Green:
        if (count_down_seconds_ != 0)
        {
            --count_down_seconds_;
        }
        str.sprintf("%02d-00-00", count_down_seconds_);
        break;
    case Off:
        break;
    default:
        break;
    }
    phase_time_lcd_->display(str);
}

void RealtimeMonitorDlg::OnCmdReadSignalerConfigFile(QByteArray &array)
{
    cfg_array_.append(array);
    if (!cfg_array_.left(4).contains("CYT4"))
    {
        cfg_array_.clear();
        sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadSignalerConfigFile(QByteArray&)));
        return;
    }
    if (!cfg_array_.contains("END"))
    {
        return;
    }
    // TODO: parse signaler configuration file
    bool res = ParseConfigContent(cfg_array_);
    int ret = -1;
    if (!res)
    {
        ret = QMessageBox::question(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_CFG + STRING_FAILED + ";\n" + STRING_RETRY + "?", STRING_YES, STRING_NO);
        if (ret == 0)
        {
            // repeat request
            cfg_array_.clear();
            sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadSignalerConfigFile(QByteArray&)));
        }
        else if (ret == 1)
        {
            cfg_array_.clear();
            return;
        }
    }
    else
    {
        QFile file(cfg_file_);
retry:  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            ret = QMessageBox::question(this, STRING_UI_SIGNALER_SAVE_TEMPFILE_FAILED + ";\n" + STRING_RETRY + "?", STRING_YES, STRING_NO);
            if (ret == 0)
            {
                goto retry;
            }
            else
            {
                QFile::remove(cfg_file_);
            }
        }
        else
        {
            file.write(cfg_array_);
//            file.flush();
            file.close();
            if (InitTscParam())
            {
                UpdateScheduleInfo();
            }
            sync_cmd_->GetLightStatus(this, SLOT(OnCmdParseParam(QByteArray&)));
        }
    }
}

void RealtimeMonitorDlg::OnCmdParseParam(QByteArray &array)
{
    recv_array_.append(array);
    if (!CheckPackage(recv_array_))
    {
        return;
    }
    if (!(recv_array_.left(3).contains("CYT") && recv_array_.contains("END")))
    {
        return;
    }
    bool status = false;
    char cmd_id = 0;
    while (true)
    {
        if (recv_array_.size() < 4)
        {
            break;
        }
        cmd_id = recv_array_.at(3);
        switch (cmd_id)
        {
        case '0':
            break;
        case '1':
            status = ParseBeginMonitorContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case '2':
            break;
        case '3':
            status = ParseLightStatusContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            else
            {
                //sync_cmd_->StartMonitoring();
                sync_cmd_->GetTscTime();
                if (!is_uitimer_started_)
                {
                    ui_timer_id_ = startTimer(3600*1000);
                    is_uitimer_started_ = true;
                }
            }
            break;
        case '4':
    //        status = ParseConfigContent(array);
            break;
        case '5':
            status = ParseCountDownContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case '6':
            break;
        case '7':
            status = ParseTSCTimeContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            else if (!is_inited_)
			{
				signaler_timer_->start(1000);
				sync_cmd_->StartMonitoring();
				is_inited_ = true;
            }
            break;
        case '8':
            break;
        case '9':
            status = ParseDetectorFlowContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case 'A':
            status = ParseDetectorFaultContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case 'B':
            status = ParseRealTimeFlowContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case 'C':
            status = ParseDriverStatusContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case 'D':
            status = ParseDriverRealTimeStatusContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        case 'E':
            status = ParseLightRealTimeStatusContent(recv_array_);
            if (!status)
            {
                QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_PARSE_PACK_ERR, STRING_OK);
            }
            break;
        default:
            break;
        }
    }
}

void RealtimeMonitorDlg::closeEvent(QCloseEvent *)
{
    StopMonitoring();
//    signaler_timer_->stop();
    count_down_timer_->stop();
    is_inited_ = false;

    if (ui_timer_id_ != 0)
    {
        killTimer(ui_timer_id_);
        ui_timer_id_ = 0;
    }
    ResetButtonStatus(NULL);
    ResetChannelColor();
    // TODO: window closed handler
}

void RealtimeMonitorDlg::timerEvent(QTimerEvent *)
{
    if (ui_timer_id_ != 0)
    {
        sync_cmd_->GetTscTime();
    }
}

void RealtimeMonitorDlg::InitPage()
{
    InitPixmap();
    road_monitor_grp_ = new QGroupBox(STRING_UI_SIGNALER_MONITOR_ROAD);
    frame_ = new QFrame(road_monitor_grp_);
    frame_->setGeometry(QRect(10, 20, 480, 480));
    frame_->setFocusPolicy(Qt::ClickFocus);
    frame_->setFrameShape(QFrame::Panel);
    frame_->setFrameShadow(QFrame::Sunken);
    frame_->setLineWidth(5);
    graphics_view_ = new QGraphicsView(frame_);
    graphics_view_->setFocusPolicy(Qt::ClickFocus);
    graphics_view_->setFrameShape(QFrame::NoFrame);
    graphics_view_->setGeometry(QRect(5, 5, 470, 470));

    main_scene_.setSceneRect(0, 0, 470, 470);
    graphics_view_->setScene(&main_scene_);
    QString dir;
    MUtility::getImageDir(dir);
    graphics_view_->setBackgroundBrush(QPixmap(dir + "/monitor/road_branch.bmp"));
    graphics_view_->setRenderHint(QPainter::Antialiasing);
    graphics_view_->setCacheMode(QGraphicsView::CacheBackground);

    param_disp_grp_ = new QGroupBox(STRING_UI_SIGNALER_MONITOR_STATUS_PARAM);
    QLabel *sched_label = new QLabel(STRING_UI_SCHEDULE_ID + ":");
    QLabel *event_label = new QLabel(STRING_UI_TIMESECTION_EVENT_ID + ":");
    QLabel *start_time_label = new QLabel(STRING_UI_TIMESECTION_TIME + ":");
    QLabel *cycle_time_label = new QLabel(STRING_UI_SIGNALER_MONITOR_CYCLE_TIME + "(s):");
    QLabel *ctrl_mode_label = new QLabel(STRING_UI_TIMESECTION_CTRL_MODE + ":");
    QLabel *stage_id_label = new QLabel(STRING_UI_SIGNALER_MONITOR_STAGE_ID + ":");
    QLabel *phase_id_label = new QLabel(STRING_UI_SIGNALER_MONITOR_PHASE_ID + ":");

    sched_id_label_ = new QLabel;
    event_id_label_ = new QLabel;
    start_time_label_ = new QLabel;
    cycle_time_label_ = new QLabel;
    ctrl_mode_label_ = new QLabel;
    stage_id_label_ = new QLabel;
    phase_id_label_ = new QLabel;

    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(sched_label, 0, 0, 1, 1);
    glayout->addWidget(sched_id_label_, 0, 1, 1, 1);
    glayout->addWidget(event_label, 1, 0, 1, 1);
    glayout->addWidget(event_id_label_, 1, 1, 1, 1);
    glayout->addWidget(start_time_label, 2, 0, 1, 1);
    glayout->addWidget(start_time_label_, 2, 1, 1, 1);
    glayout->addWidget(cycle_time_label, 3, 0, 1, 1);
    glayout->addWidget(cycle_time_label_, 3, 1, 1, 1);
    glayout->addWidget(ctrl_mode_label, 4, 0, 1, 1);
    glayout->addWidget(ctrl_mode_label_, 4, 1, 1, 1);
    glayout->addWidget(stage_id_label, 5, 0, 1, 1);
    glayout->addWidget(stage_id_label_, 5, 1, 1, 1);
    glayout->addWidget(phase_id_label, 6, 0, 1, 1);
    glayout->addWidget(phase_id_label_, 6, 1, 1, 1);

    QLabel *phase_time_label = new QLabel(STRING_UI_SIGNALER_MONITOR_PHASE_TIME + ":" + "(" + STRING_UI_SIGNALER_MONITOR_LCD_TIP + ")");
    phase_time_lcd_ = new QLCDNumber;
    phase_time_lcd_->setMinimumSize(QSize(0, 32));
    phase_time_lcd_->setMouseTracking(true);
    phase_time_lcd_->setFocusPolicy(Qt::ClickFocus);
    phase_time_lcd_->setFrameShape(QFrame::Box);
    phase_time_lcd_->setFrameShadow(QFrame::Sunken);
    phase_time_lcd_->setSmallDecimalPoint(false);
    phase_time_lcd_->setNumDigits(8);
    phase_time_lcd_->setDigitCount(8);
    phase_time_lcd_->setMode(QLCDNumber::Dec);
    phase_time_lcd_->setSegmentStyle(QLCDNumber::Flat);
    phase_time_lcd_->setProperty("intValue", QVariant(0));
    phase_time_lcd_->setStatusTip(STRING_UI_SIGNALER_MONITOR_LCD_TIP);
	phase_time_lcd_->setToolTip(STRING_UI_SIGNALER_MONITOR_LCD_TIP);

    QLabel *signaler_time_label = new QLabel(STRING_UI_SIGNALER_MONITOR_SIGNALER_TIME + ":");
    signaler_time_label_ = new QLabel;
    signaler_time_label_->setFrameShape(QFrame::NoFrame);
    signaler_time_label_->setFrameShadow(QFrame::Sunken);
    signaler_time_label_->setTextFormat(Qt::AutoText);

    QVBoxLayout *time_vlayout = new QVBoxLayout;
    time_vlayout->addWidget(phase_time_label);
    time_vlayout->addWidget(phase_time_lcd_);
    time_vlayout->addWidget(signaler_time_label);
    time_vlayout->addWidget(signaler_time_label_);

    signaler_record_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR_SIGNALER_RECORD);
    light_status_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR_LIGHT_STATUS);
    driver_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR_DRIVER_STATUS);
    detector_button_ = new QPushButton(STRING_UI_SIGNALER_MONITOR_DETECTOR_STATUS);

    signaler_record_button_->setCheckable(true);
    light_status_button_->setCheckable(true);
    driver_button_->setCheckable(true);
    detector_button_->setCheckable(true);

//    time_vlayout->addWidget(signaler_record_button_);
    time_vlayout->addWidget(light_status_button_);
    time_vlayout->addWidget(driver_button_);
    time_vlayout->addWidget(detector_button_);

    QVBoxLayout *param_vlayout = new QVBoxLayout;
    param_vlayout->addLayout(glayout);
    param_vlayout->addLayout(time_vlayout);
    param_disp_grp_->setLayout(param_vlayout);

    signaler_tree_ = new QTreeWidget;
    QStringList signaler_header(STRING_UI_SIGNALER_MONITOR_SIGNALER_STATUS);
    InitTree(signaler_tree_, signaler_header);

    light_tree_ = new QTreeWidget;
    QStringList light_header;
    light_header << STRING_UI_CHANNEL_ID << STRING_UI_SIGNALER_MONITOR_LIGHT_COLOR
                    << STRING_UI_SIGNALER_MONITOR_LIGHT_STATUS;
    InitTree(light_tree_, light_header);
    light_tree_->setColumnWidth(0, 60);
    light_tree_->setColumnWidth(1, 60);
    InitLightTreeContent();

    driver_tree_ = new QTreeWidget;
    QStringList driver_header;
    driver_header << STRING_UI_SIGNALER_MONITOR_DRIVER_ID << STRING_MAIN_STATUS;
//                     << STRING_UI_SIGNALER_MONITOR_DRIVER_TYPE;
    InitTree(driver_tree_, driver_header);
    InitDriverTreeContent();

    detector_tree_ = new QTreeWidget;
    QStringList detector_header;
    detector_header << STRING_UI_SIGNALER_MONITOR_DETECTOR_ID << STRING_UI_SIGNALER_MONITOR_DETECTOR_FLOW
                       << STRING_UI_SIGNALER_MONITOR_DETECTOR_STATUS;
    InitTree(detector_tree_, detector_header);
    detector_tree_->setColumnWidth(0, 60);
    detector_tree_->setColumnWidth(1, 60);
    InitDetectorTreeContent();

    stk_layout_ = new QStackedLayout;
    stk_layout_->addWidget(signaler_tree_); // index 0
    stk_layout_->addWidget(light_tree_);    // index 1
    stk_layout_->addWidget(driver_tree_);   // index 2
    stk_layout_->addWidget(detector_tree_); // index 3

    tree_grp_ = new QGroupBox;
    tree_grp_->setTitle(STRING_UI_SIGNALER_MONITOR_SIGNALER_RECORD);
    tree_grp_->setLayout(stk_layout_);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSizeConstraint(QLayout::SetFixedSize);
    road_monitor_grp_->setMinimumSize(500, 510);
    hlayout->addWidget(road_monitor_grp_);
    hlayout->addWidget(param_disp_grp_);
    hlayout->addWidget(tree_grp_);

    setLayout(hlayout);
}

void RealtimeMonitorDlg::InitSignalSlots()
{
    button_list_.append(signaler_record_button_);
    button_list_.append(light_status_button_);
    button_list_.append(driver_button_);
    button_list_.append(detector_button_);

    connect(signaler_record_button_, SIGNAL(toggled(bool)), this, SLOT(OnSignalerRecordButtonToggled(bool)));
    connect(light_status_button_, SIGNAL(toggled(bool)), this, SLOT(OnLightStatusButtonToggled(bool)));
    connect(driver_button_, SIGNAL(toggled(bool)), this, SLOT(OnDriverButtonToggled(bool)));
    connect(detector_button_, SIGNAL(toggled(bool)), this, SLOT(OnDetectorButtonToggled(bool)));

    connect(signaler_timer_, SIGNAL(timeout()), this, SLOT(OnSignalerTimeTimerOutSlot()));
    connect(count_down_timer_, SIGNAL(timeout()), this, SLOT(OnCountDownTimerOutSlot()));
//    connect(SyncCommand::GetInstance(), SIGNAL(connectErrorStrSignal(QString)), this, SLOT(OnConnectError(QString)));
}

void RealtimeMonitorDlg::InitPixmap()
{
    QString dir;
    MUtility::getImageDir(dir);
    main_image_.load(dir + "/monitor/road_branch.bmp");
    ld_pix_.load(dir + "/monitor/ld_img.bmp");
    lr_pix_.load(dir + "/monitor/lr_img.bmp");
    ly_pix_.load(dir + "/monitor/ly_img.bmp");
    lg_pix_.load(dir + "/monitor/lg_img.bmp");
    rd_pix_.load(dir + "/monitor/rd_img.bmp");
    rr_pix_.load(dir + "/monitor/rr_img.bmp");
    ry_pix_.load(dir + "/monitor/ry_img.bmp");
    rg_pix_.load(dir + "/monitor/rg_img.bmp");
    td_pix_.load(dir + "/monitor/td_img.bmp");
    tr_pix_.load(dir + "/monitor/tr_img.bmp");
    ty_pix_.load(dir + "/monitor/ty_img.bmp");
    tg_pix_.load(dir + "/monitor/tg_img.bmp");
    dd_pix_.load(dir + "/monitor/dd_img.bmp");
    dr_pix_.load(dir + "/monitor/dr_img.bmp");
    dy_pix_.load(dir + "/monitor/dy_img.bmp");
    dg_pix_.load(dir + "/monitor/dg_img.bmp");
    circle_d_pix_.load(dir + "/monitor/circle_d_img.bmp");
    circle_r_pix_.load(dir + "/monitor/circle_r_img.bmp");
    circle_g_pix_.load(dir + "/monitor/circle_g_img.bmp");

    QPointF *pt1 = new QPointF(165, 393);
    QPointF *pt2 = new QPointF(44, 166);
    QPointF *pt3 = new QPointF(270, 45);
    QPointF *pt4 = new QPointF(392, 271);
    QPointF *pt5 = new QPointF(200, 393);
    QPointF *pt6 = new QPointF(44, 201);
    QPointF *pt7 = new QPointF(236, 45);
    QPointF *pt8 = new QPointF(392, 237);
    QPointF *pt9 = new QPointF(131, 393);
    QPointF *pt10 = new QPointF(44, 132);
    QPointF *pt11 = new QPointF(305, 45);
    QPointF *pt12 = new QPointF(392, 306);
    QPointF *pt13 = new QPointF(93, 109);
    QPointF *pt13_2 = new QPointF(93, 343);
    QPointF *pt14 = new QPointF(109, 93);
    QPointF *pt14_2 = new QPointF(343, 93);
    QPointF *pt15 = new QPointF(358, 110);
    QPointF *pt15_2 = new QPointF(358, 344);
    QPointF *pt16_2 = new QPointF(342, 361);
    QPointF *pt16 = new QPointF(108, 361);

    channel_point_list_ << pt1 << pt2 << pt3 << pt4 << pt5 << pt6 << pt7 << pt8 << pt9 << pt10
                        << pt11 << pt12 << pt13 << pt14 << pt15 << pt16 << pt13_2 << pt14_2 << pt15_2 << pt16_2;
    pix_item_list_
            << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_) << main_scene_.addPixmap(dd_pix_)
            << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_) << main_scene_.addPixmap(ld_pix_)
            << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_) << main_scene_.addPixmap(td_pix_)
            << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_) << main_scene_.addPixmap(rd_pix_)
            << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_) << main_scene_.addPixmap(rd_pix_)
            << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_) << main_scene_.addPixmap(dd_pix_)
            << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_) << main_scene_.addPixmap(ld_pix_)
            << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_) << main_scene_.addPixmap(td_pix_)
            << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_) << main_scene_.addPixmap(ld_pix_)
            << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_) << main_scene_.addPixmap(td_pix_)
            << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_) << main_scene_.addPixmap(rd_pix_)
            << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_) << main_scene_.addPixmap(dd_pix_);
    int count = pix_item_list_.size() / 4;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pix_item_list_.at(i*4 + j)->setPos(*channel_point_list_.at(i));
        }
    }

    light_list_
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_r_pix_) << main_scene_.addPixmap(circle_d_pix_) << main_scene_.addPixmap(circle_g_pix_);
    count = light_list_.size() / 4;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            light_list_.at(i*4 + j)->setPos(*channel_point_list_.at(i+12));
        }
    }
}

void RealtimeMonitorDlg::InitTree(QTreeWidget *tree, const QStringList &header)
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

void RealtimeMonitorDlg::UpdateUI()
{
//    sync_cmd_->ReadSignalerTime(this, SLOT(OnCmdParseParam(QByteArray&)));
    phase_time_lcd_->display("00-00-00");

    QString str = date_time_.toString("yyyy-MM-dd hh:mm:ss ddd");
    signaler_time_label_->setText(str);
    tree_grp_->hide();

    if (InitTscParam())
    {
        UpdateScheduleInfo();
    }
}

void RealtimeMonitorDlg::UpdateScheduleInfo()
{
    unsigned char sched_id = 0;
    unsigned char time_section_id = 0;
    QDate curr_date = date_time_.date();
    QTime curr_time = date_time_.time();
    int m = 0;
    for (m = 0; m < tsc_param_.sched_table_.FactScheduleNum; m++)
    {
        if ((tsc_param_.sched_table_.ScheduleList[m].ScheduleMonth & (0x01 << curr_date.month())) == 0)
        {
            continue;
        }
        if (curr_date.dayOfWeek() == 7)
        {
            if ((tsc_param_.sched_table_.ScheduleList[m].ScheduleWeek & (0x01 << 1)) == 0)
            {
                continue;
            }
        }
        else
        {
            if ((tsc_param_.sched_table_.ScheduleList[m].ScheduleDay & (0x01 << curr_date.day())) == 0)
            {
                continue;
            }
        }
        if ((tsc_param_.sched_table_.ScheduleList[m].ScheduleDay & (0x01 << curr_date.day())) == 0)
        {
            continue;
        }
        sched_id = tsc_param_.sched_table_.ScheduleList[m].ScheduleId;
        time_section_id = tsc_param_.sched_table_.ScheduleList[m].TimeSectionId;
        sched_id_label_->setText(QString::number(sched_id));
        break;
    }   //:~ schedule id

    QString str;
    int n = 0;
    unsigned char pattern_id = 0;
    for (m = 0; m < tsc_param_.time_section_table_.FactTimeSectionNum; m++)
    {
        if (tsc_param_.time_section_table_.TimeSectionList[m][0].TimeSectionId != time_section_id)
        {
            continue;
        }
        for (n = 0; n < tsc_param_.time_section_table_.FactEventNum; n++)
        {
            unsigned char start_hour = tsc_param_.time_section_table_.TimeSectionList[m][n].StartHour;
            unsigned char start_min = tsc_param_.time_section_table_.TimeSectionList[m][n].StartMinute;
            if (start_hour > curr_time.hour() || (start_hour == curr_time.hour() && start_min > curr_time.minute()))
            {
                if (n == 0)
                {
                    event_id_label_->setText(" -");
                    start_time_label_->setText("--:--");
                    ctrl_mode_label_->setText(" -");
                    pattern_id = 0;
                }
                else
                {
                    unsigned char event_id = tsc_param_.time_section_table_.TimeSectionList[m][n-1].EventId;
                    start_hour = tsc_param_.time_section_table_.TimeSectionList[m][n-1].StartHour;
                    start_min = tsc_param_.time_section_table_.TimeSectionList[m][n-1].StartMinute;
                    unsigned char ctrl_mode = tsc_param_.time_section_table_.TimeSectionList[m][n-1].ControlMode;
                    QString str;
                    event_id_label_->setText(str.sprintf("%d", event_id));
                    str.sprintf("%02d:%02d", start_hour, start_min);
                    start_time_label_->setText(str);
                    str = EventLogDescriptor::GetInstance()->get_ctrl_mode_desc(ctrl_mode);
                    ctrl_mode_label_->setText(str);
                    pattern_id = tsc_param_.time_section_table_.TimeSectionList[m][n-1].PatternId;
                }
                break;
            }
            if (tsc_param_.time_section_table_.TimeSectionList[m][n+1].EventId == 0)
            {
                unsigned char event_id = tsc_param_.time_section_table_.TimeSectionList[m][n].EventId;
                str.sprintf("%d", event_id);
                event_id_label_->setText(str);
                start_hour = tsc_param_.time_section_table_.TimeSectionList[m][n].StartHour;
                start_min = tsc_param_.time_section_table_.TimeSectionList[m][n].StartMinute;
                str.sprintf("%02d:%02d", start_hour, start_min);
                start_time_label_->setText(str);
                unsigned char ctrl_mode = tsc_param_.time_section_table_.TimeSectionList[m][n].ControlMode;
                str = EventLogDescriptor::GetInstance()->get_ctrl_mode_desc(ctrl_mode);
                pattern_id = tsc_param_.time_section_table_.TimeSectionList[m][n].PatternId;
                break;
            }
        }
        break;
    }   //:~ event id, start time, control mode desc

    unsigned char time_config_id = 0;
    for (m = 0; m < tsc_param_.timing_plan_table_.FactPatternNum; m++)
    {
        if (tsc_param_.timing_plan_table_.PatternList[m].PatternId == pattern_id)
        {
            unsigned short circle_time = tsc_param_.timing_plan_table_.PatternList[m].CycleTime;
            str.sprintf("%d", circle_time);
            cycle_time_label_->setText(str);
            time_config_id = tsc_param_.timing_plan_table_.PatternList[m].TimeConfigId;
        }
    }
    unsigned char stage_count = 0;
    for (m = 0; m < tsc_param_.stage_timing_table_.FactTimeConfigNum; m++)
    {
        if (tsc_param_.stage_timing_table_.TimeConfigList[m][0].TimeConfigId != time_config_id)
        {
            continue;
        }
        for (n = 0; n < tsc_param_.stage_timing_table_.FactStageNum; n++)
        {
//            stage_phase_buff[n] = tsc_param_.stage_timing_table_.TimeConfigList[m][n].PhaseId;
            if (tsc_param_.stage_timing_table_.TimeConfigList[m][n+1].TimeConfigId == 0)
            {
                stage_count = n+1;
                break;
            }
        }
        break;
    }
    total_stage_count_ = stage_count;
    str.sprintf("%d / %d", curr_stage_id_, total_stage_count_);
    stage_id_label_->setText(str);
    //:~ stage id
}

void RealtimeMonitorDlg::InitFaultDesc()
{
    light_flag_desc_map_.insert(0, STRING_EVENT_LOG_RESTORE);
    light_flag_desc_map_.insert(1, STRING_EVENT_LOG_FAULT);

    light_fault_desc_map_.insert(1, STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_CONFLICT);
    light_fault_desc_map_.insert(2, STRING_EVENT_LOG_RG_BRIGHT + STRING_EVENT_LOG_BRIGHT_TOGETHER);
    light_fault_desc_map_.insert(3, STRING_EVENT_LOG_RED + STRING_EVENT_LOG_OFF_BRIGHT);
    light_fault_desc_map_.insert(4, STRING_EVENT_LOG_RED + STRING_EVENT_LOG_ERR_BRIGHT);
    light_fault_desc_map_.insert(5, STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_OFF_BRIGHT);
    light_fault_desc_map_.insert(6, STRING_EVENT_LOG_YELLOW + STRING_EVENT_LOG_ERR_BRIGHT);
    light_fault_desc_map_.insert(7, STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_OFF_BRIGHT);
    light_fault_desc_map_.insert(8, STRING_EVENT_LOG_GREEN + STRING_EVENT_LOG_ERR_BRIGHT);

    driver_fault_desc_map_.insert(0, STRING_DIRVER_FAULT_NOT_INSTALLED);
    driver_fault_desc_map_.insert(1, STRING_EVENT_LOG_RUN);
    driver_fault_desc_map_.insert(2, STRING_UI_NORMAL);
    driver_fault_desc_map_.insert(3, STRING_EVENT_LOG_YELLOW_FLASH_FAULT);
    driver_fault_desc_map_.insert(4, STRING_EVENT_LOG_YELLOW_FLASH_SEPERATE + STRING_EVENT_LOG_OFF);
    driver_fault_desc_map_.insert(5, STRING_EVENT_LOG_YELLOW_FLASH_SEPERATE + STRING_EVENT_LOG_CTRL);

    detector_fault_desc_map_.insert(0, STRING_EVENT_LOG_FAULT);
    detector_fault_desc_map_.insert(1, STRING_UI_NORMAL);
}

void RealtimeMonitorDlg::InitLightTreeContent()
{
    QString str;
    for (int i = 0; i < MAX_CHANNEL; i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(light_tree_);
        str.sprintf("%d", i+1);
        item->setText(0, str);
        str = STRING_LIGHT_OFF;
        item->setText(1, str);
        item->setTextColor(1, QColor(0,0,0));
        str = STRING_UI_NORMAL;
        item->setText(2, str);
        for (int j = 0; j < 3; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        light_item_list_.append(item);
    }
    light_tree_->addTopLevelItems(light_item_list_);
}

void RealtimeMonitorDlg::UpdateLightTreeColor()
{
//    light_tree_->clear();
//    QList<QTreeWidgetItem *> tree_item_list;
    QString str;
    LightColor color;
    QTreeWidgetItem *item = NULL;
    for (int i = 1; i < MAX_CHANNEL+1; i++)
    {
//        QTreeWidgetItem *item = new QTreeWidgetItem;
        item = light_item_list_.at(i-1);
        color = channel_color_array_[i];
        switch (color)
        {
        case Red:
            item->setText(0, str.sprintf("%d", i));
            str = STRING_LIGHT_RED;
            item->setText(1, str);
            item->setTextColor(1, QColor(255,0,0));
            break;
        case Yellow:
            item->setText(0, str.sprintf("%d", i));
            str = STRING_LIGHT_YELLOW;
            item->setText(1, str);
            item->setTextColor(1, QColor(255,255,0));
            break;
        case Green:
            item->setText(0, str.sprintf("%d", i));
            str = STRING_LIGHT_GREEN;
            item->setText(1, str);
            item->setTextColor(1, QColor(0,255,0));
            break;
        case Off:
            item->setText(0, str.sprintf("%d", i));
            str = STRING_LIGHT_OFF;
            item->setText(1, str);
            item->setTextColor(1, QColor(0,0,0));
            break;
        default:
            item->setText(0, str.sprintf("%d", i+1));
            str = STRING_LIGHT_OFF;
            item->setText(1, str);
            item->setTextColor(1, QColor(0,0,0));
            break;
        }
        item->setTextAlignment(1, Qt::AlignCenter);
//        tree_item_list.append(item);
    }
//    light_tree_->addTopLevelItems(tree_item_list);
}

void RealtimeMonitorDlg::UpdateLightTreeStatus(const LightFaultInfo &light_fault)
{
    if (light_fault.channel_id <= 0 || light_fault.flag > 1
            || light_fault.fault_id > 8 || light_fault.fault_id <= 0)
    {
        return;
    }
    QString str;
    QTreeWidgetItem *item = light_item_list_.at(light_fault.channel_id-1);
    str = light_fault_desc_map_.value(light_fault.fault_id) + light_flag_desc_map_.value(light_fault.flag);
    item->setText(2, str);
    item->setTextAlignment(2, Qt::AlignCenter);
}

void RealtimeMonitorDlg::InitDriverTreeContent()
{
    QString str;
    for (int i = 0; i < DRIBOARDNUM; i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(driver_tree_);
        str.sprintf("%d", i+1);
        item->setText(0, str);
        str = STRING_DIRVER_FAULT_NOT_INSTALLED;
        item->setText(1, str);
        for (int j = 0; j < 2; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        driver_item_list_.append(item);
    }
    driver_tree_->addTopLevelItems(driver_item_list_);
}

void RealtimeMonitorDlg::UpdateDriverStatusInfo(unsigned char driver_id, unsigned char status)
{
    if (driver_id <= 0 || driver_id > DRIBOARDNUM || status > 5)
    {
        return;
    }
    QTreeWidgetItem *item = driver_item_list_.at(driver_id-1);
    QString str = driver_fault_desc_map_.value(status);
    item->setText(1, str);
}

void RealtimeMonitorDlg::InitDetectorTreeContent()
{
    QString str;
    for (int i = 0; i < MAX_DETECTOR; i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(detector_tree_);
        str.sprintf("%d", i+1);
        item->setText(0, str);
        str = "-";
        item->setText(1, str);
        str = STRING_DIRVER_FAULT_NOT_INSTALLED;
        item->setText(2, str);
        for (int j = 0; j < 3; j++)
        {
            item->setTextAlignment(j, Qt::AlignCenter);
        }
        detector_item_list_.append(item);
    }
    detector_tree_->addTopLevelItems(detector_item_list_);
}

void RealtimeMonitorDlg::UpdateDetectorFlowInfo(unsigned char detector_id)
{
    if (!(detector_id > 0 && detector_id <= MAX_DETECTOR))
    {
        return;
    }
    QTreeWidgetItem *item = detector_item_list_.at(detector_id-1);
    int flow_count = item->text(2).toInt();
    item->setText(2, QString::number(++flow_count));
    item->setTextAlignment(2, Qt::AlignCenter);
}

void RealtimeMonitorDlg::UpdateDetectorStatusInfo(unsigned char detector_id, unsigned char flag)
{
    if (detector_id <= 0 || detector_id > MAX_DETECTOR)
    {
        return;
    }
    QTreeWidgetItem *item = detector_item_list_.at(detector_id-1);
    QString str = detector_fault_desc_map_.value(flag);
    item->setText(1, str);
    item->setTextAlignment(1, Qt::AlignCenter);
}

void RealtimeMonitorDlg::ResetButtonStatus(const QPushButton *self_btn)
{
    for (int i = 0; i < button_list_.size(); i++)
    {
        if (button_list_.at(i) != self_btn)
        {
            button_list_.at(i)->setChecked(false);
        }
    }
}

void RealtimeMonitorDlg::UpdateTreeGroupBox(const QString &title, QWidget *tree)
{
    tree_grp_->setTitle(title);
    stk_layout_->setCurrentWidget(tree);
}

void RealtimeMonitorDlg::InitCtrlModeDesc()
{
    ctrl_mode_desc_map_.insert(0, STRING_CTRL_AUTONOMOUS);
    ctrl_mode_desc_map_.insert(1, STRING_CTRL_CLOSE_LIGHT);
    ctrl_mode_desc_map_.insert(2, STRING_CTRL_YELLOW_FLASH);
    ctrl_mode_desc_map_.insert(3, STRING_CTRL_ALL_RED);
    ctrl_mode_desc_map_.insert(4, STRING_CTRL_COORDINATE);
    ctrl_mode_desc_map_.insert(5, STRING_CTRL_FULL_INDUCTION);
    ctrl_mode_desc_map_.insert(6, STRING_CTRL_MAIN_HALF_INDUCTION);
    ctrl_mode_desc_map_.insert(7, STRING_CTRL_SECOND_HALF_INDUC);
    ctrl_mode_desc_map_.insert(8, STRING_CTRL_SINGLE_ADAPT);
    ctrl_mode_desc_map_.insert(9, STRING_CTRL_CROSS_STREET);
    ctrl_mode_desc_map_.insert(10, STRING_CTRL_COOR_INDUCTION);
    ctrl_mode_desc_map_.insert(27, STRING_CTRL_BUS_FIRST);
    ctrl_mode_desc_map_.insert(28, STRING_CTRL_TRAFFIC_CTRL);
    ctrl_mode_desc_map_.insert(29, STRING_CTRL_MANUAL_CTRL);
    ctrl_mode_desc_map_.insert(30, STRING_CTRL_SYS_FAULT_YELLOW);
}

bool RealtimeMonitorDlg::InitTscParam()
{
    FileReaderWriter reader;
    bool res = reader.ReadFile(cfg_file_.toStdString().c_str(), tsc_param_);
    if (!res)
    {
        QMessageBox::warning(this, STRING_WARNING, STRING_UI_SIGNALER_MONITOR_PARSE_CFG + STRING_FAILED, STRING_OK);
        return false;
    }
    return true;
}

bool RealtimeMonitorDlg::CheckPackage(QByteArray &array)
{
    if (array.contains("DETECTDATAER"))
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_MONITOR_REQUIRE_DETECTOR_DATA + STRING_FAILED, STRING_OK);
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

void RealtimeMonitorDlg::CloseAllLights()
{
    for (int i = 0; i < pix_item_list_.size(); i++)
    {
        pix_item_list_.at(i)->hide();
    }

    for (int i = 0; i < light_list_.size(); i++)
    {
        light_list_.at(i)->hide();
    }
}

void RealtimeMonitorDlg::SetPedestrianLight(RealtimeMonitorDlg::LightColor color, int channel_id, bool enable)
{
    int index = channel_id % 13;
    switch (color)
    {
    case Off:
        light_list_.at(index*4)->setVisible(enable);
        light_list_.at(index*4 + 4*4)->setVisible(enable);
        break;
    case Red:
        light_list_.at(index*4 + 1)->setVisible(enable);
        light_list_.at(index*4 + 1 + 4*4)->setVisible(enable);
        break;
    case Green:
        light_list_.at(index*4 + 3)->setVisible(enable);
        light_list_.at(index*4 + 3 + 4*4)->setVisible(enable);
        break;
    default:
        break;
    }
}

void RealtimeMonitorDlg::SetVehicleLight(RealtimeMonitorDlg::LightColor color, int channel_id, bool enable)
{
    int index = (channel_id-1)*4;
    switch (color)
    {
    case Off:
        pix_item_list_.at(index + Off)->setVisible(enable);
        break;
    case Red:
        pix_item_list_.at(index + Red)->setVisible(enable);
        break;
    case Yellow:
        pix_item_list_.at(index + Yellow)->setVisible(enable);
        break;
    case Green:
        pix_item_list_.at(index + Green)->setVisible(enable);
        break;
    default:
        break;
    }
}

void RealtimeMonitorDlg::ReadSignalerConfigFile()
{
    sync_cmd_->ReadSignalerConfigFile(this, SLOT(OnCmdReadSignalerConfigFile(QByteArray&)));
}

bool RealtimeMonitorDlg::ParseConfigContent(QByteArray &array)
{
    if (array.isEmpty())
    {
        QMessageBox::information(this, STRING_TIP, STRING_UI_SIGNALER_SOCKET_ERROR, STRING_OK);
        return false;
    }
    QString head(array.left(4));
    QString tail(array.right(3));
    if (head != QString("CYT4") || tail != QString("END"))
    {
        return false;
    }
    array.remove(0, head.count());
    unsigned char temp[4] = {'\0'};
    for (int i = 0; i < 4; i++)
    {
        temp[i] = array.at(i);
    }
    int len = 0;
    memcpy(&len, temp, 4);
    array.remove(0, sizeof(len));
    int idx = array.indexOf(tail);
    array.remove(idx, tail.count());
    if (len-4-3-4 != array.count())
    {
        return false;
    }
    return true;
}

bool RealtimeMonitorDlg::ParseBeginMonitorContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);
    if ((unsigned int)array.size() < sizeof(begin_monitor_info_))
    {
        array.remove(0, 2);
        return false;
    }
    memcpy(&begin_monitor_info_, array.data(), sizeof(begin_monitor_info_));
    array.remove(0, 2);
    // TODO: left to be done. update ui
	unsigned char channel_id = begin_monitor_info_.channel_id;
	if (channel_id < 12+1)
	{
        SetVehicleLight(channel_color_array_[channel_id], channel_id, false);
		SetVehicleLight((LightColor)begin_monitor_info_.status, begin_monitor_info_.channel_id, true);
	}
    else if (channel_id > 12 && channel_id < 16+1)
	{
        SetPedestrianLight(channel_color_array_[channel_id], channel_id, false);
		SetPedestrianLight((LightColor)begin_monitor_info_.status, channel_id, true);
	}
	channel_color_array_[channel_id] = (LightColor)begin_monitor_info_.status;
    UpdateLightTreeColor();

    return true;
}
// CYT5+控制模式(1字节)+阶段编号(1字节)+灯色(1字节)+灯时(1字节)+相位编码(4字节)+END
bool RealtimeMonitorDlg::ParseCountDownContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);
    memcpy(&count_down_info_, array.data(), sizeof(count_down_info_));
    array.remove(0, 8);

    QString str;
	curr_stage_id_ = count_down_info_.stage_id;
    str.sprintf("%d / %d", curr_stage_id_, total_stage_count_);
    stage_id_label_->setText(str);
    phase_id_label_->setText(MUtility::phaseBitsDesc(count_down_info_.phase_ids));
    ctrl_mode_label_->setText(ctrl_mode_desc_map_.value(count_down_info_.ctrl_mode));
    // phase id left to be update

    count_down_seconds_ = count_down_info_.light_time;
    count_down_light_ = count_down_info_.light_corlor;
    QString count_down_text("00-00-00");
    switch (count_down_info_.light_corlor)
    {
    case Red:
        count_down_text.sprintf("00-00-%02d", count_down_info_.light_time);
        count_down_timer_->stop();
        phase_time_lcd_->display(count_down_text);
        count_down_timer_->start(1000);
        break;
    case Yellow:
        count_down_text.sprintf("00-%02d-00", count_down_info_.light_time);
        count_down_timer_->stop();
        phase_time_lcd_->display(count_down_text);
        count_down_timer_->start(1000);
        break;
    case Green:
        count_down_text.sprintf("%02d-00-00", count_down_info_.light_time);
        count_down_timer_->stop();
        phase_time_lcd_->display(count_down_text);
        count_down_timer_->start(1000);
        break;
    case Off:
        count_down_text = "00-00-00";
        count_down_timer_->stop();
        phase_time_lcd_->display(count_down_text);
        break;
    default:
        break;
    }
    return true;
}

bool RealtimeMonitorDlg::ParseTSCTimeContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);
    if (array.size() < 4)
    {
        array.remove(0, array.size());
        return false;
    }
    unsigned char temp[4] = {'\0'};
    temp[0] = array.at(0);
    temp[1] = array.at(1);
    temp[2] = array.at(2);
    temp[3] = array.at(3);
    array.remove(0, 4);
    unsigned int seconds = 0;
    memcpy(&seconds, temp, sizeof(seconds));
    if (seconds >= 60*60*8)     // east 8 time-zoon
    {
        seconds -= 60*60*8;
    }
    date_time_ = QDateTime::fromTime_t(seconds).toLocalTime();
    signaler_time_label_->setText(date_time_.toString("yyyy-MM-dd hh:mm:ss"));

    return true;
}

bool RealtimeMonitorDlg::ParseDetectorFlowContent(QByteArray &array)
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
    for (int i = 0; i < count; i++)
    {
        handler_->set_detector_flow(detector_array_[i]);
    }
    if (detector_array_ != NULL)
    {
        delete [] detector_array_;
        detector_array_ = NULL;
    }
    // TODO: update ui
    QList<DetectorFlowInfo> flow_list = handler_->get_detector_flow_list();
    for (int i = 0; i < flow_list.size(); i++)
    {
        UpdateDetectorFlowInfo(flow_list.at(i).detector_id);
        UpdateDetectorStatusInfo(flow_list.at(i).detector_id, 1);
    }
    return true;
}
// CYT3+04+01+红灯组1+黄灯组1+绿灯组1+02+红灯组2+黄灯组2+绿灯组2+03+红灯组3+黄灯组3+绿灯组3+04+红灯组4+黄灯组4+绿灯组4+工作模式(1字节)+方案号(1字节)+相位编码(4字节)+END
bool RealtimeMonitorDlg::ParseLightStatusContent(QByteArray &array)
{
    array.remove(0, 4);
    char array_size = 0;
    char num_1 = 0;
    memcpy(&array_size, array.data(), 1);
    array_size -= '0';
    array.remove(0, 1);
    memcpy(&num_1, array.data(), 1);
    RYGArray ryg;
    for (int i = 0; i < array_size; i++)
    {
        array.remove(0, 1);
        memcpy(&ryg, array.data(), 3);
        lights_status_info_.lights[i] = ryg;
        array.remove(0, 3);
    }
    memcpy(&lights_status_info_.work_mode, array.data(), sizeof(lights_status_info_.work_mode));
    array.remove(0, 1);
    memcpy(&lights_status_info_.plan_id, array.data(), sizeof(lights_status_info_.plan_id));
    array.remove(0, 1);
    memcpy(&lights_status_info_.phase_id, array.data(), sizeof(lights_status_info_.phase_id));
    array.remove(0, 4);
    int index = array.indexOf("END");
    array.remove(index, 3);

    channel_status_info_.work_mode = lights_status_info_.work_mode;
    channel_status_info_.stage_id = lights_status_info_.plan_id;
    channel_status_info_.phase_id = lights_status_info_.phase_id;
    channel_status_info_.channel_vec.clear();
    unsigned char red_tmp = 0, yellow_tmp = 0, green_tmp = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            red_tmp = lights_status_info_.lights[i].red;
            yellow_tmp = lights_status_info_.lights[i].yellow;
            green_tmp = lights_status_info_.lights[i].green;
            if ((red_tmp & (0x01 << j)) != 0x00)
            {
                channel_status_info_.channel_vec.append(Red);
            }
            else if ((yellow_tmp & (0x01 << j)) != 0x00)
            {
                channel_status_info_.channel_vec.append(Yellow);
            }
            else if ((green_tmp & (0x01 << j)) != 0x00)
            {
                channel_status_info_.channel_vec.append(Green);
            }
            else
            {
                channel_status_info_.channel_vec.append(Off);
            }
        }
    }
    // TODO: update ui
    for (int i = 1; i < channel_status_info_.channel_vec.size() + 1; i++)
    {
        if (i < 12+1)
        {
            RESET_LIGHT(SetVehicleLight(channel_status_bak_.channel_vec.at(i-1), i, false));
            SetVehicleLight(channel_status_info_.channel_vec.at(i-1), i, true);
        }
        else if (i < 16+1)
        {
            RESET_LIGHT(SetPedestrianLight(channel_status_bak_.channel_vec.at(i-1), i, false));
            SetPedestrianLight(channel_status_info_.channel_vec.at(i-1), i, true);
        }
        channel_color_array_[i] = channel_status_info_.channel_vec.at(i-1);
    }

    // back up channel status info
    channel_status_bak_.channel_vec = channel_status_info_.channel_vec;
    channel_status_bak_.phase_id = channel_status_info_.phase_id;
    channel_status_bak_.stage_id = channel_status_info_.stage_id;
    channel_status_bak_.work_mode = channel_status_info_.work_mode;
    is_first_light_ = false;

    QString str;
    curr_stage_id_ = channel_status_bak_.stage_id;
    str.sprintf("%d / %d", curr_stage_id_, total_stage_count_);
    stage_id_label_->setText(str);
    str = MUtility::phaseBitsDesc(channel_status_bak_.phase_id);
    phase_id_label_->setText(str);
    qDebug() << "stage_id:" << curr_stage_id_
             << "phase_id:" << channel_status_bak_.phase_id << str;
    UpdateLightTreeColor();

    return true;
}
// CYTA+检测器编号(1字节)+END
bool RealtimeMonitorDlg::ParseDetectorFaultContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    if (index != 1)
    {
        array.remove(0, index+3);
        return false;
    }
    unsigned char detector_id = 0;
    memcpy(&detector_id, array.data(), 1);
    array.remove(0, 4);
    // TODO: update ui
    UpdateDetectorStatusInfo(detector_id, 0);

    return true;
}
// CYTC+数据总长度（4字节）+驱动板状态数据字节流+END
bool RealtimeMonitorDlg::ParseDriverStatusContent(QByteArray &array)
{
    array.remove(0, 4);
    unsigned int len = 0;
    memcpy(&len, array.data(), 4);
    len -= (4+4+3);
    array.remove(0, 4);
    int index = array.indexOf("END");
    if (len != (unsigned int)index)
    {
        array.remove(0, index + 3);
        return false;
    }
    memcpy(&driver_info_, array.data(), sizeof(driver_info_));
    array.remove(0, index + 3);
    unsigned char driver_id = 0, driver_status = 0;
    for (int i = 0; i < DRIBOARDNUM; i++)
    {
        if (driver_info_.driboardstatusList[i].dribid != 0)
        {
            driver_id = driver_info_.driboardstatusList[i].dribid;
            driver_status = driver_info_.driboardstatusList[i].status;
            UpdateDriverStatusInfo(driver_id, driver_status);
        }
    }
    // TODO: update driver board info
    return true;
}
// CYTD+驱动板编号(1字节)+状态(1字节)+END
bool RealtimeMonitorDlg::ParseDriverRealTimeStatusContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    if (index != 3)
    {
        array.remove(0, index + 3);
        return false;
    }
    unsigned char driver_id = 0, driver_status = 0;
    driver_id = array.at(0);
    driver_status = array.at(1);
    array.remove(0, 5);
    UpdateDriverStatusInfo(driver_id, driver_status);
    return true;
}
// CYTE+通道号(1字节) + 标志(1字节) + 故障类型(1字节) + END
bool RealtimeMonitorDlg::ParseLightRealTimeStatusContent(QByteArray &array)
{
    array.remove(0, 4);
    int index = array.indexOf("END");
    if (index != 3)
    {
        array.remove(0, index+3);
        return false;
    }
    LightFaultInfo light_status;
    memcpy(&light_status, array.data(), 3);
    array.remove(0, 6);
    // TODO: update ui
    UpdateLightTreeStatus(light_status);

    return true;
}

// CYTB+检测器编号(1字节)+END
bool RealtimeMonitorDlg::ParseRealTimeFlowContent(QByteArray &array)
{
    array.remove(0, 4);
    memcpy(&detector_id_, array.data(), 1);
    array.remove(0, 1);
    if (!array.left(3).contains("END"))
    {
        return false;
    }
    array.remove(0, 3);
    UpdateDetectorFlowInfo(detector_id_);
    UpdateDetectorStatusInfo(detector_id_, 1);
    return true;
}

void RealtimeMonitorDlg::StartMonitoring()
{
    sync_cmd_->StartMonitoring(this, SLOT(OnCmdStartMonitoring(QByteArray&)));
}

void RealtimeMonitorDlg::StopMonitoring()
{
    sync_cmd_->StopMonitoring();
}

void RealtimeMonitorDlg::ResetChannelColor()
{
	for (int i = 0; i < MAX_CHANNEL + 1; i++)
	{
		channel_color_array_[i] = Off;
	}
}
