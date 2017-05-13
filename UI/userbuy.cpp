#include "userbuy.h"
#include "ui_userbuy.h"
#include "usermainwindow.h"
#include "userticket.h"

userbuy::userbuy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userbuy)
{
    ui->setupUi(this);
}

userbuy::~userbuy()
{
    delete ui;
}

void userbuy::on_pushButton_clicked()
{
    usermainwindow u_main;
    u_main.show();
}

void userbuy::on_pushButton_2_clicked()
{
    userticket u_tic;
    u_tic.show();
}
