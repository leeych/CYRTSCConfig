#ifndef SIGNALERTIMEIPSETTINGDLG_H
#define SIGNALERTIMEIPSETTINGDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

//class QPushButton;
//class QGroupBox;
//class QLineEdit;
//class QLabel;

class TimeIPDlg : public QDialog
{
    Q_OBJECT
public:
    explicit TimeIPDlg(QWidget *parent = 0);
    ~TimeIPDlg();
    void Initialize();
    
signals:
    
public slots:
    void OnReadSystimeButtonClicked();
    void OnSyncTimeButtonClicked();
    void OnRefreshButtonClicked();
    void OnWriteIPButtonClicked();

protected:
    void closeEvent(QCloseEvent *);
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();

private:
    QString dialog_name_;

private:
    QPushButton *read_sys_time_button_, *sync_time_button_;
    QPushButton *refresh_button_, *write_ip_button_;
    QLabel *sys_time_text_label_;
    QLineEdit *ip_lineedit_, *mask_lineedit_, *gateway_lineedit_;
};

#endif // SIGNALERTIMEIPSETTINGDLG_H
