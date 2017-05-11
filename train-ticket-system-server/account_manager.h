#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <fstream>
#include "vector.hpp"
#include "list.hpp"
#include "io_utilities.hpp"
#include "utility.hpp"
#include "exceptions.hpp"
#include "train_manager.h"
#include "server.h"

struct Date;
struct Ticket;
struct Station;
struct City;
struct Line;
struct Train;

class Account {
protected:
	std::wstring name;
	std::string ID;
	std::string password;//UI will encrypt it and here save the encrypted password
public:
    Account();
    Account(const std::wstring & name,
			const std::string& id,
            const std::string password = "000000");

    //double check the password at UI
    void update_password(const std::string& new_password);
};

class Admin : public Account {
private:
    bool add_line(const Line & line);

public:
	void check_log();

    /// str为操作指令，在public版本中切割str，获得正确的信息

    bool add_line(const std::wstring & str);
	bool add_train(const std::wstring & str);
	bool delete_line(const std::wstring & str);
	bool delete_train(const std::wstring & str);
	void delete_account(const std::wstring & str);

    // return whether train->selling is modified
    bool start_sell(const std::wstring & str);
    bool end_sell(const std::wstring & str);
};

class User : public Account {
private:
	list <Ticket> tickets;
	//first, check if the user have bought the same ticket before, if so, add the num
	//otherwise, append a new node

public:

    // if failed, throw ticket_error()
	void buy_ticket(const std::wstring & str) throw(ticket_error);
    //return the list's which-th ticket, quantity=num
    void return_ticket(const std::wstring & str) throw(ticket_error);

	vector<smart_ptr<Train>> query(const std::wstring & str);
    //query the corresponding tickets
    //only when the train is selling!!

    // return the tickets the user has
    list<Ticket>& current_ticket();
};


#endif
