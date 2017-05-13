#ifndef ADMINDELETE_H
#define ADMINDELETE_H

#include <QDialog>

namespace Ui {
class admindelete;
}

class admindelete : public QDialog
{
    Q_OBJECT

public:
    explicit admindelete(QWidget *parent = 0);
    ~admindelete();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::admindelete *ui;
};

#endif // ADMINDELETE_H
