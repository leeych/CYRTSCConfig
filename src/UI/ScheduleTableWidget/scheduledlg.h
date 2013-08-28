#ifndef SCHEDULEDLG_H
#define SCHEDULEDLG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include "schedulehandler.h"

class ScheduleDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ScheduleDlg(QWidget *parent = 0);
	~ScheduleDlg();
    void Initialize(unsigned char id, ScheduleHandler *handler);
    
signals:
    void updateTableSignal(unsigned char sche_id);
    
public slots:
    void OnOkButtonClicked();
    void OnCancelButtonClicked();

private slots:
    void OnMonthAllRadioButtonChecked(bool);
    void OnMonthNoneRadioButtonChecked(bool);

    void OnWeekdayAllRadioButtonChecked(bool);
    void OnWeekdayNoneRadioButtonChecked(bool);

    void OnDayAllRadioButtonChecked(bool);
    void OnDayNoneRadioButtonChecked(bool);

private:
    void InitPage();
    void InitSignalSlots();

    void UpdateUI(unsigned char sched_id);
    void ResetUI();
    bool SaveData();
    void SaveMonth();
    void SaveWeek();
    void SaveDay();

private:
    unsigned char id_;
    unsigned short month_;
    unsigned char week_;
    unsigned int day_;

    ScheduleHandler* handler_;
    QList<QCheckBox *> month_list_;
    QList<QCheckBox *> weekday_list_;
    QList<QCheckBox *> day_list_;

private:
    QComboBox* available_schedule_id_cmb_;
    QCheckBox* mon_chk_, *thue_chk_, *wed_chk_, *thur_chk_, *fri_chk_, *sat_chk_, *sun_chk_;

	QRadioButton* month_all_rbt_, *month_none_rbt_, *month_hidden_rbt_;
    QRadioButton* weekday_all_rbt_, *weekday_none_rbt_, *weekday_hidden_rbt_;
    QRadioButton* day_all_rbt_, *day_none_rbt_, *day_hidden_rbt_;

    QComboBox* timesection_id_cmb_;
    QPushButton* ok_button_;
    QPushButton* cancel_button_;
};

#endif // SCHEDULEDLG_H
