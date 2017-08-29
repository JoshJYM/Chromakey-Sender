#include "senderchatting.h"
#include "TcpPacketType.h"

SenderChatting::SenderChatting(QString id, QString pw)
{
    initVal();
    getLogInInfo(id,pw);
    initConnection();
}

SenderChatting::~SenderChatting()
{
    if(chatCon)
        chatCon->disconnectFromHost();
    qDebug("delete SenderChatting...");
}

/**
 * @brief SenderChatting::getLogInInfo
 * @param id
 * @param pw
 */
void SenderChatting::getLogInInfo(QString id, QString pw)
{
    this->id = id;
    this->pw = pw;
}

/**
 * @brief SenderChatting::initConnection
 */
void SenderChatting::initConnection()
{
    if(chatCon)
        chatCon->disconnectFromHost();

    chatCon = new TcpConnection(TCP_CHAT_CONNECTION_ADDRESS,
                                TCP_CHAT_CONNECTION_PORT);

    connect(chatCon, SIGNAL(signalLogin(TcpConnection*,int)),
            this, SLOT(slotLogin(TcpConnection*,int)));
    connect(chatCon,SIGNAL(signalChatting(TcpConnection*,int)),
            SLOT(slotGetChatInfor(TcpConnection*,int)));
    connect(chatCon,SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(slotSocketError(QAbstractSocket::SocketError)));
}

/**
 * @brief SenderChatting::slotSocketError
 * @param socketError
 */
void SenderChatting::slotSocketError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        /*when chatting server is dead,
         * this slot will be called and chatCon's value
         * will be setted 'NULL' for ~SenderChatting() function*/
        chatCon = NULL;
        qDebug("set chatCon value to NULL");

        /*noice to maintWidget about disconnect with
         * chatting server for change ui*/
        emit noticeChatErr();


        break;
    case QAbstractSocket::ConnectionRefusedError:
        /*noice to maintWidget about disconnect with
         * chatting server for change ui*/
        emit noticeChatErr();
        qDebug("ConnectionRefusedError");
        break;
    default:
        qDebug("Default Err");
    }
}

/**
 * @brief SenderChatting::slotLogin
 * @param _connection
 * @param msg
 */
void SenderChatting::slotLogin(TcpConnection *_connection, int msg)
{
    QByteArray packet;
    QString idNum;

    switch(msg){
    case Login_Connect:
        packet.append(id);
        packet.push_back('\0');
        packet.append(pw);
        packet.push_back('\0');
        _connection->writePacket(Major_Login, Login_Request, packet);
        break;

    case Login_Success:
        chatCon->setIDNum(chatCon->getToken().toInt());
        chatCon->writePacket(Major_Chatting,Chatting_Connect,
                             idNum.setNum(chatCon->getIDNum()));

        /*noice to maintWidget about to success connect with
         * chatting server for making new instans*/
        emit noticechatConnectionSuccess();
        break;

    case Login_Fail_Dupli:
        qDebug("Login Fail...(Duplicated ID-num");
        break;
    }
}

/**
 * @brief SenderChatting::initVal
 */
void SenderChatting::initVal()
{
    chatCon = NULL;
    currentMemberCnt = 0;
}


/**
 * @brief SenderChatting::slotSendChatContext
 * @param context
 */
void SenderChatting::slotSendChatContext(QString context)
{
    /*when Sender send some chatting text, this slot will be called*/
    QString text = QString("%1%2%3").arg(chatCon->getIDNum()).arg('\0')
            .arg(context);

    chatCon->writePacket(Major_Chatting, Chatting_Chat, text);
}

/**
 * @brief SenderChatting::slotGetChatInfor
 * @param _connection
 * @param msg
 */
void SenderChatting::slotGetChatInfor(TcpConnection *_connection, int msg)
{
    switch(msg)
    {
    case Chatting_Join:
        qDebug("chat member join!");
        getChatMember();

        break;
    case Chatting_Exit:
        qDebug("chat member exit!");
        getChatExitMember();

        break;
    case Chatting_Chat:
        qDebug("get chat context!");
        getChatContext();

        break;
    case Chatting_Kick:
        break;
    case Chatting_Ban:
        break;
    }
}

/**
 * @brief SenderChatting::getChatExitMember
 */
void SenderChatting::getChatExitMember()
{
    int     idNum;
    QString nick;

    while(true)
    {
        idNum = chatCon->getToken().toInt();
        nick  = list.value(idNum);

        qDebug()<<idNum;
        qDebug()<<nick;

        /*if idNum is 0 or nick name is null
         * do not excute remove list value*/
        if(idNum == 0 || nick==NULL)
            break;

        list.remove(idNum);
        emit sendChatExitMember(nick);
    }
}

/**
 * @brief SenderChatting::getChatMember
 */
void SenderChatting::getChatMember()
{
    int     idNum ;
    QString nick  ;

    while(true)
    {
        idNum = chatCon->getToken().toInt();
        nick  = chatCon->getToken();

        if(idNum == 0)
            break;

        list.insert(idNum,nick);
        emit sendChatmember(nick,idNum);
    }
}

/**
 * @brief SenderChatting::slotGetKickMember
 * @param nick
 */
void SenderChatting::slotGetKickMember(QString nick)
{
    int     temp = list.key(nick);
    QString id ;

    /*Get id number kicked and remove that id in list*/
    id = id.setNum(temp);
    list.remove(temp);

    /*Inform to Server about member kicked by Sender*/
    chatCon->writePacket(Major_Chatting, Chatting_Kick,id);
}

/**
 * @brief SenderChatting::getChatContext
 */
void SenderChatting::getChatContext()
{
    int     id      = chatCon->getToken().toInt();
    QString context = chatCon->getToken();
    QString nick    = list.value(id);

    emit sendChatContext(nick,context);
}
