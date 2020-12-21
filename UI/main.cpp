#include "mainwindow.h"
#include <QApplication>
#include "../tts_server/header/account_manager.h"
#include "../tts_server/header/train_manager.h"
#include "../tts_server/header/forward_declaration.h"
#include "../tts_server/header/server.h"
#include "../memory.hpp"

sjtu::TTS tts;
int ID;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
