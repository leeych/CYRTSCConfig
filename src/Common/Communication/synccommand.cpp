#include "synccommand.h"
#include "request.h"
#include "command.h"

SyncCommand *SyncCommand::instance_ = NULL;

SyncCommand *SyncCommand::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new SyncCommand;
    }
    return instance_;
}

QTcpSocket *SyncCommand::getSocket()
{
    return socket_;
}

void SyncCommand::connectToHost(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
}

void SyncCommand::disconnectFromHost()
{
    socket_->disconnectFromHost();
}

void SyncCommand::ReadSignalerConfigFile(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetConfigure.c_str());
    qDebug() << "write GetConfigure bytes: " << sz;
}

void SyncCommand::ReadSignalerTime(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    qint64 sz = socket_->write(Command::GetTSCtime.c_str());
    qDebug() << "write GetTSCTime bytes: " << sz;
}

void SyncCommand::ReadSignalerNetworkInfo(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write(Command::GetNetAddress.c_str());
}

void SyncCommand::ReadEventLogFile(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write(Command::GetEventInfo.c_str());
}

// const std::string &param: represent for log_id and log_time string
void SyncCommand::DeleteEventLog(const std::string &param, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write((Command::ClearEventInfo + param).c_str());
}

void SyncCommand::SyncSignalerTime(unsigned int seconds, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    char temp[11] = {'C','Y','T','7','\0','\0','\0','\0','E','N','D'};
    memcpy(temp+4, &seconds, sizeof(seconds));
    socket_->write(temp);
}

void SyncCommand::ConfigNetwork(const QStringList &net_info, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    QString cmd_str("CYT8,DHCP=\"%1\",DefaultGateway=\"%2\",IPAddress=\"%3\",SubnetMask=\"%4\",END");
    cmd_str.arg(net_info.at(0)).arg(net_info.at(1)).arg(net_info.at(2)).arg(net_info.at(3));
    socket_->write(cmd_str.toStdString().c_str());
}

void SyncCommand::SetConfiguration(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write(Command::SetConfigure.c_str());
}

void SyncCommand::SendConfigData(const QByteArray &byte_array, QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write(byte_array);
}

void SyncCommand::ReadTscVersion(QObject *target, const std::string &slot)
{
    InitParseHandler(target, slot);
    socket_->write(Command::GetVerId.c_str());
}

void SyncCommand::OnConnectEstablished()
{
    emit connectedSignal();
    qDebug() << "connect succeeded";
}

void SyncCommand::OnDisconnected()
{
    emit disconnectedSignal();
}

void SyncCommand::OnConnectError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    emit connectErrorSignal();
}

void SyncCommand::parseReply()
{
    qDebug() << "parse reply";
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

SyncCommand::SyncCommand(QObject *parent) :
    QObject(parent)
{
    socket_ = new QTcpSocket(this);
    target_obj_ = NULL;
//    connect(socket_, SIGNAL(readyRead()), this, SLOT(parseReply()));
    connect(socket_, SIGNAL(connected()), this, SLOT(OnConnectEstablished()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnConnectError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));
}

SyncCommand::~SyncCommand()
{
    delete socket_;
    socket_ = NULL;
}


void SyncCommand::RegParseHandler()
{
    qDebug() << "register parse handler";
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        connect(this, SIGNAL(readyRead(QByteArray&)), target_obj_, slot_.c_str());
    }
}

void SyncCommand::UnRegParseHandler()
{
    qDebug() << "unregister parse handler";
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
