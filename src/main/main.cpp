#include "mainwindow.h"
#include "mutility.h"
#include "login.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *tc=QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForCStrings(tc);

    QString dir;
    MUtility::getMainDir(dir);
    dir += "CYTSC.qm";
    QTranslator translator;
    translator.load(dir);
    a.installTranslator(&translator);

//    Login log_dlg;
//    log_dlg.Initialize();

    MainWindow w;
    w.show();
    
    return a.exec();
}
