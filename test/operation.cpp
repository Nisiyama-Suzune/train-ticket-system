#include "vector.hpp"
#include <QDir>
#include <qfile.h>
#include <iostream>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qstring.h>

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
	int date;
	friend QTextStream& operator << (QTextStream& out, const buy_data& x) {
		out << "name         = " << x.name << endl;
		out << "ID           = " << x.ID << endl;
		out << "operation    = " << x.operation << endl;
		out << "num          = " << x.num << endl;
		out << "kind_of_seat = " << x.kind_of_seat << endl;
		out << "train_ID     = " << x.train_ID << endl;
		out << "from_station = " << x.from_station << endl;
		out << "to_station   = " << x.to_station << endl;
		out << "date         = " << x.date << endl;
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
	cout << ans << endl;
	return ans;
}
void test4()
{
	QString directory = QDir::currentPath();
	QTextStream cout(stdout);
	cout << directory << endl;
	directory += "/../load/operation.out";
	QFile file(directory);
//	QFile file("/home/yanhongyu/Git/Data_structure/load/operation.out");
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		std::cout << "open file fail" << std::endl;
		return ;
	}
	QTextStream fin(&file);
//	QTextStream cout(stdout);

	QString ans;
	while (fin.readLineInto(&ans)) {
		buy_data buy_ans = operation_transform(ans);
	}
}
