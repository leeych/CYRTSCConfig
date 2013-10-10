#include <QtCore>
#include <QRegExp>
#include "mutility.h"


void MUtility::getMainDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath();
    strDir += "/";
}

void MUtility::getLanguageDir(QString &strDir)
{
    strDir = ":";
    strDir += "/language/";
}

void MUtility::getImageDir(QString &strImageDir)
{
	strImageDir = ":";//QCoreApplication::applicationDirPath();
    strImageDir += "/images/";
}

void MUtility::getStyleSheetDir(QString &strCssDir)
{
    strCssDir = ":";/*QCoreApplication::applicationDirPath();*/
    strCssDir += "/qstylesheet/";
}

void MUtility::getUserDir(QString &userDir)
{
    userDir = QCoreApplication::applicationDirPath();
    userDir += "/user/";
}

void MUtility::getLogDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/user/log/";
}

void MUtility::getReportDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/user/report/";
}

void MUtility::getTempDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/user/temp/";
}

void MUtility::getMonitorDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/user/monitor/";
}

void MUtility::getConfigDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/user/config/";
}

void MUtility::getDataDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath() + "/data/";
}

void MUtility::getCurIconDir(QString &strCurIconDir)
{
    strCurIconDir = QCoreApplication::applicationDirPath();
    strCurIconDir += "/images/toolbaricon/currenticon/";
}

void MUtility::getCurseIconDir(QString &strCurseIconDir)
{
    strCurseIconDir = QCoreApplication::applicationDirPath();
    strCurseIconDir += "/images/toolbaricon/curseicon/";
}

void MUtility::getClickIconDir(QString &strClickIconDir)
{
    strClickIconDir = QCoreApplication::applicationDirPath();
    strClickIconDir += "/images/toolbaricon/clickedicon/";
}

void MUtility::GetPolarIconDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath();
    strDir += "/images/polar_toolbar_icon/";
}

QByteArray MUtility::fetchQrc(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return NULL;
    }
    return file.readAll();
}

void MUtility::trimFloatString(QString &strFloat)
{
    if(strFloat.isEmpty())
    {
        return;
    }

    if(!strFloat.contains('.'))
    {
        return;
    }

    for(int i = strFloat.count() - 1;i >= 0;--i)
    {
        if(strFloat[i] != '0')
        {
            if(strFloat[i] == '.')
            {
                strFloat = strFloat.mid(0,i);
            }
            else
            {
                strFloat = strFloat.mid(0,i+1);
            }

            return;
        }
    }
}

bool MUtility::checkIPString(const QString &ip)
{
    QRegExp reg("^([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])$");
    if (!reg.exactMatch(ip))
    {
        return false;
    }
    return true;
}

QString MUtility::trimmedAll(const QString &ip)
{
    int index = ip.indexOf(" ");
    if (index < 0)
    {
        return ip;
    }
    return trimmedAll(ip);
}

QString MUtility::secondsToDateTime(unsigned long seconds)
{
    if (seconds >= 60*60*8)     // east 8 time-zoon
    {
        seconds -= 60*60*8;
    }
    QDateTime datetime = QDateTime::fromTime_t(sec);
    return datetime.toString("yyyy-MM-dd hh:mm:ss ddd");
}
