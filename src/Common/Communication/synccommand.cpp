#include "synccommand.h"
#include "request.h"
#include "command.h"
#include "macrostring.h"

#define MDEBUG_BRIEF_ENABLE
#include "mdebug.h"

SyncCommand *SyncCommand::instance_ = NULL;

SyncCommand *SyncCommand::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new SyncCommand;
    }
    return instance_;
}

void SyncCommand::DistroyInstance()
{
    this->~SyncCommand();
}

QTcpSocket *SyncCommand::getSocket()
{
    return socket_;
}

void SyncCommand::setSocketAddress(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
}

QString SyncCommand::getSocketIp() const
{
    return ip_;
}

unsigned int SyncCommand::getSocketPort() const
{
    return port_;
}

//int SyncCommand::getConnectionStatus() const
//{
//    return conn_status_;
//}

void SyncCommand::connectToHost(const QString &ip, unsigned int port)
{
//    conn_status_ = QAbstractSocket::UnconnectedState;
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
}

bool SyncCommand::syncConnectToHost(const QString &ip, unsigned port)
{
//    conn_status_ = QAbstractSocket::UnconnectedState;
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
    if (!socket_->waitForConnected(SOCKET_WAIT_MS))
    {
        OnConnectError(socket_->error());
        return false;
    }
    return true;
}

void SyncCommand::closeConnection()
{
//    socket_->close();
    socket_->abort();
//    conn_status_ = QAbstractSocket::UnconnectedState;
}

bool SyncCommand::isConnectionValid() const
{
    return socket_->isValid();
}

void SyncCommand::disconnectFromHost()
{
    socket_->disconnectFromHost();
//    conn_status_ = QAbstractSocket::UnconnectedState;
}

void SyncCommand::ReadSignalerConfigFile(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetConfigure.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetConfigure.c_str() << " bytes:" << sz;
}

void SyncCommand::ReadSignalerTime(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetTSCtime.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetTSCtime.c_str() << " bytes:" << sz;
}

void SyncCommand::ReadSignalerNetworkInfo(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetNetAddress.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetNetAddress.c_str() << " bytes:" << sz;
}

void SyncCommand::ReadEventLogFile(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetEventInfo.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetEventInfo.c_str() << " bytes:" << sz;
}

// const std::string &param: represent for log_id and log_time string
void SyncCommand::ClearEventLogs(const std::string &param, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write((Command::ClearEventInfo + param).c_str());
    BRIEF_DEBUG(DModule_Communication) << (Command::ClearEventInfo + param).c_str() << " bytes:" << sz;
}

void SyncCommand::ClearEventLogs(const std::string &param)
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write((Command::ClearEventInfo + param).c_str());
        BRIEF_DEBUG(DModule_Communication) << (Command::ClearEventInfo + param).c_str() << "bytes:" << sz;
    }
}

void SyncCommand::ClearEventLogs()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::ClearEventInfo.c_str());
        BRIEF_DEBUG(DModule_Communication) << (Command::ClearEventInfo).c_str() << "bytes:" << sz;
    }
}

void SyncCommand::StartMonitoring(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::BeginMonitor.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::BeginMonitor.c_str() << " bytes:" << sz;
}

void SyncCommand::StartMonitoring()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::BeginMonitor.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::BeginMonitor.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::StopMonitoring(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::EndMonitor.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::EndMonitor.c_str() << " bytes:" << sz;
}

void SyncCommand::StopMonitoring()
{
    UnRegParseHandler();
    qint64 sz = socket_->write(Command::EndMonitor.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::EndMonitor.c_str() << " bytes:" << sz;
}

void SyncCommand::GetLightStatus(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetLampStatus.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetLampStatus.c_str() << " bytes:" << sz;
}

void SyncCommand::GetLightStatus()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::GetLampStatus.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::GetLampStatus.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::GetTscTime()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::GetTSCtime.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::GetTSCtime.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::GetDetectorFlowData(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetDetectInfo.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetDetectInfo.c_str() << " bytes:" << sz;
}

void SyncCommand::GetDetectorFlowData()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::GetDetectInfo.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::GetDetectInfo.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::ClearDetectorFlowInfo(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::ClearDetectInfo.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::ClearDetectInfo.c_str() << " bytes:" << sz;
}

void SyncCommand::ClearDetectorFlowInfo()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::ClearDetectInfo.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::ClearDetectInfo.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::GetDriverBoardInfo(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetDriverInfo.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetDriverInfo.c_str() << " bytes:" << sz;
}

void SyncCommand::GetDriverBoardInfo()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::GetDriverInfo.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::GetDriverInfo.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::SendHeartBeat(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::HearBeat.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::HearBeat.c_str() << " bytes:" << sz;
}

void SyncCommand::SendHeartBeat()
{
    if (target_obj_ != NULL && !slot_.empty())
    {
        qint64 sz = socket_->write(Command::HearBeat.c_str());
        BRIEF_DEBUG(DModule_Communication) << Command::HearBeat.c_str() << " bytes:" << sz;
    }
}

void SyncCommand::ReleaseSignalSlots()
{
    UnRegParseHandler();
}

void SyncCommand::SyncSignalerTime(unsigned int seconds, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    char temp[11] = {'C','Y','T','7','\0','\0','\0','\0','E','N','D'};
    memcpy(temp+4, &seconds, sizeof(seconds));
    qint64 sz = socket_->write(temp);
    BRIEF_DEBUG(DModule_Communication) << temp << " bytes:" << sz;
}

void SyncCommand::ConfigNetwork(const QStringList &net_info, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    QString cmd_str("CYT8,DHCP=\"%1\",DefaultGateway=\"%2\",IPAddress=\"%3\",SubnetMask=\"%4\",END");
    cmd_str = cmd_str.arg(net_info.at(0)).arg(net_info.at(1)).arg(net_info.at(2)).arg(net_info.at(3));
    qint64 sz = socket_->write(cmd_str.toStdString().c_str());
    BRIEF_DEBUG(DModule_Communication) << cmd_str << " bytes:" << sz;
}

void SyncCommand::ConnectConfigNetworkHandler(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
}

void SyncCommand::SetConfiguration(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::SetConfigure.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::SetConfigure.c_str() << " bytes:" << sz;
}

void SyncCommand::SendConfigData(const QByteArray &byte_array, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(byte_array);
    BRIEF_DEBUG(DModule_Communication) << "Send signaler config file bytes:" << sz;
}

void SyncCommand::ReadTscVersion(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetVerId.c_str());
    BRIEF_DEBUG(DModule_Communication) << Command::GetVerId.c_str() << " bytes:" << sz;
}

void SyncCommand::OnConnectEstablished()
{
    emit connectedSignal();
}

void SyncCommand::OnDisconnected()
{
    emit disconnectedSignal();
}

void SyncCommand::OnConnectError(QAbstractSocket::SocketError err)
{
    emit connectErrorSignal();
    emit connectErrorStrSignal(socket_err_desc_.value(err));
}

void SyncCommand::parseReply()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        connect(this, SIGNAL(readyRead()), target_obj_, slot_.c_str());
    }
//    emit readyRead(QByteArray);
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        disconnect(this, SIGNAL(readyRead()), target_obj_, slot_.c_str());
    }
}

void SyncCommand::socketReadyReadSlot()
{
    sock_array_ = socket_->readAll();
    emit readyRead(sock_array_);
}

void SyncCommand::GenConnectErrDesc()
{
    socket_err_desc_.insert(QAbstractSocket::ConnectionRefusedError, STRING_CONNECT_HOST_REFUSED);
    socket_err_desc_.insert(QAbstractSocket::RemoteHostClosedError, STRING_CONNECT_HOST_CLOSED);
    socket_err_desc_.insert(QAbstractSocket::HostNotFoundError, STRING_CONNECT_HOST_NOTFIND);
    socket_err_desc_.insert(QAbstractSocket::SocketAccessError, STRING_CONNECT_SOCK_ACCESS_ERR);
    socket_err_desc_.insert(QAbstractSocket::SocketResourceError, STRING_CONNECT_SOCK_RESOURCE_E);
    socket_err_desc_.insert(QAbstractSocket::SocketTimeoutError, STRING_CONNECT_SOCK_TIMEOUT);
    socket_err_desc_.insert(QAbstractSocket::DatagramTooLargeError, STRING_CONNECT_DATA_TOO_LARGE);
    socket_err_desc_.insert(QAbstractSocket::NetworkError, STRING_CONNECT_NETWORK_ERR);
    socket_err_desc_.insert(QAbstractSocket::AddressInUseError, STRING_CONNECT_ADDRESS_INUSE);
    socket_err_desc_.insert(QAbstractSocket::SocketAddressNotAvailableError, STRING_CONNECT_ADDR_NOT_ABLE);
    socket_err_desc_.insert(QAbstractSocket::UnsupportedSocketOperationError, STRING_CONNECT_UNSUPPORTED);
    socket_err_desc_.insert(QAbstractSocket::UnfinishedSocketOperationError, STRING_CONNECT_UNFINISHED);
    socket_err_desc_.insert(QAbstractSocket::ProxyAuthenticationRequiredError, STRING_CONNECT_PROXY_AUTH);
    socket_err_desc_.insert(QAbstractSocket::SslHandshakeFailedError, STRING_CONNECT_SSL_ERROR);
    socket_err_desc_.insert(QAbstractSocket::ProxyConnectionRefusedError, STRING_CONNECT_PROXY_REFUSED);
    socket_err_desc_.insert(QAbstractSocket::ProxyConnectionClosedError, STRING_CONNECT_PROXY_CLOSED);
    socket_err_desc_.insert(QAbstractSocket::ProxyConnectionTimeoutError, STRING_CONNECT_PROXY_TIMEOUT);
    socket_err_desc_.insert(QAbstractSocket::ProxyNotFoundError, STRING_CONNECT_PROXY_NOTFIND);
    socket_err_desc_.insert(QAbstractSocket::ProxyProtocolError, STRING_CONNECT_PROXY_PROTOCOL);
    socket_err_desc_.insert(QAbstractSocket::UnknownSocketError, STRING_CONNECT_UNKNOWN);
}

SyncCommand::SyncCommand(QObject *parent) :
    QObject(parent)
{
    socket_ = new QTcpSocket(this);
    target_obj_ = NULL;
//    conn_status_ = QAbstractSocket::UnconnectedState;

//    connect(socket_, SIGNAL(readyRead()), this, SLOT(parseReply()));
    connect(socket_, SIGNAL(connected()), this, SLOT(OnConnectEstablished()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnConnectError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));

    GenConnectErrDesc();
}

SyncCommand::~SyncCommand()
{
    delete socket_;
    socket_ = NULL;
}


void SyncCommand::RegParseHandler()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        connect(this, SIGNAL(readyRead(QByteArray&)), target_obj_, slot_.c_str());
    }
}

void SyncCommand::UnRegParseHandler()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        disconnect(this, SIGNAL(readyRead(QByteArray&)), target_obj_, slot_.c_str());
    }
}

void SyncCommand::InitParseHandler(QObject *target, const std::string &slot)
{
    UnRegParseHandler();
    target_obj_ = target;
    slot_ = slot;
    RegParseHandler();
    socket_->readAll();
}
