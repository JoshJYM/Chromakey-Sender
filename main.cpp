#include "loginwidget.h"
#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWidget login;

    login.show();

    return a.exec();
}
