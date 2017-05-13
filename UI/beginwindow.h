#ifndef BEGINWINDOW_H
#define BEGINWINDOW_H

#include <QDialog>

namespace Ui {
class beginwindow;
}

class beginwindow : public QDialog
{
    Q_OBJECT

public:
    explicit beginwindow(QWidget *parent = 0);
    ~beginwindow();

private:
    Ui::beginwindow *ui;
};

#endif // BEGINWINDOW_H
