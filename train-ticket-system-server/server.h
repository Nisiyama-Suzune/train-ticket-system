#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "map.h"
#include "list.h"

class Server{
private:
	typedef map<int, User> UserContainer; //ID -> account find an account, insert, remove
	typedef map<int, Admin> AdminContainer; //ID -> account
	typedef map<Train_info, Train> TrainContainer; //can be easily changed in the future //mayby map or hash
	typedef map<std::string, Line> LineContainer; //ID -> Line
	typedef map<std::string, City> CityContainer; //name -> City
	typedef list<Log> LogContainer;
	UserContainer     users;
	AdminContainer    admins;
	TrainContainer    trains;
	LineContainer     lines;
	CityContainer     cities;
	LogContainer      logs;
	Account*          now_account;
public:
	bool Register(const std::string& ID, const std::string& password, const std::string& name);
	//if have the same ID, return false, register failed
	Account* login(const std::string &ID, const std::string& password);
	//if login failed, return nullptr
	void import_options(ifstream& fin);
	void import_trains(ifstream& fin);
	void import_accounts(ifstream& fin);
	void startup();
	void exiting(); //before exit, save the train_info and account_info into the files 
	void add_log(Log log);
	//当前内存中log较多的时候(比如达到100条)，丢到文件中去
	friend Admin;
	/**
	 *  friend Admin::check_log(); //virtual functions
	 *  friend Admin::add_line();
	 *  friend Admin::delete_line();
	 *  friend Admin::
	 */
}server;

#endif