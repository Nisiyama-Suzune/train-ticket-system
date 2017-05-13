//
// Created by Aaron Ren on 13/05/2017.
//

#ifndef TTS_FORWARD_DECLARATION_H
#define TTS_FORWARD_DECLARATION_H


#include "../../memory.hpp"

namespace sjtu {

class Account;
class User;     // 5
class Admin;    // 6
struct Date;
struct Station; // 0
struct City;    // 1
struct Line;    // 2
struct Train;   // 3
struct Ticket;  // 4

typedef memory_pool<User>::pool_ptr    user_ptr;
typedef memory_pool<Admin>::pool_ptr   admin_ptr;
typedef memory_pool<Station>::pool_ptr station_ptr;
typedef memory_pool<City>::pool_ptr    city_ptr;
typedef memory_pool<Line>::pool_ptr    line_ptr;
typedef memory_pool<Train>::pool_ptr   train_ptr;
typedef memory_pool<Ticket>::pool_ptr  ticket_ptr;
}


#endif //TTS_FORWARD_DECLARATION_H
