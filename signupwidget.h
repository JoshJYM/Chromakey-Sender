#ifndef SIGNUPWIDGET_H
#define SIGNUPWIDGET_H

#include <QWidget>

namespace Ui {
class SignupWidget;
}

class SignupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignupWidget(QWidget *parent = 0);
    ~SignupWidget();

private:
    Ui::SignupWidget *ui;
    QByteArray getSignUpData();

private slots:
    void slotApplySignup();
    void slotCancelSignup();
};

#endif // SIGNUPWIDGET_H
