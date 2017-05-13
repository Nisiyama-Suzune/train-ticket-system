#ifndef ADMINREGISTER_H
#define ADMINREGISTER_H

#include <QDialog>

namespace Ui {
class Adminregister;
}

class Adminregister : public QDialog
{
    Q_OBJECT

public:
    explicit Adminregister(QWidget *parent = 0);
    ~Adminregister();

private slots:
    void on_confirm_clicked();

private:
    Ui::Adminregister *ui;
};

#endif // ADMINREGISTER_H
