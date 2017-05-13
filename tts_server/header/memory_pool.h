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
    static vector<void (*)(int)> put; // num -> put_xxx
    

    // containers
    static vector<Station> stations;
    static vector<City>    cities;
    static vector<Line>    lines;
    static vector<Train>   trains;
    static vector<Ticket>  tickets;

    static vector<User>  users;
    static vector<Admin> admins;

    // recycle
    static vector<vector<int>> recycle;

    // cnt
    static vector<vector<int>> cnt;
    
public:
    static pool_ptr<Station> get_station();
    static pool_ptr<City> get_city();
    static pool_ptr<Line> get_line();
    static pool_ptr<Train> get_train();
    static pool_ptr<Ticket> get_ticket(int from = -1, int to = -1, int kind = -1, double price = -1, int num = -1);

    static pool_ptr<User>  get_user();
    static pool_ptr<Admin> get_admin();

public:
    static void init_pool();

private:
    template <typename> friend class pool_ptr;

    static void put_station(int pos);
    static void put_city(int pos);
    static void put_line(int pos);
    static void put_train(int pos);
    static void put_ticket(int pos);

    static void put_user(int pos);
    static void put_admin(int pos);
};



}

#endif //TTS_MEMORY_POOL_H
