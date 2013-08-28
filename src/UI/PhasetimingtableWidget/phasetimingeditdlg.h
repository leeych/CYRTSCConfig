#ifndef PHASETIMINGEDITDLG_H
#define PHASETIMINGEDITDLG_H

#include <QDialog>
#include <QComboBox>
#include <QTreeWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenu>
#include <QAction>
#include "phasetiminghandler.h"

class PhasetimingeditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PhasetimingeditDlg(QWidget *parent = 0);
	~PhasetimingeditDlg();
    void Initialize(unsigned char phase_timing_id, PhasetimingHandler* handler);

signals:
    void updatePhasetimingSignal(unsigned char phase_timing_id);
    
public slots:
    void OnSaveButtonClicked();
    void OnResetButtonClicked();
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();
    void OnTreeItemDoubleClicked(QTreeWidgetItem*,int);
    void OnTreeCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
    void OnCustomContextMenuRequested(QPoint);
    
private:
    void InitPage();
    void InitSignalSlots();
    void InitTree();
    void UpdateTree(const QList<PhaseTiming>& plan_list);
    void InitContextMenu();

    QTreeWidgetItem *AddTreeTopLevelItem();

    void UpdateUI();
    void UpdateSettingUI(unsigned char stage_id);
    void EnableSettingUI(bool enable);
    bool SaveData();
    bool SaveRowData();

    unsigned int get_phase_ids();
    unsigned char get_spec_func();
    int index_of_phasetiming_tmp_list(const PhaseTiming& plan);

private:
    unsigned char curr_timing_id_;
	unsigned char curr_stage_id_;
    PhasetimingHandler* handler_;
    QList<PhaseTiming> phase_timing_tmp_list_;

    QList<QCheckBox*> release_phase_id_list_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_;

    QTreeWidget* tree_widget_;
    QComboBox* phase_timing_id_cmb_, *phase_id_cmb_, *release_phase_id_cmb_;
    QCheckBox* induction_id_chk_;
    QSpinBox* green_time_spinbox_, *yellow_time_spinbox_, *red_time_spinbox_;
    QSpinBox* delay_time_spinbox_;
    QPushButton* save_button_, *reset_button_;
    QPushButton* ok_button_, *cancel_button_;
};

#endif // PHASETIMINGEDITDLG_H
