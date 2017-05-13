#include "userticket.h"
#include "ui_userticket.h"
#include "userbuy.h"
#include "userreturn.h"
#include "userhistory.h"
#include "usermainwindow.h"

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
