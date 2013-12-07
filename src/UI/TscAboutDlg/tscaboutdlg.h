#ifndef TSCABOUTDLG_H
#define TSCABOUTDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
//#include "TscAboutDlg_global.h"

class ConfigManager;

class /*TSCABOUTDLGSHARED_EXPORT*/ TscAboutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TscAboutDlg(QWidget *parent = 0);
    ~TscAboutDlg();
    void initialize();

signals:

public slots:
    void OnOKButtonClicked();
    void OnAboutButtonClicked();

private:
    void InitPage();
    void InitSignalSlots();

private:
    ConfigManager *cfg_manager_;
    QString ver_prefix_;

private:
    QLabel *software_declare_label_, *all_right_label_, *logo_label_;
    QLabel *version_label_;
    QPushButton *ok_button_, *about_button_;
};

#endif // TSCABOUTDLG_H
