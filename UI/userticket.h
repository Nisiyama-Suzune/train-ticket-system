#ifndef USERTICKET_H
#define USERTICKET_H

#include <QDialog>

namespace Ui {
class userticket;
}

class userticket : public QDialog
{
    Q_OBJECT

public:
    explicit userticket(QWidget *parent = 0);
    ~userticket();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::userticket *ui;
};

#endif // USERTICKET_H
