#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "../vector.h"
#include "../hash_map.h"
#include <string>

struct ticket_data {
    int index;
    int source;
    int destination;
    int count;
};

struct account_data {
    std::string ID;
    std::string password;
    std::string name;
    vector <ticket_data> ticket;
    bool is_deleted;
};

class account_manager
{
private:
    vector <account_data> m_data;
    hash_map <100000, std::string, int> m_hash_table;
    vector <int> m_recycle;
public:
    account_manager();
    int size ();
    const account_data get_data (int index);
    void import_from_file (char *filename); //import train data from a file, append to current data.
    void export_to_file (char *filename); //export current data to a file
    int query (const std::string &ID);
    void add_ticket (int index, const ticket_data &ticket);
    void remove_ticket (int index, int ticket_index);
    bool add_account (account_data data);
    void delete_account (int index);
};

#endif // ACCOUNT_MANAGER_H
