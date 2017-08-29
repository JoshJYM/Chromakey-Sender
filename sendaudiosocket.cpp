#include "sendaudiosocket.h"

SendAudioSocket::SendAudioSocket(QObject *parent)
{}

SendAudioSocket::~SendAudioSocket()
{
    qDebug("delete SendAudioSocket...");
}

/**
 * @brief SendAudioSocket::setDestInfo
 * @param port
 * @param addr
 */
void SendAudioSocket::setDestInfo(int port, QString addr[], int serverCnt)
{
    qDebug("set audio udp socket ip and port");
    destAddress  = new QString[serverCnt];
    audioUdpSocket = new QUdpSocket();

    /* get destnation ip address and port number*/
    this->destPort      = port;
    this->serverNumber  = serverCnt;
    for( int i=0; i<serverNumber; i++ )
    {
        destAddress[i] = addr[i];
        qDebug("audio socket Server Num (%d)'s ip adress...",i);
        qDebug()<<destAddress[i];
    }

}

/**
 * @brief SendAudioSocket::sendAudioData
 * @param audioData
 */
void SendAudioSocket::sendAudioData(QByteArray audioData)
{
    for( int i=0 ; i<serverNumber; i++)
    {
        audioUdpSocket->writeDatagram(audioData, QHostAddress(destAddress[i]),
                                      destPort);
    }
}
