#ifndef SYNCCOMMAND_H
#define SYNCCOMMAND_H

#include <QtCore>
#include <QTcpSocket>

#define CONNECT_WAIT_TIME   (2000)
#define WRITE_WAIT_TIME     (30000)
#define READ_WAIT_TIME      (30000)
#define VERSION_CHECK_TIME  (1000)

#define REPLY_HEAD_FIX      ("CYT")

struct Request;

class SyncCommand : public QObject
{
    Q_OBJECT

public:
    static SyncCommand *GetInstance();
    QTcpSocket *getSocket();

    void connectToHost(const QString &ip, unsigned int port);
    void disconnectFromHost();

    void ReadSignalerConfigFile(QObject *target, const std::string &slot);
    void ReadSignalerTime(QObject *target, const std::string &slot);
    void ReadSignalerNetworkInfo(QObject *target, const std::string &slot);
    void ReadEventLogFile(QObject *target, const std::string &slot);
    void DeleteEventLog(const std::string &param, QObject *target, const std::string &slot);

    void ReadTscVersion(QObject *target, const std::string &slot);

signals:
    void connectedSignal();
    void connectErrorSignal();
    void connectErrorStrSignal(QString);
    void disconnectedSignal();

    void readyRead(QByteArray &content);
    void readyRead();

public slots:
    void OnConnectEstablished();
    void OnDisconnected();
    void OnConnectError(QAbstractSocket::SocketError);

private slots:
    void parseReply();
    void socketReadyReadSlot();

private:
    void WriteRequest();
    void RegParseHandler();
    void UnRegParseHandler();
    void InitParseHandler(QObject *target, const std::string &slot);

private:
    SyncCommand(QObject *parent = 0);
    ~SyncCommand();
    static SyncCommand *instance_;

private:
    QTcpSocket *socket_;
    QString ip_;
    unsigned int port_;
    QByteArray sock_array_;

    QObject *target_obj_;
    std::string slot_;
};

#endif // SYNCCOMMAND_H
