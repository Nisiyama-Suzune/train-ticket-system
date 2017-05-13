//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_ACCOUNT_MANAGER_H
#define TTS_ACCOUNT_MANAGER_H

#include "forward_declaration.h"

#include "train_manager.h"
#include "../../memory.hpp"

#include <string>

/// Account
namespace sjtu {

class Account {
public:
	QString name;
    int ID;
	QString password;

public:
    Account(){}

    Account(const QString &name,
            const int &id,
            const QString password = "000000");

    void update_password(const QString &new_password);
    bool check_password(const QString &other_password);
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

    deque<ticket_ptr> tickets;

public:
    // 会检查是否有相同的票，如果有则只增加其张数。
    void add_ticket(ticket_ptr ticket);
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
