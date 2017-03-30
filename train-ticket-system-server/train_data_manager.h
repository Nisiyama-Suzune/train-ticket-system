#ifndef TRAIN_DATA_MANAGER_H
#define TRAIN_DATA_MANAGER_H

#include "../vector.h"
#include "../hash_map.h"
#include <string>

const int PRICE_UNAVAILABLE = -1;

struct date {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    date (int y, int m, int d, int h, int mi) : year (y), month (m), day (d), hour (h), minute (mi) {}
    bool operator < (const date &rhs);
};

struct station_data {
    std::string name;
    date arrival_time, departure_time;
    int mileage;
    vector <int> price; //use 0 for the first station, PRICE_UNAVAILABLE for other stations without the ticket, fen-based
};

struct train_data {
    std::string name;
    vector <station_data> station;
    vector <int> station_available_ticket[3];
    /* saves the number of remaining tickets for each station
     * e.g. station 1--2--3--4--5 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200, 200}
     * if a customer bought a ticket from 2 to 4, then
     * station_available_ticket[] = {200, 199, 199, 200, 200}
     */
    vector <std::string> ticket_name;
    bool is_available; //whether can be sold
    bool is_deleted; //whether deleted
};

class train_data_manager
{
private:
    vector <train_data> m_data;
    hash_map <100000, std::string, int> m_station; //each station
    vector <vector <int> > m_source; //m_source[station_index] = trains passing through station_index
    hash_map <100000, std::string, int> m_name; //hash for train name
    vector <int> m_recycle; //marks the deleted trains for recycling
public:
    train_data_manager();
    int size ();
    const train_data get_data (int index);
    void import_from_file (char *filename); //import train data from a file, append to current data.
    void export_to_file (char *filename); //export current data to a file
    void update (date time); //delete all trains ended before time.
    vector <int> query (date lower_bound, date upper_bound, std::string source, std::string destination);
    vector <int> query (date lower_bound, date upper_bound, std::string source);
    vector <int> query (std::string name, date lower_bound, date upper_bound);
    bool book_train (int index, int source, int destination, int count); //true = success
    void cancel_train (int index, int source, int destination, int count);
    void add_train (train_data data);
    void delete_train (int index);
};

#endif // TRAIN_DATA_MANAGER_H
