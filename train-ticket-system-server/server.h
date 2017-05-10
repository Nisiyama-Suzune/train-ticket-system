#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "map.hpp"
#include "list.hpp"
#include "train_manager.h"
#include "account_manager.h"

struct Date;
struct Train_info;
struct Ticket;
struct Station;
struct City;
class Line;
class Train;
class Admin;
class User;
class Account;



class Server{
public:
    // 需要一个wstring[]，表示几等座，从int到wstring

    /// 临时
    typedef int Log;
    typedef int KIND;

private:
	typedef map<int, User> UserContainer; //ID -> account find an account, insert, remove
	typedef map<int, Admin> AdminContainer; //ID -> account
	typedef map<Train_info, Train> TrainContainer; //can be easily changed in the future //mayby map or hash
	typedef map<std::wstring, Line> LineContainer; //ID -> Line
	typedef map<std::wstring, City> CityContainer; //name -> City
	typedef list<Log> LogContainer;
	UserContainer     users;
	AdminContainer    admins;
	TrainContainer    trains;
	LineContainer     lines;
	CityContainer     cities;
	LogContainer      logs;
	Account*          now_account;
public:
	bool Register(const std::wstring& ID, const std::wstring& password, const std::wstring& name);
	//if have the same ID, return false, register failed
	Account* login(const std::wstring &ID, const std::wstring& password);
	//if login failed, return nullptr
	void import_options(std::ifstream& fin);
	void import_trains(std::ifstream& fin);
	void import_accounts(std::ifstream& fin);
	void startup();
	void exiting(); //before exit, save the train_info and account_info into the files 
	void add_log(Log log);
	//当前内存中log较多的时候(比如达到100条)，丢到文件中去
//	friend Admin;
	/**
	 *  friend Admin::check_log(); //virtual functions
	 *  friend Admin::add_line();
	 *  friend Admin::delete_line();
	 *  friend Admin::
	 */

    /// 接口

    Train& get_train(const Train_info & key);
    // 添加一条线路，返回是否成功添加。（如果本来已有，则返回false）
    bool add_line(const Line &x);
    // 如果需要添加train的line不存在，抛出。
    // 如果添加的date已经存在，则返回false
    // 默认_selling = 0
    bool add_train(const std::wstring &name, const Date &date) throw(line_error);

}server;

#endif
