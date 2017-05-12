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
    /* station, city, line, train, ticket, user, admin
     * 0,       1,    2,    3,     4,      5,    6
     * All named Type
     */

    // map
    static vector<void*> container; // num -> containers
    static vector<void (*)(size_t)> put; // num -> put_xxx


    // containers
    static vector<Station> stations;
    static vector<City>    cities;
    static vector<Line>    lines;
    static vector<Train>   trains;
    static vector<Ticket>  tickets;

    static vector<User>  users;
    static vector<Admin> admins;

    // recycle
    static vector<size_t> stations_recycle;
    static vector<size_t> cities_recycle;
    static vector<size_t> lines_recycle;
    static vector<size_t> trains_recycle;
    static vector<size_t> tickets_recycle;

    static vector<size_t> users_recycle;
    static vector<size_t> admins_recycle;

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

public:
    static void init_pool();

private:
    template <typename> friend class pool_ptr;

    static void put_station(size_t pos);
    static void put_city(size_t pos);
    static void put_line(size_t pos);
    static void put_train(size_t pos);
    static void put_ticket(size_t pos);

    static void put_user(size_t pos);
    static void put_admin(size_t pos);
};



}

#endif //TTS_MEMORY_POOL_H
