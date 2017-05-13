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

private:
    Ui::Adminregister *ui;
};

#endif // ADMINREGISTER_H
