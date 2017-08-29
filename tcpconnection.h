#ifndef TCPConnection_H
#define TCPConnection_H

#include <QTcpSocket>
#include <QtNetwork/QtNetwork>

class TcpConnection : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject *parent = 0);
    explicit TcpConnection(QString server, int port, QObject *parent = 0);
    QByteArray* getIn();
    int getIDNum();
    QString getNickname();
    void setIDNum(int _idNum);
    void setNickname(QString);
    QByteArray getToken();
    qint64 writePacket(int major, int minor, const char *data = NULL, qint64 len = 0);
    qint64 writePacket(int major, int minor, const QByteArray &data);
    qint64 writePacket(int major, int minor, const QString &data);

private:
    int idNum;
    QString nick;
    int index;
    QByteArray in;

signals:
    void signalLogin(TcpConnection*, int);
    void signalBroadSet(TcpConnection*, int);
    void signalBroadView(TcpConnection*, int);
    void signalChatting(TcpConnection*, int);
    void signalDisconnected(TcpConnection*);

public slots:
    void slotRead();

private slots:
    void slotDisconnected();
};


#endif // TCPConnection_H


