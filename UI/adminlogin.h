#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include <QDialog>

namespace Ui {
class adminlogin;
}

class adminlogin : public QDialog
{
    Q_OBJECT

public:
    explicit adminlogin(QWidget *parent = 0);
    ~adminlogin();

private slots:
    void on_pushButton_3_clicked();

    void on_register_2_clicked();

    void on_loginBtn_clicked();

private:
    Ui::adminlogin *ui;
};

#endif // ADMINLOGIN_H
