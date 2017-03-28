#ifndef SERVER_WINDOW_H
#define SERVER_WINDOW_H

#include <QMainWindow>

namespace Ui {
class server_window;
}

class server_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit server_window(QWidget *parent = 0);
    ~server_window();

private:
    Ui::server_window *ui;
};

#endif // SERVER_WINDOW_H
