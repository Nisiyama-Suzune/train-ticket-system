//
// Created by Aaron Ren on 15/04/2017.
//

#ifndef TTS_TEST_IO_UTILITIES_HPP
#define TTS_TEST_IO_UTILITIES_HPP

#include <fstream>
#include <string>
#include "list.h"

#define trivial_sl true
#define nontrivial_sl false

std::ofstream& write_str(std::ofstream & fout, const std::string & str) {
    size_t len = str.length();
    fout.write(reinterpret_cast<char*> (&len), sizeof(size_t));
    fout.write(str.c_str(), sizeof(char) * len);
    return fout;
}

std::ifstream& read_str(std::ifstream & fin, std::string & str) {
    size_t len;
    fin.read(reinterpret_cast<char*> (&len), sizeof(size_t));
    char *tmp_str = new char[len];
    fin.read(tmp_str, sizeof(char) * len);
    str = std::string(tmp_str);
    delete [] tmp_str;
    return fin;
}

template <class T>
std::streampos default_save(std::ofstream & fout, T & x) {
    std::streampos result = fout.tellp();
    fout.write(reinterpret_cast<char*>(&x), sizeof(T));
    return result;
}
template <class T>
void default_load(std::ifstream & fin, T & x) {
    fin.read(reinterpret_cast<char*>(&x), sizeof(T));
}

template <class T>
std::streampos write_list(std::ofstream & fout, list<T> & x, bool trivial) {
    std::streampos result = fout.tellp();
    list<T>::iterator iter = x.begin();
    while (iter != x.end()) {
        if (trivial)
            default_save(fout, *iter);
        else
            iter->save(fout);
        ++iter;
    }
    return result;
}
template <class T>
void read_list(std::ifstream & fin, list<T> & x, bool trivial) {
    list<T>::iterator iter = x.begin();
    while (iter != x.end()) {
        if (trivial)
            default_load(fout, *iter);
        else
            iter->load(fout);
    }
}

#endif //TTS_TEST_IO_UTILITIES_HPP
