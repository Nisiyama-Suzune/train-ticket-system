#ifndef QUERY_H
#define QUERY_H

#include <QString>

namespace sjtu {



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

struct query_my_order
{
    QString train_name;
    int start_date;
    QString start_station;
    QString start_time;
    QString end_station;
    QString end_time;
    QString seat_kind;
    int ticket_number;
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
