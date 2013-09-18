#ifndef EVENTLOGDLG_H
#define EVENTLOGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>

#include "tsc.h"

class EventLogDlg : public QDialog
{
    Q_OBJECT
public:
    explicit EventLogDlg(QWidget *parent = 0);
    ~EventLogDlg();
    void Initialize();
    
signals:
    
public slots:
    void OnReadLogButtonClicked();
    void OnRemoveEventButtonClicked();
    void OnExportLogButtonClicked();
    void OnExportReportButtonClicked();

    // cmd return caller
    void OnCmdReadEventLog(void *content);
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();

    void InitTree(QTreeWidget *tree, const QStringList &header);

private:
    void ParseEventLogArray(QByteArray &byte_arr);

private:
    QByteArray event_log_array_;

    class EventLog
    {
    public:
        EventLog()
        {
            length = 0;
            memset(header, 0x00, sizeof(header));
            memset(&event_type_info, 0x00, sizeof(event_type_info));
            memset(&log_info, 0x00, sizeof(log_info));
        }
        EventLog(const EventLog &rhs)
        {
            this->length = rhs.length;
            memcpy(&event_type_info, &rhs.event_type_info, sizeof(rhs.event_type_info));
            memcpy(&log_info, &rhs.log_info, sizeof(rhs.log_info));
        }

        unsigned int length;
        char header[18];
        EventClass_t event_type_info;
        EventLog_t log_info;
    };

private:
    QPushButton *read_log_button_, *remove_event_button_, *export_log_button_, *export_report_button_;
    QPushButton *ok_button_, *cancel_button_;

    QTreeWidget *event_tree_, *event_detail_tree_;
};

#endif // EVENTLOGDLG_H
