#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"
#include "admindelete.h"
#include "adminadd.h"

adminmainwindow::adminmainwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminmainwindow)
{
    ui->setupUi(this);
}

adminmainwindow::~adminmainwindow()
{
    delete ui;
}

void adminmainwindow::on_pushButton_clicked()
{
    admindelete a_del;
    a_del.exec();
}

void adminmainwindow::on_pushButton_2_clicked()
{
    adminadd a_add;
    a_add.exec();
}
