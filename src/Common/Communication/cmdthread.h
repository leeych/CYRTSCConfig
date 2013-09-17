#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>

//#include "cmdthread.h"
#include "communication_global.h"

#define DEBUG
#define CONNECT_WAIT_MS (2000)
#define WRITE_WAIT_MS   (30000)
#define READ_WAIT_MS    (30000)

struct RequestHead;
struct ReplayHead;
class Request;

class CmdThread : public QThread
{
    Q_OBJECT
public:
    static CmdThread *GetInstance()
    {
        if (instance_ == NULL)
        {
            instance_ = new CmdThread;
        }
        return instance_;
    }

signals:
    void connectEstablished();
    void netError(QString);
    void connectError(QAbstractSocket::SocketError);
    void connectError(QString);

    void readyRead(int cmd_id, void *content, int reply_len);
    void readyReadRaw(Request *request);

public slots:
    void threadStart(const QString &ip, int port);
    void threadStop();

protected:
    virtual void run();
    bool repeat();

private slots:
    void parseReply(Request *request);
    void OnConnectError(QAbstractSocket::SocketError);
    void OnConnectEstablished();
    void OnDisconnected();

private:
    explicit CmdThread(QObject *parent = 0);
    ~CmdThread();
    static CmdThread *instance_;

private:
    Request *writeHead(int cmd_id, int content_len, QObject *target, std::string slot);
    void AddToList(Request *);

private:
    std::list<Request *> request_list_;

    QMutex socket_mutex_;
    QMutex request_list_mutex_;
    QMutex ip_mutex_;

    QWaitCondition request_list_not_empty_;
    QWaitCondition socket_connected_;
    QTcpSocket *socket_;

    bool stop_;
    QString ip_;
    int port_;

    int repeat_count_;
};

#endif // CMDTHREAD_H
