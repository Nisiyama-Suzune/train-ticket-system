//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/train_manager.h"

sjtu::pool_ptr<sjtu::Station> sjtu::train_memory_pool::get_station() {
    return sjtu::get_T(&stations);
}

sjtu::pool_ptr<sjtu::City> sjtu::train_memory_pool::get_city() {
    return sjtu::get_T(&cities);
}

sjtu::pool_ptr<sjtu::Line> sjtu::train_memory_pool::get_line() {
    return sjtu::get_T(&lines);
}

sjtu::pool_ptr<sjtu::Train> sjtu::train_memory_pool::get_train() {
    return sjtu::get_T(&trains);
}

sjtu::pool_ptr<sjtu::Ticket>
sjtu::train_memory_pool::get_ticket(int from, int to, int kind, double price, int num) {
    return sjtu::get_T(&tickets);
}
