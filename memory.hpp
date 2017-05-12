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



public:
    pool_ptr(size_t _pos, vector<T> *_container, void (*_put)(size_t))
            : pos(_pos), container(_container), put(_put) {}
    pool_ptr() : pos(0), container(nullptr), put(nullptr) {}

    pool_ptr(const pool_ptr &other)
            : pos(other.pos), container(other.container), put(other.put) {}

    bool operator==(const pool_ptr &other) {
        return pos == other.pos && container == other.container;
    }

    bool operator!=(const pool_ptr &other) {
        return !(*this == other);
    }


    T &operator*() const {
        return (*container)[pos];
    }

    T *operator->() const {
        return &(operator*());
    }
};

template <class T>
pool_ptr<T> get_T(vector<T> * container, void (*put)(size_t)) {
    container->push_back(T());
    return pool_ptr<T>(container->size(), container, put);
}

}

#endif //TTS_MEMORY_H
