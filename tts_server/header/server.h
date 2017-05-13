//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_SERVER_H
#define TTS_SERVER_H

#include "forward_declaration.h"

#include "train_manager.h"
#include "account_manager.h"
#include "../../memory.hpp"
#include "../../smart_ptr.hpp"


/// Server
namespace sjtu {
class Server {
private:
    typedef map<int, user_ptr> UserContainer; //ID -> account find an account, insert, remove
    typedef map<int, admin_ptr> AdminContainer; //ID -> account
    typedef map<QString, line_ptr> LineContainer; //ID -> Line
    typedef map<QString, city_ptr> CityContainer; //name -> City
    typedef map<QString, station_ptr> StationContainer;
//	typedef list<Log> LogContainer;
    UserContainer     users;
    AdminContainer    admins;
    LineContainer     lines;
    CityContainer     cities;
    StationContainer  stations;
//	LogContainer      logs;

public:
    bool check_city(const QString & name) const;
    bool check_user(const int & ID) const;
    bool check_admin(const int & ID) const;
    bool check_station(const QString & name) const;
    bool check_line(const QString & name) const;

    city_ptr    find_city(const QString & name) const;
    user_ptr    find_user(const int & ID) const;
    admin_ptr   find_admin(const int & ID) const;
    station_ptr find_station(const QString & name) const;
    line_ptr    find_line(const QString & name) const;

    bool add_line(const line_ptr & line);
    bool add_station(const station_ptr & station);
    bool add_city(const city_ptr & city);
};


}

/// wrapper TTS
namespace sjtu {
class TTS {

private:
    /// forward declaration
    struct LineData;
    struct BuyReturnData;
    struct StationData;
    struct CityData;
    struct TrainData;

private:
    Server server;
    user_ptr  current_user;
    admin_ptr current_admin;

    /// query
    smart_ptr<vector<train_ptr>>
    query_train(const City & from, const City & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const Station & from, const City & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const City & from, const Station & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const Station & from, const Station & to, Date date) const;


    /// add (admin permission required)
    bool add_station(const StationData &);
    bool add_line(const LineData &);
    bool add_city(const CityData &);
    bool add_train(const TrainData &);

    /// user
    /* 买票，如果票不够了或者没开票，则返回false。
     * 否则则修改余票，并且往当前登陆账户的票数里新增一张票。
     */
    bool buy_ticket(train_ptr train, int from, int to, int kind, int num);

    /* 退票，如果当前该张票余票不够，则返回false
     */
    bool return_ticket(ticket_ptr ticket, int num);

    // 返回用户当前的票的
    const deque<ticket_ptr> & current_tickets();

    // 登陆账户，返回登录成功与否（只检查ID和密码是否匹配），
    // 出问题会抛出异常
    bool login_user(const int & ID, const QString password);
    bool login_admin(const int & ID, const QString password);

private:
    /// parser
	LineData line_transform(QString str);
	BuyReturnData operation_transform(QString str);
public:
    /// API




public:
    /// init
    TTS();
};
}

// Data
namespace sjtu {

struct TTS::LineData {
public:
	QString name;
	vector<QString> seat_kind_names;
	vector<QString> stations;
	vector<int> time_arrive, time_stop;
	vector<int> miles;
	vector<vector<double> > prices;
};

struct TTS::BuyReturnData
{
    QString name;
    int ID;
    QString operation;
    int num;
    QString kind_of_seat;
    QString train_ID;
    QString from_station;
    QString to_station;
    QString date;
};

struct TTS::StationData {
    QString name;
    QString location;

    StationData(){}
    StationData(QString _name, QString _loc)
        : name(_name), location(_loc) {}
};

struct TTS::CityData {
    QString name;

    CityData(){}
    CityData(QString _name)
            : name(_name) {}
};

struct TTS::TrainData {
    QString line_name;
    int date; // 2017.1.20 -> 20170120
    bool selling = 0;
    vector<vector<int>> station_available_tickets;
    /* saves the number of remaining tickets for each station
     * e.g. station 0--1--2--3--4 with capacity 200 seats, then
     * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
     * if a customer bought a ticket from 1 to 3, then
     * station_available_ticket[] = {200, 199, 199, 200}
	 */
};

}

#endif //TTS_SERVER_H
