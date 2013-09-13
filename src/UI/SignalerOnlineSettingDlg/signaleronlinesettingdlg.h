#ifndef SIGNALERONLINESETTINGDLG_H
#define SIGNALERONLINESETTINGDLG_H

#include <QDialog>
#include <QPushButton>
#include "signaleronlinesettingdlg_global.h"

#include "mtabwidget.h"

#include "unitparamtablewidget.h"
#include "scheduletablewidget.h"
#include "timesectiontablewidget.h"
#include "timingplanwidget.h"
#include "phasetimingtablewidget.h"
#include "phasetablewidget.h"
#include "phaseconflicttablewidget.h"
#include "channeltablewidget.h"
#include "detectortablewidget.h"

//class TimeIPDlg;
//class UnitparamtableWidget;
//class ScheduleTableWidget;
//class TimesectiontableWidget;
//class TimingplanWidget;
//class PhasetimingtableWidget;
//class PhasetableWidget;
//class PhaseconflicttableWidget;
//class ChanneltableWidget;
//class DetectortableWidget;


#include "timeipdlg.h"
#include "detectorflowdlg.h"
#include "eventlogdlg.h"
#include "realtimemonitordlg.h"


class SIGNALERONLINESETTINGDLGSHARED_EXPORT SignalerOnlineSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignalerOnlineSettingDlg(QWidget *parent = 0);
    ~SignalerOnlineSettingDlg();
    void Initialize();

signals:

public slots:
    void OnConnectButtonClicked();
    void OnReadButtonClicked();
    void OnUpdateButtonClicked();
    void OnSendButtonClicked();
    void OnMonitorButtonClicked();
    void OnLogButtonClicked();
    void OnFlowButtonClicked();
    void OnSettingButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();
    void InitTabPage();
    void UpdateUI();

private:
    MTabWidget *dialog_tab_;
//    QTabWidget *dialog_tab_;

    UnitparamtableWidget *unitparam_widget_;
    ScheduleTableWidget *schedule_widget_;
    TimesectiontableWidget *timesection_widget_;
    TimingplanWidget *timing_widget_;
    PhasetimingtableWidget* stage_timing_widget_;
    PhasetableWidget *phase_widget_;
    PhaseconflicttableWidget *phase_err_widget_;
    ChanneltableWidget *channel_widget_;
    DetectortableWidget *detector_widget_;

    QPushButton *conn_button_, *read_button_, *update_button_;
    QPushButton *send_button_, *monitor_button_, *log_button_;
    QPushButton *flow_button_, *setting_button_;

    TimeIPDlg *time_ip_dlg_;
    DetectorFlowDlg *flow_dlg_;
    EventLogDlg *event_log_dlg_;
    RealtimeMonitorDlg *monitor_dlg_;
};

#endif // SIGNALERONLINESETTINGDLG_H
