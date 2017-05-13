#include "userlogin.h"
#include "ui_userlogin.h"
#include "userregister.h"

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
