#include "account_manager.h"

/// Account
Account::Account() {}
Account::Account(const std::wstring &name, const std::string &id, const std::string password)
 : name(name), ID(id), password(password){}
void Account::update_password(const std::string &new_password) {
    password = new_password;
}
