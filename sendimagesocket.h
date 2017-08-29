#ifndef SENDIMAGESOCKET_H
#define SENDIMAGESOCKET_H
#include <QObject>
#include <QUdpSocket>

class SendImageSocket : public QObject
{
    Q_OBJECT
public:
    explicit SendImageSocket(QObject *parent);
    ~SendImageSocket();

    void setDestInfo(int port, QString addr[], int serverCnt);
    void sendImageData(QByteArray imageData);

private:
    QString         *destAddress;
    QUdpSocket      *imgUdpSocket;
    int             destPort;
    int             serverNumber;
};

#endif // SENDIMAGESOCKET_H
