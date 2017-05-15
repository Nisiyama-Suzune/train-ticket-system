#include "adminadd.h"
#include "ui_adminadd.h"
#include "adminmainwindow.h"
#include <QMessageBox>
#include "../tts_server/header/query.h"
#include "../tts_server/header/server.h"

extern sjtu::TTS tts;

adminadd::adminadd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminadd)
{
    ui->setupUi(this);
}

adminadd::~adminadd()
{
    delete ui;
}

void adminadd::on_pushButton_clicked()
{
    adminmainwindow a_main;
    a_main.exec();
}

void adminadd::on_pushButton_2_clicked()
{
    if(tts.add_line(sjtu::add_line_data(ui->train_name->text(), ui->add_line_info->toPlainText()).toQString()))
    {
        QMessageBox::information(this, tr("添加成功"),tr("新的线路已被载入"),QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(this, tr("添加失败"),tr("您可尝试重新添加"),QMessageBox::Yes);
    }
}
