#ifndef SENDERCHATTING_H
#define SENDERCHATTING_H

#define MAX_MEMBER 1500
#include "tcpconnection.h"


class SenderChatting : public QObject
{
    Q_OBJECT
public:
    explicit SenderChatting(QString id, QString pw);
    ~SenderChatting();

private:
    TcpConnection *chatCon;
    QString        id;
    QString        pw;


    int currentMemberCnt;

    QHash<int, QString> list;

    void initConnection();
    void initVal();
    void getChatContext();
    void getChatMember();
    void getChatExitMember();
    void getLogInInfo(QString id, QString pw);


private slots:
    void slotGetChatInfor(TcpConnection* _connection, int msg);
    void slotLogin(TcpConnection* _connection, int msg);
    void slotSendChatContext(QString context);
    void slotGetKickMember(QString nick);
    void slotSocketError(QAbstractSocket::SocketError socketError);

signals:
    void sendChatContext(QString nickname, QString context);
    void sendChatmember(QString nick,int IdNum);
    void sendChatExitMember(QString nick);
    void noticeChatErr();
    void noticechatConnectionSuccess();
};

#endif // SENDERCHATTING_H
