#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "adminregister.h"
#include "adminmainwindow.h"
#include "../tts_server/header/server.h"
#include <QMessageBox>

extern sjtu::TTS tts;
extern int ID;

adminlogin::adminlogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminlogin)
{
    ui->setupUi(this);
}

adminlogin::~adminlogin()
{
    delete ui;
}

void adminlogin::on_pushButton_3_clicked()
{
    Adminregister a_reg;
    a_reg.exec();
}

void adminlogin::on_loginBtn_clicked()
{
    if(tts.login_admin(ui->userLineEdit->text().toInt(),ui->pwdLineEdit->text() ))
    {
        ID = ui->userLineEdit->text().toInt();
        adminmainwindow a_main;
        a_main.exec();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("user name or password error!"), QMessageBox::Yes);
    }
}
