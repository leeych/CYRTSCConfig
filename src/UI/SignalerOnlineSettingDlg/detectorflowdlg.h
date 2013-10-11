#ifndef DETECTORFLOWDLG_H
#define DETECTORFLOWDLG_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QList>

#include "detectorflowhandler.h"
#include "tsc.h"

class DetectorFlowHandler;
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
    void OnDetectorIDTreeDoubleClicked(QTreeWidgetItem *, int);
    void OnDateTimeChanged(const QDateTime &datetime);

    // Parse all the reply content
    void OnCmdParseParam(QByteArray &array);
    void OnConnectError(const QString &str);

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();

    void InitDetectorTreeContent();
    void UpdateDetectorTree();
    void UpdateFlowInfoTree();
    void UpdateFlowInfoTree(const QList<DetectorFlowInfo> &flow_list);
    void SetDetectorFlow(unsigned char detector_id);

    void InitTree(QTreeWidget *tree, const QStringList &header);
    void SetDateTimeEdit(QDateTimeEdit *edit);

    bool CheckPackage(QByteArray &array);
    bool ParseDetectorDataContent(QByteArray &array);
    bool ParseRealtimeFlowInfoContent(QByteArray &array);

private:
    unsigned char curr_detector_id_;
    DetectorFlowHandler *handler_;
    SyncCommand *sync_cmd_;
    DetectorFlowInfo detector_status_info_;
    DetectorData_t *detector_array_;
    QList<DetectorFlowInfo> detector_list_;
    QList<QTreeWidgetItem *> detector_item_list_;
    QList<unsigned char> detector_flow_list_;

    QByteArray recv_array_;

private:
    QTreeWidget *detector_tree_, *flow_tree_;
    QDateTimeEdit *start_time_editor_, *end_time_editor_;
    QLineEdit *total_flow_lineedit_;

    QPushButton *read_flow_button_, *clear_flow_button_, *ok_button_;
};

#endif // DETECTORFLOWDLG_H
