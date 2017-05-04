#include "account_manager.h"

/// Account
Account::Account() {}
Account::Account(const std::wstring &name, const std::string &id, const std::string password)
 : name(name), ID(id), password(password){}
void Account::update_password(const std::string &new_password) {
    password = new_password;
}


/// Admin
bool Admin::start_sell(Account::train_iter train) {
    if (train->second.selling())
        return 0;
    train->second.change_selling();
    return 1;
}

bool Admin::end_sell(Account::train_iter train) {
    if (!train->second.selling())
        return 0;
    train->second.change_selling();
    return false;
}


/// User
void User::buy_ticket(Account::train_iter info_train, int from, int to, KIND kind, int num) {
    Train &train = info_train->second;
    if (!train.have_ticket(from, to, kind, num))
        throw ticket_error();
    train.add_remaining_tickets(from, to, kind, -num);
}

void User::return_ticket(int which, int num) {
    list<Ticket>::iterator iter;
    try {
        // 没有这么多票
        iter = tickets.at_iter(which);
    } catch (...) {
        throw ticket_error();
    }
    if (iter->num < num)
        throw ticket_error();

    Train &train = server.get_train(iter->train_info);
    train.add_remaining_tickets(iter->from, iter->to, iter->kind, num);
}

list<Ticket> &User::current_ticket() {
    return tickets;
}

