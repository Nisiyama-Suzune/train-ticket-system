#include "userregister.h"
#include "ui_userregister.h"
#include "userlogin.h"
#include "../tts_server/header/server.h"

extern sjtu::TTS tts;

userregister::userregister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userregister)
{
    ui->setupUi(this);
}

userregister::~userregister()
{
    delete ui;
}

void userregister::on_confirm_clicked()
{
    int id = tts.register_user(ui->userLineEdit->text(), ui->pwdLineEdit->text());
    QLabel *label = new QLabel("你的id号是");
    userlogin u_log;
    u_log.exec();
}
