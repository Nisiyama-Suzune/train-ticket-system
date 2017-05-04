#include "account_manager.h"

void Account::check_log() {
    throw no_permission();
}
bool Account::add_line(Line &line) {
    throw no_permission();
}
bool Account::delete_line(const std::wstring& name) {
    throw no_permission();
}
void Account::delete_account(const std::wstring& ID) {
    throw no_permission();
}
void Account::start_sell(weak_ptr<Train> train) {
    throw no_permission();
}
void Account::end_sell(weak_ptr<Train> train) {
    throw no_permission();
}

bool Account::buy_ticket(weak_ptr<Train> train, int from, int to, KIND kind, int num) {
    throw no_permission();
}
bool Acccount::return_ticket(int which, int num) {
    throw no_permission();
}
vector<pair<weak_ptr<Train>, pair<int,int>>
Account::query(const std::wstring& from, const std::wstring& to, const Date& date) {
    throw no_permission();
}

void Account::update_password(const std::wstring& new_password) {
    password = new_password;
}
Account::Account(const std::wstring & name, const std::wstring& id, const std::wstring password= "000000")
    : name(name), ID(id), password(password) {}


// user
bool User::buy_ticket(weak_ptr<Train> train, int from, int to, KIND kind, int num) {
    if (!train.have_ticket(from, to, kind, num))
        return false;
    // nun \in Z
    train.add_ramaining_tickets(from, to, kind, -num);
    return true;
}

bool User::return_ticket(int num, int which) {
    try {
        list<Ticket>::iterator iter = tickets.at_iter(which);
    } catch(...) {
        return false;
    }
    if (iter->num < num)
        return false;
    (server.get_train(iter->train_info)).add_remaining_tickets(iter->from, iter->to, iter->kind, num);

    return true;
}

// admin
bool Admin::start_sell(weak_ptr<Train> train) {
    if (train->selling())
        return 0;
    train->change_selling();

}
