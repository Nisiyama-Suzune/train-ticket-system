//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_TRAIN_MANAGER_H
#define TTS_TRAIN_MANAGER_H

#include "../../map.hpp"
#include "../../vector.hpp"
#include "../../list.hpp"
#include "../../memory.hpp"

#include <string>

/// Stations, etc.
namespace sjtu {

typedef std::wstring QString;

/// forward declaration
struct Date;
struct Station;
struct City;
struct Line;
struct Train;

struct Date {
    int year, month, day;
    int hour, min, sec;


    Date(int y = 0, int m = 0, int d = 0, int h = 0, int min = 0, int sec = 0) :
            year(y), month(m), day(d), hour(h), min(min), sec(sec) {}

    friend bool operator<(const Date &lhs, const Date &rhs) {
        if (lhs.year != rhs.year) return lhs.year < rhs.year;
        else if (lhs.month != rhs.month) return lhs.month < rhs.month;
        else if (lhs.day != rhs.day) return lhs.day < rhs.day;
        else if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;
        else if (lhs.min != rhs.min) return lhs.min < rhs.min;
        else return lhs.sec < rhs.sec;
    }

    bool same_day(const Date &rhs) const {
        return year == rhs.year && month == rhs.month && day == rhs.day;
    }

    friend bool same_day(const Date &lhs, const Date &rhs) {
        return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
    }

    bool operator==(const Date &rhs) {
        return year == rhs.year && month == rhs.month && day == rhs.day
               && hour == rhs.hour && min == rhs.min && sec == rhs.sec;
    }

    class cmp_date {
    public:
        bool operator()(const Date &a, const Date &b) const {
            return a.same_day(b);
        }
    };
};

struct Station {
    static const int Type = 0;

    std::wstring name;
    // 车站所处城市
    pool_ptr<City> location;
    // 经过这个车站的线路
    vector<pool_ptr<Line>> lines;

    Station(){}
};

struct City {
    static const int Type = 1;

    std::wstring name;
    // 城市中的车站
    vector<pool_ptr<Station>> stations;
};

struct Line {
    static const int Type = 2;

    QString name;                     // K1234, G27 etc
    vector<QString> seat_kind_names; // 座位种类名
    vector<pool_ptr<Station>> stations;  // 经过的车站，0为起点站
    vector<Date> arr_time;               // 到站时间
    vector<Date> dep_time;               // 离站时间
    vector<int> miles;                   // 距离始发站的里程
    vector<vector<double>> price;            // 从第i到第i+1站的第j种票票价

    // 给定日期的train
    map<Date, pool_ptr<Train>, Date::cmp_date> trains;

    bool check_date(const Date & date) const {
        return trains.find(date) != trains.cend();
    }
};

/**Same line share one line object
 * Only Date is different
 */
struct Train {
    static const int Type = 3;

    pool_ptr<Line> line;
    Date date;
    bool selling = 0;
    vector<vector<int>> station_available_tickets;
    /* saves the number of remaining tickets for each station
     * e.g. station 0--1--2--3--4 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
     * if a customer bought a ticket from 1 to 3, then
     * station_available_ticket[] = {200, 199, 199, 200}
     */
};

struct Ticket {
    static const int Type = 4;

    pool_ptr<Train> train;    // 所属的train
    int from, to;              // 起点站终点站在line中的位置
    int kind;                  // 这张票的种类
    int price;                 // 票价 = Sigma price[i][kind]
    int num;                   // user拥有的张数

    bool equal_ex_num(const Ticket & rhs) {
        return train == rhs.train && from == rhs.from && to == rhs.to
               && kind == rhs.kind;
    }
};
}


#endif //TTS_TRAIN_MANAGER_H
