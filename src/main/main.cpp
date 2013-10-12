#include "mainwindow.h"
#include "mutility.h"
#include "login.h"
#include "macrostring.h"
#include "systeminiter.h"

#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QSplashScreen>
#include <QTime>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *tc=QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForCStrings(tc);

    SystemIniter initializer;
    SystemIniter::DirError err = initializer.InitUserDir();
    if (err == SystemIniter::None)
    {
        err = initializer.InitDataDir();
        if (err != SystemIniter::None)
        {
            QMessageBox::information(NULL, STRING_TIP, STRING_MAIN_INIT_USER_DIR + STRING_FAILED, STRING_OK);
            return -1;
        }
    }
    else
    {
        QMessageBox::information(NULL, STRING_TIP, STRING_MAIN_INIT_DATA_DIR + STRING_FAILED, STRING_OK);
        return -1;
    }

    QString dir;
    MUtility::getLanguageDir(dir);
    dir += "CYTSC.qm";
    QTranslator translator;
    translator.load(dir);
    a.installTranslator(&translator);

	MUtility::getImageDir(dir);
    Login *log_dlg = new Login;
	log_dlg->setWindowIcon(QIcon(dir + "login_icon.png"));
    int ret = log_dlg->Initialize();
    if (ret != 1)
    {
        delete log_dlg;
        return -1;
    }

    MainWindow window;
    QSplashScreen *splash = new QSplashScreen(QPixmap(dir + "splash.png"));
    splash->show();
    splash->showMessage(STRING_MAIN_INITIALIZE, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QTime t;
    t.start();
    while (t.elapsed() < 2000)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    splash->finish(&window);
    delete splash;
    splash = NULL;

    dir += "icon.png";
    window.setWindowIcon(QIcon(dir));

    QDesktopWidget *desktop = QApplication::desktop();
    window.move((desktop->width() - window.width())/2,(desktop->height() - window.height())/2);
    window.show();
    
    return a.exec();
}
