//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/account_manager.h"


void sjtu::User::add_ticket(sjtu::pool_ptr<sjtu::Ticket> ticket) {
    for (auto iter = tickets.begin(); iter != tickets.end(); ++iter) {
        if ((*iter)->equal_ex_num(*ticket)) {
            (*iter)->num += ticket->num;
            return;
        }
    }
    tickets.push_back(ticket);
}

void sjtu::Account::load(QDataStream &in)
{
	in >> name >> ID >> password;
}

void sjtu::Account::save(QDataStream &out)
{
	out << name << ID << password;
}

bool sjtu::Account::check_password(const std::string & other_password) {
    return password == other_password;
}

void sjtu::Account::update_password(const std::string &new_password) {
    password = new_password;
}

void sjtu::User::load(QDataStream &in)
{
	Account.load(in);
	tickets.load(in);
}

void sjtu::User::save(QDataStream &out)
{
	Account.save(out);
	tickets.save(out);
}

void sjtu::Admin::load(QDataStream &in)
{
	Account.load(in);
}

void sjtu::Admin::save(QDataStream &out)
{
	Account.save(out);
}
