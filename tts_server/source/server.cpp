//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/server.h"

/// server

sjtu::city_ptr sjtu::Server::find_city(const sjtu::Server::QString &name) const {
    auto result = cities.find(name);
    if (result == cities.cend())
        throw exception("city", "does not exist.");
    return result->second;
}

sjtu::user_ptr sjtu::Server::find_user(const int &ID) const {
    auto user = users.find(ID);
    if (user == users.cend()) {
        throw exception("user", "does not exist.");
    }
    return user->second;
}

sjtu::admin_ptr sjtu::Server::find_admin(const int &ID) const {
    auto admin = admins.find(ID);
    if (admin == admins.cend()) {
        throw exception("admin", "does not exist");
    }
    return admin->second;
}

sjtu::station_ptr sjtu::Server::find_station(const sjtu::QString &name) const {
    auto station = stations.find(name);
    if (station == stations.cend())
        throw exception("station", "does not exist");
    return station->second;
}

sjtu::line_ptr sjtu::Server::find_line(const sjtu::QString &name) const {
    auto line = lines.find(name);
    if (line == lines.cend())
        throw exception("line", "does not exist");
    return line->second;
}


bool sjtu::Server::check_city(const sjtu::Server::QString &name) const {
    return cities.find(name) != cities.cend();
}

bool sjtu::Server::check_user(const int &ID) const {
    return users.find(ID) != users.cend();
}

bool sjtu::Server::check_admin(const int &ID) const {
    return admins.find(ID) != admins.cend();
}

bool sjtu::Server::check_station(const sjtu::QString &name) const {
    return stations.find(name) != stations.cend();
}

bool sjtu::Server::check_line(const sjtu::QString &name) const {
    return lines.find(name) != lines.cend();
}

bool sjtu::Server::add_line(const sjtu::line_ptr &line) {
    return lines.insert(sjtu::make_pair(line->name, line)).second;
}

bool sjtu::Server::add_station(const sjtu::station_ptr &station) {
    return stations.insert(sjtu::make_pair(station->name, station)).second;
}

bool sjtu::Server::add_city(const sjtu::city_ptr &city) {
    return cities.insert(sjtu::make_pair(city->name, city)).second;
}







/// TTS

smart_ptr<sjtu::vector<sjtu::train_ptr>>
sjtu::TTS::query_train(const sjtu::City &from, const sjtu::City &to, sjtu::Date date) const {
    /* 实现：
     * 首先枚举出发城市中的每一个车站，再枚举经过这个车站的每一条线路，再枚举线路上的每一个车站，
     * 检查它所在的城市是否是目的地。
     * 其中如果当前线路在该日期没有车次，则跳过。（有date的情况）
     */

    smart_ptr<vector<train_ptr>> result(new vector<train_ptr>);

    vector<station_ptr>::const_iterator from_station;
    for (from_station = from.stations.cbegin(); from_station != from.stations.cend() ; ++from_station) {
        vector<line_ptr>::const_iterator cur_line;
        for (cur_line = (*from_station)->lines.cbegin(); cur_line != (*from_station)->lines.cend(); ++cur_line) {
            if (!(*cur_line)->check_date(date)) {
                continue;
            }
            vector<station_ptr>::const_iterator cur_station = (*cur_line)->stations.cbegin();
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

bool sjtu::TTS::buy_ticket(sjtu::train_ptr train, int from, int to, int kind, int num) {
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
    ticket_ptr ticket = memory_pool<Ticket>::get_T();//(from, to, kind, ticket_price, num);
    ticket->from  = from;
    ticket->to    = to;
    ticket->kind  = kind;
    ticket->price = ticket_price;
    ticket->num   = num;
    ticket->train = train;
    current_user->add_ticket(ticket);
    return true;
}

bool sjtu::TTS::return_ticket(sjtu::ticket_ptr ticket, int num) {
    deque<ticket_ptr> &tickets = current_user->tickets;
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

const sjtu::deque<sjtu::ticket_ptr> &sjtu::TTS::current_tickets() {
    return current_user->tickets;
}

bool sjtu::TTS::login_user(const int & ID, const std::string password) {
    user_ptr user = server.find_user(ID);
    if (user->check_password(password)) {
        current_user = user;
        current_admin = admin_ptr();
        return true;
    }
    return false;
}

bool sjtu::TTS::login_admin(const int &ID, const std::string password) {
    admin_ptr admin = server.find_admin(ID);
    if (admin->check_password(password)) {
        current_admin = admin;
        current_user = user_ptr();
        return true;
    }
    return false;
}

bool sjtu::TTS::add_line(const sjtu::TTS::LineData &line_data) {
    // station
    line_ptr line = memory_pool<Line>::get_T();
    line->name = line_data.name;
    line->seat_kind_names = line_data.seat_kind_names;
    line->miles = line_data.miles;
    line->price = line_data.prices;
    for (int i = 0; i < line_data.time_arrive.size(); ++i) {
        line->arr_time.push_back(line_data.time_arrive[i]);
        line->dep_time.push_back(line_data.time_stop[i]);
    }
    for (int i = 0; i < line_data.stations.size(); ++i) {
        const QString &station_name = line_data.stations[i];
        if (!server.check_station(station_name))
            add_station(StationData(station_name, station_name));

        line->stations.push_back(
                server.find_station(station_name));
        line->stations.back()->lines.push_back(line);
    }

    return server.add_line(line);
}

bool sjtu::TTS::add_station(const sjtu::TTS::StationData &station_data) {
    station_ptr station = memory_pool<Station>::get_T();
    station->name = station_data.name;
    if (!server.check_city(station_data.location))
        add_city(CityData(station_data.location));
    station->location = server.find_city(station_data.location);
    station->location->stations.push_back(station);
    return server.add_station(station);
}

bool sjtu::TTS::add_city(const sjtu::TTS::CityData &city_data) {
    city_ptr city = memory_pool<City>::get_T();
    city->name = city_data.name;
    return server.add_city(city);
}

bool sjtu::TTS::add_train(const sjtu::TTS::TrainData & train_data) {
    if (!server.check_line(train_data.line_name))
        return false;

    train_ptr train = memory_pool<Train>::get_T();
    line_ptr line = server.find_line(train_data.line_name);
    train->line = line;
    train->date = train_data.date;
    train->selling = train_data.selling;
    train->station_available_tickets = train_data.station_available_tickets;
    return line->trains.insert(make_pair(train_data.date, train)).second;
}
