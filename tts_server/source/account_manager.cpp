//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/account_manager.h"


void sjtu::User::add_ticket(sjtu::ticket_ptr ticket) {
    for (auto iter = tickets.begin(); iter != tickets.end(); ++iter) {
        if ((*iter)->equal_ex_num(*ticket)) {
            (*iter)->num += ticket->num;
            return;
        }
    }
    tickets.push_back(ticket);
}

bool sjtu::Account::check_password(const QString & other_password) {
    return password == other_password;
}

void sjtu::Account::update_password(const QString &new_password) {
    password = new_password;
}
