#include "login.h"
#include "macrostring.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

Login::Login(QWidget *parent)
    : QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

Login::~Login()
{
}

void Login::Initialize()
{
    UpdateUI();
    exec();
}

void Login::OnLoginButtonClicked()
{
}

void Login::InitPage()
{
    bg_label_ = new QLabel;
    bg_label_->setPixmap(QPixmap("login_bg.png"));
    username_label_ = new QLabel(STRING_UI_LOGIN_USERNAME);
    passwd_label_ = new QLabel(STRING_UI_LOGIN_PASSWD);
    username_lineedit_ = new QLineEdit;
    passwd_lineedit_ = new QLineEdit;

    login_button_ = new QPushButton(STRING_UI_LOGIN_LOGIN);
    QFrame* bottom_frame = new QFrame;
    QString dir;
    QString frame_qss = "border-image:url(" + dir + "/bottom_img.png);";
    bottom_frame->setStyleSheet(frame_qss);
    username_lineedit_->setAttribute(Qt::WA_TranslucentBackground);
    passwd_lineedit_->setAttribute(Qt::WA_TranslucentBackground);
    passwd_lineedit_->setEchoMode(QLineEdit::Password);

    QHBoxLayout* bottom_hlayout = new QHBoxLayout;
    bottom_hlayout->addWidget(username_label_);
    bottom_hlayout->addWidget(username_lineedit_);
    bottom_hlayout->addStretch(1);
    bottom_hlayout->addWidget(passwd_label_);
    bottom_hlayout->addWidget(passwd_lineedit_);
    bottom_hlayout->addStretch(1);
    bottom_hlayout->addWidget(login_button_);
    bottom_frame->setLayout(bottom_hlayout);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(bg_label_);
    vlayout->addWidget(bottom_frame);
    setLayout(vlayout);
}

void Login::InitSignalSlots()
{
    connect(login_button_, SIGNAL(clicked()), this, SLOT(OnLoginButtonClicked()));
}

void Login::UpdateUI()
{
}
