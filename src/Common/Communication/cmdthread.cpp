#include "cmdthread.h"

CmdThread *CmdThread::instance_ = NULL;

CmdThread::CmdThread(QObject *parent)
    : QThread(parent)
{
    connect(this, SIGNAL(readyReadRaw(Request*)), this, SLOT(parseReply(Request*)), Qt::QueuedConnection);
    socket_mutex_.lock();
    socket_ = NULL;
    socket_mutex_.unlock();
}

CmdThread::~CmdThread()
{
    socket_mutex_.lock();
    if (socket_ != NULL)
    {
        delete socket_;
        socket_ = NULL;
    }
    socket_mutex_.unlock();
}

void CmdThread::threadStart(const QString &ip, int port)
{
    if (isRunning())
    {
        return;
    }

    stop_ = false;
    ip_mutex_.lock();
    ip_ = ip;
    port_ = port;
    ip_mutex_.unlock();
    start();
}

void CmdThread::threadStop()
{
    if (isRunning())
    {
        stop_ = true;
        request_list_not_empty_.wakeAll();
        socket_connected_.wakeAll();
        wait();
    }
}

void CmdThread::run()
{
    socket_mutex_.lock();
    if (socket_ != NULL)
    {
        delete socket_;
        socket_ = NULL;
    }
    socket_ = new QTcpSocket;
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    connect(socket_, SIGNAL(connected()), this, SLOT(OnConnectEstablished()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnConnectError(QAbstractSocket::SocketError)));
    ip_mutex_.lock();
    socket_->connectToHost(ip_, port_);
    ip_mutex_.unlock();
    bool flag = socket_->waitForConnected();
    socket_mutex_.unlock();

    if (!flag)
    {
        emit netError("can not connect to signaler");
        return;
    }
    while (!stop_)
    {
        repeat_count_++;
        if (!repeat())
        {
            break;
        }
    }
    stop_ = true;
    socket_mutex_.lock();
    socket_->close();
    socket_mutex_.unlock();
}

bool CmdThread::repeat()
{
    return true;
}

void CmdThread::parseReply(Request *request)
{
    Q_UNUSED(request);
}

void CmdThread::OnConnectError(QAbstractSocket::SocketError err)
{
    QString str = tr("error code: %1 des: %2").arg(err).arg(socket_->errorString());
    threadStop();
    if ((err == QAbstractSocket::ConnectionRefusedError)
            || (QAbstractSocket::RemoteHostClosedError)
            || (QAbstractSocket::HostNotFoundError)
            || (QAbstractSocket::NetworkError))
    {
        emit connectError(err);
        emit connectError(str);
    }
}

void CmdThread::OnConnectEstablished()
{
    emit connectEstablished();
    socket_connected_.wakeAll();
}

void CmdThread::OnDisconnected()
{
    emit connectError("disconnect");
}
