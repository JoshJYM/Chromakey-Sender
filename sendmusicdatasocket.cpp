#include "sendmusicdatasocket.h"

SendMusicDataSocket::SendMusicDataSocket(QObject *parent)
{}

SendMusicDataSocket::~SendMusicDataSocket()
{
    qDebug("delete SendMusicDataSocket...");
}

/**
 * @brief SendMusicDataSocket::setDestInfo
 * @param port
 * @param addr
 */
void SendMusicDataSocket::setDestInfo(int port, QString addr[], int serverCnt)
{
    qDebug("set music udp socket ip and port");
    destAddress  = new QString[serverCnt];
    musicUdpSocket = new QUdpSocket();

    /* get destnation ip address and port number*/
    this->destPort      = port;
    this->serverNumber  = serverCnt;
    for( int i=0; i<serverNumber; i++ )
    {
        destAddress[i] = addr[i];
        qDebug("music socket Server Num (%d)'s ip adress...",i);
        qDebug()<<destAddress[i];
    }
}

/**
 * @brief SendMusicDataSocket::sendMudicData
 * @param musicData
 */
void SendMusicDataSocket::sendMudicData(QByteArray musicData)
{
    for( int i=0 ; i<serverNumber; i++)
    {
        musicUdpSocket->writeDatagram(musicData.data(),musicData.size(),
                                      QHostAddress(destAddress[i]),destPort);
    }
}
