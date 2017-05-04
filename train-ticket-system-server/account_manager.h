#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <fstream>
#include "../vector.h"
#include "list.hpp"
#include "io_utilities.hpp"
#include "exceptions.hpp"
#include "train_manager.h"
#include "server.h"

class Account
{
protected:
	std::wstring name;
	std::wstring ID;
	std::wstring password;//UI will encrypet it and here save the encrypted password
public:
    /// save & load
    std::streampos save(std::ofstream & fout) {
        write_str(fout, name);
        write_str(fout, ID);
        write_str(fout, password);
    }
    void load(std::ifstream & fin) {
        read_str(fin, name);
        read_str(fin, ID);
        read_str(fin, password);
    }

	void update_password(const std::wstring& new_password);//double check the password at UI
    Account(const std::wstring & name, const std::wstring& id, const std::wstring password = "000000");

	virtual void check_log(); // only Admin have these virtual functions
	virtual bool add_line(Line &line); //maybe we can let the orginal virtual functions puts"You don't have the permission!"?
	virtual bool add_line(const std::wstring& name, Date date);
	virtual bool delete_line(const std::wstring& name);
	virtual void delete_account(const std::wstring& ID);
	virtual void start_sell(weak_ptr<Train> train);
	virtual void end_sell(weak_ptr<Train> train);

	//User's rights
	virtual bool buy_ticket(weak_ptr<Train> train, int from, int to, KIND kind, int num);
	virtual bool return_ticket(int which, int num);
	virtual vector<pair<weak_ptr<Train>, pair<int,int>>
		query(const std::wstring& from, const std::wstring& to, const Date& date);
};

class Admin : public Account
{
public:
    // TODO ALL
	void check_log();
	bool add_line(Line &line);//
	bool add_line(const std::wstring& name, Date date);
	bool delete_line(const std::wstring &name); //Ex. D2333
	bool delete_line(const Train_info& train_info);
	bool delete_line(const std::wstring &name, const Date& date);//same as last one
	void delete_account(const std::wstring &ID);

    // return whether train->selling is modified
    bool start_sell(weak_ptr<Train> train);
    bool end_sell(weak_ptr<Train> train);
};

class User : public Account
{
private:
	list <Ticket> tickets;
	//first, check if the user have bought the same ticket before, ifso, add the num
	//otherwise, append a new node
public:
    /// save & load
    std::streampos save(std::ofstream & fout) {
        return write_list(fout, tickets, nontrivial_sl);
    }
    void load(std::ifstream & fin) {
        read_list(fin, tickets, nontrivial_sl);
    }

	bool buy_ticket(weak_ptr<Train> train, int from, int to, KIND kind, int num);
    bool return_ticket(int which, int num); //return the list's which-th ticket, quantity=num
	//using the train_info to find the corresponding train

	vector<pair<weak_ptr<Train>, pair<int, int> > 
        query(const std::wstring& from, const std::wstring &to, const Date &date); //query the corresponding tickets
		//only when the train is selling!!

    // return the tickets the user has
    list<Ticket>& query_ticket() {
		return tickets;
    }
	/*
	query
		string -> city
		merge two city.lines
		Train, start, end
	*/
};

#endif
