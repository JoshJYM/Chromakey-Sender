#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QDesktopServices>
#include <QUrl>
#include "tcpconnection.h"
#include "widget.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

private slots:
    void slotLogin(TcpConnection* _connection, int msg);
    void slotSocketError(QAbstractSocket::SocketError socketError);
    void slotClickPush();
    void slotOpenSignUpPage();


public:
    explicit LoginWidget(QWidget *parent = 0);
    char* getToken(char* a);
    ~LoginWidget();

private:
    Ui::LoginWidget *ui;
    QTcpSocket      *tcpSocket;
    TcpConnection   *connection;
    quint16         blockSize;
    Widget          *mainWidget;
    QString         *destAddress;

    int             index;
    int             clicked_x;
    int             clicked_y;

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // LOGINWIDGET_H

