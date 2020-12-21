#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP
#include "exceptions.hpp"
#include <iostream>
#include <climits>
#include <cstddef>
#include <QDataStream>
#include <QTextStream>


namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
	T * storage;
	int MaxSize;
	int NowLength;
    void DoubleSpace()
    {
        T * tem = storage;
        MaxSize *= 2;
        storage = (T*)operator new[](MaxSize * sizeof(T));
        for(int i = 0; i < NowLength; ++i)
        {
            new(storage + i) T(tem[i]);
            tem[i].~T();
        }
        operator delete [](tem);
    }
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
		friend class vector;
	private:
		int pos;
		vector<T> * vec;
 		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**

		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		iterator(int obj = 0, vector<T> *p = nullptr) : pos(obj), vec(p){}
		iterator operator+(const int &n) const
		{
			return iterator(pos + n, vec);
			//TODO
		}
		iterator operator-(const int &n) const
		{
			return iterator(pos - n, vec);
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if(vec != rhs.vec) throw invalid_iterator();
			if(pos > rhs.pos) return pos - rhs.pos;
			return rhs.pos - pos;
		}
		iterator operator+=(const int &n)
		{
			pos += n;
			return *this;
			//TODO
		}
		iterator operator-=(const int &n)
		{
			pos -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			int obj = pos;
			++pos;
			return iterator(obj, vec);
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			++pos; return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			int obj = pos;
			++pos;
			return iterator(obj, vec);
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			--pos;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return vec -> storage[pos];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const
		{
			return (vec == rhs.vec && pos == rhs.pos);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return (vec != rhs.vec || pos != rhs.pos);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{

		friend class vector;
	private:
		int pos;
		const vector<T> * vec;
	public:
	    const_iterator(int obj = 0,const vector<T> * p = nullptr) : pos(obj), vec(p){}
		const_iterator operator+(const int &n) const
		{
			return const_iterator(pos + n, vec);
			//TODO
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(pos - n, vec);
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			if(vec != rhs.vec) throw invalid_iterator();
			if(pos > rhs.pos) return pos - rhs.pos;
			return rhs.pos - pos;
			//TODO
		}
		const T& operator*() const {
			return vec -> storage[pos];
		}
		const T* operator->() const {
            return &(operator*());
		}

		bool operator==(const const_iterator &rhs) const
		{
			return (vec == rhs.vec && pos = rhs.pos);
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return (vec != rhs.vec || pos != rhs.pos);
		}
		const_iterator operator++(int)
		{
			int obj = pos;
			++pos;
			return const_iterator(obj, vec);
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
			++pos; return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			int obj = pos;
			--pos;
			return const_iterator(obj, vec);
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
			--pos; return *this;
		}


	};

	vector(int initial = 100)
	{
		storage = (T*)operator new[](initial * sizeof(T));
		MaxSize = initial;
		NowLength = 0;
	}
	vector(const vector &other)
	{
		MaxSize = other.MaxSize;
		NowLength = other.NowLength;
		storage = (T*)operator new[](MaxSize * sizeof(T));
		//std::cout << "hey" << '\n';
		for(int i = 0; i < NowLength; ++i)
		{
		    new(storage + i)T(other.storage[i]);
			//storage[i] = other.storage[i];
		}
	}

	~vector()
	{
	    for(int i = 0; i < NowLength; ++i)
        {
            (storage + i) -> ~T();
        }
		operator delete [](storage);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
		if (this == &other)
			return *this;
		MaxSize = other.MaxSize;
		NowLength = other.NowLength;
		for(int i = 0; i < NowLength; ++i)
		{
			new(storage + i)T(other.storage[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
		if(pos < 0 || pos >= NowLength)
			throw index_out_of_bound();
		return storage[pos];
	}
	const T & at(const size_t &pos) const
	{
		if(pos < 0 || pos >= NowLength)
			throw index_out_of_bound();
		return storage[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
		if(NowLength == 0)
			throw container_is_empty();
		return storage[pos];
	}
	const T & operator[](const size_t &pos) const
	{
		if(NowLength == 0)
			throw container_is_empty();
		return storage[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const
	{
		if(NowLength == 0)
			throw container_is_empty();
		return storage[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
	{
		if(NowLength == 0)
			throw container_is_empty();
		return storage[NowLength - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		return iterator(0, this);
	}
	const_iterator cbegin() const
	{
		return const_iterator(0, this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		return iterator(NowLength, this);
	}
	const_iterator cend() const
	{
		return const_iterator(NowLength, this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const
	{
		return (NowLength == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return NowLength;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const
	{
		return MaxSize;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		NowLength = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
		if(NowLength == MaxSize - 1) DoubleSpace();
		new(storage + NowLength) T(storage[NowLength - 1]);
		for(iterator iter = end() - 1; iter != pos; --iter)
		{
			*iter = *(iter - 1);
		}
		++NowLength;
		*pos = value;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
		if(NowLength == MaxSize) DoubleSpace();
		if(ind > NowLength) throw index_out_of_bound();
		new(storage + NowLength) T(storage[NowLength - 1]);
		for(iterator iter = end() - 1; iter != iterator(ind, this); --iter)
		{
			*iter = *(iter - 1);
		}
		++NowLength;

		//new (storage + int) T(value);
		storage[ind] = value;
		return iterator(ind, this);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
		if(*pos == NowLength - 1)
		{
			--NowLength;
			return end();
		}
		for(iterator iter = pos; iter != end() - 1; ++iter)
		{
			*iter = *(iter + 1);
		}
		--NowLength;
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
		if(ind == NowLength - 1)
		{
			--NowLength;
			return end();
		}
		for(iterator iter = iterator(ind, this); iter != end() - 1; ++iter)
		{
			*iter = *(iter + 1);
		}
		--NowLength;
		return iterator(ind, this);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
		if(NowLength == MaxSize - 1)
			DoubleSpace();
        new(storage + NowLength) T(value);
		NowLength++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
	{
		if(NowLength == 0) throw container_is_empty();
		--NowLength;
	}
	friend QDataStream& operator >> (QDataStream &in, vector &rhs) {
		int tmp;
		in >> tmp;
		rhs.clear();
		for (int i = 0; i < tmp; ++i) {
            rhs.push_back(T());
            in >> rhs.storage[rhs.NowLength - 1];
		}
		return in;
	}
	friend QDataStream& operator << (QDataStream &out, const vector &rhs) {
		out << rhs.NowLength;
		for (int i = 0; i < rhs.NowLength; ++i)
			out << rhs.storage[i];
		return out;
	}
};

}


#endif
