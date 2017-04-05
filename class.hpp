/* Need to do:
 * Train & Line's definition
 * How to save train info
 *
 *
 *
 */
#include <string>
#include "vector.h"
#include "smart_ptr.h"
class Server
{
	class Date;
	class Ticket;
	class Station; class Line; class Train;
	class Account; class Admin; class User;
	class Log; //TODO

	enum KIND
	{
		first_seat, second_seat, hard_seat, hard_bed, soft_bed, no_seat//no_seat = standing
	};
	class Date
	{
	private:
		int year, month, day;
		int hour, min, sec;
	public:
		Date (int y = 0, int m = 0, int d = 0, int h = 0, int min = 0, int sec = 0):
			year(y), month(m), day(d), hour(h), min(min), sec(sec) {}
		friend bool operator < (const Date &lhs, const Date &rhs);
	};
	class Ticket : public Date //users return tickets by date? //TODO
	{
	private:
		int from, to;
		KIND kind;
		std::string name; // kind_name Ex. hard_seat
	public:
		Ticket (int from, int to, KIND kd):
			from(from), to(to), kind(kd) {}
	};

	class Station
	{
		Date arrival_time, departure_time;
		std::string name;
		int miles;
		vector<Line*> lines; //maybe ? set<pair<Line*, int> > //this is (*line)'s int'th station
	};
	class Line
	{
	private:
		vector<Station> stations;
	};

	/**Same line share one line object
	 * Only Date is different
	 */
	class Train
	{
	private:
		share_ptr<Line> which_line;
		Date date;
		map<KIND, vector<int> >station_available_ticket;
		/* saves the number of remaining tickets for each station
		 * e.g. station 1--2--3--4--5 with capacity 200 seats, then
		 * station_available_ticket[] = {200, 200, 200, 200} //Only four interval
		 * if a customer bought a ticket from 2 to 4, then
		 * station_available_ticket[] = {200, 199, 199, 200}
		 */
		bool selling;
	public:
		Train (const Line &line, const Date &date) : date(date)
		{
			which_line = &line;
		}
		bool have_ticket (int from, int to, KIND kind, int num);
		friend void buy_ticket(Train& train, int from, int to, KIND kind, int num);
		//when buying, change the number of remained tickets
		friend void return_ticket(Train& train, int from, int to, KIND kind, int num);
		//if (lines[i].have_ticket(from, to, num)) now_user->buy_ticket(i, from, to, kind, num);
	};

	class Account
	{
	protected:
		std::string ID;
		std::string password;//UI will encrypet it and here save the encrypted password
		std::string name;
	public:
		Account();
		virtual void check_log() = 0; // only Admin have these virtual functions
		virtual void add_line() = 0; //maybe we can let the orginal virtual functions puts"You don't have the permission!"?
		virtual void delete_line() = 0;

		//User's rights
		virtual void buy_ticket() = 0;
		virtual void return_ticket() = 0;
		virtual void query() = 0;
	};

	class Admin : public Account
	{
		void check_log();
		void add_line();
		void delete_line();
		void delete_account();
	};

	class User : public Account
	{
	private:
		vector <Ticket> tickets;
	public:
		void buy_ticket(Train& train, int from, int to, KIND kind, int num);
		void return_ticket(Train& train, int from, int to, KIND kind, int num);
		void update_password(const std::string& pwd);//double check at UI
	};

//Server's definition
	typedef list<Train> TrainContainer; //can be easily changed in the future //mayby map or hash
private:
	AccountContainer  accounts;
	TrainContainer    trains;
	LineContainer     lines;
	StationContainer  stations;
	LogContainer      logs;
public:
	void Register(const std::string& ID, const std::string& password, const std::string& name);
	Account* login(const std::string &ID, const std::string& password);
	//if login failed, return nullptr
	friend Admin;
	/**
	 *  friend Admin::check_log(); //virtual functions
	 *  friend Admin::add_line();
	 *  friend Admin::delete_line();
	 *  friend Admin::
	 */
}server;

