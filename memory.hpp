//
// Created by Aaron Ren on 11/05/2017.
//

#ifndef TTS_MEMORY_H
#define TTS_MEMORY_H

#include "vector.hpp"
#include "exceptions.hpp"
#include <QDataStream>


namespace sjtu {

template<class T>
class memory_pool {
    friend class pool_ptr;

private:
    static vector<T>   container;
    static vector<int> recycler;
    static vector<int> counter;
    static bool is_counted;
public:
	static void save(QDataStream& out) {
		end_counting();
		out << counter;
		out << recycler;
		out << container;
		start_counting();
	}
	static void load(QDataStream &in) {
		end_counting();
		in >> counter;
		using std::cout;
		using std::endl;
		cout << "counter.size() = " << counter.size()  << endl;
		for (int i = 0; i < counter.size(); ++i)
			cout << counter[i] << ' ' ;
		cout << endl;
		in >> recycler;
		in >> container;
		start_counting();
	}

public:
    class pool_ptr {
        friend class memory_pool;

    private:
        int pos;

        void terminate();

    public:
        pool_ptr() : pos(-1) {}
        explicit pool_ptr(int _pos) : pos(_pos) {
            if (pos != -1 && is_counted)
                memory_pool::counter[pos]++;
        }
        pool_ptr(const pool_ptr & other) : pos(other.pos) {
            if (pos != -1 && is_counted)
                memory_pool::counter[pos]++;
        }
        ~pool_ptr() {
            terminate();
        }
        const pool_ptr &operator=(const pool_ptr & other) {
            if (this == &other)
                return *this;
            terminate();
            pos = other.pos;
            if (pos != -1 && is_counted)
                memory_pool<T>::counter[pos]++;
            return *this;
        }

        T& operator*() const {
            if (pos == -1)
                throw invalid_iterator();
            return memory_pool::container[pos];
        }
        T* operator->() const {
            return &(operator*());
        }

        bool operator==(const pool_ptr& rhs) {
            return pos == rhs.pos;
        }
        bool operator!=(const pool_ptr& rhs) {
            return pos != rhs.pos;
        }

        friend QDataStream& operator >>(QDataStream &in, pool_ptr &rhs) {
            in >> rhs.pos;
            return in;
        }
        friend QDataStream& operator <<(QDataStream &out,const pool_ptr& rhs) {
            out << rhs.pos;
            return out;
        }
    };

    static void put_T(int pos);
public:
    static pool_ptr get_T(T a = T());
    static int size();
	static void start_counting() {
            is_counted = 1;
        }
	static void end_counting() {
            is_counted = 0;
        }
};

/// memory_pool
template<class T>
vector<T> memory_pool<T>::container;
template <class T>
vector<int> memory_pool<T>::recycler;
template <class T>
vector<int> memory_pool<T>::counter;
template <class T>
bool memory_pool<T>::is_counted = 0;

// get & put
template<class T>
typename memory_pool<T>::pool_ptr memory_pool<T>::get_T(T a) {
    if (recycler.empty()) {
        container.push_back(a);
		counter.push_back(1);
        return pool_ptr((int)container.size() -1);
    }
    int pos = recycler.back();
    counter[pos] = 0;
	container[pos] = a;
    recycler.pop_back();
    return pool_ptr(pos);
}
template <class T>
void memory_pool<T>::put_T(int pos) {
    if (pos != -1 && is_counted) {
        recycler.push_back(pos);
    }
}

template <class T>
int memory_pool<T>::size() {
	return container.size() - recycler.size();
}


/// pool_ptr
template <class T>
void memory_pool<T>::pool_ptr::terminate() {
    if (pos == -1 || !is_counted)
        return;
    --memory_pool<T>::counter[pos];
	if (memory_pool<T>::counter[pos] == 0) {
        put_T(pos);
    }
}

}

#endif //TTS_MEMORY_H
