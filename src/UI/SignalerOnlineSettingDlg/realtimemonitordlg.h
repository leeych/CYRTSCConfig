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
#include <QList>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QFrame>

class RealtimeMonitorDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RealtimeMonitorDlg(QWidget *parent = 0);
    ~RealtimeMonitorDlg();
    void Initialize();

signals:
    
public slots:
    void OnSignalerRecordButtonToggled(bool checked);
    void OnLightStatusButtonToggled(bool checked);
    void OnDriverButtonToggled(bool checked);
    void OnDetectorButtonToggled(bool checked);

private:
    void InitPage();
    void InitSignalSlots();
    void InitPixmap();

    void InitTree(QTreeWidget *tree, const QStringList &header);

    void UpdateUI();
    void ResetButtonStatus(const QPushButton *self_btn);
    void UpdateTreeGroupBox(const QString &title, QWidget *tree);

private:
    QList<QPushButton *> button_list_;

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
