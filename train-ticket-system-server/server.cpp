#include "server.h"
#include "account_manager.h"

void Server::Register(const std::wstring& ID, const std::wstring& password, const std::wstring& name)
{
    //if insert failed
    if (!users.insert(pair(ID, (User)(ID, password, name))).second)
        throw Have_registered();
}

Account* Server::login(const std::__cxx11::wstring &ID, const std::__cxx11::wstring &password)
{
    Account** ans;
    auto iter = users.find(ID);
    if (iter != users.end()) {
        ans = &(iter -> second);
    }
    return ans;
}

void Server::import_accounts(ifstream &fin)
{

}

void Server::import_options(ifstream &fin)
{

}

void Server::import_trains(ifstream &fin)
{

}
