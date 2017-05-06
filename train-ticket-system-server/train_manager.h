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
struct Train_info;
struct Ticket;
struct Station;
struct City;
class Line;
class Train;

/// if you modify the data members,
/// please rewrite the save & load functions
struct Date
{
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
};

struct Train_info
{
    std::wstring Line_name;
    Date date;
    bool operator < (const Train_info& rhs) const {
        if (Line_name != rhs.Line_name) return Line_name < rhs.Line_name;
        return date < rhs.date;
    }

};

struct Ticket //users return tickets by date? //TODO
{
    typedef int KIND;

    Train_info train_info;
    int from, to;
    std::wstring from_name, to_name;
    int price;
    KIND kind;
    int num;
};

struct Station
{
    std::wstring name;
    Date arrival_time, departure_time;
    int miles;
    Station (const std::wstring& name, const Date& arr, const Date& dep, const int& miles)
            : name(name), arrival_time(arr), departure_time(dep), miles(miles) {}
};

struct City
{
    std::wstring name;
    list<map<std::wstring, Line>::iterator> lines; //maybe ? set<pair<Line*, int> > //this is (*line)'s int'th station
};

struct Line
{
//protected:
    typedef int KIND;

    char type; //K G etc
    std::wstring name; //K1234, G27 etc
    vector<Station> stations; // stations[i].miles means from start(0) to i-th station's distance
    vector<KIND> kinds; //kind of seats
    vector<vector<int> > price; //price[kind_number][station] fen-based
    list<map<Train_info, Train>::iterator> trains;//反正只有30趟
public:
    Line();
    ~Line() {
        // TODO
    } // delete trains

    //TODO add_train
};

/**Same line share one line object
 * Only Date is different
 */
class Train
{
protected:
    typedef int KIND;
    typedef map<Train_info, Train>::iterator train_iter;

private:
    map<std::wstring, Line>::iterator which_line;
    Date date;
    bool _selling;
    vector<vector<int> > station_available_ticket;
    /* saves the number of remaining tickets for each station
     * e.g. station 0--1--2--3--4 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
     * if a customer bought a ticket from 1 to 3, then
     * station_available_ticket[] = {200, 199, 199, 200}
     */

public:
    // 构造时修改line里的trains
    Train (const Line &line, const Date &date, const int& initial_ticket = 2000);
    // 检查(from, to, kind)的票是否还有num张
    bool have_ticket (int from, int to, KIND kind, int num);

    // 给(from, to, kind)的票加上num，其中num可以为负数
    void add_remaining_tickets(int from, int to, KIND kind, int num);

    //if (lines[i].have_ticket(from, to, kind, num)) now_user->buy_ticket(i, from, to, kind, num);
    bool selling();
    void change_selling();

//    friend istream& operator >> (istrema& in, Line & rhs);
//    friend ostream& operator << (ostream& out, const Line & rhs);
};

#endif
