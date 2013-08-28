#ifndef TIMESECTIONEDITDLG_H
#define TIMESECTIONEDITDLG_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QSpinBox>
#include <QMenu>
#include "timesectionhandler.h"

class TimesectioneditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit TimesectioneditDlg(QWidget *parent = 0);
	~TimesectioneditDlg();
    void Initialize(unsigned char time_section_id, TimesectionHandler* handler);
    
signals:
    void updateTreeSignal(unsigned char time_section_id);
    
public slots:
    void OnSaveButtonClicked();
    void OnResetButtonClicked();
    void OnOkButtonClicked();
    void OnCancelButtonClicked();

    void OnCustomContextMenuRequested(QPoint);
    void OnAddActionClicked();
    void OnEditActionClicked();
    void OnDeleteActionClicked();

    void OnTreeItemDoubleClicked(QTreeWidgetItem*item, int col);
    void OnTreeCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem *);

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();
    void UpdateSettingUI(unsigned char event_id);
    void InitTree();
    void UpdateTree(const QList<TimeSection> &time_section_list);
    void EnableSettingUI(bool enable);
    void InitContextMenu();
    bool SaveData();
    bool SaveRowData();

    QTreeWidgetItem* AddTreeItem();

    unsigned char get_start_hour(const QString& str);
    unsigned char get_start_minute(const QString& str);
    unsigned char get_ctrl_mode(const QString& str);
    int get_ctrl_mode_current_index(const unsigned char mode);
    unsigned char get_brightness();
    unsigned char get_spec_func();
    int index_of_tmp_timesection_list(const TimeSection& section);

private:
    unsigned char curr_section_id_;
	unsigned char curr_event_id_;
    TimesectionHandler* handler_;
    QList<QCheckBox*> spec_func_chk_list_;
	QList<TimeSection> time_section_tmp_list_;

private:
    QMenu* context_menu_;
    QAction* add_action_, *edit_action_, *delete_action_;

    QPushButton* save_button_, *reset_button_;
    QPushButton* ok_button_, *cancel_button_;
    QComboBox* timesection_id_cmb_;
    QTreeWidget* event_table_;
    QComboBox* section_event_id_cmb_;
	QComboBox* ctrl_mode_cmb_, *timing_id_cmb_;
    QSpinBox* start_hour_spinbox_, *start_minute_spinbox_;
    QCheckBox* brightness_ctrl_chk_;
    QCheckBox* func1_chk_, *func2_chk_, *func3_chk_, *func4_chk_,*func5_chk_,*func6_chk_, *func7_chk_;
    QCheckBox* bus_chk_;
};

#endif // TIMESECTIONEDITDLG_H
