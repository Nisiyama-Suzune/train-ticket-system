#include "userticket.h"
#include "ui_userticket.h"
#include "userbuy.h"
#include "userreturn.h"
#include "userhistory.h"
#include "usermainwindow.h"
#include "../tts_server/header/server.h"
#include <QString>
#include "vector.hpp"
#include <QListWidget>
#include <QListWidgetItem>

extern sjtu::TTS tts;
extern int ID;

userticket::userticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userticket)
{
    ui->setupUi(this);
}

userticket::~userticket()
{
    delete ui;
}

void userticket::on_pushButton_clicked()
{
    userbuy u_buy;
    u_buy.exec();
}

void userticket::on_pushButton_2_clicked()
{
    userreturn u_ret;
    u_ret.exec();
}

void userticket::on_pushButton_5_clicked()
{
    userhistory u_his;
    u_his.exec();
}

void userticket::on_pushButton_3_clicked()
{
    usermainwindow u_main;
    u_main.exec();
}

void userticket::on_pushButton_6_clicked()
{
    sjtu::vector<QString> vec = tts.current_tickets(ID);
    for(int i = 0; i < vec.size(); ++i)
    {
        QListWidgetItem * Qlw = new QListWidgetItem(vec[i], ui->listWidget);
    }
}
