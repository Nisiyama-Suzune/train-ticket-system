//
// Created by Aaron Ren on 15/04/2017.
//

#ifndef TTS_TEST_IO_UTILITIES_HPP
#define TTS_TEST_IO_UTILITIES_HPP

#include <fstream>
#include <string>
#include "list.hpp"

#define trivial_sl true
#define nontrivial_sl false

std::ofstream& write_str(std::ofstream & fout, const std::wstring & str);

std::ifstream& read_str(std::ifstream & fin, std::wstring & str);

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

#endif //TTS_TEST_IO_UTILITIES_HPP
