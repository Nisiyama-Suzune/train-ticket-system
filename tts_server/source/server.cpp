//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/server.h"

/// server

sjtu::pool_ptr<sjtu::City> sjtu::Server::find_city(const sjtu::Server::Qstring &name) const {
    auto result = cities.find(name);
    if (result == cities.cend())
        throw exception("city", "does not exist.");
    return result->second;
}

sjtu::pool_ptr<sjtu::User> sjtu::Server::find_user(const int &ID) const {
    auto user = users.find(ID);
    if (user == users.cend()) {
        throw exception("user", "does not exist.");
    }
    return user->second;
}

sjtu::pool_ptr<sjtu::Admin> sjtu::Server::find_admin(const int &ID) const {
    auto admin = admins.find(ID);
    if (admin == admins.cend()) {
        throw exception("admin", "does not exist");
    }
    return admin->second;
}

bool sjtu::Server::check_city(const sjtu::Server::Qstring &name) const {
    return cities.find(name) != cities.cend();
}

bool sjtu::Server::check_user(const int &ID) const {
    return users.find(ID) != users.cend();
}

bool sjtu::Server::check_admin(const int &ID) const {
    return admins.find(ID) != admins.cend();
}









/// TTS

smart_ptr<sjtu::vector<sjtu::pool_ptr<sjtu::Train>>>
sjtu::TTS::query_train(const sjtu::City &from, const sjtu::City &to, sjtu::Date date) const {
    /* 实现：
     * 首先枚举出发城市中的每一个车站，再枚举经过这个车站的每一条线路，再枚举线路上的每一个车站，
     * 检查它所在的城市是否是目的地。
     * 其中如果当前线路在该日期没有车次，则跳过。（有date的情况）
     */

    smart_ptr<vector<pool_ptr<Train>>> result(new vector<pool_ptr<Train>>);

    vector<pool_ptr<Station>>::const_iterator from_station;
    for (from_station = from.stations.cbegin(); from_station != from.stations.cend() ; ++from_station) {
        vector<pool_ptr<Line>>::const_iterator cur_line;
        for (cur_line = (*from_station)->lines.cbegin(); cur_line != (*from_station)->lines.cend(); ++cur_line) {
            if (!(*cur_line)->check_date(date)) {
                continue;
            }
            vector<pool_ptr<Station>>::const_iterator cur_station = (*cur_line)->stations.cbegin();
            while ((*cur_station)->name != (*from_station)->name) {
                ++cur_station;
            }
            while (cur_station != (*cur_line)->stations.cend()) {
                if ((*cur_station)->location->name != to.name) {
                    break;
                }
                ++cur_station;
            }
            if (cur_station != (*cur_line)->stations.cend()) {
                result->push_back((*cur_line)->trains[date]);
            }
        }
    }

    return result;
}

/// User
bool sjtu::TTS::buy_ticket(sjtu::pool_ptr<sjtu::Train> train, int from, int to, int kind, int num) {
    vector<vector<double>> &price = train->line->price;
    vector<vector<int>> &remaining = train->station_available_tickets;
    if (!train->selling)
        return false;

    // 检查票是否足够
    for (int i = from; i < to; ++i) {
        if (remaining[i][kind] < num)
            return false;
    }

    // 更新余票，计算价格
    double ticket_price = 0;
    for (int i = from; i < to; ++i) {
        remaining[i][kind] -= num;
        ticket_price += price[i][kind];
    }

    // 更新账户
    pool_ptr<Ticket> ticket = t_m_p.get_ticket(from, to, kind, ticket_price, num);
    ticket->train = train;
    current_user->add_ticket(ticket);
    return true;
}

bool sjtu::TTS::return_ticket(sjtu::pool_ptr<sjtu::Ticket> ticket, int num) {
    list<pool_ptr<Ticket>> &tickets = current_user->tickets;
    for (auto iter = tickets.begin(); iter != tickets.end(); ++iter) {
        if ((*iter)->equal_ex_num(*ticket)) {
            if ((*iter)->num < num)
                return false;
            (*iter)->num -= num;
            return true;
        }
    }
    return false;
}

const sjtu::list<sjtu::pool_ptr<sjtu::Ticket>> &sjtu::TTS::current_tickets() {
    return current_user->tickets;
}

bool sjtu::TTS::login_user(const int & ID, const std::string password) {
    pool_ptr<User> user = server.find_user(ID);
    if (user->check_password(password)) {
        current_user = user;
        current_admin = pool_ptr<Admin>();
        return true;
    }
    return false;
}

bool sjtu::TTS::login_admin(const int &ID, const std::string password) {
    pool_ptr<Admin> admin = server.find_admin(ID);
    if (admin->check_password(password)) {
        current_admin = admin;
        current_user = pool_ptr<User>();
        return true;
    }
    return false;
}

