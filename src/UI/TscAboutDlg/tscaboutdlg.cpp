#include "tscaboutdlg.h"
#include "macrostring.h"
#include "mutility.h"
#include "configmanager.h"

#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>

#define WIDGET_CENTER
#include "utilmacros.h"


TscAboutDlg::TscAboutDlg(QWidget *parent) :
    QDialog(parent)
{
    cfg_manager_ = ConfigManager::getInstance();
    InitPage();
    InitSignalSlots();
}

TscAboutDlg::~TscAboutDlg()
{
}

void TscAboutDlg::initialize()
{
    QString ini_path;
    MUtility::getMainDir(ini_path);
    ini_path += "app.ini";
    QString ver = cfg_manager_->getAppSettingInfo(ini_path).soft_version_;
    version_label_->setText(ver_prefix_+ver);
    exec();
}

void TscAboutDlg::OnOKButtonClicked()
{
    accept();
}

void TscAboutDlg::OnAboutButtonClicked()
{
    QString file_path = MUtility::helpDir().toUtf8() + "help.pdf";
    if (!QFile::exists(file_path))
    {
        QMessageBox::warning(this, STRING_TIP, STRING_FILE_NOT_EXISTS, STRING_OK);
        return;
    }
    bool state = QDesktopServices::openUrl(QUrl::fromLocalFile(file_path));
    if (!state)
    {
        QMessageBox::warning(this, STRING_TIP, STRING_FILE_OPEN + STRING_FAILED, STRING_OK);
        return;
    }
}

void TscAboutDlg::InitPage()
{
    setWindowTitle(STRING_UI_ABOUT);
    software_declare_label_ = new QLabel("<font size=\"8\" color=\"Green\">" + STRING_UI_ABOUT_SOFTWARE +
                                         "</font><br /><br /><font size=\"3\">" + STRING_UI_ABOUT_DECLARE + "</font>");
    software_declare_label_->setAlignment(Qt::AlignCenter);
    all_right_label_ = new QLabel(STRING_UI_ABOUT_COPYRIGHT);
    version_label_ = new QLabel(STRING_UI_ABOUT_VERSION+" :v");
    ver_prefix_ = version_label_->text().trimmed();
    QString dir;
    MUtility::getImageDir(dir);
    logo_label_ = new QLabel;
    logo_label_->setPixmap(QPixmap(dir + "/about.png"));
    logo_label_->setMinimumSize(200, 160);

    QVBoxLayout *left_vlayout = new QVBoxLayout;
    left_vlayout->addWidget(software_declare_label_);
    left_vlayout->addWidget(version_label_,0, Qt::AlignCenter);
    left_vlayout->addWidget(all_right_label_);

    QHBoxLayout *declare_hlayout = new QHBoxLayout;
    declare_hlayout->addLayout(left_vlayout);
    declare_hlayout->addStretch(1);
    declare_hlayout->addWidget(logo_label_);

    ok_button_ = new QPushButton(STRING_OK);
    about_button_ = new QPushButton(STRING_DOCUMENT);
    QHBoxLayout *button_hlayout = new QHBoxLayout;
    button_hlayout->addStretch(1);
    button_hlayout->addWidget(ok_button_);
    button_hlayout->addWidget(about_button_);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(declare_hlayout);
    vlayout->addLayout(button_hlayout);
    setLayout(vlayout);
//    setStyleSheet("QWidget{background-color:#F5F5F5;}"
//                  "QPushButton {border: 1px solid #DCDCDC; border-radius: 3px;"
//                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);"
//                  "min-width: 60px; min-height: 20px;}"
//                  "QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}"
//                  "QPushButton:flat { border: none;}"
//                  "QPushButton:default { border-color: navy;}"
//                  );

    setFixedSize(530,270);
    WIDGET_CENTRALIZE(this)
}

void TscAboutDlg::InitSignalSlots()
{
    connect(ok_button_, SIGNAL(clicked()), this, SLOT(OnOKButtonClicked()));
    connect(about_button_, SIGNAL(clicked()), this, SLOT(OnAboutButtonClicked()));
}
