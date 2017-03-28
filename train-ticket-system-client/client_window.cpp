#include "client_window.h"
#include "ui_client_window.h"

client_window::client_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client_window)
{
    ui->setupUi(this);
}

client_window::~client_window()
{
    delete ui;
}
