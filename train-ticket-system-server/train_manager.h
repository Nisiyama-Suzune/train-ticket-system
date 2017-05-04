#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <fstream>
#include "map.h"
#include "list.hpp"
//#include "smart_ptr.h"
#include "io_utilities.hpp"
#include "../vector.h"
#include "account_manager.h"

// illegal headers !!!
#include <memory>
using std::shared_ptr;
using std::weak_ptr;

class Date;
struct Train_info;
class Ticket;
struct Station;
struct City;
class Line;
class Train;

/// if you modify the data members,
/// please rewrite the save & load functions
class Date
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

    /// save & load
    // if the size of this class is no longer constant,
    // these functions should be rewrite.
    std::streampos save(std::ofstream & fout) {
        std::streampos result = fout.tellp();
        default_save(fout, this);
        return result;
    }
    void load(std::ifstream & fin) {
        default_load(fin, this);
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

    /// save & load
    std::streampos save(std::ofstream & fout) {
        std::streampos result = fout.tellp();
        write_str(fout, Line_name);
        date.save(fout);
        return result;
    }
    void load(std::ifstream & fin) {
        read_str(fin, Line_name);
        date.load(fin);
    }
};

class Ticket //users return tickets by date? //TODO
{
private:
    Train_info train_info;
    std::wstring from, to;
    int price;
    KIND kind;
    int num;
public:
    Ticket (const Date& date, const std::wstring& from, const std::wstring& to,
            const std::wstring& line_name, const int& price, const KIND& kind,
            const int & num);

    /// save & load
    std::streampos save(std::ofstream & fout) {
        std::streampos result = fout.tellp();
        train_info.save(fout);
        write_str(fout, from);
        write_str(fout, to);
        default_save(fout, price);
        default_save(fout, kind);
        default_save(fout, num);
        return result;
    }
    void load(std::ifstream & fin) {
        train_info.load(fin);
        read_str(fin, from);
        read_str(fin, to);
        default_load(fin, price);
        default_load(fin, kind);
        default_load(fin, num);
    }
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
    list<weak_ptr<Line> > lines; //maybe ? set<pair<Line*, int> > //this is (*line)'s int'th station
};

class Line
{
protected:
    char type; //K G etc
    std::wstring name; //K1234, G27 etc
    vector<Station> stations; // stations[i].miles means from start(0) to i-th station's distance
    vector<KIND> kinds; //kind of seats
    vector<vector<int> > price; //price[kind_number][station] fen-based
    list<weak_ptr<Train> >trains;//反正只有30趟
public:
    Line();
    friend std::istream& operator >> (istrema& in, Line & rhs);
    friend std::ostream& operator << (ostream& out, const Line & rhs);
    friend Train;
};

/**Same line share one line object
 * Only Date is different
 */
class Train
{
private:
    weak_ptr<Line> which_line;
    Date date;
    bool _selling;
    vector<vector<int> > station_available_ticket;
    /* saves the number of remaining tickets for each station
     * e.g. station 0--1--2--3--4 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
     * if a customer bought a ticket from 1 to 3, then
     * station_available_ticket[] = {200, 199, 199, 200}
     */

    // private version
    // TODO
    void add_remaining_tickets(int from, int to, Kind kind, int num);

public:
    Train (const Line &line, const Date &date, const int& initial_ticket = 2000);
    bool have_ticket (int from, int to, KIND kind, int num);

    //if (lines[i].have_ticket(from, to, kind, num)) now_user->buy_ticket(i, from, to, kind, num);
    bool selling() {
        return _selling;
    }
    void change_selling() {
        _selling = ~_selling;
    }

    friend istream& operator >> (istrema& in, Line & rhs);
    friend ostream& operator << (ostream& out, const Line & rhs);
};

#endif
