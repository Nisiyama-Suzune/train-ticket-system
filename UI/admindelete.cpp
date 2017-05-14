#include "admindelete.h"
#include "ui_admindelete.h"
#include "adminmainwindow.h"
#include "../tts_server/header/server.h"
#include <QMessageBox>

extern sjtu::TTS tts;

admindelete::admindelete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admindelete)
{
    ui->setupUi(this);
}

admindelete::~admindelete()
{
    delete ui;
}

void admindelete::on_pushButton_2_clicked()
{
    adminmainwindow a_main;
    a_main.exec();
}

void admindelete::on_pushButton_6_clicked()
{
    if(tts.delete_line(ui->delete_train->text()))
    {
        QMessageBox::information(this, tr("删除成功"), tr("线路已被移除"), QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(this, tr("删除失败"), tr("你可尝试重新删除"), QMessageBox::Yes);
    }
}
