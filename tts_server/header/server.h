//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_SERVER_H
#define TTS_SERVER_H

/// forward declaration
namespace sjtu {

class Account;
class User;
class Admin;
struct Date;
struct Station;
struct City;
struct Line;
struct Train;


}

#include "train_manager.h"
#include "account_manager.h"
#include "../../memory.hpp"
#include "../../smart_ptr.hpp"


/// Server
namespace sjtu {

class Server {
    typedef std::wstring QString;
private:
    typedef map<int, pool_ptr<User>> UserContainer; //ID -> account find an account, insert, remove
    typedef map<int, pool_ptr<Admin>> AdminContainer; //ID -> account
    typedef map<QString, pool_ptr<Line>> LineContainer; //ID -> Line
    typedef map<QString, pool_ptr<City>> CityContainer; //name -> City
    typedef map<QString, pool_ptr<Station>> StationContainer;
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

    pool_ptr<City>    find_city(const QString & name) const;
    pool_ptr<User>    find_user(const int & ID) const;
    pool_ptr<Admin>   find_admin(const int & ID) const;
    pool_ptr<Station> find_station(const QString & name) const;

    bool add_line(const pool_ptr<Line> & line);
    bool add_station(const pool_ptr<Station> & station);
};


}

/// wrapper
namespace sjtu {



class TTS {
    typedef std::wstring QString;

private:
    /// forward declaration
    struct LineData;
    struct BuyReturnData;
    struct StationData;
    struct CityData;

private:
    Server server;
    train_memory_pool   t_m_p;
    account_memory_pool a_m_p;
    pool_ptr<User>  current_user;
    pool_ptr<Admin> current_admin;

    /// query
    smart_ptr<vector<pool_ptr<Train>>>
    query_train(const City & from, const City & to, Date date) const;
    smart_ptr<vector<pool_ptr<Train>>>
    query_train(const Station & from, const City & to, Date date) const;
    smart_ptr<vector<pool_ptr<Train>>>
    query_train(const City & from, const Station & to, Date date) const;
    smart_ptr<vector<pool_ptr<Train>>>
    query_train(const Station & from, const Station & to, Date date) const;


    /// add (admin permission required)
    bool add_station(const StationData & station);
    bool add_line(const LineData & line);

    /// user
    /* 买票，如果票不够了或者没开票，则返回false。
     * 否则则修改余票，并且往当前登陆账户的票数里新增一张票。
     */
    bool buy_ticket(pool_ptr<Train> train, int from, int to, int kind, int num);

    /* 退票，如果当前该张票余票不够，则返回false
     */
    bool return_ticket(pool_ptr<Ticket> ticket, int num);

    // 返回用户当前的票的
    const list<pool_ptr<Ticket>> & current_tickets();

    // 登陆账户，返回登录成功与否（只检查ID和密码是否匹配），
    // 出问题会抛出异常
    bool login_user(const int & ID, const std::string password);
    bool login_admin(const int & ID, const std::string password);

private:
    /// parser
    

    

public:

    /**
     * QString query_train(const QString & str);
     *
     * bool account_register(const QString & str);
     * bool login(const QString & str);
     *
     * add_line
     * add_train
     *
     * @User
     *
     */


};

struct TTS::LineData {
    QString name;
    vector<QString> seat_kind_names;
    vector<QString> stations;
    vector<int> time_arrive, time_stop;
    vector<int> miles;
    vector<vector<double> > prices;
};

struct TTS::BuyReturnData {
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

};


}

#endif //TTS_SERVER_H
