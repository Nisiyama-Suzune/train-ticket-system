#ifndef QUERY_H
#define QUERY_H

#include <QString>

namespace sjtu {

// add_line completed

struct login_user_data {
    int ID;
    QString password;
    login_user_data(int _ID, QString _p)
        : ID(_ID), password(_p){}
};
typedef bool login_user_ans;

struct login_admin_data {
    int ID;
    QString password;

    login_admin_data(int _ID, QString _p)
        : ID(_ID), password(_p) {}
};
typedef bool login_admin_ans;

struct query_ticket_cc_data {
    QString dep_city;
    QString arr_city;
    int date;
    query_ticket_cc_data(QString a, QString b, int c) :
        dep_city(a), arr_city(b), date(c){}
};
struct query_ticket_ss_data {
    QString dep_satation;
    QString arr_station;
    int date;
    query_ticket_ss_data(QString a, QString b, int c) :
        dep_satation(a), arr_station(b), date(c){}
};
struct query_ticket_ans {
    // 返回vector
    QString train_name;
    int start_date;
    QString start_station;
    QString start_time;
    QString end_station;
    QString end_time;
    QString seat_kind;
    int ticket_left;
    QString toQString()
    {
        return (train_name + ' ' + start_date + ' ' + start_station + ' ' + start_time + ' ' + end_station + ' '
                + end_time + ' ' + seat_kind + ' ' + ticket_left + '\n');
    }
};

struct query_my_order_data {
    int ID;
    query_my_order_data(int _id) : ID(_id) {}
};
struct query_my_order_ans
{
    // 返回vector
    QString train_name;
    QString start_date;
    QString start_station;
    QString start_time;
    QString end_station;
    QString end_time;
    QString seat_kind;
    int ticket_number;
    QString toQString()
    {
        return (train_name + ' ' + start_date + ' ' + start_station + ' ' + start_time + ' ' + end_station + ' '
                + end_time + ' ' + seat_kind + ' ' + ticket_number + '\n');
    }
};

struct return_tickets_data {
    int ID;
    QString train_name;
    QString start_date;
    QString start_station;
    QString end_station;
    QString seat_kind;
    int ticket_number;
    QString toQString()
    {
        return (train_name + ' ' + start_date + ' ' + start_station +  ' ' + end_station + ' '
                + seat_kind + ' ' + ticket_number + '\n');
    }
};
typedef bool return_tickets_ans;

struct buy_tickets_data {
    int ID;
    QString train_name;
    int start_date;
    QString start_station;
    QString end_station;
    QString seat_kind;
    int ticket_num;
    buy_tickets_data(){}
    buy_tickets_data(int id ,QString a, int b, QString c, QString d, QString e, int f):
        ID(id), train_name(a), start_date(b), start_station(c), end_station(d), seat_kind(e), ticket_num(f){}
    // 数据成员太多了，构造函数太长了，
    // 你到时候声明一个变量逐步赋值好了。
    // 活在梦里，构造函数和变量逐步赋值哪个长
};
typedef bool buy_tickets_ans;

typedef QString delete_line_data; // line_name
typedef bool delete_line_ans;

struct register_user_data {
    QString name;
    QString password;
    register_user_data(QString _name, QString _p)
        : name(_name), password(_p) {}
};
typedef int register_user_ans; // ID

struct register_admin_data {
    QString name;
    QString password;
    register_admin_data(QString _name, QString _p)
        : name(_name), password(_p) {}
};
typedef int register_admin_ans; // ID

struct add_line_data
{
    QString train_name;
    QString more_info;
    add_line_data(QString a, QString b):
        train_name(a), more_info(b){}
    QString toQString()
    {
        return (train_name + '\n' + more_info);
    }
};

/*函数接口名
 *
 * query_city_city
 * query_station_station
 * add_line
 * delete_line
 * */

}

#endif // QUERY_H
