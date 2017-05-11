#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <iostream>
#include "exceptions.hpp"
#include "utility.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque {
	enum {buffer_size = 300};
	struct node {
		T* v;
		node *prev;
		node *next;
		node () : v(nullptr), prev(nullptr), next(nullptr) {}
		node (const T& val) : v(new T(val)), prev(nullptr), next(nullptr) {}
		/*
		node (const node &src) : prev(nullptr), next(nullptr) {
			if (src.v == nullptr) v = nullptr;
			else v = new T(*src.v);
			if (src.next != nullptr) {
				next = new node(*src.next);
				next -> prev = this;
			}
		}
		*/
		node (const node &src) : prev(nullptr), next(nullptr) {
			if (src.v == nullptr) v = nullptr;
			else v = new T(*src.v);
		}
		~node() {
			if (v != nullptr) delete v;
		}
		void destruct() {
			if (next != nullptr) next -> destruct();
			delete next;
		}
		void copy(const node* j) {
			node *i = this;
//			const node* j = src;
			while (j -> next != nullptr) {
				i -> next = new node(*j -> next);
				i -> next -> prev = i;
				i = i -> next;
				j = j -> next;
			}
		}
	};
	struct map_node {
		node *first, *last;
		int size;
		map_node() {
			size = 0; 
			first = last = nullptr;
		}
		map_node (const map_node & src) {
			first = src.first; last = src.last;
			size = src.size;
		}
	};
	/*
	void __insert(map_node *x, int pos, const T& val) {
		node *t = x -> first;
		for (int i = 0; i < pos; ++i) t = t -> next;
		node *y = new T(val);
		y -> next = t -> next;
		t -> next = y;
		y -> prev = t;
		if (t -> next != nullptr) t -> next -> prev = y;
		++(x -> size);
		
	}
	void mergeback(map_node *x) {
		if (x -> next == nullptr) return;
		if (x -> size >= buffer_size >> 1) return;
		if (x -> next -> size > buffer_size >> 1) return;
		map_node *y = x -> next;
		x -> size += y -> size;
		x -> next = y -> next;
		x -> last = y -> last;
		delete y;
	}
	*/
	pair<map_node, map_node> split(map_node x, int where) {
		if (where > x.size) throw "NO!";
		node *t = x.first;
		for (int i = 1; i < where; ++i) t = t -> next;
		map_node y;
		y.first = t -> next;
		y.size = x.size - where;
		y.last = x.last;

		x.last = t;
		x.size = where;
		return pair<map_node, map_node>(x, y);
	}
public:
	class const_iterator;
	class iterator {
		friend deque;
	private:
		node *p;
		int pos;
		const deque* which;
		/**
		 * todo add data members
		 *   just add whatever you want.
		 */
	public:
		iterator () {
			p = nullptr;
			which = nullptr;
			pos = -1;
		}
		iterator (node* const& p, int pos, const deque* const& which)
			: p(p), pos(pos), which(which) {}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator + (const int &n) const {
			if (n < 0) return *this - (-n);
			iterator ans(*this);
			int tmp = n;
			map_node *map = ans.which -> map;
			while (ans.p != map[ans.pos].last && tmp) {
				ans.p = ans.p -> next;
				--tmp;
			}
			if (tmp && ans.p == map[ans.pos].last) {
				if (ans.pos < ans.which -> last) {
					ans.p = ans.p -> next;
					++ans.pos;
					--tmp;
				} else throw invalid_iterator(); //undefined behavior
			}
			while (tmp >= map[ans.pos].size) {
				tmp -= map[ans.pos].size;
				++ans.pos;
				ans.p = map[ans.pos].first;
			}
			while (tmp--) {
				ans.p = ans.p -> next;
			}
			return ans;
		}
		iterator operator - (const int &n) const {
			if (n < 0) return *this + (-n);
			iterator ans(*this);
			int tmp = n;
			map_node *map = which -> map;
			while (ans.p != map[pos].first && tmp) {
				ans.p = ans.p -> prev;
				--tmp;
			}
			if (tmp && ans.p == map[ans.pos].first) {
				if (ans.pos > which -> first) {
					ans.p = ans.p -> prev;
					--ans.pos;
					--tmp;
				} else throw invalid_iterator();
			}
			while (tmp >= map[ans.pos].size) {
				tmp -= map[ans.pos].size;
				--ans.pos;
				ans.p = map[ans.pos].last;
			}
			while (tmp--) {
				ans.p = ans.p -> prev;
			}
			return ans;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invalid_iterator.
		int operator - (const iterator &rhs) const {
			if (which != rhs.which) throw invalid_iterator();
			map_node *map = which -> map;
			if (pos == rhs.pos) {
				node *x = p, *y = rhs.p;
				int ans = 0;
				bool find = 0;
				for (; x != map[pos].last -> next; x = x -> next) {
					++ans;
					if (x == y) {
						find = 1;
						break;
					}
				}
				if (find) return -ans + 1;
				for (ans = 0, x = p; y != x; y = y -> next) ++ans;
//				std::cout << " ans = " << ans << std::endl;
				return ans;
			} else {
				node *x = p, *y = rhs.p;
				int sign = -1, from = pos, to = rhs.pos;
				if (rhs.pos < pos) {
					x = rhs.p; from = rhs.pos;
					y = p;     to = pos;
					sign = 1;
				} else {
					x = p;     from = pos;
					y = rhs.p; to = rhs.pos;
					sign = -1;
				}
				int ans = 0;
				for (; x != map[from].last -> next; x = x -> next) ++ans;
				for (int tmp = from + 1; tmp < to; ++tmp) ans += map[tmp].size;
				x = map[to].first;
				for (; x != y && x != map[to].last -> next; x = x -> next) ++ans;
				if (x != y) throw "???";
				return ans * sign;
			}
		}
		iterator& operator+=(const int &n) {
			return *this = *this + n;
		}
		iterator& operator-=(const int &n) {
			return *this = *this - n;
		}
		iterator operator++(int) {
			if (p -> next == nullptr) throw index_out_of_bound();
			iterator tmp(*this);
			*this = *this + 1;
			return tmp;
		}
		iterator& operator++() {
			if (p -> next == nullptr) throw index_out_of_bound();
			return *this = *this + 1;
		}
		iterator operator--(int) {
			if (p -> prev == nullptr) throw index_out_of_bound();
			iterator tmp(*this);
			*this = *this - 1;
			return tmp;
		}
		iterator& operator--() {
			if (p -> prev == nullptr) throw index_out_of_bound();
			return *this = *this - 1;
		}
		T& operator*() const {
			if (p == nullptr) throw invalid_iterator();
			if (p -> v != nullptr) return *(p -> v);
			else throw invalid_iterator();
		}
		T* operator->() const noexcept {
			if (p == nullptr) throw invalid_iterator();
			return (p -> v);
		}
		bool operator==(const iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	class const_iterator {
	// it should has similar member method as iterator.
	//  and it should be able to construct from an iterator.
	private:
		const node *p;
		int pos;
		const deque *which;
		// data members.
	public:
		const_iterator() {
			p = nullptr;
			which = nullptr;
			pos = -1;
			// todo
		}
		const_iterator(const const_iterator &other) : p(other.p),
			pos(other.pos), which(other.which) {}
		const_iterator(const iterator &other) : p(other.p),
			pos(other.pos), which(other.which) {}
		const_iterator(const node* const& p, const int& pos, const deque* const& which)
			:p(p), pos(pos), which(which) {}
		const_iterator operator + (const int &n) const {
			if (n < 0) return *this - (-n);
			const_iterator ans(*this);
			int tmp = n;
			map_node *map = ans.which -> map;
			while (ans.p != map[ans.pos].last && tmp) {
				ans.p = ans.p -> next;
				--tmp;
			}
			if (tmp && ans.p == map[ans.pos].last) {
				if (ans.pos < ans.which -> last) {
					ans.p = ans.p -> next;
					++ans.pos;
					--tmp;
				} else throw invalid_iterator(); //undefined behavior
			}
			while (tmp >= map[ans.pos].size) {
				tmp -= map[ans.pos].size;
				++ans.pos;
				ans.p = map[ans.pos].first;
			}
			while (tmp--) {
				ans.p = ans.p -> next;
			}
			return ans;
		}
		const_iterator operator - (const int &n) const {
			if (n < 0) return *this + (-n);
			const_iterator ans(*this);
			int tmp = n;
			map_node *map = which -> map;
			while (ans.p != map[pos].first && tmp) {
				ans.p = ans.p -> prev;
				--tmp;
			}
			if (tmp && ans.p == map[ans.pos].first) {
				if (ans.pos > which -> first) {
					ans.p = ans.p -> prev;
					--ans.pos;
					--tmp;
				} else throw invalid_iterator();
			}
			while (tmp >= map[ans.pos].size) {
				tmp -= map[ans.pos].size;
				--ans.pos;
				ans.p = map[ans.pos].last;
			}
			while (tmp--) {
				ans.p = ans.p -> prev;
			}
			return ans;
		}
		int operator - (const const_iterator &rhs) const {
			if (which != rhs.which) throw invalid_iterator();
			map_node *map = which -> map;
			if (pos == rhs.pos) {
				const node *x = p, *y = rhs.p;
				int ans = 0;
				bool find = 0;
				for (; x != map[pos].last -> next; x = x -> next) {
					++ans;
					if (x == y) {
						find = 1;
						break;
					}
				}
				if (find) return -ans + 1;
				for (ans = 0, x = p; y != x; y = y -> next) ++ans;
				return ans;
			} else {
				const node *x = p, *y = rhs.p;
				int sign = -1, from = pos, to = rhs.pos;
				if (rhs.pos < pos) {
					x = rhs.p; from = rhs.pos;
					y = p;     to = pos;
					sign = 1;
				} else {
					x = p;     from = pos;
					y = rhs.p; to = rhs.pos;
					sign = -1;
				}
				int ans = 0;
				for (; x != map[from].last -> next; x = x -> next) ++ans;
				for (int tmp = from + 1; tmp < to; ++tmp) ans += map[tmp].size;
				x = map[to].first;
				for (; x != y && x != map[to].last -> next; x = x -> next) ++ans;
				if (x != y) throw "???";
				return ans * sign;
			}
		}
		const_iterator& operator+=(const int &n) {
			return *this = *this + n;
		}
		const_iterator& operator-=(const int &n) {
			return *this = *this - n;
		}
		const_iterator operator++(int) {
			if (p -> next == nullptr) throw index_out_of_bound();
			const_iterator tmp(*this);
			*this = *this + 1;
			return tmp;
		}
		const_iterator& operator++() {
			if (p -> next == nullptr) throw index_out_of_bound();
			return *this = *this + 1;
		}
		const_iterator operator--(int) {
			if (p -> prev == nullptr) throw index_out_of_bound();
			const_iterator tmp(*this);
			*this = *this - 1;
			return tmp;
		}
		const_iterator& operator--() {
			if (p -> prev == nullptr) throw index_out_of_bound();
			return *this = *this - 1;
		}
		const T& operator*() const {
			return *(p -> v);
		}
		const T* operator->() const noexcept {
			return p -> v;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return p == rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
private:
	map_node *map;
//	node *start, *finish;
	int map_size, first, last;
public:
	/**
	 * TODO Constructors
	 */
	node* start() {
		return map[first].first;
	}
	const node* start() const {
		return map[first].first;
	}
	node* finish() {
		return map[last].last;
	}
	const node* finish() const {
		return map[last].last;
	}
	deque() {
		map = new map_node [map_size = 2000]();
		first = last = 1000;
//		map[first] = new map_node();
		map[first].last = new node();
		map[first].first = map[first].last;
		map[first].size = 1;
	}
	void Triple_map_space() {
		int len = last - first + 1;
		map_node *tmp = new map_node[len * 3]();
		map_size = len * 3;
		for (int i = 0; i < len; ++i)
			tmp[i + len] = map[i + first];
		delete [] map;
		first = len; last = len * 2 - 1;
		map = tmp;
	}
	deque(const deque &other) {
		map = new map_node[map_size = other.map_size]();
		first = other.first; last = other.last;
//		for (int i = first; i <= last; ++i)
//			map[i] = new map_node();
		map[first].first = new node(*other.start());
		map[first].first -> copy (other.start());
		node *t = start();
		for (int i = first; i <= last; ++i) {
			map[i].size = other.map[i].size;
			map[i].first = t;
			for (int j = 1; j < map[i].size; ++j)
				t = t -> next;
			map[i].last = t;
			t = t -> next;
		}
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
//		map[first].first -> destruct();
		node *tmp = map[last].last, *p;
		while (tmp != nullptr) {
			p = tmp -> prev;
			delete tmp;
			tmp = p;
		}
//		delete map[first].first;
//		for (int i = first; i <= last; ++i) delete map[i];
		delete [] map;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
		if (this == &other) return *this;

		node *tmp = map[last].last, *p;
		while (tmp != nullptr) {
			p = tmp -> prev;
			delete tmp;
			tmp = p;
		}
//		for (int i = first; i <= last; ++i) delete map[i];
		delete [] map;

		map = new map_node [map_size = other.map_size]();
		first = other.first; last = other.last;
		map[first].first = new node(*other.start());
		map[first].first -> copy (other.start());
		node *t = start();
		for (int i = first; i <= last; ++i) {
			map[i].size = other.map[i].size;
			map[i].first = t;
			for (int j = 1; j < map[i].size; ++j)
				t = t -> next;
			map[i].last = t;
			t = t -> next;
		}
		return *this;
	}
	void move_right(int l, int r) {
		if (r == map_size - 1) {
			int len = last - first + 1;
			l = l - first; r = r - first;
			Triple_map_space();
			move_right(l + len, r + len);
			return;
		}
		for (int i = r; i >= l; --i) map[i + 1] = map[i];
//		map[l] = nullptr;
	}
	void move_left(int l, int r) {
		if (l == 0) {
			int len = last - first + 1;
			l = l - first; r = r - first;
			Triple_map_space();
			move_left(l + len, r + len);
			return;
		}
		for (int i = l; i <= r; ++i) map[i - 1] = map[i];
//		map[r] = nullptr;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if (pos > size()) throw index_out_of_bound();
		int t = first;
		size_t tmp = pos;
		for(t = first; tmp >= map[t].size; ++t) {
			if (t > last) throw index_out_of_bound();
			tmp -= map[t].size;
		}
		node *ans = map[t].first;
		while (tmp--) {
			ans = ans -> next;
			if (ans == finish()) throw index_out_of_bound();
		}
		return *(ans -> v);
	}
	const T & at(const size_t &pos) const {
		if (pos > size()) throw index_out_of_bound();
		int t = first;
		size_t tmp = pos;
		for(t = first; tmp >= map[t].size; ++t) {
			if (t > last) throw index_out_of_bound();
			tmp -= map[t].size;
		}
		const node *ans = map[t].first;
		while (tmp--) {
			ans = ans -> next;
			if (ans == finish()) throw index_out_of_bound();
		}
		return *(ans -> v);
	}
	T & operator[](const size_t &pos) {
		return at(pos);
	}
	const T & operator[](const size_t &pos) const {
		return at(pos);
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if (empty()) throw container_is_empty();
		return *(start() -> v);
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if (empty()) throw container_is_empty();
		const node *tmp = finish();
		tmp = tmp -> prev;
		return *(tmp -> v);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(start(), first, this);
	}
	const_iterator cbegin() const {
		return const_iterator(start(), first, this);
//		return const_iterator(begin());
//		return const_iterator(start(), this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(finish(), last, this);
	}
	const_iterator cend() const {
		return const_iterator(finish(), last, this);
//		return const_iterator(end());
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
		return start() == finish();
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		size_t ans = 0;
		for (int i = first; i <= last; ++i) ans += map[i].size;
		return ans - 1;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		node *tmp = map[last].last, *p;
		while (tmp != nullptr) {
			p = tmp -> prev;
			delete tmp;
			tmp = p;
		}
//		map[first].first -> destruct();
//		delete map[first].first;
		delete [] map;
		map = new map_node [map_size = 20]();
		first = last = 10;
		map[first].first = new node();
		map[first].last = map[first].first;
		map[first].size = 1;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.which != this) throw invalid_iterator();
		if (pos.p == start()) {
			push_front(value);
			return begin();
		}
		if (pos.p == finish()) {
			push_back(value);
			return end() - 1;
			/*
			iterator ans = end();
			--ans;
			return ans;
			*/
		}
		++(map[pos.pos].size);
		node *x = new node(value);
		x -> next = pos.p;
		x -> prev = pos.p -> prev;
		x -> prev -> next = x;
		x -> next -> prev = x;

		map_node* map = pos.which -> map;
		if (pos.p == map[pos.pos].first) {
			map[pos.pos].first = x;
		}
		int belong = pos.pos;
		if (map[pos.pos].size == buffer_size * 2) {
			int which = pos.pos - first;
			pair<map_node, map_node> ans = split(map[pos.pos], buffer_size);
			if (first == 0) {
				move_right(pos.pos, last);
				++last;
			} else {
				move_left(first, pos.pos);
				--first;
			}
			map[first + which] = ans.first;
			map[first + which + 1] = ans.second;
			belong = first + which + 1;
			for (node *t = map[first + which].first;
				t != map[first + which].last -> next;
				t = t -> next) 
				if (t == x) {
					belong = first + which;
					break;
				}
		}
		return iterator(x, belong, this);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		if (pos.which != this) throw invalid_iterator();
		if (pos.which -> empty()) throw container_is_empty();
		if (pos == end()) throw invalid_iterator();
		if (pos.pos < first || pos.pos > last) throw invalid_iterator();
		using std::cout;
		using std::endl;
//		cout << pos.p << endl << pos.pos << endl << endl << endl;
		if (pos == begin()) { pop_front(); return begin(); }
		if (pos == end() - 1) { pop_back(); return end(); }

		node *t = pos.p;
		int now = pos.pos;
		--(map[now].size);

		t -> prev -> next = t -> next;
		t -> next -> prev = t -> prev;
		pos.p = pos.p -> next;

		bool sign = 0;
		if (map[now].size == 0) {
			move_left(now + 1, last);
			--last;
		} else if (t == map[now].first) {
			map[now].first = t -> next;
		} else if (t == map[now].last) {
			map[now].last = t -> prev;
			++pos.pos;
			sign = 1;
		}
		if (map[now].size <= buffer_size / 2 && 
			now < last &&
			map[now + 1].size <= buffer_size / 2) {

			map[now + 1].size += map[now].size;
			map[now + 1].first = map[now].first;
			move_left(now + 1, last);
			--last;
			if (sign) --pos.pos;
		}/* else if (map[now -> size <= buffer_size / 2 &&
			now > first &&
			map[now - 1].size <= buffer_size / 2 */
		delete t;
		return pos;
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		node *x = new node(value);
		x -> next = map[last].last;
		x -> prev = map[last].last -> prev;
		x -> next -> prev = x;
		if (x -> prev) x -> prev -> next = x;
		if (map[last].size == 1) map[last].first = x;
		++(map[last].size);
		if ((map[last].size) >= (buffer_size * 2)) {
			if (last == map_size - 1) Triple_map_space();
			pair<map_node, map_node> ans = split(map[last], buffer_size / 2 * 3);
			map[last] = ans.first;
			++last;
			map[last] = ans.second;
		}
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if (empty()) throw container_is_empty();
		--(map[last].size);
		node *tmp = map[last].last;
		tmp = tmp -> prev;
		if (tmp -> prev) tmp -> prev -> next = tmp -> next;
		tmp -> next -> prev = tmp -> prev;
		delete tmp;
		if (map[last].size == 1) {
			if (first < last) {
			++(map[last - 1].size);
			/*
			using namespace std;
			cout << "first = " << first << " last = " << last << endl;
			cout << "map[last - 1].last -> next\n";
			cout << map[last - 1].last << endl
				<< map[last - 1].last -> next << endl
				<< map[last].last  << endl
				<< map[last].last -> prev << endl;
			cout << endl;
			*/
			map[last - 1].last = map[last].last;
//			delete map[last];
//			map[last] = nullptr;
			--last;
			} else {
				map[last].first = map[last].last;
			}
		}
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
		node *x = new node(value);
		x -> next = map[first].first;
		map[first].first -> prev = x;
		map[first].first = x;
		++(map[first].size);
		if ((map[first].size) >= (buffer_size * 2)) {
			if (first == 0) Triple_map_space();
			pair<map_node, map_node> ans = split(map[first], buffer_size / 2);
			map[first] = ans.second;
			--first;
			map[first] = ans.first;
		}
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		if (empty()) throw container_is_empty();
		--(map[first].size);
		node *tmp = map[first].first;
		map[first].first = tmp -> next;
		map[first].first -> prev = nullptr;
		delete tmp;
		if (map[first].size == 0 && first < last) {
			++first;
		}
	}
};

}

#endif
