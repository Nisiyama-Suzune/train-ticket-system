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
#include "tts_server/header/memory_pool.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	using namespace sjtu;
	TTS tts;
	pool_ptr<City> tmp;
	tmp = sjtu::memory_pool::get_city();
	QDataStream fin;
	tmp.load<sjtu::memory_pool>(fin);
    return 0;
}
