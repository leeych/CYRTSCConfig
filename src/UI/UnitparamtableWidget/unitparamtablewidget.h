#ifndef UNITPARAMTABLEWIDGET_H
#define UNITPARAMTABLEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include "unitparamtablewidget_global.h"
#include "unitparamhandler.h"

class UNITPARAMTABLEWIDGETSHARED_EXPORT UnitparamtableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnitparamtableWidget(const QString& name, QWidget* parent = 0);
	~UnitparamtableWidget();
    const QString& widget_name();
	void PageFontSetting(const QFont &font);

    void UpdateUI();

signals:
    void unitparamEditedSignal();

public slots:
    bool OnOkButtonClicked();
    void OnCancelButtonClicked();
    void OnUpdateDataSlot();
    void OnInitDatabase(void *ptr);

private:
    void InitPage();
    void InitSignalSlots();

    bool SaveData();
    bool ValidateUI();

private:
    UnitparamHandler *handler_;
    QString widget_name_;
	QList<QWidget *> font_setting_list_;

private:
    QLineEdit* signal_ip_lineedit_;
    QLineEdit* server_ip_lineedit_;
    QLineEdit* server_port_lineedit_;
    QSpinBox* flash_time_spinbox_;
    QSpinBox* all_red_time_spinbox_;
    QComboBox* signal_status_cmb_;

    QRadioButton* man_signal_rbt_;
    QRadioButton* multi_period_timer_signal_rbt_;
    QRadioButton* induction_signal_rbt_;
    QRadioButton* other_signal_rbt_;
    QComboBox* state1_cmb_, *state2_cmb_, *state3_cmb_, *state4_cmb_;

    QPushButton* ok_button_;
    QPushButton* cancel_button_;
};

#endif // UNITPARAMTABLEWIDGET_H
