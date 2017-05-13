#include "userregister.h"
#include "ui_userregister.h"

userregister::userregister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userregister)
{
    ui->setupUi(this);
}

userregister::~userregister()
{
    delete ui;
}
