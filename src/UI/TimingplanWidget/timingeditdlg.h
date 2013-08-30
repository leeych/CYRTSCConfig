#ifndef TIMINGEDITDLG_H
#define TIMINGEDITDLG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include "timinghandler.h"

class TimingeditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit TimingeditDlg(QWidget *parent = 0);
    ~TimingeditDlg();
    void Initialize(unsigned char id, TimingHandler* handler);
    
signals:
    void updateTableRow(unsigned char id);
    
public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();

    void OnStageTimingIdSelected(QString);

private:
    void InitPage();
    void InitSignalSlots();

    void ResetUI();
    void UpdateUI();
    bool SaveData();

private:
    TimingHandler* handler_;
    unsigned char curr_timing_id_;

private:
    QComboBox* timing_id_cmb_, *phase_timing_id_cmb_, *coor_phase_cmb_;
    QSpinBox* cycle_spinbox_, *phase_spinbox_;

    QPushButton* ok_button_, *cancel_button_;
};

#endif // TIMINGEDITDLG_H
