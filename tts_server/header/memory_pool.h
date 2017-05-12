//
// Created by Aaron Ren on 12/05/2017.
//

#ifndef TTS_MEMORY_POOL_H
#define TTS_MEMORY_POOL_H


class memory_pool {

};

#include "train_manager.h"
#include "account_manager.h"

/// memory_pool
namespace sjtu {

class memory_pool {
private:

    static vector<Station> stations;
    static vector<City>    cities;
    static vector<Line>    lines;
    static vector<Train>   trains;
    static vector<Ticket>  tickets;

    static vector<User>  users;
    static vector<Admin> admins;

    static vector<int> ints;

public:
    static pool_ptr<Station> get_station();
    static pool_ptr<City> get_city();
    static pool_ptr<Line> get_line();
    static pool_ptr<Train> get_train();
    static pool_ptr<Ticket> get_ticket(int from = -1, int to = -1, int kind = -1, double price = -1, int num = -1);


    static pool_ptr<User>  get_user() {
        users.push_back(User());
        return pool_ptr<User>(users.size() - 1, &users);
    }
    static pool_ptr<Admin> get_admin() {
        admins.push_back(Admin());
        return pool_ptr<Admin>(admins.size() - 1, &admins);
    }

    static void get_int() {
        ints.push_back(1);
    }
};



}

#endif //TTS_MEMORY_POOL_H
