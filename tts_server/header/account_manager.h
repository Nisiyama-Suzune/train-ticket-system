//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_ACCOUNT_MANAGER_H
#define TTS_ACCOUNT_MANAGER_H

#include "train_manager.h"

#include <string>

/// Account
namespace sjtu {

class Account {
protected:
    std::wstring name;
    std::string ID;
    std::string password;

public:
    Account(){}

    Account(const std::wstring &name,
            const std::string &id,
            const std::string password = "000000");

    void update_password(const std::string &new_password);
    bool check_password(const std::string &other_password);
};

class Admin : public Account {

};

class User : public Account {
public:
    list<pool_ptr<Ticket>> tickets;

public:
    // 会检查是否有相同的票，如果有则只增加其张数。
    void add_ticket(pool_ptr<Ticket> ticket);

};

}


#endif //TTS_ACCOUNT_MANAGER_H
