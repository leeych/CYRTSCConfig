#include "basethread.h"

BaseThread::BaseThread(QObject *parent) :
    QThread(parent),
    repeat_count_(0),
    stop_(true)
{
}

BaseThread::~BaseThread()
{
    threadStop();
}

void BaseThread::threadStart(QThread::Priority priority)
{
    stop_ = false;
    if (isRunning() == false)
    {
        start(priority);
    }
}

void BaseThread::threadStop()
{
    stop_ = true;
    wait();
}

void BaseThread::threadSwitch()
{
    if (isRunning())
    {
        threadStop();
    }
    else
    {
        threadStart();
    }
}

bool BaseThread::repeat()
{
    std::cout << "repeat count: " << repeat_count_ << std::endl;
    return true;
}

void BaseThread::run()
{
    while (!stop_)
    {
        repeat_count_++;
        if (repeat() != true)
        {
            break;
        }
    }
    stop_ = true;
}

void BaseThread::start(QThread::Priority priority)
{
    stop_ = false;
    QThread::start(priority);
}
