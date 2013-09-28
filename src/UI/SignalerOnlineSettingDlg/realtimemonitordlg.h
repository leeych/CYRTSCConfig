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

#include "tscparam.h"

// 1. GetConfigure
// 2. BegineMonitor
// 3. GetLampStatus

class SyncCommand;

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
    unsigned int phase_id;
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


class RealtimeMonitorDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RealtimeMonitorDlg(QWidget *parent = 0);
    ~RealtimeMonitorDlg();
    void Initialize(const QString &ip);

signals:
    
public slots:
    void OnSignalerRecordButtonToggled(bool checked);
    void OnLightStatusButtonToggled(bool checked);
    void OnDriverButtonToggled(bool checked);
    void OnDetectorButtonToggled(bool checked);

    void OnConnectError(QString);
    void OnSignalerTimeTimerOut();

    // on cmd
    void OnCmdReadSignalerConfigFile(QByteArray &array);
    void OnCmdStartMonitoring(QByteArray &array);
    void OnCmdStopMonitoring(QByteArray &array);
    void OnCmdGetLightParam(QByteArray &array);
    // Parse all the tcp socket command return result
    void OnCmdParseParam(QByteArray &array);

protected:
    void closeEvent(QCloseEvent *);

private:
    void InitPage();
    void InitSignalSlots();
    void InitPixmap();
    void InitTree(QTreeWidget *tree, const QStringList &header);

    void UpdateUI();
    void UpdateScheduleInfo();

    void ResetButtonStatus(const QPushButton *self_btn);
    void UpdateTreeGroupBox(const QString &title, QWidget *tree);
    void InitCtrlModeDesc();
    bool InitTscParam();
    bool CheckPackage(const QByteArray &array);

    void ReadSignalerConfigFile();

    void StartMonitoring();
    void StopMonitoring();

    bool ParseConfigContent(QByteArray &array);
    bool ParseBeginMonitorContent(QByteArray &array);
    bool ParseLightStatusContent(QByteArray &array);
    bool ParseCountDownContent(QByteArray &array);
    bool ParseTSCTimeContent(QByteArray &array);
    bool ParseSysFaultContent(QByteArray &array);
    bool ParseDriverStatusContent(QByteArray &array);
//    bool ParseDetectorContent(QByteArray &array);
    bool ParseDetectorRealTimeContent(QByteArray &array);

private:
    SyncCommand *sync_cmd_;
    QList<QPushButton *> button_list_;
    QMap<unsigned char, QString> ctrl_mode_desc_map_;

    QString ip_;
    QString cfg_file_;
    TSCParam tsc_param_;
    QByteArray cfg_array_;
    QByteArray recv_array_;

    // used for signaler time display
    bool is_inited_;
    unsigned int second_count_;
    QDateTime date_time_;
    QTimer *signaler_timer_;

    // request reply
    BeginMonitorInfo begin_monitor_info_;
    CountDownInfo count_down_info_;
    LightStatusInfo lights_status_info_;

private:
    QStackedLayout *stk_layout_;
    QGroupBox *road_monitor_grp_, *param_disp_grp_, *tree_grp_;
    QLabel *road_pix_label_;

    QFrame *frame_;
    QGraphicsView *graphics_view_;
    QGraphicsScene main_scene_;
    QList<QGraphicsPixmapItem *> pix_item_list_;

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
