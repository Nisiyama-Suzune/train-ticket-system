//
// Created by Aaron Ren on 12/05/2017.
//

#include "../header/memory_pool.h"

sjtu::vector<void*>             sjtu::memory_pool::container;
sjtu::vector<void (*)(int)>     sjtu::memory_pool::put;
sjtu::vector<sjtu::vector<int>> sjtu::memory_pool::cnt;
sjtu::vector<sjtu::vector<int>> sjtu::memory_pool::recycle;

sjtu::vector<sjtu::Station> sjtu::memory_pool::stations;
sjtu::vector<sjtu::City>    sjtu::memory_pool::cities;
sjtu::vector<sjtu::Line>    sjtu::memory_pool::lines;
sjtu::vector<sjtu::Train>   sjtu::memory_pool::trains;
sjtu::vector<sjtu::Ticket>  sjtu::memory_pool::tickets;

sjtu::vector<sjtu::User>    sjtu::memory_pool::users;
sjtu::vector<sjtu::Admin>   sjtu::memory_pool::admins;

// station, city, line, train, ticket, user, admin
// 0,       1,    2,    3,     4,      5,    6

sjtu::pool_ptr<sjtu::Station> sjtu::memory_pool::get_station() {
    return sjtu::get_T<Station, memory_pool>();
}

sjtu::pool_ptr<sjtu::City> sjtu::memory_pool::get_city() {
    return sjtu::get_T<City, memory_pool>();
}

sjtu::pool_ptr<sjtu::Line> sjtu::memory_pool::get_line() {
    return sjtu::get_T<Line, memory_pool>();
}

sjtu::pool_ptr<sjtu::Train> sjtu::memory_pool::get_train() {
    return sjtu::get_T<Train, memory_pool>();
}

sjtu::pool_ptr<sjtu::Ticket>
sjtu::memory_pool::get_ticket(int from, int to, int kind, double price, int num) {
    return sjtu::get_T<Ticket, memory_pool>();
}

sjtu::pool_ptr<sjtu::User> sjtu::memory_pool::get_user() {
    return sjtu::get_T<User, memory_pool>();
}

sjtu::pool_ptr<sjtu::Admin> sjtu::memory_pool::get_admin() {
    return sjtu::get_T<Admin, memory_pool>();
}



void sjtu::memory_pool::put_station(int pos) {
    recycle[Station::Type].push_back(pos);
}

void sjtu::memory_pool::put_city(int pos) {
    recycle[City::Type].push_back(pos);
}

void sjtu::memory_pool::put_line(int pos) {
    recycle[Line::Type].push_back(pos);
}

void sjtu::memory_pool::put_train(int pos) {
    recycle[Train::Type].push_back(pos);
}

void sjtu::memory_pool::put_ticket(int pos) {
    recycle[Ticket::Type].push_back(pos);
}

void sjtu::memory_pool::put_user(int pos) {
    recycle[User::Type].push_back(pos);
}

void sjtu::memory_pool::put_admin(int pos) {
    recycle[User::Type].push_back(pos);
}

void sjtu::memory_pool::init_pool() {
    /* station, city, line, train, ticket, user, admin
     * 0,       1,    2,    3,     4,      5,    6
     * All named Type
     */
    container.push_back(&stations);
    container.push_back(&cities);
    container.push_back(&lines);
    container.push_back(&trains);
    container.push_back(&tickets);
    container.push_back(&users);
    container.push_back(&admins);

    put.push_back(put_station);
    put.push_back(put_city);
    put.push_back(put_line);
    put.push_back(put_train);
    put.push_back(put_ticket);
    put.push_back(put_user);
    put.push_back(put_admin);

    for (int i = 0; i <= 6; ++i)
        cnt.push_back(vector<int>());
}




