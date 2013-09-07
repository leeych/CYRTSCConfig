#include "login.h"
#include "macrostring.h"
#include "mutility.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

Login::Login(QWidget *parent)
    : QDialog(parent)
{
    passwd_str_ = "123456";
    InitPage();
    InitSignalSlots();
}

Login::~Login()
{
}

int Login::Initialize()
{
    UpdateUI();
    return exec();
}

void Login::OnLoginButtonClicked()
{
    if (passwd_lineedit_->text().trimmed().isEmpty())
    {
        err_label_->setText(STRING_UI_LOGIN_PASSWD_EMPTY);
        return;
    }
    else if (passwd_str_ != passwd_lineedit_->text().trimmed())
    {
        err_label_->setText(STRING_UI_LOGIN_PASSWD_ERR);
        return;
    }
    else
    {
        err_label_->setText("");
        accept();
        return;
    }
}

void Login::OnPasswdEditedSlot(QString)
{
    err_label_->setText("");
}

void Login::InitPage()
{
    QString dir;
    MUtility::getImageDir(dir);
    bg_label_ = new QLabel(this);
    bg_label_->setPixmap(QPixmap(dir + "login_bg.png"));
    bg_label_->setGeometry(0, 0, 539, 255);
    QFrame *bottom_frame = new QFrame(this);
    QString tmp = "border-image:url(" + dir + "/bottom_img.png);";
    bottom_frame->setStyleSheet(tmp);
    bottom_frame->setGeometry(0, 255, 539, 35);
    username_label_ = new QLabel(STRING_UI_LOGIN_USERNAME, bottom_frame);
    username_label_->setGeometry(4, 7, 60, 22);
    username_label_->setAttribute(Qt::WA_TranslucentBackground);
    username_lineedit_ = new QLineEdit(bottom_frame);
    username_lineedit_->setGeometry(60, 7, 100, 22);
    tmp = "border-image:url(" + dir + "/edit_img.png);";
    username_lineedit_->setStyleSheet(tmp);
    username_lineedit_->setText(QString(""));
    passwd_label_ = new QLabel(STRING_UI_LOGIN_PASSWD, bottom_frame);
    passwd_label_->setGeometry(162, 7, 40, 22);
    passwd_label_->setAttribute(Qt::WA_TranslucentBackground);
    passwd_lineedit_ = new QLineEdit(bottom_frame);
    passwd_lineedit_->setGeometry(203, 7, 100, 22);
    passwd_lineedit_->setStyleSheet(tmp);
    passwd_lineedit_->setEchoMode(QLineEdit::Password);
    passwd_lineedit_->setText(QString(""));
    err_label_ = new QLabel(bottom_frame);
    err_label_->setGeometry(310, 10, 120, 15);
    err_label_->setAttribute(Qt::WA_TranslucentBackground);
    err_label_->setStyleSheet("color:red;font:12px");
    login_button_ = new QPushButton(bottom_frame);
    QString login_button_qss = "QPushButton:!hover:!pressed{border-image:url(" + dir + "/enter_cur.png)}"
                           "QPushButton:hover{border-image:url(" + dir + "/enter_hover.png)}"
                           "QPushButton:pressed{border-image:url(" + dir + "/touch_enter.png)}";
    login_button_->setStyleSheet(login_button_qss);
    login_button_->setGeometry(440, 6, 88, 24);
    setFixedSize(539, 290);
    setWindowTitle(STRING_UI_LOGIN_TITLE);
	setWindowFlags(this->windowFlags() & ~Qt::WindowMinMaxButtonsHint);
	//setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
}

void Login::InitSignalSlots()
{
    connect(login_button_, SIGNAL(clicked()), this, SLOT(OnLoginButtonClicked()));
    connect(passwd_lineedit_, SIGNAL(textEdited(QString)), this, SLOT(OnPasswdEditedSlot(QString)));
}

void Login::UpdateUI()
{
    username_lineedit_->setText("ChaoYuan");
    username_lineedit_->setEnabled(false);
}
