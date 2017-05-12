//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_MEMORY_H
#define TTS_MEMORY_H

#include "vector.hpp"

/// pool_ptr, allocate
namespace sjtu {
template<class T>
class pool_ptr {
    friend pool_ptr<T> get_T(vector<T> * container, void (*put)(size_t));

protected:
    size_t pos;
    vector<T> *container;
    void (*put)(size_t);

    int *cnt;
    void terminate();

public:
    pool_ptr();
    explicit pool_ptr(size_t, vector<T>*, void (*)(size_t));
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
};

template <class T>
void pool_ptr<T>::terminate() {
    if (cnt != nullptr) {
        --(*cnt);
        if (*cnt == 0) {
            put(pos);
            delete cnt;
        }
    }
}

template <class T>
pool_ptr<T> get_T(vector<T> * container, void (*put)(size_t)) {
    container->push_back(T());
    return pool_ptr<T>(container->size(), container, put);
}

template <class T>
pool_ptr<T>::pool_ptr() {
    pos = 0;
    container = nullptr;
    put = nullptr;
    cnt = nullptr;
}

template <class T>
pool_ptr<T>::pool_ptr(size_t _pos, vector <T> * _container, void (*_put)(size_t)) {
    pos = _pos;
    container = _container;
    put = _put;
    cnt = new int(1);
}

template <class T>
pool_ptr<T>::pool_ptr(const pool_ptr<T> &other) {
    pos = other.pos;
    container = other.container;
    put = other.put;
    cnt = other.cnt;
    if (cnt != nullptr)
        ++(*cnt);
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
        ++(*cnt);
    return ptr;
}

template <class T>
bool pool_ptr<T>::expired() const {
    return (cnt == nullptr);
}


}

#endif //TTS_MEMORY_H
