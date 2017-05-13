#include "usermainwindow.h"
#include "ui_usermainwindow.h"
#include "userbuy.h"
#include "userreturn.h"
#include "userticket.h"
#include "userhistory.h"

usermainwindow::usermainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::usermainwindow)
{
    ui->setupUi(this);
}

usermainwindow::~usermainwindow()
{
    delete ui;
}

void usermainwindow::on_pushButton_10_clicked()
{
    userbuy u_buy;
    u_buy.exec();
}

void usermainwindow::on_pushButton_11_clicked()
{
    userreturn u_ret;
    u_ret.exec();
}

void usermainwindow::on_pushButton_12_clicked()
{
    userticket u_tic;
    u_tic.exec();
}

void usermainwindow::on_pushButton_13_clicked()
{
    userhistory u_his;
    u_his.exec();
}
