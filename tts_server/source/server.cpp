//
// Created by Aaron Ren on 11/05/2017.
//

#include "../header/server.h"
#include <QFile>
#include <QDir>
#include <QIODevice>
#include "../header/query.h"

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
    auto user2 = server.find_user(ID);
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
//    directory = "/Users/aaronren/Projects/CLionProjects/train-ticket-system/trains.csv";
	QFile file(directory);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "No trains' ascii file!" << std::endl;
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
//    directory = "/Users/aaronren/Projects/CLionProjects/train-ticket-system/operation.out";
	QFile file2(directory);
	if (!file2.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "No operation ascii file!" << std::endl;
		return false;
	}
	QTextStream fin2(&file2);
	while (fin2.readLineInto(&str)) {
		BuyReturnData ans = operation_transform(str);
		if (server.check_user(ans.ID)) {
            auto user = server.find_user(ans.ID);
            user->name = ans.name;
        } else {
           _add_user(ans.name, ans.ID);
        }
		buy_tickets_data tmp;
		tmp.ID = ans.ID;
		tmp.train_name = ans.train_ID;
		tmp.start_date = ans.date;
		tmp.start_station = ans.from_station;
		tmp.end_station = ans.to_station;
		tmp.seat_kind = ans.kind_of_seat;
		tmp.ticket_num = ans.num;
		buy_tickets(tmp);
//		buy_ticket(server.find_line(ans.train_ID)->trains[ans.date], )
	}
	using std::cout;
	using std::endl;
	cout << "ASCII load success!!!" << endl;
	std::cout << "! User's size = " << memory_pool<User>::size() << std::endl;
	std::cout << "! Admin's size = " << memory_pool<Admin>::size() << std::endl;
	std::cout << "! Station's size = " << memory_pool<Station>::size() << std::endl;
	std::cout << "! City's size = " << memory_pool<City>::size() << std::endl;
	std::cout << "! Line's size = " << memory_pool<Line>::size() << std::endl;
	std::cout << "! Train's size = " << memory_pool<Train>::size() << std::endl;
	std::cout << "! Ticket's size = " << memory_pool<Ticket>::size() << std::endl;

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
	QString path = QDir::currentPath();
	path += "/../train-ticket-system/data/";

	memory_pool<User>::end_counting();
	QString User_file_name = "Users.dat";
	QFile User_file(path + User_file_name);
	if (!User_file.open(QIODevice::ReadOnly)) return false;
	QDataStream User_fin(&User_file);
	memory_pool<User>::save(User_fin);
	std::cout << "User load success! User's size = " << memory_pool<User>::size() << std::endl;

	memory_pool<Admin>::end_counting();
	QString Admin_file_name = "Admin.dat";
	QFile Admin_file(path + Admin_file_name);
	Admin_file.open(QIODevice::ReadOnly);
	QDataStream Admin_fin(&Admin_file);
	memory_pool<Admin>::save(Admin_fin);
	std::cout << "Admin load success! Admin's size = " << memory_pool<Admin>::size() << std::endl;

	memory_pool<Station>::end_counting();
	QString Station_file_name = "Station.dat";
	QFile Station_file(path + Station_file_name);
	Station_file.open(QIODevice::ReadOnly);
	QDataStream Station_fin(&Station_file);
	memory_pool<Station>::save(Station_fin);
	std::cout << "Station load success! Station's size = " << memory_pool<Station>::size() << std::endl;

	memory_pool<City>::end_counting();
	QString City_file_name = "City.dat";
	QFile City_file(path + City_file_name);
	City_file.open(QIODevice::ReadOnly);
	QDataStream City_fin(&City_file);
	memory_pool<City>::save(City_fin);
	std::cout << "City load success! City's size = " << memory_pool<City>::size() << std::endl;

	memory_pool<Line>::end_counting();
	QString Line_file_name = "Line.dat";
	QFile Line_file(path + Line_file_name);
	Line_file.open(QIODevice::ReadOnly);
	QDataStream Line_fin(&Line_file);
	memory_pool<Line>::save(Line_fin);
	std::cout << "Line load success! Line's size = " << memory_pool<Line>::size() << std::endl;

	memory_pool<Train>::end_counting();
	QString Train_file_name = "Train.dat";
	QFile Train_file(path + Train_file_name);
	Train_file.open(QIODevice::ReadOnly);
	QDataStream Train_fin(&Train_file);
	memory_pool<Train>::save(Train_fin);
	std::cout << "Train load success! Train's size = " << memory_pool<Train>::size() << std::endl;

	memory_pool<Ticket>::end_counting();
	QString Ticket_file_name = "Ticket.dat";
	QFile Ticket_file(path + Ticket_file_name);
	Ticket_file.open(QIODevice::ReadOnly);
	QDataStream Ticket_fin(&Ticket_file);
	memory_pool<Ticket>::save(Ticket_fin);
	std::cout << "Ticket load success! Ticket's size = " << memory_pool<Ticket>::size() << std::endl;

	QString server_file_name = "Server.dat";
	QFile server_file(path + server_file_name);
	if (!server_file.open(QIODevice::ReadOnly)) {
		std::cout << "Load failed!" << std::endl;
		return false;
	}
	QDataStream fin(&server_file);

	fin >> server;
	memory_pool<User>::start_counting();
	memory_pool<Admin>::start_counting();
	memory_pool<Station>::start_counting();
	memory_pool<City>::start_counting();
	memory_pool<Line>::start_counting();
	memory_pool<Train>::start_counting();
	memory_pool<Ticket>::start_counting();
//	fin >> users >> admins >> lines >> cities >> stations;
	return true;
}

void sjtu::TTS::save_binary() {
	QString path = QDir::currentPath();
	path += "/../train-ticket-system/data/";
//	directory = path + "/../train-ticket-system/operation.dat";

	QString User_file_name = "Users.dat";
	QFile User_file(path + User_file_name);
	User_file.open(QIODevice::WriteOnly);
	QDataStream User_fout(&User_file);
	memory_pool<User>::save(User_fout);

	QString Admin_file_name = "Admin.dat";
	QFile Admin_file(path + Admin_file_name);
	Admin_file.open(QIODevice::WriteOnly);
	QDataStream Admin_fout(&Admin_file);
	memory_pool<Admin>::save(Admin_fout);

	QString Station_file_name = "Station.dat";
	QFile Station_file(path + Station_file_name);
	Station_file.open(QIODevice::WriteOnly);
	QDataStream Station_fout(&Station_file);
	memory_pool<Station>::save(Station_fout);

	QString City_file_name = "City.dat";
	QFile City_file(path + City_file_name);
	City_file.open(QIODevice::WriteOnly);
	QDataStream City_fout(&City_file);
	memory_pool<City>::save(City_fout);

	QString Line_file_name = "Line.dat";
	QFile Line_file(path + Line_file_name);
	Line_file.open(QIODevice::WriteOnly);
	QDataStream Line_fout(&Line_file);
	memory_pool<Line>::save(Line_fout);

	QString Train_file_name = "Train.dat";
	QFile Train_file(path + Train_file_name);
	Train_file.open(QIODevice::WriteOnly);
	QDataStream Train_fout(&Train_file);
	memory_pool<Train>::save(Train_fout);

	QString Ticket_file_name = "Ticket.dat";
	QFile Ticket_file(path + Ticket_file_name);
	Ticket_file.open(QIODevice::WriteOnly);
	QDataStream Ticket_fout(&Ticket_file);
	memory_pool<Ticket>::save(Ticket_fout);

	QString server_file_name = "Server.dat";
	QFile server_file(path + server_file_name);
	server_file.open(QIODevice::WriteOnly);
	QDataStream fout(&server_file);
	fout << server;
//	fout << users << admins << lines << cities << stations;
}


//Constructor
sjtu::TTS::TTS() {
	if (load_binary()) return;
	else load_ascii();
}
sjtu::TTS::~TTS() {
	save_binary();
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
        if (!server.check_station(station_name)) {
            QString city_name;
            city_name.push_back(station_name[0]);
            city_name.push_back(station_name[1]);
            add_station(StationData(city_name, station_name));
        }
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
    train->date = Date(train_data.date);
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
    const Station & from = *server.find_station(f);
    const Station & to   = *server.find_station(t);
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
        add.to   = ed;

        for (int i = 0; i < train.line->seat_kind_names.size(); ++i)
        {
            add.kind = i;
            int num;
            double price = 0;
            for (int p = st; p < ed; ++p) {
                price += train.line->price[i][p];
                if (num > train.station_available_tickets[i][p]) {
                    num = train.station_available_tickets[i][p];
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

sjtu::vector<QString> sjtu::TTS::q_ss(const QString &f, const QString &t, int date) {
    vector<sjtu::Ticket> tmp = q_ss_ticket(f, t, date);
    vector<QString> result;
    QString str;
    for (int i = 0; i < tmp.size(); ++i) {
        const Ticket & t = tmp[i];
        str = "";
        str += t.train->line->name + " ";
        str += t.train->line->stations[t.from]->name[t.from];
        str += "(" + t.train->line->arr(t.from) + ")" + "->";
        str += t.train->line->stations[t.to]->name[t.to];
        str += "(" + t.train->line->dep(t.to) + ")" + " ";
        str += t.train->line->seat_kind_names[t.kind] + "剩余";
        str += QString::number(t.num);

    }
    return result;
}




sjtu::vector<sjtu::query_ticket_ans> sjtu::TTS::query_city_city(const sjtu::query_ticket_cc_data & data) {
    const City & from = *server.find_city(data.dep_city);
    const City & to   = *server.find_city(data.arr_city);
    const vector<train_ptr> & trains = *query_train(from, to, Date(data.date));

    vector<query_ticket_ans> ans;

    for (int i = 0; i < (int)trains.size(); ++i) {
        Train & train = *trains[i];
        query_ticket_ans tmp;
        tmp.train_name = train.get_name();
        tmp.start_date = data.date;
        tmp.start_station = train.get_station_name(from.name);
        tmp.start_time = train.line->dep(tmp.start_station);
        tmp.end_station = train.get_station_name(to.name);
        tmp.end_time = train.line->arr(tmp.end_station);
        for (int i = 0; i < (int)train.line->seat_kind_names.size(); ++i) {
            tmp.seat_kind = train.line->seat_kind_names[i];
            tmp.ticket_left = train.min_avail(tmp.start_station, tmp.end_station, tmp.seat_kind);
            ans.push_back(tmp);
        }
    }
    return ans;
}
sjtu::vector<sjtu::query_ticket_ans> sjtu::TTS::query_station_station(const sjtu::query_ticket_ss_data & data) {
    const Station & from = *server.find_station(data.dep_satation);
    const Station & to   = *server.find_station(data.arr_station);
    const vector<train_ptr> & trains = *query_train(from, to, Date(data.date));

    vector<query_ticket_ans> ans;

    for (int i = 0; i < (int)trains.size(); ++i) {
        Train & train = *trains[i];
        query_ticket_ans tmp;
        tmp.train_name = train.get_name();
        tmp.start_date = data.date;
        tmp.start_station = data.dep_satation;
        tmp.start_time = train.line->dep(tmp.start_station);
        tmp.end_station = data.arr_station;
        tmp.end_time = train.line->arr(tmp.end_station);
        for (int i = 0; i < (int)train.line->seat_kind_names.size(); ++i) {
            tmp.seat_kind = train.line->seat_kind_names[i];
            tmp.ticket_left = train.min_avail(tmp.start_station, tmp.end_station, tmp.seat_kind);
            ans.push_back(tmp);
        }
    }
    return ans;

}

sjtu::vector<sjtu::query_my_order_ans> sjtu::TTS::query_my_order(const sjtu::query_my_order_data & data) {
    user_ptr user = server.find_user(data.ID);
    deque<ticket_ptr> &tickets = user->tickets;

    vector<query_my_order_ans> result;
    for (auto iter = tickets.begin(); iter != tickets.end(); ++iter) {
        query_my_order_ans tmp;
        Ticket &ticket = **iter;
        tmp.train_name = ticket.train->get_name();
        tmp.start_date = ticket.train->date.toStr();
        tmp.start_station = ticket.train->get_station_name(ticket.from);
        tmp.start_time = ticket.train->line->dep(ticket.from);
        tmp.end_station = ticket.train->get_station_name(ticket.to);
        tmp.end_time   = ticket.train->line->dep(ticket.to);
        tmp.seat_kind = ticket.train->line->seat_kind_names[ticket.kind];
        tmp.ticket_number = ticket.num;
        result.push_back(tmp);
    }
    return result;
}

sjtu::login_user_ans sjtu::TTS::login_user(const sjtu::login_user_data & data) {
    if (!server.check_user(data.ID)) {
        return false;
    }
    if (!server.find_user(data.ID)->check_password(data.password)) {
        return false;
    }
    return true;
}

sjtu::login_admin_ans sjtu::TTS::login_admin(const sjtu::login_admin_data & data) {
    if (!server.check_admin(data.ID)) {
        return false;
    }
    if (!server.find_admin(data.ID)->check_password(data.password)) {
        return false;
    }
    return true;
}

sjtu::return_tickets_ans sjtu::TTS::return_tickets(const sjtu::return_tickets_data & data) {
    // TODO
    Ticket tmp;
    Line &line = *server.find_line(data.train_name);
    tmp.train = line.trains[Date(data.start_date)];
    tmp.from = tmp.train->line->find_pos(data.start_station);
    tmp.to   = tmp.train->line->find_pos(data.end_station);
    tmp.kind = tmp.train->line->seat_type(data.seat_kind);
    User &user = *server.find_user(data.ID);
    for (auto iter = user.tickets.begin(); iter != user.tickets.end(); ++iter) {
        Ticket &ticket = **iter;
        if (tmp.equal_ex_num(ticket)) {
            if (ticket.num < data.ticket_number)
                return false;
            ticket.num -= data.ticket_number;
            ticket.train->add_tickets(data.start_station, data.end_station, data.seat_kind,data.ticket_number);
            if (ticket.num == 0)
                user.tickets.erase(iter);
            return true;
        }
    }
    return false;
}

sjtu::buy_tickets_ans sjtu::TTS::buy_tickets(const sjtu::buy_tickets_data & data) {
    if (!server.check_user(data.ID))
        return false;
    if (!server.check_line(data.train_name))
        return false;

    User &user = *server.find_user(data.ID);
    Line &line = *server.find_line(data.train_name);
    if (!line.check_date(Date(data.start_date)))
        return false;
    Train &train = *(line.trains[Date(data.start_date)]);
    if (train.min_avail(data.start_station, data.end_station, data.seat_kind) < data.ticket_num) {
        return false;
    }
    ticket_ptr tmp = memory_pool<Ticket>::get_T();
    tmp->train = line.trains[Date(data.start_date)];
    tmp->from = line.find_pos(data.start_station);
    tmp->to   = line.find_pos(data.end_station);
    tmp->kind = line.seat_type(data.seat_kind);
    train.add_tickets(data.start_station, data.end_station, data.seat_kind,-data.ticket_num);
    for (auto iter = user.tickets.begin(); iter != user.tickets.end(); ++iter) {
        if ((*iter)->equal_ex_num(*tmp)) {
            (*iter)->num += data.ticket_num;
            return true;
        }
    }
    tmp->price = train.calulate_price(data.start_station, data.end_station, data.seat_kind);
    tmp->num = data.ticket_num;
    user.tickets.push_back(tmp);
    return true;
}

sjtu::delete_line_ans sjtu::TTS::delete_line(const sjtu::delete_line_data & data) {
    if (!server.check_line(data))
        return false;
    server.delete_line(data);
    return true;
}

sjtu::register_user_ans sjtu::TTS::register_user(const sjtu::register_user_data & data) {
    return register_user(data.name, data.password);
}

sjtu::register_admin_ans sjtu::TTS::register_admin(const sjtu::register_admin_data & data) {
    return register_admin(data.name, data.password);
}












