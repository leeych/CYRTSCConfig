#ifndef PHASEEDITDLG_H
#define PHASEEDITDLG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include "phasehandler.h"

class PhaseeditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PhaseeditDlg(QWidget *parent = 0);
	~PhaseeditDlg();
    void Initialize(unsigned char phase_id, PhaseHandler* handler);

    enum PhaseErr
    {
        MinLargerThanMax1,
        GreenFlashZero,
        ManGreenClearZero,
        FixGreenZero,
        None
    };

signals:
    void updateTreeItemSignal(unsigned char id);
    
public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
	void OnPhaseTypeSelected(const QString& text);

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void ResetUI();
    void UpdatePhaseType(unsigned char phase_type);

    PhaseErr ValidateUI();
    bool SaveData();

	unsigned int get_channels();

private:
	unsigned char curr_phase_id_;
	unsigned char detector_num_;
    PhaseHandler* handler_;
    QList<QCheckBox*> channel_list_;

private:
    QComboBox* phase_id_cmb_;
    QCheckBox* enable_phase_chk_;
    QSpinBox* man_green_time_spinbox_, *man_clear_time_spinbox_, *min_green_time_spinbox_;
    QSpinBox* delay_green_time_spinbox_, *max1_green_time_spinbox_, *max2_green_time_spinbox_;
    QSpinBox* fix_green_time_spinbox_, *green_flash_time_spinbox_;
    QComboBox* phase_mode_cmb_;

    QCheckBox* man_corss_street_chk_, *to_be_determined_chk_;
    QCheckBox* man_motor_chk_, *together_chk_, *demote_chk_;
    QSpinBox* detector_num_spinbox_;

    QPushButton* ok_button_, *cancel_button_;
};

#endif // PHASEEDITDLG_H
