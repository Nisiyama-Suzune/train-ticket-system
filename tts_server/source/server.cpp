//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/server.h"
#include <QFile>
#include <QDir>
#include <QIODevice>

/// server

sjtu::city_ptr sjtu::Server::find_city(const QString &name) const {
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

sjtu::station_ptr sjtu::Server::find_station(const QString &name) const {
	auto station = stations.find(name);
	if (station == stations.cend())
		throw exception("station", "does not exist");
	return station->second;
}

sjtu::line_ptr sjtu::Server::find_line(const QString &name) const {
	auto line = lines.find(name);
	if (line == lines.cend())
		throw exception("line", "does not exist");
	return line->second;
}


bool sjtu::Server::check_city(const QString &name) const {
	return cities.find(name) != cities.cend();
}

bool sjtu::Server::check_user(const int &ID) const {
	return users.find(ID) != users.cend();
}

bool sjtu::Server::check_admin(const int &ID) const {
	return admins.find(ID) != admins.cend();
}

bool sjtu::Server::check_station(const QString &name) const {
	return stations.find(name) != stations.cend();
}

bool sjtu::Server::check_line(const QString &name) const {
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

bool sjtu::Server::add_user(const sjtu::user_ptr & user) {
	return users.insert(sjtu::make_pair(user->ID, user)).second;
}

bool sjtu::Server::add_admin(const sjtu::admin_ptr & admin) {
	return admins.insert(sjtu::make_pair(admin->ID, admin)).second;
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

smart_ptr<sjtu::vector<sjtu::train_ptr>>
sjtu::TTS::query_train(const sjtu::Station & from, const sjtu::City & to, sjtu::Date date) const {
	smart_ptr<vector<train_ptr>> result(new vector<train_ptr>);

//    vector<station_ptr>::const_iterator from_station;
//    for (from_station = from.stations.cbegin(); from_station != from.stations.cend() ; ++from_station) {
		vector<line_ptr>::const_iterator cur_line;
		for (cur_line = from.lines.cbegin(); cur_line != from.lines.cend(); ++cur_line) {
			if (!(*cur_line)->check_date(date)) {
				continue;
			}
			vector<station_ptr>::const_iterator cur_station = (*cur_line)->stations.cbegin();
			while ((*cur_station)->name != from.name) {
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
//    }

	return result;
}

smart_ptr<sjtu::vector<sjtu::train_ptr>>
sjtu::TTS::query_train(const sjtu::City & from, const sjtu::Station & to, sjtu::Date date) const {
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
				if ((*cur_station)->name != to.name) {
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

smart_ptr<sjtu::vector<sjtu::train_ptr>>
sjtu::TTS::query_train(const sjtu::Station & from, const sjtu::Station & to, sjtu::Date date) const {
	smart_ptr<vector<train_ptr>> result(new vector<train_ptr>);

//    vector<station_ptr>::const_iterator from_station;
//    for (from_station = from.stations.cbegin(); from_station != from.stations.cend() ; ++from_station) {
		vector<line_ptr>::const_iterator cur_line;
		for (cur_line = from.lines.cbegin(); cur_line != from.lines.cend(); ++cur_line) {
			if (!(*cur_line)->check_date(date)) {
				continue;
			}
			vector<station_ptr>::const_iterator cur_station = (*cur_line)->stations.cbegin();
			while ((*cur_station)->name != from.name) {
				++cur_station;
			}
			while (cur_station != (*cur_line)->stations.cend()) {
				if ((*cur_station)->name != to.name) {
					break;
				}
				++cur_station;
			}
			if (cur_station != (*cur_line)->stations.cend()) {
				result->push_back((*cur_line)->trains[date]);
			}
		}
//    }

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

bool sjtu::TTS::login_user(const int & ID, const QString & password) {
	user_ptr user = server.find_user(ID);
	if (user->check_password(password)) {
		current_user = user;
		current_admin = admin_ptr();
		return true;
	}
	return false;
}

bool sjtu::TTS::login_admin(const int &ID, const QString & password) {
	admin_ptr admin = server.find_admin(ID);
	if (admin->check_password(password)) {
		current_admin = admin;
		current_user = user_ptr();
		return true;
	}
	return false;
}

bool sjtu::TTS::is_train_type(QChar ch) {
	return (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9');
}
int sjtu::TTS::register_user(const QString & name, const QString & password) {
	while (server.check_user(id_cnt))
		++id_cnt;
	int ID = id_cnt++;
	user_ptr user = memory_pool<User>::get_T();
	user->ID = ID;
	user->name = name;
	user->password = password;
	server.add_user(user);
	return ID;
}

int sjtu::TTS::register_admin(const QString & name, const QString & password) {
	while (server.check_admin(id_cnt))
		++id_cnt;
	int ID = id_cnt++;
	admin_ptr admin = memory_pool<Admin>::get_T();
	admin->ID = ID;
	admin->name = name;
	admin->password = password;
	server.add_admin(admin);
	return ID;
}

bool sjtu::TTS::load_ascii() {
	QDir dir = QDir::current();
	QString directory = QDir::currentPath();
	directory += "/../train-ticket-system/trains.csv";
	QFile file(directory);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "No such file!" << std::endl;
		return false;
	}
	QTextStream fin(&file);

	QString str, tmp;
	str = fin.readLine();

	while (fin.readLineInto(&tmp)) {
		if (is_train_type(tmp[0])) {
			add_line(line_transform(str));
			str = tmp;
		} else str = str + '\n' + tmp;
	}
	add_line(line_transform(str));
	file.close();

	directory = QDir::currentPath();
	directory += "/../train-ticket-system/operation.out";
	QFile file2(directory);
	if (!file2.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "No operation file!" << std::endl;
		return false;
	}
	QTextStream fin2(&file2);
	while (fin2.readLineInto(&str)) {
		BuyReturnData ans = operation_transform(str);
		if (server.check_user(ans.ID)) {
			current_user = server.find_user(ans.ID);
		} else {
			current_user = register_user(ans.name, "000000");
			current_user->ID = ans.ID;
		}
//		buy_ticket(server.find_line(ans.train_ID)->trains[ans.date], )
	}
	return true;
}

sjtu::user_ptr sjtu::TTS::_add_user(const QString &name, int ID) {
    user_ptr user = memory_pool<User>::get_T();
    user->name = name;
    user->ID = ID;
    server.add_user(user);
    return user;
}

bool sjtu::TTS::load_binary() {
	QString directory = QDir::currentPath();
	directory += "/../train-ticket-system/operation.dat";
	QFile file(directory);
	if (!file.open(QIODevice::ReadOnly)) {
		std::cout << "Load failed!" << std::endl;
		return false;
	}
	QDataStream fin(&file);
	memory_pool<User>::load(fin);
	memory_pool<Admin>::load(fin);
	memory_pool<Station>::load(fin);
	memory_pool<City>::load(fin);
	memory_pool<Line>::load(fin);
	memory_pool<Train>::load(fin);
	memory_pool<Ticket>::load(fin);
	fin >> server;
//	fin >> users >> admins >> lines >> cities >> stations;
	return true;
}

void sjtu::TTS::save_binary() {
	QString directory = QDir::currentPath();
	directory += "/../train-ticket-system/operation.dat";
	QFile file(directory);
	if (!file.open(QIODevice::WriteOnly)) {
	}
	QDataStream fout(&file);
	memory_pool<User>::save(fout);
	memory_pool<Admin>::save(fout);
	memory_pool<Station>::save(fout);
	memory_pool<City>::save(fout);
	memory_pool<Line>::save(fout);
	memory_pool<Train>::save(fout);
	memory_pool<Ticket>::save(fout);
	fout << server;
//	fout << users << admins << lines << cities << stations;
}


//Constructor
sjtu::TTS::TTS() {
	QString directory = QDir::currentPath();
	directory += "/../train-ticket-system/operation.dat";
	QFile file(directory);
	if(!file.open()) {
		file.close();
		load_ascii();
	} else {
		file.close();
		load_binary();
	}
}
sjtu::TTS::~TTS() {
	save_binary();
}

sjtu::user_ptr sjtu::TTS::_add_user(const QString &name, int ID) {
	user_ptr user = memory_pool<User>::get_T();
	user->name = name;
	user->ID = ID;
	server.add_user(user);
	return user;
}


bool sjtu::TTS::add_line(const sjtu::TTS::LineData &line_data) {
	// station
	line_ptr line = memory_pool<Line>::get_T();
	line->name = line_data.name;
	line->seat_kind_names = line_data.seat_kind_names;
	line->miles = line_data.miles;
	line->price = line_data.prices;
	for (int i = 0; i < (int)line_data.time_arrive.size(); ++i) {
		line->arr_time.push_back(line_data.time_arrive[i]);
		line->dep_time.push_back(line_data.time_stop[i]);
	}
	for (int i = 0; i < (int)line_data.stations.size(); ++i) {
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

sjtu::TTS::LineData sjtu::TTS::line_transform(QString str)
{
	QTextStream sin(&str);
	QTextStream cout(stdout);

	LineData ans;
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

sjtu::TTS::BuyReturnData sjtu::TTS::operation_transform(QString str)
{
	QTextStream sin(&str);
	QTextStream cout(stdout);

	BuyReturnData ans;
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

	QStringList date_parts = parts[13].split('-');
	int date = 0;
	for (int i = 0; i < 4; ++i)
		date = date * 10 + (date_parts[0][i].toLatin1() - '0');
	date = date * 100;
	if (date_parts[1][1].toLatin1() >= '0' && date_parts[1][1].toLatin1() <= '9') {
		date += 10 * (date_parts[1][0].toLatin1() - '0');
		date += date_parts[1][1].toLatin1() - '0';
	} else date += date_parts[1][0].toLatin1() - '0';
	date *= 100;
	if (date_parts[2][1].toLatin1() >= '0' && date_parts[2][1].toLatin1() <= '9') {
		date += 10 * (date_parts[2][0].toLatin1() - '0');
		date += date_parts[2][1].toLatin1() - '0';
	} else date += date_parts[2][0].toLatin1() - '0';
	ans.date = date;
	#ifdef output_debug
	cout << ans << endl;
	#endif //output_debug
	return ans;
}

bool sjtu::TTS::add_line(const QString & str) {
	LineData data = line_transform(str);
	return add_line(data);
}

sjtu::vector<QString> sjtu::TTS::query_city_city(const QString &f, const QString &t, int date) {
    /// TODO
    Date t_date(date);
    const City &from = server.find_city(f);
    const City &to   = server.find_city(t);
    smart_ptr<vector<train_ptr>> tmp = query_train(from, to, t_date);
    QString str;
    for (auto iter = tmp->begin(); iter != tmp->end(); ++iter) {
        const Train& train = **iter;
        for (auto iter = train.line->seat_kind_names.begin();
             iter != train.line->seat_kind_names.end(); ++iter) {
            str = "";
            str += train.line->name + "  ";
            str += "起点：" + f + "（" + train.line->arr()
        }
    }
}

sjtu::vector<QString> sjtu::TTS::current_tickets(int ID) {
    vector<QString> result;
    QString str;

    current_user =  server.find_user(ID);
    const deque<ticket_ptr> & cur_tickets = current_tickets();
    for (auto iter = cur_tickets.begin(); iter != cur_tickets.end(); ++iter) {
        const Ticket & ticket = **iter;
        str = "";
        str += ticket.train->line->name + " ";
        str += "从" + ticket.train->line->stations[ticket.from]
                + "到" + ticket.train->line->stations[ticket.to];
        str += " " + ticket.train->date.toStr();
        str += " " + QString::number(ticket.num) + "张";
        result.push_back(str);
    }
   return result;
}

/* 查询：找出从车站到车站之间的所有可行车票
 * 需要：车次；起点车站；出发时间；终点车站；到达时间；座位名字，价格，余票
 * 实现：
 * 通过query_train可以得到经过起点和终点的所有车次，车次有了
 * 起点车站和终点车站的名字本来就有
 * 出发时间和到达时间，需要这个站在line中的位置，就可以得到了
 * 座位名字和价格也可以通过line来找到。
 * 从Ticket中，可以得到车次；起点
 */

sjtu::vector<sjtu::Ticket> sjtu::TTS::q_ss_ticket(const QString &f, const QString &t, int date) {
    const Station & from = server.find_station(f);
    const Station & to   = server.find_station(t);
    Date t_date(date);
    smart_ptr<vector<train_ptr>> tmp = query_train(from, to, t_date);

    vector<Ticket> result;

    for (auto iter = tmp->begin(); iter != tmp->end(); ++iter) {
        const Train & train = **iter;
        Ticket add;

        int st, ed;
        for (st = 0; st < train.line->stations.size(); ++st) {
            if (f == train.line->stations[st]->name)
                break;
        }
        for (ed = 0; ed < train.line->stations.size(); ++ed) {
            if (t == train.line->stations[ed]->name)
                break;
        }
        add.train = *iter;
        add.from = st;
        add.to   = to;

        for (int i = 0; i < train.line->seat_kind_names.size(); ++i)
        {
            add.kind = i;
            int num;
            double price;
            for (int p = st; p < ed; ++p) {
                price += train.line->price[p];
                if (num > train.station_available_tickets[p]) {
                    num = train.station_available_tickets[p];
                    if (num == 0)
                        break;
                }
            }
            if (!num) {
                result.push_back(add);
            }
        }
    }

    return result;
}








sjtu::TTS::TTS() {

}
