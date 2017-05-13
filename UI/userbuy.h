#ifndef USERBUY_H
#define USERBUY_H

#include <QDialog>

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

private:
    Ui::userbuy *ui;
};

#endif // USERBUY_H
