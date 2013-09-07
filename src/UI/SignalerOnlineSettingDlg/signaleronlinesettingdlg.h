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

class SIGNALERONLINESETTINGDLGSHARED_EXPORT SignalerOnlineSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignalerOnlineSettingDlg(const QString &dialog_name, QWidget *parent = 0);
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
    QString dialog_title_;

private:
    MTabWidget *dialog_tab_;

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
};

#endif // SIGNALERONLINESETTINGDLG_H
