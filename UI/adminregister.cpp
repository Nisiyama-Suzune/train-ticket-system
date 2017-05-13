#include "adminregister.h"
#include "ui_adminregister.h"
#include "../tts_server/header/server.h"
#include <QLabel>
#include "adminlogin.h"

extern sjtu::TTS tts;

Adminregister::Adminregister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adminregister)
{
    ui->setupUi(this);
}

Adminregister::~Adminregister()
{
    delete ui;
}

void Adminregister::on_confirm_clicked()
{
    int id = tts.register_admin(ui->userLineEdit->text(), ui->pwdLineEdit->text());
    QLabel * label = new QLabel("你的id号是");
    label->show();
    adminlogin a_log;
    a_log.exec();
}
