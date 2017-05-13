//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_MEMORY_H
#define TTS_MEMORY_H

#include "vector.hpp"
#include <QDataStream>

/// pool_ptr, allocate
namespace sjtu {
template<class T>
class pool_ptr {
    friend pool_ptr<T> get_T(vector<T> * container, void (*put)(int), vector<int>*);

protected:
    int pos;
    vector<T> *container;
    void (*put)(int);
    vector<int> *cnt;

    void terminate();

public:
    pool_ptr();
    explicit pool_ptr(int, vector<T>*, void (*)(int), vector<int>*);
    pool_ptr(const pool_ptr &other);
    ~pool_ptr();
    const pool_ptr &operator=(const pool_ptr &ptr);

    bool expired() const;

    /// ptr operations

    T &operator*() const {
        return (*container)[pos];
    }

    T *operator->() const {
        return &(operator*());
    }

    bool operator==(const pool_ptr &other) {
        return pos == other.pos && container == other.container;
    }

    bool operator!=(const pool_ptr &other) {
        return !(*this == other);
    }

public:
    /// output
    void save(QDataStream& out) {
        out << pos;
    }

	///input
	template <class pool>
	void load(QDataStream& in) {
		in >> pos;
		pos = pos;
		container = (vector<T>*)pool::container[T::Type];
		put = pool::put[T::Type];
        cnt = pool::cnt[T::Type];
	}
};



template <class T>
pool_ptr<T> get_T(vector<T> * container,
                  void (*put)(int), vector<int>* cnt) {
    container->push_back(T());
    return pool_ptr<T>((int)container->size(), container, put, cnt);
}

template <class T>
void pool_ptr<T>::terminate() {
    if (cnt != nullptr) {
        --(*cnt)[pos];
        if ((*cnt)[pos] == 0) {
            put(pos);
        }
    }
}

template <class T>
pool_ptr<T>::pool_ptr() {
    pos = 0;
    container = nullptr;
    put = nullptr;
    cnt = nullptr;
}

template <class T>
pool_ptr<T>::pool_ptr(int _pos, vector <T> * _container,
                      void (*_put)(int), vector<int>* _cnt) {
    pos = _pos;
    container = _container;
    put = _put;
    cnt = _cnt;
    (*cnt)[pos] = 1;
}

template <class T>
pool_ptr<T>::pool_ptr(const pool_ptr<T> &other) {
    pos = other.pos;
    container = other.container;
    put = other.put;
    cnt = other.cnt;
    if (cnt != nullptr)
        ++(*cnt)[pos];
}

template <class T>
pool_ptr<T>::~pool_ptr() {
    terminate();
}

template <class T>
const pool_ptr<T> &pool_ptr<T>::operator=(const pool_ptr<T> &ptr) {
    if (this == &ptr)
        return *this;
    terminate();
    pos = ptr.pos;
    container = ptr.container;
    put = ptr.put;
    cnt = ptr.cnt;
    if (cnt != nullptr)
        ++(*cnt)[pos];
    return ptr;
}

template <class T>
bool pool_ptr<T>::expired() const {
    return (*cnt)[pos] == 0;
}


}

#endif //TTS_MEMORY_H
