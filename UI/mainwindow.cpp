#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userlogin.h"
#include "adminlogin.h"
#include "adminregister.h"
#include "userregister.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    userlogin u_log;
    u_log.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    adminlogin a_log;
    a_log.exec();
}

void MainWindow::on_commandLinkButton_clicked()
{
    userregister u_reg;
    u_reg.exec();
}

void MainWindow::on_commandLinkButton_2_clicked()
{
    Adminregister a_reg;
    a_reg.exec();
}
