#ifndef SIGNALERONLINESETTINGDLG_H
#define SIGNALERONLINESETTINGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTcpSocket>
#include <QDesktopWidget>
//#include "signaleronlinesettingdlg_global.h"

class MDatabase;
class MTabWidget;
class UnitparamtableWidget;
class ScheduleTableWidget;
class TimesectiontableWidget;
class TimingplanWidget;
class PhasetimingtableWidget;
class PhasetableWidget;
class PhaseconflicttableWidget;
class ChanneltableWidget;
class DetectortableWidget;

class TimeIPDlg;
class DetectorFlowDlg;
class EventLogDlg;
class RealtimeMonitorDlg;

class SyncCommand;
class EventLogHandler;

class /*SIGNALERONLINESETTINGDLGSHARED_EXPORT*/ SignalerOnlineSettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignalerOnlineSettingDlg(QWidget *parent = 0);
    ~SignalerOnlineSettingDlg();
    void Initialize(const QString &ip, unsigned int port);

signals:
    void updateTabPageSignal(void *);

public slots:
    void OnConnectButtonClicked();
    void OnReadButtonClicked();
    void OnUpdateButtonClicked();
    void OnSendButtonClicked();
    void OnMonitorButtonClicked();
    void OnLogButtonClicked();
    void OnFlowButtonClicked();
    void OnSettingButtonClicked();
    void OnSaveAsbuttonClicked();
    void OnMoreButtonToggled(bool toggled);

    void OnConnectedSlot();
    void OnDisconnectedSlot();
    void OnConnectErrorSlot(QString err);

    // cmd call back
    void OnCmdGetVerId(QByteArray &content);
    void OnCmdReadConfig(QByteArray &content);
    void OnCmdSetConfig(QByteArray &content);
    void OnCmdSendConfig(QByteArray &content);

protected:
    void closeEvent(QCloseEvent *);
    void timerEvent(QTimerEvent *);

private:
    void InitPage();
    void InitSignalSlots();
    void InitTabPage();

    void UpdateUI();
    void UpdateConnectStatus(bool status);
    void UpdateButtonStatus(bool enable);
    void UpdateTabPage();

    void EnableDialogs(bool enable);
    bool ParseConfigArray(QByteArray &byte_array);

private:
    MDatabase *db_ptr_;
    SyncCommand *sync_cmd_;
    EventLogHandler *handler_;
    QTcpSocket *socket_;

    QString cfg_file_;
    QString tmp_file_;  // used for send and update config file
    QString ip_;
    unsigned int port_;
    bool conn_status_;

    bool is_ver_correct_;
    int ver_check_id_;  // check version
    int ui_lock_id_;    // enable ui except for buttons

    QByteArray config_byte_array_;

private:
    QWidget *more_widget_;
    MTabWidget *dialog_tab_;

    QDesktopWidget *desktop_;

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
    QPushButton *saveas_button_, *more_button_;

    TimeIPDlg *time_ip_dlg_;
    DetectorFlowDlg *flow_dlg_;
    EventLogDlg *event_log_dlg_;
    RealtimeMonitorDlg *monitor_dlg_;
};

#endif // SIGNALERONLINESETTINGDLG_H
