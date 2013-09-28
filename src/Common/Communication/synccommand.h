#ifndef SYNCCOMMAND_H
#define SYNCCOMMAND_H

#include <QtCore>
#include <QTcpSocket>
#include <QMap>

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
    void DistroyInstance();
    QTcpSocket *getSocket();

    void connectToHost(const QString &ip, unsigned int port);
    void disconnectFromHost();
    void ReadTscVersion(QObject *target, const std::string &slot);

    void ReadSignalerConfigFile(QObject *target, const std::string &slot);
    void ReadSignalerTime(QObject *target, const std::string &slot);
    void ReadSignalerNetworkInfo(QObject *target, const std::string &slot);
    void SyncSignalerTime(unsigned int seconds, QObject *target, const std::string &slot);
    void ConfigNetwork(const QStringList &netinfo, QObject *target, const std::string &slot);

    void SetConfiguration(QObject *target, const std::string &slot);
    void SendConfigData(const QByteArray &byte_array, QObject *target, const std::string &slot);

    void ReadEventLogFile(QObject *target, const std::string &slot);
    void DeleteEventLog(const std::string &param, QObject *target, const std::string &slot);

    void StartMonitoring(QObject *target, const std::string &slot);
    void StopMonitoring(QObject *target, const std::string &slot);
    void StopMonitoring();
    void GetLightStatus(QObject *target, const std::string &slot);
    void GetLightStatus();
    void GetTscTime();

    void GetDetectorFlowData(QObject *target, const std::string &slot);
    void ClearDetectorFlowInfo(QObject *target, const std::string &slot);
    void GetDriverBoardInfo(QObject *target, const std::string &slot);

    void ReleaseSignalSlots();

signals:
    void connectedSignal();
    void connectErrorSignal();
    void connectErrorStrSignal(const QString&);
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
    void GenConnectErrDesc();

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

    QMap<QAbstractSocket::SocketError, QString> socket_err_desc_;
};

#endif // SYNCCOMMAND_H
