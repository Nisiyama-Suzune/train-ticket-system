#ifndef USERREGISTER_H
#define USERREGISTER_H

#include <QDialog>

namespace Ui {
class userregister;
}

class userregister : public QDialog
{
    Q_OBJECT

public:
    explicit userregister(QWidget *parent = 0);
    ~userregister();

private slots:
    void on_confirm_clicked();

private:
    Ui::userregister *ui;
};

#endif // USERREGISTER_H
