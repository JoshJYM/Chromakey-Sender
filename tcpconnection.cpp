#include "tcpConnection.h"
#include "TcpPacketType.h"


TcpConnection::TcpConnection(QObject *parent) :
    QTcpSocket(parent)
{
    idNum = 0;
    connect(this, SIGNAL(disconnected()), SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), SLOT(slotDisconnected()));
    connect(this, SIGNAL(readyRead()), SLOT(slotRead()));
}

TcpConnection::TcpConnection(QString server, int port, QObject *parent) :
    QTcpSocket(parent)
{
    idNum = 0;
    connect(this, SIGNAL(disconnected()), SLOT(deleteLater()));
    connect(this, SIGNAL(signalDisconnected(TcpConnection*)),
            this, SLOT(slotDisconnected()));
    connect(this, SIGNAL(readyRead()), SLOT(slotRead()));
    abort();
    connectToHost(server, port);
}

QByteArray* TcpConnection::getIn()
{
    return &in;
}

int TcpConnection::getIDNum()
{
    return idNum;
}

void TcpConnection::setIDNum(int _idNum)
{
    idNum = _idNum;
}

QString TcpConnection::getNickname()
{
    return nick;
}

void TcpConnection::setNickname(QString nickname)
{
    nick = nickname;
}

QByteArray TcpConnection::getToken()
{
    int o_index = index;
    QByteArray a;

    if(index > in.size())
        return NULL;

    index = in.indexOf('\0', index);

    if(index == 0)
        return NULL;

    a = in.mid(o_index, index - o_index);
    index++;

    return a;
}

void TcpConnection::slotRead()
{
    if(bytesAvailable() < PACKET_SIZE)
    {
        qDebug("Received imperfect packet(size:%d)", bytesAvailable());
        read(bytesAvailable());
        return;
    }

    in = read(PACKET_SIZE);
    if(strcmp(in, "AWTV"))
    {
        qDebug("HEAD is not \"AWTV\", Received %s", in.data());
        return;
    }

    index = HEADER_SIZE;
    switch(in[Order_MajorCate])
    {
    case Major_Login:
        emit signalLogin(this, in[Order_MinorCate]);
        break;

    case Major_BroadcastSettring:
        emit signalBroadSet(this, in[Order_MinorCate]);
        break;

    case Major_BroadcastView:
        emit signalBroadView(this, in[Order_MinorCate]);
        break;

    case Major_Chatting:
        emit signalChatting(this, in[Order_MinorCate]);
        break;

    default:
        qDebug("'C0' has unknown value (%d)", in[Order_MajorCate]);
    }

    if(bytesAvailable() >= PACKET_SIZE)
        emit readyRead();

}

void TcpConnection::slotDisconnected()
{
    emit signalDisconnected(this);
}

qint64 TcpConnection::writePacket(int major, int minor,
                               const char *data, qint64 len)
{
    QByteArray a;
    a.append("AWTV").append('\0');
    a.append(major).append(minor).append('\0');
    a.append(data, len);

    a.resize(PACKET_SIZE);
    return write(a);
}

qint64 TcpConnection::writePacket(int major, int minor, const QByteArray &data)
{
    return writePacket(major, minor, data.data(), data.size());
}

qint64 TcpConnection::writePacket(int major, int minor, const QString &data)
{
    return writePacket(major, minor, data.toUtf8());
}
