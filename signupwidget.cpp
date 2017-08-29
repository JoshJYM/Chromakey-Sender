#include "signupwidget.h"
#include "ui_signupwidget.h"

SignupWidget::SignupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignupWidget)
{
    ui->setupUi(this);

    connect(ui->signupBtn,SIGNAL(clicked()),SLOT(slotApplySignup()));
    connect(ui->cancelBtn,SIGNAL(clicked()),SLOT(slotCancelSignup()));
}

SignupWidget::~SignupWidget()
{
    delete ui;
    qDebug("delete SignupWidget...");
}

void SignupWidget::slotApplySignup()
{
    QByteArray signupPacket;

    signupPacket = getSignUpData();

    /*
     * sende data to server code
     * */
}

void SignupWidget::slotCancelSignup()
{
    this->close();
}

QByteArray SignupWidget::getSignUpData()
{
    QByteArray signupDataPacket;

    QString email           = ui->email->text();
    QString passward        = ui->password->text();
    QString passwardRecheck = ui->passwordRecheck->text();
    QString name            = ui->name->text();
    QString nickname        = ui->nickname->text();
    QString sex = "null";

    if(ui->manCheck->isChecked())
        sex = "M";
    if(ui->womenCheck->isCheckable())
        sex = "W";

    signupDataPacket.append(email);
    signupDataPacket.push_back('\0');
    signupDataPacket.append(passward);
    signupDataPacket.push_back('\0');
    signupDataPacket.append(passwardRecheck);
    signupDataPacket.push_back('\0');
    signupDataPacket.append(name);
    signupDataPacket.push_back('\0');
    signupDataPacket.append(nickname);
    signupDataPacket.push_back('\0');
    signupDataPacket.append(sex);
    signupDataPacket.push_back('\0');

    return signupDataPacket;
}
