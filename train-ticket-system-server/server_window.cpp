#include "server_window.h"
#include "ui_server_window.h"

server_window::server_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server_window)
{
    ui->setupUi(this);
}

server_window::~server_window()
{
    delete ui;
}
