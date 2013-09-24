#ifndef EVENTLOGDLG_H
#define EVENTLOGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>
#include <QLabel>

#include "tsc.h"

class EventLogHandler;

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
    void OnEventTypeTreeItemDoubleClicked(QTreeWidgetItem *, int);

    // cmd return caller
    void OnCmdReadEventLog(QByteArray &array);
    void OnCmdDeleteEventLog(QByteArray &array);
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void UpdateEventTypeTree();
    void UpdateEventDetailTree();

    void InitTree(QTreeWidget *tree, const QStringList &header);

private:
    void ParseEventLogArray(QByteArray &byte_arr);

private:
    EventLogHandler *handler_;
    QByteArray event_log_array_;
    QString file_name_;
    QString ip_;

private:
    QPushButton *read_log_button_, *remove_event_button_, *export_log_button_, *export_report_button_;
    QPushButton *ok_button_, *cancel_button_;
    QLabel *tip_label_;

    QTreeWidget *event_tree_, *event_detail_tree_;
};

#endif // EVENTLOGDLG_H
