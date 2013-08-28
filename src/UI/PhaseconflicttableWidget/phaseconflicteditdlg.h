#ifndef PHASECONFLICTEDITDLG_H
#define PHASECONFLICTEDITDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include "phaseconflicthandler.h"

class PhaseconflicteditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PhaseconflicteditDlg(QWidget *parent = 0);
	~PhaseconflicteditDlg();
    void Initialize(unsigned char phase_id, PhaseconflictHandler* handler);
    
signals:
    void updateTableRowSignal(unsigned char phase_id);
    
public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
    
private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void ResetUI();

    bool SaveData();

private:
    PhaseconflictHandler* handler_;
    unsigned char current_phase_id_;
    QList<QCheckBox*> conflict_list_;

private:
    QLabel* curr_phase_label_;
    QPushButton* ok_button_;
    QPushButton* cancel_button_;
};

#endif // PHASECONFLICTEDITDLG_H
