#include "admindelete.h"
#include "ui_admindelete.h"
#include "adminmainwindow.h"

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
