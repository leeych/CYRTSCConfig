#ifndef SIGNALERBASICEDITDLG_H
#define SIGNALERBASICEDITDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

class SignalerbasiceditDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SignalerbasiceditDlg(QWidget *parent = 0);
	~SignalerbasiceditDlg();
    void Initialize();
    
signals:
    
public slots:

private:
    void InitPage();
    void InitSignalSlots();
    void UpdateUI();

private:
    QLabel* id_label_;
    QLineEdit* name_lineedit_;
    QLineEdit* ip_lineedit_;
    QLineEdit* port_lineedit_;
    QLineEdit* pos_lineedit_;
    QLineEdit* branch_lineedit_;
    QLineEdit* version_lineedit_;
    QLineEdit* mode_lineedit_;
    QLineEdit* markup_lineedit_;
    QPushButton* ok_button_, *cancel_button_;
};

#endif // SIGNALERBASICEDITDLG_H
