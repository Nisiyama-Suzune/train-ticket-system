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
#include "query.h"

#include <QDir>
#include <cstdlib>

namespace sjtu {
template class memory_pool<Train>;
template class memory_pool<City>;
template class memory_pool<Station>;
template class memory_pool<Line>;
template class memory_pool<User>;
template class memory_pool<Account>;
template class memory_pool<Ticket>;
}


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
	bool add_user(const user_ptr & user);
	bool add_admin(const admin_ptr & admin);

public:
	friend QDataStream& operator >> (QDataStream &in, Server &rhs) {
		in >> rhs.users >> rhs.admins >> rhs.lines >> rhs.cities
		   >> rhs.stations;
		return in;
	}
	friend QDataStream& operator << (QDataStream &out, Server &rhs) {
		out << rhs.users << rhs.admins << rhs.lines << rhs.cities
			<< rhs.stations;
		return out;
	}
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
	const QDir log_path;
	const QDir data_path;

private:

    Server server;
    user_ptr  current_user;
    admin_ptr current_admin;
    int id_cnt;

    /// query
    smart_ptr<vector<train_ptr>>
    query_train(const City & from, const City & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const Station & from, const City & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const City & from, const Station & to, Date date) const;
    smart_ptr<vector<train_ptr>>
    query_train(const Station & from, const Station & to, Date date) const;


    /// add (loading)
    user_ptr _add_user(const QString &name, int ID);

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

private: // 返回用户当前的票的
    const deque<ticket_ptr> & current_tickets();
public:
    vector<QString> current_tickets(int ID);

public:
	// 登陆账户，返回登录成功与否（只检查ID和密码是否匹配），
	// 出问题会抛出异常
	bool login_user(const int & ID, const QString & password);
	bool login_admin(const int & ID, const QString & password);
	int register_user(const QString & name, const QString & password);
	int register_admin(const QString & name, const QString & password);

private:
	/// parser
	LineData line_transform(QString str);
	BuyReturnData operation_transform(QString str);
public:
	bool add_line(const QString & str);

private: // 查找station-station的票
    vector<Ticket> q_ss_ticket(const QString &f, const QString &t, int date);
public:
    vector<QString> q_ss(const QString &f, const QString &t, int date);
    vector<QString> query_city_city(const QString &f, const QString &t, int date);



private:
	bool is_train_type(QChar ch);
public:
	/// init
	bool load_ascii();
	bool load_binary();
	void save_binary();
	TTS();
	~TTS();

public:
    /// API
    vector<query_ticket_ans> query_city_city(const query_ticket_cc_data & data);
    vector<query_ticket_ans> query_station_station(const query_ticket_ss_data & data);
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
	int date;
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

/*当前订单函数(user)
 *传参：无(当前id)
 *返回值：一个字符串的vector
 *
 * 历史购票函数（user)
 * 传参：无（当前id）
 * 返回值：一个字符串的vector
 *
 * 查票函数_站到站（user, admin)
 * 传参：出发站（qstr)，终点站(qstr)，出发时间(int)
 * 返回：字符串vector
 *
 * 查票函数_城到城（user,admin)
 * 传参：出发站(qstr)， 终点站(qstr)，出发时间(int)
 *
 * 添加线路函数(admin)
 * 传参：出发站（qstr)，出发时间(int)，终点站(qstr)，到达时间(int），车次编号（qstr)
 * 返回值：bool（是否添加成功）//可不需要返回值
 *
 * 删除线路函数（admin)
 * 传参：车次编号（qstr)
 * 返回值：bool（是否删除成功）//可不需要返回值
 *
 */

