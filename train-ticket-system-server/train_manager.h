#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <fstream>
#include "map.hpp"
#include "list.hpp"
#include "server.h"
#include "smart_ptr.h"
#include "io_utilities.hpp"
#include "vector.hpp"
#include "account_manager.h"


struct Date;
struct Ticket;
struct Station;
struct City;
struct Line;
struct Train;


struct Date {
private:
    int year, month, day;
    int hour, min, sec;
public:
    Date (int y = 0, int m = 0, int d = 0, int h = 0, int min = 0, int sec = 0):
            year(y), month(m), day(d), hour(h), min(min), sec(sec) {}
    friend bool operator < (const Date &lhs, const Date &rhs) {
        if      (lhs.year  != rhs.year)    return lhs.year  < rhs.year;
        else if (lhs.month != rhs.month)   return lhs.month < rhs.month;
        else if (lhs.day   != rhs.day)     return lhs.day   < rhs.day;
        else if (lhs.hour  != rhs.hour)    return lhs.hour  < rhs.hour;
        else if (lhs.min   != rhs.min)     return lhs.min   < rhs.min;
        else return lhs.sec < rhs.sec;
    }
    bool same_day(const Date& rhs) const {
        return year == rhs.year && month == rhs.month && day == rhs.day;
    }
    friend bool same_day(const Date& lhs, const Date &rhs)
    {
        return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
    }

    bool operator==(const Date &rhs) {
        return year == rhs.year && month == rhs.month && day == rhs.day
               && hour == rhs.hour && min == rhs.min && sec == rhs.sec;
    }

    class cmp_date {
    public:
        bool operator()(const Date & a, const Date & b) {
            return same_day(a, b);
        }
    };
};

struct Station {
    std::wstring name;
    // 车站所处城市
    smart_ptr<City> location;
    // 经过这个车站的线路
    vector<smart_ptr<Line>> lines;
};

struct City {
    std::wstring name;
    // 城市中的车站
    vector<smart_ptr<Station>> stations;
};

struct Line {
    std::string name;                     // K1234, G27 etc
    vector<std::wstring> seat_kind_names; // 座位种类名
    vector<smart_ptr<Station>> stations;  // 经过的车站，0为起点站
    vector<vector<int>> price;            // 从第i到第i+1站的第j种票票价

    // 给定日期的train
    map<Date, smart_ptr<Train>, Date::cmp_date> trains;

    bool check_date(const Date & date) const {
        return trains.find(date) != trains.cend();
    }
};

/**Same line share one line object
 * Only Date is different
 */
struct Train {
    smart_ptr<Line> line;
    Date _date;
    bool _selling = 0;
    vector<vector<int>> station_available_tickets;
    /* saves the number of remaining tickets for each station
     * e.g. station 0--1--2--3--4 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
     * if a customer bought a ticket from 1 to 3, then
     * station_available_ticket[] = {200, 199, 199, 200}
     */


};

struct Ticket {
    smart_ptr<Train> train;    // 所属的train
    smart_ptr<User>  user;     // 所属的user
    int from, to;              // 起点站终点站在line中的位置
    int kind;                  // 这张票的种类
    int price;                 // 票价 = Sigma price[i][kind]
    int num;                   // user拥有的张数
};


#endif
