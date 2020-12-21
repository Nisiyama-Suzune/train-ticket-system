#include "beginwindow.h"
#include "ui_beginwindow.h"

beginwindow::beginwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::beginwindow)
{
    ui->setupUi(this);
}

beginwindow::~beginwindow()
{
    delete ui;
}
