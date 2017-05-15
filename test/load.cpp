#include <iostream>
#include "vector.hpp"
#include <qchar.h>
#include "vector.hpp"
#include <string>
#include <sstream>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>

const int N = 100010;
using namespace sjtu;
/*
void Line::load_from_wstring(wstring str)
{
	isstream sin(str);
	sin >> name;
	for (int i = 1; i <= 7; ++i) sin >> ',';
	sin >> "站名";
}
*/
wchar_t data[] = 
L"D5911/D5914,,,,,,,\n站名,日期,到达时间,停车时间,里程,二等座,一等座,无座\n利川,2017/3/28,起点站,14:38,0km,-,-,-\n恩施,2017/3/28,15:10,15:17,61km,￥18,￥21.5,￥18\n建始,2017/3/28,15:39,15:41,96km,￥28,￥33.5,￥28\n宜昌东,2017/3/28,17:08,17:13,275km,￥81,￥97,￥81\n枝江北,2017/3/28,17:31,17:33,319km,￥93.5,￥112,￥93.5\n荆州,2017/3/28,17:51,17:54,363km,￥106.5,￥128,￥106.5\n潜江,2017/3/28,18:20,18:22,433km,￥127,￥152.5,￥127\n汉川,2017/3/28,18:54,18:56,521km,￥153,￥183.5,￥153\n汉口,2017/3/28,19:21,19:25,567km,￥166.5,￥199.5,￥166.5\n武汉,2017/3/28,19:49,19:53,603km,￥176.5,￥212.5,￥176.5\n黄冈西,2017/3/28,20:18,20:20,657km,￥194.5,￥234.5,￥194.5\n黄冈,2017/3/28,20:25,20:27,663km,￥196.5,￥236.5,￥196.5\n黄冈东,2017/3/28,20:35,终到站,668km,￥198.5,￥238.5,￥198.5\n";

void pretest()
{
    using namespace std;
    cout << "-----------------PRETEST-----------------" << endl;
    char s[101] = "D5911/D5914,,,,,,,\n站名,日期,到达时间,停车时间,里程,二等座,一等座,无座\n";
    string name;
    istringstream sin(s);
    cout << string(name) << endl;
    getline(sin, name, ',');
    cout << name << endl;
    getline(sin, name, ',');
}
void test1()
{
	using namespace std;
    cout << "------------------TEST1 -----------------" << endl;
	wistringstream sin(data);
	std::wstring name, tmp;
    getline(sin, name, L',');
	wcout << name << endl;
    for (int i = 1; i <= 20; ++i) {
		getline(sin, tmp, L',');
		wcout << tmp << endl;
	}
    cout << endl << endl;
}

void test2()
{
    std::cout << "------------------TEST2 ------------------" << std::endl;
	QFile file("/home/yanhongyu/Git/Data_structure/load/data.csv");
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "open file fail" << std::endl;
		return;
	}
    QString name;
    if (true) {
        QTextStream sin(&file);
		name = sin.readLine();
		QStringList parts = name.split(",");
		QTextStream cout(stdout);
		cout << parts[0] << endl;
	}
}

struct Line_Data
{
public:
	QString name;
	vector<QString> seat_kind_names;
	vector<QString> stations;
	vector<int> time_arrive, time_stop;
	vector<int> miles;
	vector<vector<double> > prices;
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
};

Line_Data transform(QString str)
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
	cout << ans << endl;
	return ans;
}

bool is_train_type(QChar ch)
{
	return ch == 'D' || ch == 'G' || ch == 'K' || ch == 'Z' || ch == 'C';
}

void test3()
{
	QFile file("/home/yanhongyu/Git/Data_structure/load/data.csv");
//	QFile file("/home/yanhongyu/Git/Data_structure/load/data.csv");
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "open file fail" << std::endl;
		return;
	}
	QTextStream fin(&file);
	QTextStream cout(stdout);

	QString ans = fin.readLine(), tmp;

	while (fin.readLineInto(&tmp)) {
		if (is_train_type(tmp[0])) {
			Line_Data data_ans = transform(ans);
			ans = tmp;
			continue;
		} else ans = ans + '\n' + tmp;
	}
	Line_Data data_ans = transform(ans);
}

/*
extern void test4();
int main() 
{
//	pretest ();
//	test1();
//	test2();
	test3();
	test4();
	return 0;
}
*/

/* __add_station( name_of_station, name_of_city)
 * __add_station( name_of_station);
 */
