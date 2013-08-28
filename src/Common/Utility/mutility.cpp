#include <QtCore>
#include "mutility.h"


void MUtility::getMainDir(QString &strDir)
{
    strDir = QCoreApplication::applicationDirPath();
    strDir += "/";
}

void MUtility::getImageDir(QString &strImageDir)
{
    strImageDir = ":";
    strImageDir += "/images/";
}

void MUtility::getCssDir(QString &strCssDir)
{
    strCssDir = QCoreApplication::applicationDirPath();
    strCssDir += "/css/";
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
