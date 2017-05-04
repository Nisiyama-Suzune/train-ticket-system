#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <fstream>
#include "../vector.h"
#include "list.hpp"
#include "io_utilities.hpp"
#include "utility.hpp"
#include "exceptions.hpp"
#include "train_manager.h"
#include "server.h"

class Account {
protected:
//    typedef map<int, int>::iterator test_iter;
    typedef map<Train_info, Train> info_train_map;
    typedef info_train_map::iterator train_iter;

	std::wstring name;
	std::string ID;
	std::string password;//UI will encrypet it and here save the encrypted password
public:
    Account();
    Account(const std::wstring & name, const std::string& id, const std::string password = "000000");

    void update_password(const std::string& new_password);//double check the password at UI
};

class Admin : public Account
{
public:
	void check_log();
	bool add_line(Line &line);//
	bool add_line(const std::wstring& name, Date date);
	bool delete_line(const std::wstring &name); //Ex. D2333
	bool delete_line(const Train_info& train_info);
	bool delete_line(const std::wstring &name, const Date& date);//same as last one
	void delete_account(const std::wstring &ID);

    // return whether train->selling is modified
    bool start_sell(train_iter train);
    bool end_sell(train_iter train);
};

class User : public Account
{
private:
	list <Ticket> tickets;
	//first, check if the user have bought the same ticket before, ifso, add the num
	//otherwise, append a new node
public:

    // if failed, throw ticket_error()
	void buy_ticket(train_iter train, int from, int to, KIND kind, int num) : throw ticket_error();
    //return the list's which-th ticket, quantity=num
    void return_ticket(int which, int num) : throw ticket_error();

    //using the train_info to find the corresponding train

    // TODO ???
	vector<pair<train_iter, pair<int, int> > 
        query(const std::wstring& from, const std::wstring &to, const Date &date); //query the corresponding tickets
		//only when the train is selling!!

    // return the tickets the user has
    list<Ticket>& current_ticket();
	/*
	query
		string -> city
		merge two city.lines
		Train, start, end
	*/
};

#endif
