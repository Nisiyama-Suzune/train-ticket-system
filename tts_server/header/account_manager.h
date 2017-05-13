//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_ACCOUNT_MANAGER_H
#define TTS_ACCOUNT_MANAGER_H

namespace sjtu {
struct Ticket;
struct Train;
struct Line;
struct City;
struct Date;
struct Station;
class Account;
class User;
class Admin;
}
#include "train_manager.h"

#include <string>

/// Account
namespace sjtu {

class Account {
protected:
	QString name;
	QString ID;
	QString password;

public:
    Account(){}

    Account(const std::wstring &name,
            const std::string &id,
            const std::string password = "000000");

    void update_password(const std::string &new_password);
    bool check_password(const std::string &other_password);
	friend QDataStream& operator >> (QDataStream &in, Account &rhs) {
		in >> rhs.name >> rhs.ID >> rhs.password;
		return in;
	}
	friend QDataStream& operator << (QDataStream &out, const Account &rhs) {
		out << rhs.name << rhs.ID << rhs.password;
		return out;
	}
};

class User : public Account {
public:
    static const int Type = 5;

    list<pool_ptr<Ticket>> tickets;

public:
    // 会检查是否有相同的票，如果有则只增加其张数。
    void add_ticket(pool_ptr<Ticket> ticket);
	friend QDataStream& operator >> (QDataStream &in, User &rhs) {
		in >> rhs.name >> rhs.ID >> rhs.password;
		return in;
	}
	friend QDataStream& operator << (QDataStream &out, const User &rhs) {
		out << rhs.name << rhs.ID << rhs.password;
		return out;
	}
};

class Admin : public Account {
public:
    static const int Type = 6;
	friend QDataStream& operator >> (QDataStream &in, Admin &rhs) {
		in >> rhs.name >> rhs.ID >> rhs.password;
		return in;
	}
	friend QDataStream& operator << (QDataStream &out, const Admin &rhs) {
		out << rhs.name << rhs.ID << rhs.password;
		return out;
	}
};

}


#endif //TTS_ACCOUNT_MANAGER_H
