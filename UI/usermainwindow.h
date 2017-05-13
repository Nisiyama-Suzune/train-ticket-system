#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include <QDialog>

namespace Ui {
class usermainwindow;
}

class usermainwindow : public QDialog
{
    Q_OBJECT

public:
    explicit usermainwindow(QWidget *parent = 0);
    ~usermainwindow();

private slots:
    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::usermainwindow *ui;
};

#endif // USERMAINWINDOW_H
