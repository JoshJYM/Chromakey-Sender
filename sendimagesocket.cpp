#include "sendimagesocket.h"

SendImageSocket::SendImageSocket(QObject *parent)
{}

SendImageSocket::~SendImageSocket()
{
    qDebug("delete SendImageSocket...");
}

/**
 * @brief SendImageSocket::setDestInfo
 * @param port
 * @param addr
 */
void SendImageSocket::setDestInfo(int port, QString addr[], int serverCnt)
{ 
    qDebug("set image udp socket ip and port");

    destAddress  = new QString[serverCnt];
    imgUdpSocket = new QUdpSocket();

    /* get destnation ip address and port number*/
    this->destPort      = port;
    this->serverNumber  = serverCnt;
    for( int i=0; i<serverNumber; i++ )
    {
        destAddress[i] = addr[i];
        qDebug("image socket Server Num (%d)'s ip adress...",i);
        qDebug()<<destAddress[i];
    }
}

/**
 * @brief SendImageSocket::sendImageData
 * @param imageData
 */
void SendImageSocket::sendImageData(QByteArray imageData)
{
    for( int i=0 ; i<serverNumber; i++ )
    {
        imgUdpSocket->writeDatagram(imageData,imageData.size(),
                                    QHostAddress(destAddress[i]),destPort);
    }
}

