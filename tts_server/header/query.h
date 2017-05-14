#ifndef QUERY_H
#define QUERY_H

#include <QString>

namespace sjtu {

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

/*函数接口名
 *
 * query_city_city
 * query_station_station
 * add_line
 * delete_line
 * */

}

#endif // QUERY_H
