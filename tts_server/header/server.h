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
#include "memory_pool.h"
#include "../../memory.hpp"
#include "../../smart_ptr.hpp"
#include <qstring.h>
#include <QTextStream>
#include <QDataStream>


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
    bool check_line(const QString & name) const;

    pool_ptr<City>    find_city(const QString & name) const;
    pool_ptr<User>    find_user(const int & ID) const;
    pool_ptr<Admin>   find_admin(const int & ID) const;
    pool_ptr<Station> find_station(const QString & name) const;
    pool_ptr<Line>    find_line(const QString & name) const;

    bool add_line(const pool_ptr<Line> & line);
    bool add_station(const pool_ptr<Station> & station);
    bool add_city(const pool_ptr<City> & city);
};

}

/// wrapper TTS
namespace sjtu {
class TTS {
    typedef std::wstring QString;

private:
    /// forward declaration
    struct LineData;
    struct BuyReturnData;
    struct StationData;
    struct CityData;
    struct TrainData;

private:
    Server server;
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
    bool add_station(const StationData &);
    bool add_line(const LineData &);
    bool add_city(const CityData &);
    bool add_train(const TrainData &);

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
struct Line_Data {
public:
	QString name;
	vector<QString> seat_kind_names;
	vector<QString> stations;
	vector<int> time_arrive, time_stop;
	vector<int> miles;
	vector<vector<double> > prices;
	#ifdef output_debug
	friend QTextStream& operator << (QTextStream& out, const Line_Data& x) {
		out << "name = " <<  x.name << endl;

		out << "seat_kind_names" << endl;
		for (int i = 0; i < x.seat_kind_names.size(); ++i)
			out << x.seat_kind_names[i] << ' ';
		out << endl;

		out << "stations" << endl;
		for (int i = 0; i < x.stations.size(); ++i)
			out << x.stations[i] << ' ';
		out << endl;

		out << "time_arrive" << endl;
		for (int i = 0; i < x.time_arrive.size(); ++i)
			out << x.time_arrive[i] << ' ';
		out << endl;

		out << "time_stop" << endl;
		for (int i = 0; i < x.time_stop.size(); ++i)
			out << x.time_stop[i] << ' ';
		out << endl;

		out << "miles" << endl;
		for (int i = 0; i < x.miles.size(); ++i)
			out << x.miles[i] << ' ';
		out << endl;

		out << "prices" << endl;
		for (int i = 0; i < x.prices.size(); ++i) {
			for (int j = 0; j < x.prices[i].size(); ++j)
				out << x.prices[i][j] << ' ';
			out << endl;
		}
		out << endl;
		return out;
	}
	#endif //output_debug
};
Line_Data line_transform(QString str)
{
	QTextStream sin(&str);
	QTextStream cout(stdout);

	Line_Data ans;
	QString tmp = sin.readLine();
	QStringList parts = tmp.split(",");
	ans.name = parts[0];
	tmp = sin.readLine();
	parts = tmp.split(",");
	int kinds = 0;
	for (int i = 5; i < parts.size(); ++i) {
		ans.seat_kind_names.push_back(parts[i]);
		++kinds;
	}
	for (int i = 0; i < kinds; ++i)
		ans.prices.push_back(vector<double>());
//	ans.prices.reserve(kinds);

	while (sin.readLineInto(&tmp)) {
		parts = tmp.split(",");
		ans.stations.push_back(parts[0]);

		int time_arrive = 0, x, time_stop;

		QChar maohao;
		QTextStream cin2(&parts[2]);
		cin2 >> time_arrive >> maohao >> x;
		time_arrive = time_arrive * 100 + x;
		ans.time_arrive.push_back(time_arrive);

		QTextStream cin3(&parts[3]);
		cin3 >> time_stop >> maohao >> x;
		time_stop = time_stop * 100 + x;
		ans.time_stop.push_back(time_stop);

		QTextStream cin4(&parts[4]);
		cin4 >> x;
		ans.miles.push_back(x);

		double price = 0.0;
		for (int i = 0; i < kinds; ++i) {
			if (parts[5 + i][0] == '-')
				price = -1;
			else {
				QTextStream cin5(&parts[5 + i]);
				cin5 >> maohao;
				cin5 >> price;
			}
			ans.prices[i].push_back(price);
		}
	}
#ifdef output_debug
	cout << ans << endl;
#endif //output_debug
	return ans;
}
struct buy_data
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
	friend QTextStream& operator << (QTextStream& out, const buy_data& x) {
		out << "name         = " << x.name << endl;
		out << "ID           = " << x.ID << endl;
		out << "operation    = " << x.operation << endl;
		out << "num          = " << x.num << endl;
		out << "kind_of_seat = " << x.kind_of_seat << endl;
		out << "train_ID     = " << x.train_ID << endl;
		out << "from_station = " << x.from_station << endl;
		out << "to_station   = " << x.to_station << endl;
		out << "data         = " << x.date << endl;
		return out;
	}
};
buy_data operation_transform(QString str)
{
	QTextStream sin(&str);
	QTextStream cout(stdout);

	buy_data ans;
	QString tmp = sin.readLine();
	QStringList parts = tmp.split(' ');

	ans.name = parts[0];

	QTextStream cin1(&parts[1]);
//	ans.ID = parts[1];
	cin1 >> ans.ID;

	ans.operation = parts[2];

	QTextStream cin2(&parts[3]);
	cin2 >> ans.num;

	ans.kind_of_seat = parts[4];
	ans.train_ID = parts[7];
	ans.from_station = parts[9];
	ans.to_station = parts[11];
	ans.date = parts[13];
	cout << ans << endl;
	return ans;
}
	///parser end

	///save & load
	friend QDataStream& operator << (QDataStream& out, );
public:
    /// API


};

struct TTS::LineData {
    QString name;
    vector<QString> seat_kind_names;
    vector<QString> stations;
    vector<int> time_arrive, time_stop;
    vector<int> miles;
    vector<vector<double> > prices;
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


}

#endif //TTS_SERVER_H
