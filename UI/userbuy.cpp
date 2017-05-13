#include "userbuy.h"
#include "ui_userbuy.h"
#include "usermainwindow.h"
#include "userticket.h"
#include "../tts_server/header/server.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "vector.hpp"
#include <QString>
#include "chooseseat.h"

extern sjtu::TTS tts;

userbuy::userbuy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userbuy)
{
    ui->setupUi(this);
}

userbuy::~userbuy()
{
    delete ui;
}

void userbuy::on_pushButton_clicked()
{
    usermainwindow u_main;
    u_main.show();
}

void userbuy::on_pushButton_2_clicked()
{
    userticket u_tic;
    u_tic.show();
}

void userbuy::on_comboBox_activated(const QString &arg1)
{

}

void userbuy::on_pushButton_6_clicked()
{

}

void userbuy::on_search_by_station_clicked()
{
    sjtu::vector<QString> vec = tts.q_ss(ui->start_by_station->currentText(), ui->end_by_station->currentText(), ui->time_by_station->currentData().toInt());
    for(int i = 0; i < vec.size(); ++i)
    {
        QListWidgetItem * Qlw = new QListWidgetItem(vec[i], ui->listWidget);
    }
}

void userbuy::on_search_by_city_clicked()
{
    sjtu::vector<QString> vec = tts.query_city_city(ui->start_by_city->currentText(), ui->end_by_city->currentText(), ui->time_by_city->currentData().toInt());
    for(int i = 0; i < vec.size(); ++i)
    {
        QListWidgetItem * Qlw = new QListWidgetItem(vec[i], ui->listWidget);
    }
}

void userbuy::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    chooseseat chs;
    chs.exec();
}
