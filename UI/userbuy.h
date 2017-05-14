#ifndef USERBUY_H
#define USERBUY_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class userbuy;
}

class userbuy : public QDialog
{
    Q_OBJECT

public:
    explicit userbuy(QWidget *parent = 0);
    ~userbuy();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_search_by_station_clicked();

    void on_search_by_city_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::userbuy *ui;
};

#endif // USERBUY_H
