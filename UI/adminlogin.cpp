#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "adminregister.h"

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
