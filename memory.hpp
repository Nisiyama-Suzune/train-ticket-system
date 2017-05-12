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
    friend class train_memory_pool;
    friend class account_memory_pool;
    friend pool_ptr<T> get_T(vector<T> * container);

protected:
    size_t pos;
    vector<T> *container;


public:
    pool_ptr(size_t _pos, vector<T> *_container)
            : pos(_pos), container(_container) {}
    pool_ptr() : pos(0), container(nullptr) {}

    pool_ptr(const pool_ptr &other)
            : pos(other.pos), container(other.container) {}

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
pool_ptr<T> get_T(vector<T> * container) {
    container->push_back(T());
    return pool_ptr<T>(container->size(), container);
}

}

#endif //TTS_MEMORY_H
