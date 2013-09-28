#ifndef DETECTORFLOWDLG_H
#define DETECTORFLOWDLG_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>

typedef struct DetectorInfoTag
{
    quint32 seconds;
    quint32 phase_id;
    quint8 detector_id;
}DetectorInfo;


class SyncCommand;

class DetectorFlowDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DetectorFlowDlg(QWidget *parent = 0);
    ~DetectorFlowDlg();
    void Initialize();
    
signals:
    
public slots:
    void OnReadFlowButtonClicked();
    void OnClearFowButtonClicked();
    void OnOkButtonClicked();

    void OnCmdGetDetectorData(QByteArray &array);
    void OnCmdClearDetectorInfo(QByteArray &array);
    void OnCmdGetDetectorRealTimeInfo(QByteArray &array);
    // Parse all the reply content
    void OnCmdParseParam(QByteArray &array);

    void OnConnectError(const QString &str);

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void InitTree(QTreeWidget *tree, const QStringList &header);

    bool ParseDetectorDataContent(QByteArray &array);
    bool ParseDetectorRealTimeContent(QByteArray &array);

private:
    SyncCommand *sync_cmd_;
    DetectorInfo detector_status_info_;

private:
    QTreeWidget *detector_tree_, *flow_tree_;
    QDateTimeEdit *start_time_editor_, *end_time_editor_;
    QLineEdit *total_flow_lineedit_;

    QPushButton *read_flow_button_, *clear_flow_button_, *ok_button_;
};

#endif // DETECTORFLOWDLG_H
