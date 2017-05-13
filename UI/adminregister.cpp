#include "adminregister.h"
#include "ui_adminregister.h"

Adminregister::Adminregister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adminregister)
{
    ui->setupUi(this);
}

Adminregister::~Adminregister()
{
    delete ui;
}
