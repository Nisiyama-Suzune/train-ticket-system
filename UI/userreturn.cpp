#include "userreturn.h"
#include "ui_userreturn.h"
#include "usermainwindow.h"
#include "userbuy.h"
#include "userticket.h"
#include "userhistory.h"

userreturn::userreturn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userreturn)
{
    ui->setupUi(this);
}

userreturn::~userreturn()
{
    delete ui;
}

void userreturn::on_pushButton_clicked()
{
    usermainwindow u_main;
    u_main.exec();
}

void userreturn::on_pushButton_2_clicked()
{
    userbuy u_buy;
    u_buy.exec();
}

void userreturn::on_pushButton_4_clicked()
{
    userticket u_tic;
    u_tic.exec();
}

void userreturn::on_pushButton_3_clicked()
{
    userhistory u_his;
    u_his.exec();
}
