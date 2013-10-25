#ifndef MUTILITY_H
#define MUTILITY_H

#include <QString>

class MUtility
{
public:
    static void getMainDir(QString& strDir);
    static void getLanguageDir(QString &strDir);
    static void getImageDir(QString& strImageDir);
    static void getStyleSheetDir(QString& strCssDir);
    static void getUserDir(QString &userDir);
    static void getLogDir(QString &strDir);
    static void getReportDir(QString &strDir);
    static void getTempDir(QString &strDir);
    static void getMonitorDir(QString &strDir);
    static void getConfigDir(QString &strDir);
    static void getDataDir(QString &strDir);
    static void getCurIconDir(QString& strCurIconDir);
    static void getCurseIconDir(QString& strCurseIconDir);
    static void getClickIconDir(QString& strClickIconDir);
    static void GetPolarIconDir(QString& strDir);
    static QByteArray fetchQrc(const QString &fileName);
    static void trimFloatString(QString& strFloat);

    static bool checkIPString(const QString &ip);
    static QString trimmedAll(const QString &ip);

    static QString secondsToDateTime(unsigned long seconds);
    static QString phaseBitsDesc(unsigned int phase);
    static QString bitsDesc(unsigned int phase_ids);
    static QString helpDir();
};

#endif // MUTILITY_H
