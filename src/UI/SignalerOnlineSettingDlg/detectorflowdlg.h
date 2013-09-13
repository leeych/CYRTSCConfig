#ifndef DETECTORFLOWDLG_H
#define DETECTORFLOWDLG_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>

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
    void onClearFowButtonClicked();
    void OnOkButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void InitTree(QTreeWidget *tree, const QStringList &header);

private:
    QTreeWidget *detector_tree_, *flow_tree_;
    QDateTimeEdit *start_time_editor_, *end_time_editor_;
    QLineEdit *total_flow_lineedit_;

    QPushButton *read_flow_button_, *clear_flow_button_, *ok_button_;
};

#endif // DETECTORFLOWDLG_H
