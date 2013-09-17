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

void SyncCommand::connectToHost(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
    bool status = socket_->waitForConnected();
    if (!status)
    {
        qDebug() << "can not connect to signaler";
        emit connectErrorStrSignal("can not connect to signaler");
        return;
    }
}

void SyncCommand::disconnectFromHost()
{
    socket_->disconnectFromHost();
    bool status = socket_->waitForDisconnected();
    if (!status)
    {
        qDebug() << "can not disconnect from signaler";
        emit connectErrorStrSignal("can not disconnect from signaler");
    }
}

void SyncCommand::ReadSignalerConfigFile()
{
    socket_->readAll();
    socket_->write(Command::GetConfigure);
//    bool write_flag = socket_->waitForBytesWritten();
}

void SyncCommand::ReadSignalerTime(const QObject *target, const std::string &slot)
{
    target_obj_ = target;
    slot_ = slot;
    socket_->readAll();
    socket_->write(Command::GetTSCtime);
    bool write_flag = socket_->waitForBytesWritten();
    bool read_flag = false;
    unsigned int recv_len = 0;
    ReplyHead head;
    if (write_flag)
    {
        while (socket_->bytesAvailable() < 7)   // 7 present for minimum size
        {
            read_flag = socket_->waitForReadyRead();
            if (!read_flag)
            {
                socket_->readAll();
                qDebug() << "recv data failed, time out" << (socket_->bytesAvailable());
                break;
            }
        }
        while (read_flag)
        {
            int read_len = socket_->read((char *)&head, sizeof(head));
            if (read_len != sizeof(head))
            {
                qDebug() << "socket read head error";
                socket_->readAll();     // clear socket data
                read_flag = false;
                break;
            }
            if (strcmp(head.prefix_, REPLY_HEAD_FIX) != 0)
            {
                qDebug() << "head not begin with" << REPLY_HEAD_FIX;
                socket_->readAll();     // clear socket data
                read_flag = false;
                break;
            }
        }
    }

    if (write_flag && read_flag)
    {
        // TODO: left to be done.
    }
    // TODO: wait for write return.
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
        connect(this, SIGNAL(readyRead()), target_obj_, slot_);
    }
    emit readyRead(0, NULL);
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        disconnect(this, SIGNAL(readyRead()), target_obj_, slot_);
    }
}

SyncCommand::SyncCommand()
{
    socket_ = new QTcpSocket;
//    connect(socket_, SIGNAL(readyRead()), this, SLOT(parseReply()));
    connect(socket_, SIGNAL(connected()), this, SLOT(OnConnectEstablished()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnConnectError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
}

SyncCommand::~SyncCommand()
{
    delete socket_;
    socket_ = NULL;
}
