#ifndef SYNCCOMMAND_H
#define SYNCCOMMAND_H

#include <QtCore>
#include <QTcpSocket>

#define CONNECT_WAIT_TIME   (2000)
#define WRITE_WAIT_TIME     (30000)
#define READ_WAIT_TIME      (30000)

#define REPLY_HEAD_FIX      ("CYT")

struct Request;

class SyncCommand : public QObject
{
    Q_OBJECT

public:
    static SyncCommand *GetInstance();
    void connectToHost(const QString &ip, unsigned int port);
    void disconnectFromHost();

    void ReadSignalerConfigFile();
    void ReadSignalerTime(const QObject *target, const std::string &slot);

signals:
    void connectedSignal();
    void connectErrorSignal();
    void connectErrorStrSignal(QString);
    void disconnectedSignal();

    void readyRead(int cmd_id, void *content);

public slots:
    void OnConnectEstablished();
    void OnDisconnected();
    void OnConnectError(QAbstractSocket::SocketError);

private slots:
    void parseReply();

private:
    void WriteRequest();

private:
    SyncCommand();
    ~SyncCommand();
    static SyncCommand *instance_;

private:
    QTcpSocket *socket_;
    QString ip_;
    unsigned int port_;

    QObject *target_obj_;
    std::string slot_;
};

#endif // SYNCCOMMAND_H
