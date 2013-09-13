#ifndef EVENTLOGDLG_H
#define EVENTLOGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QTreeWidget>

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
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();

    void InitTree(QTreeWidget *tree, const QStringList &header);

private:

private:
    QPushButton *read_log_button_, *remove_event_button_, *export_log_button_, *export_report_button_;
    QPushButton *ok_button_, *cancel_button_;

    QTreeWidget *event_tree_, *event_detail_tree_;
};

#endif // EVENTLOGDLG_H
