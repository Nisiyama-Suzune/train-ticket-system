#include "userhistory.h"
#include "ui_userhistory.h"

userhistory::userhistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userhistory)
{
    ui->setupUi(this);
}

userhistory::~userhistory()
{
    delete ui;
}
