#ifndef EVENTLOGDLG_H
#define EVENTLOGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>
#include <QLabel>
#include <QDateTimeEdit>

#include "tsc.h"
#include "eventloghandler.h"

//class EventLogHandler;

class EventLogDlg : public QDialog
{
    Q_OBJECT
public:
    explicit EventLogDlg(QWidget *parent = 0);
    ~EventLogDlg();
    void Initialize(const QString &ip, EventLogHandler *handler);

signals:
    
public slots:
    void OnReadLogButtonClicked();
    void OnDeleteEventButtonClicked();
    void OnExportLogButtonClicked();
    void OnExportReportButtonClicked();
    void OnEventTypeTreeItemSelected(QTreeWidgetItem *, int);
    void OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem *, int);
    void OnOKButtonClicked();

    // cmd return caller
    void OnCmdReadEventLog(QByteArray &array);
    void OnCmdClearEventLog(QByteArray &array);
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void UpdateEventTypeTree();
    void UpdateEventDetailTree();
    void UpdateEventDetailTree(const QList<LogParam> &log_param_list);
    void InitTree(QTreeWidget *tree, const QStringList &header);

    void setDateTimeEdit(QDateTimeEdit *edit);

private:
    void ParseEventLogArray(QByteArray &byte_arr);

private:
    unsigned char curr_event_type_id_;
    EventLogHandler *handler_;
    QByteArray event_log_array_;
    QString file_name_;
    QString ip_;

private:
    QPushButton *read_log_button_, *remove_event_button_, *export_log_button_, *export_report_button_;
    QPushButton *ok_button_, *cancel_button_;
    QLabel *tip_label_;
    QLabel *count_tip_label_;

    QTreeWidget *event_tree_, *event_detail_tree_;

    QDateTimeEdit *start_datetime_, *end_datetime_;
};

#endif // EVENTLOGDLG_H
