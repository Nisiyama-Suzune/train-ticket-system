#ifndef USERHISTORY_H
#define USERHISTORY_H

#include <QDialog>

namespace Ui {
class userhistory;
}

class userhistory : public QDialog
{
    Q_OBJECT

public:
    explicit userhistory(QWidget *parent = 0);
    ~userhistory();

private:
    Ui::userhistory *ui;
};

#endif // USERHISTORY_H
