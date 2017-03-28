#include "server_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    server_window w;
    w.show();

    return a.exec();
}
