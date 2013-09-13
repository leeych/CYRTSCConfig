#ifndef BASETHREAD_H
#define BASETHREAD_H

#include <QThread>
#include "communication_global.h"

class COMMUNICATIONSHARED_EXPORT BaseThread : public QThread
{
    Q_OBJECT
public:
    explicit BaseThread(QObject *parent = 0);
    virtual ~BaseThread();
    
signals:
    
public slots:
    virtual void threadStart(Priority priority = InheritPriority);
    virtual void threadStop();
    virtual void threadSwitch();

protected:
    virtual bool repeat();
    virtual void run();

protected:
    int repeat_count_;
    volatile bool stop_;

private:
    void start(Priority = InheritPriority);
};

#endif // BASETHREAD_H
