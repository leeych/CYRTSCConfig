#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
//#include "tsclogin_global.h"

class /*TSCLOGINSHARED_EXPORT*/ Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget* parent = 0);
    ~Login();
    int Initialize();

signals:

public slots:
    void OnLoginButtonClicked();
    void OnPasswdEditedSlot(QString);

private:
    void InitPage();
    void InitSignalSlots();

    void UpdateUI();

private:
    QString passwd_str_;

private:
    QLabel* bg_label_;
    QLabel* username_label_, *passwd_label_, *err_label_;
    QLineEdit* username_lineedit_, *passwd_lineedit_;
    QPushButton* login_button_;
};

#endif // LOGIN_H
