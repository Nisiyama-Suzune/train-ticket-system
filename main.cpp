#include "mainwindow.h"
#include <QApplication>
//#include "vector.hpp"
//#include "tts_server/header/train_manager.h"
//#include "tts_server/header/account_manager.h"
#include "smart_ptr.hpp"
#include "vector.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include "tts_server/header/server.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	using namespace sjtu;
	TTS tts;

    return 0;
}
