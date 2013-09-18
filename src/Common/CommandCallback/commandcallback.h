#ifndef COMMANDCALLBACK_H
#define COMMANDCALLBACK_H

#include <QObject>
#include "commandcallback_global.h"

class COMMANDCALLBACKSHARED_EXPORT CommandCallback : public QObject
{
    Q_OBJECT
public:
    static CommandCallback *GetInstance();
    void ReadTscTime();

signals:

public slots:
    void OnCmdParam();
    void OnReadTscTimeSlot();

private:
    explicit CommandCallback(QObject *parent = 0);
    ~CommandCallback();
    static CommandCallback* instance_;
};

#endif // COMMANDCALLBACK_H
