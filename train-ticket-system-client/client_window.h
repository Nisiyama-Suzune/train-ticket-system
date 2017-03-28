#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include <QMainWindow>

namespace Ui {
class client_window;
}

class client_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit client_window(QWidget *parent = 0);
    ~client_window();

private:
    Ui::client_window *ui;
};

#endif // CLIENT_WINDOW_H
