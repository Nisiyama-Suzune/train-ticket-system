#include "userlogin.h"
#include "ui_userlogin.h"
#include "userregister.h"
#include "usermainwindow.h"
#include "../tts_server/header/server.h"
#include <QMessageBox>

extern sjtu::TTS tts;
extern int ID;

userlogin::userlogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userlogin)
{
    ui->setupUi(this);
}

userlogin::~userlogin()
{
    delete ui;
}

void userlogin::on_pushButton_3_clicked()
{
    userregister u_reg;
    u_reg.exec();
}

void userlogin::on_loginBtn_clicked()
{
    if(tts.login_user(ui->userLineEdit->text().toInt(), ui->pwdLineEdit->text()))
    {
        ID = ui->userLineEdit->text().toInt();
        usermainwindow u_main;
        u_main.exec();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("user name or password error!"), QMessageBox::Yes);
    }
}
