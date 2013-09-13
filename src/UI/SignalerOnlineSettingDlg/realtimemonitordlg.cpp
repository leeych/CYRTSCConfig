#include "realtimemonitordlg.h"
#include "macrostring.h"
#include "mutility.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QMessageBox>

RealtimeMonitorDlg::RealtimeMonitorDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

RealtimeMonitorDlg::~RealtimeMonitorDlg()
{
}

void RealtimeMonitorDlg::Initialize()
{
    setWindowTitle(STRING_UI_SIGNALER_MONITOR);
    UpdateUI();
    exec();
}

void RealtimeMonitorDlg::OnSignalerRecordButtonToggled(bool checked)
{
    if (checked)
    {
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_SIGNALER_RECORD, signaler_tree_);
//        resize(QSize(960, 532));
//        tree_grp_->setHidden(false);
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
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_DRIVER_STATUS, driver_tree_);
        ResetButtonStatus(driver_button_);
    }
    tree_grp_->setHidden(!checked);
}

void RealtimeMonitorDlg::OnDetectorButtonToggled(bool checked)
{
    if (checked)
    {
        UpdateTreeGroupBox(STRING_UI_SIGNALER_MONITOR_DETECTOR_STATUS, detector_tree_);
        ResetButtonStatus(detector_button_);
    }
    tree_grp_->setHidden(!checked);
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

//    QVBoxLayout *road_pix_vlayout = new QVBoxLayout;
//    road_pix_vlayout->addWidget(graphics_view_);

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

    QLabel *phase_time_label = new QLabel(STRING_UI_SIGNALER_MONITOR_PHASE_TIME + ":");
    phase_time_lcd_ = new QLCDNumber;
    phase_time_lcd_->setMinimumSize(QSize(0, 32));
    phase_time_lcd_->setMouseTracking(true);
    phase_time_lcd_->setFocusPolicy(Qt::ClickFocus);
    phase_time_lcd_->setFrameShape(QFrame::Box);
    phase_time_lcd_->setFrameShadow(QFrame::Sunken);
    phase_time_lcd_->setSmallDecimalPoint(false);
    phase_time_lcd_->setNumDigits(7);
    phase_time_lcd_->setDigitCount(7);
    phase_time_lcd_->setMode(QLCDNumber::Dec);
    phase_time_lcd_->setSegmentStyle(QLCDNumber::Flat);
    phase_time_lcd_->setProperty("intValue", QVariant(0));

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

    time_vlayout->addWidget(signaler_record_button_);
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

    driver_tree_ = new QTreeWidget;
    QStringList driver_header;
    driver_header << STRING_UI_SIGNALER_MONITOR_DRIVER_ID << STRING_MAIN_STATUS
                     << STRING_UI_SIGNALER_MONITOR_DRIVER_TYPE;
    InitTree(driver_tree_, driver_header);

    detector_tree_ = new QTreeWidget;
    QStringList detector_header;
    detector_header << STRING_UI_SIGNALER_MONITOR_DETECTOR_ID << STRING_UI_SIGNALER_MONITOR_DETECTOR_FLOW
                       << STRING_UI_SIGNALER_MONITOR_DETECTOR_STATUS;
    InitTree(detector_tree_, detector_header);

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
    connect(signaler_record_button_, SIGNAL(toggled(bool)), this, SLOT(OnSignalerRecordButtonToggled(bool)));
    connect(light_status_button_, SIGNAL(toggled(bool)), this, SLOT(OnLightStatusButtonToggled(bool)));
    connect(driver_button_, SIGNAL(toggled(bool)), this, SLOT(OnDriverButtonToggled(bool)));
    connect(detector_button_, SIGNAL(toggled(bool)), this, SLOT(OnDetectorButtonToggled(bool)));

    button_list_.append(signaler_record_button_);
    button_list_.append(light_status_button_);
    button_list_.append(driver_button_);
    button_list_.append(detector_button_);
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

    pix_item_list_
            << main_scene_.addPixmap(dd_pix_) << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_)
            << main_scene_.addPixmap(ld_pix_) << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_)
            << main_scene_.addPixmap(td_pix_) << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_)
            << main_scene_.addPixmap(rd_pix_) << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_)
            << main_scene_.addPixmap(rd_pix_) << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_)
            << main_scene_.addPixmap(dd_pix_) << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_)
            << main_scene_.addPixmap(ld_pix_) << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_)
            << main_scene_.addPixmap(td_pix_) << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_)
            << main_scene_.addPixmap(ld_pix_) << main_scene_.addPixmap(lr_pix_) << main_scene_.addPixmap(ly_pix_) << main_scene_.addPixmap(lg_pix_)
            << main_scene_.addPixmap(td_pix_) << main_scene_.addPixmap(tr_pix_) << main_scene_.addPixmap(ty_pix_) << main_scene_.addPixmap(tg_pix_)
            << main_scene_.addPixmap(rd_pix_) << main_scene_.addPixmap(rr_pix_) << main_scene_.addPixmap(ry_pix_) << main_scene_.addPixmap(rg_pix_)
            << main_scene_.addPixmap(dd_pix_) << main_scene_.addPixmap(dr_pix_) << main_scene_.addPixmap(dy_pix_) << main_scene_.addPixmap(dg_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_)
            << main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_r_pix_)<<main_scene_.addPixmap(circle_d_pix_)<<main_scene_.addPixmap(circle_g_pix_);

    // TODO: left to be done.
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
    sched_id_label_->setText("1");
    event_id_label_->setText("2");
    start_time_label_->setText("16:02");
    cycle_time_label_->setText("82");
    ctrl_mode_label_->setText("Ctrl mode");
    stage_id_label_->setText("10");
    phase_id_label_->setText("3");
    phase_time_lcd_->display("0-12-0");

    QDateTime datetime = QDateTime::currentDateTime();
    QString str = datetime.toString("yyyy-MM-dd hh:mm:ss ddd");
    signaler_time_label_->setText(str);

    tree_grp_->hide();
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
