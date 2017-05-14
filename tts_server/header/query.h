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
};
struct query_ticket_ss_data {
    QString dep_satation;
    QString arr_station;
    int date;
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
};

struct return_tickets_data {
    int ID;
    QString train_name;
    QString start_date;
    QString start_station;
    QString end_station;
    QString seat_kind;
    int ticket_number;
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
    // 数据成员太多了，构造函数太长了，
    // 你到时候声明一个变量逐步赋值好了。
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

/*函数接口名
 *
 * query_city_city
 * query_station_station
 * add_line
 * delete_line
 * */

}

#endif // QUERY_H
