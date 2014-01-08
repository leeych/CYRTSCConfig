#ifndef REALTIMEMONITORDLG_H
#define REALTIMEMONITORDLG_H

#include <QDialog>
#include <QGroupBox>
#include <QLCDNumber>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QStackedLayout>
#include <QTreeWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QFrame>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QDateTime>

#include <QKeyEvent>

#include "tscparam.h"

// 1. GetConfigure
// 2. GetLampStatus
// 3. GetTscTime
// 4. BeginMonitor

class SyncCommand;
class DetectorFlowHandler;

typedef struct BeginMonitorTag
{
    unsigned char channel_id;
    unsigned char status;
}BeginMonitorInfo;

typedef struct CountDownTag
{
    unsigned char ctrl_mode;
    unsigned char stage_id;
    unsigned char light_corlor;
    unsigned char light_time;
    unsigned int phase_ids;
}CountDownInfo;

typedef struct RedYellowGreenTag
{
    unsigned char red;
    unsigned char yellow;
    unsigned char green;

    RedYellowGreenTag() :
        red(0), yellow(0), green(0)
    {}

}RYGArray;

typedef struct LightStatusTag
{
    RYGArray lights[4];
    unsigned char work_mode;
    unsigned char plan_id;
    unsigned int phase_id;
}LightStatusInfo;

typedef struct LightFaultTag
{
    LightFaultTag():
        channel_id(0),
        flag(0),
        fault_id(0)
    { }

    unsigned char channel_id;
    unsigned char flag;
    unsigned char fault_id;

}LightFaultInfo;


class RealtimeMonitorDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RealtimeMonitorDlg(QWidget *parent = 0);
    ~RealtimeMonitorDlg();
    void Initialize(const QString &ip);

signals:
    void realtimeMonitorClosedSignal();
    void readyReadConfigSignal();
    
public slots:
    void OnSignalerRecordButtonToggled(bool checked);
    void OnLightStatusButtonToggled(bool checked);
    void OnDriverButtonToggled(bool checked);
    void OnDetectorButtonToggled(bool checked);

    void OnConnectError(QString);
    void OnSignalerTimeTimerOutSlot();
    void OnCountDownTimerOutSlot();
    void OnCheckSocketTimerOutSlot();
    void OnHeartBeatTimerOutSlot();

    // on cmd
    void OnCmdReadSignalerConfigFile(QByteArray &array);
    void updateUISlot();
    // Parse all the tcp socket command return result
    void OnCmdParseParam(QByteArray &array);

protected:
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *key);
//    void keyReleaseEvent(QKeyEvent *);

private:
    void InitPage();
    void InitSignalSlots();
    void InitPixmap();
    void InitTree(QTreeWidget *tree, const QStringList &header);

    void UpdateScheduleInfo();
    void exitOnClose();

    void InitFaultDesc();
    void InitLightTreeContent();
    void UpdateLightTreeColor();
    void UpdateLightTreeStatus(const LightFaultInfo &light_fault);

    void InitDriverTreeContent();
    void UpdateDriverStatusInfo(unsigned char driver_id, unsigned char status);
    void InitDetectorTreeContent();
    void UpdateDetectorFlowInfo(unsigned char detector_id, unsigned char flow_data);
    void UpdateDetectorFlowInfo(unsigned char detector_id);
    void UpdateDetectorStatusInfo(unsigned char detector_id, unsigned char flag);

    void ResetButtonStatus(const QPushButton *self_btn);
    void UpdateTreeGroupBox(const QString &title, QWidget *tree);
    void InitCtrlModeDesc();
    bool InitTscParam();
    bool CheckPackage(QByteArray &array);

    void heartBeatHandler();

    void test();

    enum LightColor
    {
        Red = 0,
        Yellow,
        Green,
        Off,
        Invalid
    };
    void ResetChannelColor(LightColor color);

    enum Direction
    {
        South = 0,
        West,
        North,
        East,
        SouthRight,
        WestDown,
        NorthLeft,
        EastUp,
        SouthLeft,
        WestUp,
        NorthRight,
        EastDown
    };

    void CloseAllLights();
    void SetPedestrianLight(LightColor color, int channel_id, bool enable);
    void SetVehicleLight(LightColor color, int channel_id, bool enable);

    void ReadSignalerConfigFile();
    void StartMonitoring();
    void StopMonitoring();

    bool ParseConfigContent(QByteArray &array);
    bool ParseBeginMonitorContent(QByteArray &array);
    bool ParseLightStatusContent(QByteArray &array);
    bool ParseCountDownContent(QByteArray &array);
    bool ParseTSCTimeContent(QByteArray &array);
    bool ParseDetectorFlowContent(QByteArray &array);
    bool ParseDetectorFaultContent(QByteArray &array);
    bool ParseDriverStatusContent(QByteArray &array);
    bool ParseRealTimeFlowContent(QByteArray &array);
    bool ParseDriverRealTimeStatusContent(QByteArray &array);
    bool ParseLightRealTimeStatusContent(QByteArray &array);
    bool ParseAllLightOnContent(QByteArray &array);
    bool ParseCycleEndContent(QByteArray &array);
    bool DefaultParser(QByteArray &array);

private:
    SyncCommand *sync_cmd_;
    QList<QPushButton *> button_list_;
    DetectorFlowHandler *handler_;
    QList<QTreeWidgetItem *> light_item_list_;
    QList<QTreeWidgetItem *> detector_item_list_;
    QList<QTreeWidgetItem *> driver_item_list_;

    QList<unsigned char> channel_id_list_;

    QString ip_;
    QString cfg_file_;
    TSCParam tsc_param_;
    QByteArray cfg_array_;
    QByteArray recv_array_;

    unsigned char total_stage_count_;	// used for stage id disp
    unsigned char curr_stage_id_;		// used for stage id disp
    unsigned char count_down_seconds_;  // used for count_down disp
    unsigned char count_down_light_;

    unsigned char detector_id_;

    bool is_first_light_;       // used for update light status
    bool is_uitimer_started_;
    int ui_timer_id_;
    bool is_cycle_end_;

    // used for signaler time display
    bool is_inited_;
    unsigned int second_count_;
    QDateTime date_time_;
    QTimer *signaler_timer_;
    QTimer *count_down_timer_;
//    QTimer *check_socket_timer_;
    QTimer *heart_beat_timer_;
    int milliseconds_;

    DetectorData_t *detector_array_;    // used for get detectorinfo
    driboardstatus_t driver_info_;      // used for driver board

    LightColor pre_color_;      // used for CYTF

    // request reply
    BeginMonitorInfo begin_monitor_info_;
	LightColor channel_color_array_[MAX_CHANNEL + 1];
    CountDownInfo count_down_info_;
    LightStatusInfo lights_status_info_;

    typedef struct ChannelStatusInfoTag
    {
        QVector<LightColor> channel_vec;
        unsigned char work_mode;
        unsigned char stage_id;
        unsigned int phase_id;
    }ChannelStatusInfo;

    ChannelStatusInfo channel_status_info_;
    ChannelStatusInfo channel_status_bak_;  // used for revert lights' status

    QMap<unsigned char, QString> ctrl_mode_desc_map_;
    QMap<unsigned char, QString> light_flag_desc_map_;
    QMap<unsigned char, QString> light_fault_desc_map_;
    QMap<unsigned char, QString> driver_fault_desc_map_;
    QMap<unsigned char, QString> detector_fault_desc_map_;

private:
    QStackedLayout *stk_layout_;
    QGroupBox *road_monitor_grp_, *param_disp_grp_, *tree_grp_;
    QLabel *road_pix_label_;

    QFrame *frame_;
    QGraphicsView *graphics_view_;
    QGraphicsScene main_scene_;
    QList<QGraphicsPixmapItem *> pix_item_list_;
    QList<QGraphicsPixmapItem *> light_list_;
    QList<QPointF *> channel_point_list_;

    QImage main_image_;
    QPixmap ld_pix_, lr_pix_, ly_pix_, lg_pix_;
    QPixmap rd_pix_, rr_pix_, ry_pix_, rg_pix_;
    QPixmap td_pix_, tr_pix_, ty_pix_, tg_pix_;
    QPixmap dd_pix_, dr_pix_, dy_pix_, dg_pix_;
    QPixmap circle_d_pix_, circle_r_pix_, circle_g_pix_;

    QTreeWidget *signaler_tree_, *light_tree_;
    QTreeWidget *driver_tree_, *detector_tree_;
    QLabel *sched_id_label_, *event_id_label_;
    QLabel *start_time_label_, *cycle_time_label_;
    QLabel *ctrl_mode_label_, *stage_id_label_, *phase_id_label_;
    QLCDNumber *phase_time_lcd_;
    QLabel *signaler_time_label_;

    QPushButton *signaler_record_button_;
    QPushButton *light_status_button_;
    QPushButton *driver_button_;
    QPushButton *detector_button_;
};

#endif // REALTIMEMONITORDLG_H
