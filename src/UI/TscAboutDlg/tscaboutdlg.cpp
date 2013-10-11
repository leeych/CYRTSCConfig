#include "tscaboutdlg.h"
#include "macrostring.h"
#include "mutility.h"
#include <QtGui>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>


TscAboutDlg::TscAboutDlg(QWidget *parent) :
    QDialog(parent)
{
    InitPage();
    InitSignalSlots();
}

TscAboutDlg::~TscAboutDlg()
{
}

void TscAboutDlg::OnOKButtonClicked()
{
    accept();
}

void TscAboutDlg::InitPage()
{
    setWindowTitle(STRING_UI_ABOUT);
    software_declare_label_ = new QLabel("<font size=\"15\" color=\"Green\">" + STRING_UI_ABOUT_SOFTWARE +
                                         "</font><br /><font size=\"5\">" + STRING_UI_ABOUT_DECLARE + "</font>");
    software_declare_label_->setAlignment(Qt::AlignCenter);
    all_right_label_ = new QLabel(STRING_UI_ABOUT_COPYRIGHT);
    QString dir;
    MUtility::getImageDir(dir);
    logo_label_ = new QLabel;
    logo_label_->setPixmap(QPixmap(dir + "/about.png"));
    logo_label_->setMinimumSize(200, 160);

    QVBoxLayout *left_vlayout = new QVBoxLayout;
    left_vlayout->addWidget(software_declare_label_);
    left_vlayout->addWidget(all_right_label_);

    QHBoxLayout *declare_hlayout = new QHBoxLayout;
    declare_hlayout->addLayout(left_vlayout);
    declare_hlayout->addStretch(1);
//    left_vlayout->addSpacing(5);
    declare_hlayout->addWidget(logo_label_);

    ok_button_ = new QPushButton(STRING_OK);
    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(3);
    button_hlayout->addWidget(ok_button_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(declare_hlayout);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
    setStyleSheet("QWidget{background-color:#F5F5F5;}"
                  "QPushButton {border: 2px solid #000000; border-radius: 3px;"
                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
                  "min-width: 60px; min-height: 20px;}"
                  "QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}"
                  "QPushButton:flat { border: none;}"
                  "QPushButton:default { border-color: navy;}"
                  );

    setFixedSize(530,290);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
}

void TscAboutDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOKButtonClicked()));
}
