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

class User : public Account {
public:
    static const int Type = 5;

    deque<ticket_ptr> tickets;

public:
    // 会检查是否有相同的票，如果有则只增加其张数。
    void add_ticket(ticket_ptr ticket);

};

class Admin : public Account {
public:
    static const int Type = 6;
};

}


#endif //TTS_ACCOUNT_MANAGER_H
