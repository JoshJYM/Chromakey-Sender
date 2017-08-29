#include "loginwidget.h"
#include "TcpPacketType.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(0, Qt::FramelessWindowHint),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    connection = NULL;
    connect(ui->logInBtn, SIGNAL(clicked()),       SLOT(slotClickPush()));
    connect(ui->email,    SIGNAL(returnPressed()), SLOT(slotClickPush()));
    connect(ui->password, SIGNAL(returnPressed()), SLOT(slotClickPush()));
    connect(ui->SignUp,   SIGNAL(clicked()),       SLOT(slotOpenSignUpPage()));
}

char* LoginWidget::getToken(char* a)
{
    int len = 0;
    if(index > PACKET_SIZE)
        return NULL;

    while(len < PACKET_SIZE - HEADER_SIZE)
    {
        if(*(a+len+index) == '\0')
            break;
        len++;
    }

    if(len == PACKET_SIZE - HEADER_SIZE + 1)
        return NULL;
    index += len+1;
    return a+index-len-1;
}

void LoginWidget::slotOpenSignUpPage()
{
    QString link = "http://172.16.100.166/form.html";
    QDesktopServices::openUrl(QUrl(link));
}

void LoginWidget::slotClickPush()
{
    ui->logInBtn->setFocus();
    if(connection)
        delete connection;
    connection = new TcpConnection(TCP_CONNECTION_ADDRESS, TCP_CONNECTION_PORT);
    connect(connection, SIGNAL(signalLogin(TcpConnection*,int)),
            this, SLOT(slotLogin(TcpConnection*,int)));
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotSocketError(QAbstractSocket::SocketError)));
}

void LoginWidget::slotLogin(TcpConnection *_connection, int msg)
{
    QByteArray packet;

    index = HEADER_SIZE;

    switch(msg){
    case Login_Connect:
        packet.append(ui->email->text());
        packet.push_back('\0');
        packet.append(ui->password->text());
        packet.push_back('\0');
        _connection->writePacket(Major_Login, Login_Request, packet);
        break;

    case Login_Success:
        /*send connection data to maintWidget*/
        connection->setIDNum(connection->getToken().toInt());
        mainWidget = new Widget(connection,ui->email->text(),ui->password->text());
        mainWidget->show();
        this->hide();

        break;

    case Login_Fail:
        ui->loginInfor->setText(tr("Please recheck you ID or Password"));
        break;

    case Login_Fail_Dupli:
        ui->loginInfor->setText(tr("Login Fail...(Duplicated ID-num"));
        break;
    }
}

void LoginWidget::slotSocketError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        /*when server is deleted, sender must
         * relogin. So, delte mainWidget, connetion
         * and then reshow login widget*/
        delete mainWidget;
        connection = NULL;
        this->show();
        qDebug("RemoteHostClosedError");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug("ConnectionRefusedError");
        break;
//    default:
//        qDebug("Error signal : %s", tcpSocket->errorString().toLocal8Bit().data());
    }
}

void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    clicked_x = event->x();
    clicked_y = event->y();
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->x()-clicked_x + ((event->x())),this->y()-clicked_y+((event->y())));
}


LoginWidget::~LoginWidget()
{  
    qDebug("delete LoginWidget...");
    delete ui;
    delete connection;
}
