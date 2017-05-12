//
// Created by Aaron Ren on 12/05/2017.
//

#include "../header/memory_pool.h"

sjtu::vector<void*>            sjtu::memory_pool::container;
sjtu::vector<void (*)(size_t)> sjtu::memory_pool::put;

sjtu::vector<sjtu::Station> sjtu::memory_pool::stations;
sjtu::vector<sjtu::City>    sjtu::memory_pool::cities;
sjtu::vector<sjtu::Line>    sjtu::memory_pool::lines;
sjtu::vector<sjtu::Train>   sjtu::memory_pool::trains;
sjtu::vector<sjtu::Ticket>  sjtu::memory_pool::tickets;

sjtu::vector<sjtu::User>    sjtu::memory_pool::users;
sjtu::vector<sjtu::Admin>   sjtu::memory_pool::admins;

sjtu::vector<size_t>        sjtu::memory_pool::stations_recycle;
sjtu::vector<size_t>        sjtu::memory_pool::cities_recycle;
sjtu::vector<size_t>        sjtu::memory_pool::lines_recycle;
sjtu::vector<size_t>        sjtu::memory_pool::trains_recycle;
sjtu::vector<size_t>        sjtu::memory_pool::tickets_recycle;

sjtu::vector<size_t>        sjtu::memory_pool::users_recycle;
sjtu::vector<size_t>        sjtu::memory_pool::admins_recycle;

sjtu::pool_ptr<sjtu::Station> sjtu::memory_pool::get_station() {
    return sjtu::get_T(&stations, put_station);
}

sjtu::pool_ptr<sjtu::City> sjtu::memory_pool::get_city() {
    return sjtu::get_T(&cities, put_city);
}

sjtu::pool_ptr<sjtu::Line> sjtu::memory_pool::get_line() {
    return sjtu::get_T(&lines, put_line);
}

sjtu::pool_ptr<sjtu::Train> sjtu::memory_pool::get_train() {
    return sjtu::get_T(&trains, put_train);
}

sjtu::pool_ptr<sjtu::Ticket>
sjtu::memory_pool::get_ticket(int from, int to, int kind, double price, int num) {
    return sjtu::get_T(&tickets, put_ticket);
}

sjtu::pool_ptr<sjtu::User> sjtu::memory_pool::get_user() {
    return sjtu::get_T(&users, put_user);
}

sjtu::pool_ptr<sjtu::Admin> sjtu::memory_pool::get_admin() {
    return sjtu::get_T(&admins, put_admin);
}



void sjtu::memory_pool::put_station(size_t pos) {
    stations_recycle.push_back(pos);
}

void sjtu::memory_pool::put_city(size_t pos) {
    cities_recycle.push_back(pos);
}

void sjtu::memory_pool::put_line(size_t pos) {
    lines_recycle.push_back(pos);
}

void sjtu::memory_pool::put_train(size_t pos) {
    trains_recycle.push_back(pos);
}

void sjtu::memory_pool::put_ticket(size_t pos) {
    tickets_recycle.push_back(pos);
}

void sjtu::memory_pool::put_user(size_t pos) {
    users_recycle.push_back(pos);
}

void sjtu::memory_pool::put_admin(size_t pos) {
    admins_recycle.push_back(pos);
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
}




