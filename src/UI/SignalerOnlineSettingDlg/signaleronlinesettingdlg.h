#ifndef SIGNALERONLINESETTINGDLG_H
#define SIGNALERONLINESETTINGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
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

#include "timeipdlg.h"
#include "detectorflowdlg.h"
#include "eventlogdlg.h"
#include "realtimemonitordlg.h"

class SyncCommand;
class EventLogHandler;

class SIGNALERONLINESETTINGDLGSHARED_EXPORT SignalerOnlineSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignalerOnlineSettingDlg(QWidget *parent = 0);
    ~SignalerOnlineSettingDlg();
    void Initialize(const QString &ip, unsigned int port);

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

    void OnConnectedSlot();
    void OnDisconnectedSlot();

    // cmd call back
    void OnCmdGetVerId(QByteArray &content);
    void OnCmdReadConfig(QByteArray &content);
    void OnCmdSetConfig(QByteArray &content);
    void OnCmdSendConfig(QByteArray &content);

protected:
    void timerEvent(QTimerEvent *);

private:
    void InitPage();
    void InitSignalSlots();
    void InitTabPage();
    void UpdateUI();

    void UpdateConnectStatus(bool status);
    void UpdateButtonStatus(bool enable);

    bool ParseConfigArray(QByteArray &byte_array);

private:
    SyncCommand *sync_cmd_;
    EventLogHandler *handler_;
    QString ip_;
    unsigned int port_;
    bool conn_status_;
    QTcpSocket *socket_;

    bool is_ver_correct_;
    int ver_check_id_;  // check version

    QByteArray config_byte_array_;

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

    QLabel *conn_tip_label_;
    QPushButton *conn_button_, *read_button_, *update_button_;
    QPushButton *send_button_, *monitor_button_, *log_button_;
    QPushButton *flow_button_, *setting_button_;

    TimeIPDlg *time_ip_dlg_;
    DetectorFlowDlg *flow_dlg_;
    EventLogDlg *event_log_dlg_;
    RealtimeMonitorDlg *monitor_dlg_;
};

#endif // SIGNALERONLINESETTINGDLG_H
