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
    static void getCurIconDir(QString& strCurIconDir);
    static void getCurseIconDir(QString& strCurseIconDir);
    static void getClickIconDir(QString& strClickIconDir);
    static void GetPolarIconDir(QString& strDir);
    static QByteArray fetchQrc(const QString &fileName);
    static void trimFloatString(QString& strFloat);
};

#endif // MUTILITY_H
