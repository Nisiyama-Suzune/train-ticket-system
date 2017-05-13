#include "adminadd.h"
#include "ui_adminadd.h"
#include "adminmainwindow.h"

adminadd::adminadd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminadd)
{
    ui->setupUi(this);
}

adminadd::~adminadd()
{
    delete ui;
}

void adminadd::on_pushButton_clicked()
{
    adminmainwindow a_main;
    a_main.exec();
}
