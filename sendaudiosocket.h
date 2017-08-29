#ifndef SENDAUDIOSOCKET_H
#define SENDAUDIOSOCKET_H

#include <QUdpSocket>
#include <QObject>

class SendAudioSocket : public QObject
{
    Q_OBJECT
public:
    explicit SendAudioSocket(QObject *parent);
    ~SendAudioSocket();

    void setDestInfo(int port, QString addr[], int serverCnt);
    void sendAudioData(QByteArray audioData);

private:
    QString         *destAddress;
    QUdpSocket      *audioUdpSocket;
    int             destPort;
    int             serverNumber;
};

#endif // SENDAUDIOSOCKET_H
