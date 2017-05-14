#include "chooseseat.h"
#include "ui_chooseseat.h"

chooseseat::chooseseat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseseat)
{
    ui->setupUi(this);
}

chooseseat::~chooseseat()
{
    delete ui;
}


