#ifndef SENDMUSICDATASOCKET_H
#define SENDMUSICDATASOCKET_H

#include <QUdpSocket>
#include <QObject>

class SendMusicDataSocket : public QObject
{
    Q_OBJECT
public:
    explicit SendMusicDataSocket(QObject *parent);
    ~SendMusicDataSocket();
    void setDestInfo(int port, QString addr[], int serverCnt);
    void sendMudicData(QByteArray musicData);

private:
    QString         *destAddress;
    QUdpSocket      *musicUdpSocket;
    int             destPort;
    int             serverNumber;
};

#endif // SENDMUSICDATASOCKET_H
