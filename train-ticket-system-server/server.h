#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "map.hpp"
#include "list.hpp"
#include "train_manager.h"
#include "account_manager.h"

struct Date;
struct Ticket;
struct Station;
struct City;
struct Line;
struct Train;
class Admin;
class User;
class Account;



class Server {
private:
	typedef map<int, User> UserContainer; //ID -> account find an account, insert, remove
	typedef map<int, Admin> AdminContainer; //ID -> account
	typedef map<std::string, Line> LineContainer; //ID -> Line
	typedef map<std::wstring, City> CityContainer; //name -> City
//	typedef list<Log> LogContainer;
	UserContainer     users;
	AdminContainer    admins;
	LineContainer     lines;
	CityContainer     cities;
//	LogContainer      logs;

    smart_ptr<User> current_user;
    smart_ptr<Admin> current_admin;

public:
    // register, login
    bool account_register(const std::wstring & str);
    bool login(const std::wstring & str);

	void import_options(std::ifstream& fin);
	void import_trains(std::ifstream& fin);
	void import_accounts(std::ifstream& fin);
	void startup();
	void exiting(); //before exit, save the train_info and account_info into the files 
//	void add_log(Log log);
	//当前内存中log较多的时候(比如达到100条)，丢到文件中去


    /// 接口
    vector<sma>

}server;

#endif
