#ifndef DETECTOREDITDLG_H
#define DETECTOREDITDLG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMap>
#include "detectorhandler.h"

class DetectoreditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DetectoreditDlg(QWidget *parent = 0);
	~DetectoreditDlg();
    void Initialize(unsigned char detector_id, DetectorHandler* handler);

signals:
    void updateTableRowSignal(unsigned char detector_id);

public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
	void OnMainLaneChecked(bool status);

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void ResetUI();

    bool SaveData();

	void update_detector_phase_ids(unsigned int phase_ids);
	unsigned int get_detector_phase_ids();
    void update_detector_type(unsigned char detector_type);
    unsigned char get_detector_type();
    void update_detector_direction(unsigned char detector_direction);
    unsigned char get_detector_direction();
	void update_detector_spec_func(unsigned char spec_func);
	unsigned char get_detector_spec_func();

private:
    DetectorHandler* handler_;
    unsigned char curr_detector_id_;
    QList<QCheckBox*> phase_chk_list_;
    QMap<int, int> detector_type_map_;
    QMap<int, int> detector_direction_map_;

private:
    QComboBox* detector_id_cmb_;
    QComboBox* detector_mode_cmb_, *detector_direction_cmb_;
    QSpinBox* eff_time_spinbox_, *failure_time_spinbox_;
    QCheckBox* model_chk_, *main_lane_chk_;
    QSpinBox* flow_spinbox_, *share_spinbox_;
    QPushButton* ok_button_, *cancel_button_;
};

#endif // DETECTOREDITDLG_H
