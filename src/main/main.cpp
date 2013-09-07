#include "mainwindow.h"
#include "mutility.h"
#include "login.h"
#include "macrostring.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDebug>
#include <QSplashScreen>
#include <QTime>
#include <QCoreApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *tc=QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForCStrings(tc);

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

    MainWindow w;
    QSplashScreen *splash = new QSplashScreen(QPixmap(dir + "splash.png"));
    splash->show();
    splash->showMessage(STRING_MAIN_INITIALIZE, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QTime t;
    t.start();
    while (t.elapsed() < 2000)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    splash->finish(&w);
    delete splash;

    dir += "icon.png";
    w.setWindowIcon(QIcon(dir));
    w.show();

	QDesktopWidget *desktop = QApplication::desktop();
	w.move((desktop->width() - w.width())/2,(desktop->height() - w.height())/2);
    
    return a.exec();
}
