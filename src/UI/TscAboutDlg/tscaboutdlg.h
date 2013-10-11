#ifndef TSCABOUTDLG_H
#define TSCABOUTDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "TscAboutDlg_global.h"

class TSCABOUTDLGSHARED_EXPORT TscAboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TscAboutDlg(QWidget *parent = 0);
    ~TscAboutDlg();

signals:

public slots:
    void OnOKButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();

private:
    QLabel *software_declare_label_, *all_right_label_, *logo_label_;
    QPushButton *ok_button_;
};

#endif // TSCABOUTDLG_H
