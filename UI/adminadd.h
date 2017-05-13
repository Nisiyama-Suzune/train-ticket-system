#ifndef ADMINADD_H
#define ADMINADD_H

#include <QDialog>

namespace Ui {
class adminadd;
}

class adminadd : public QDialog
{
    Q_OBJECT

public:
    explicit adminadd(QWidget *parent = 0);
    ~adminadd();

private slots:
    void on_pushButton_clicked();

private:
    Ui::adminadd *ui;
};

#endif // ADMINADD_H
