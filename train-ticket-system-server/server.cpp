#include "server.h"

bool Server::add_line(const Line &x) {
    pair<std::wstring, Line> tmp(x.name, x);
    return lines.insert(tmp).second;
}

Train &Server::get_train(const Train_info &key) {
    return trains[key];
}

bool Server::add_train(const std::wstring &name, const Date &date) throw(line_error) {
    // 首先检查是否有所要添加train的line，如果没有，throw
    auto line_iter = lines.find(name);
    if (line_iter == lines.end())
        throw line_error();
    // 如果有，则检查这个date的train是否已经存在，如果有，不添加，返回false
    Line &line = line_iter->second;
    for (auto iter = line.trains.begin(); iter != line.trains.end(); ++iter) {
        if ((*iter)->date() == date)
            return false;
    }
    // 首先把train添加到server的trains里，在
    Train tmp_train(line, date);
    pair<Train_info, Train> tmp_pair(tmp_train.train_info(), tmp_train);
    return true;
}

