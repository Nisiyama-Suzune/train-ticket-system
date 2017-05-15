/**
 * implement a container like st::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"
#include <QDataStream>

namespace sjtu {

template<
		class Key,
		class T,
		class Compare = std::less<Key>
>
class map {
public:
	typedef pair<const Key, T> value_type;
private:
	Compare cmp;
	size_t m_size;
	long long mod = 1E16 + 7, a = 17, b = 5E15 + 13;
	long long seed = 1E15 + 7;

	long long get_rand() {
		seed = (seed * a + b) % mod;
		return seed;
	}

	struct node {
		long long heap;
		value_type *value;
		node *prev, *next;
		node *left, *right;

		node(long long heap = 0, value_type *value = nullptr, node *prev = nullptr, node *next = nullptr,
			 node *left = nullptr, node *right = nullptr) :
				heap(heap), value(value), prev(prev), next(next), left(left), right(right) {}

		~node() {
			if (value) delete value;
			if (prev) delete prev;
		}
	};

	pair<node *, node *> in(std::istream &is, node *prevnode) {
		long long heap;
		is >> heap;
		if (heap < 0) return pair<node *, node *>(nullptr, prevnode);
		m_size++;
		Key k;
		T v;
		is >> k >> v;
		node *ans = new node(heap, new value_type(k, v));
		pair<node *, node *> left = in(is, prevnode);
		ans->left = left.first;
		ans->prev = left.second;
		if (left.second)
			left.second->next = ans;
		else
			nbegin = ans;
		pair<node *, node *> right = in(is, ans);
		ans->right = right.first;
		return pair<node *, node *>(ans, right.second);
	}

	void out(std::ostream &os, node *n) {
		if (!n) {
			long long tmp = -1;
			os << tmp << std::endl;
			return;
		}
		os << n->heap << std::endl;
		os << n->value->first << std::endl;
		os << n->value->second << std::endl;
		out(os, n->left);
		out(os, n->right);
	}
	pair<node *, node *> in(QDataStream &is, node *prevnode) {
		long long heap;
		is >> heap;
		if (heap < 0) return pair<node *, node *>(nullptr, prevnode);
		m_size++;
		Key k;
		T v;
		is >> k >> v;
		node *ans = new node(heap, new value_type(k, v));
		pair<node *, node *> left = in(is, prevnode);
		ans->left = left.first;
		ans->prev = left.second;
		if (left.second)
			left.second->next = ans;
		else
			nbegin = ans;
		pair<node *, node *> right = in(is, ans);
		ans->right = right.first;
		return pair<node *, node *>(ans, right.second);
	}

	void out(QDataStream &os, node *n) {
		if (!n) {
			long long tmp = -1;
			os << tmp;
			return;
		}
		os << n->heap;
		os << n->value->first;
		os << n->value->second;
		out(os, n->left);
		out(os, n->right);
	}

	bool less(node *a, node *b) const {
		if (!a->value) return false;
		if (!b->value) return true;
		return cmp(a->value->first, b->value->first);
	}

	bool less(const Key &a, node *b) const {
		if (!b->value) return true;
		return cmp(a, b->value->first);
	}

	bool less(node *a, const Key &b) const {
		if (!a->value) return false;
		return cmp(a->value->first, b);
	}

	node *nbegin, *nend;

	/*
	 * n         r
	 * |\        |\
	 * l r   ->  n b
	 *   |\      |\
	 *   a b     l a
	 */
	node *spin_left(node *n) {
		node *l = n->left, *r = n->right, *a = r->left, *b = r->right;
		r->left = n;
		r->right = b;
		n->left = l;
		n->right = a;
		return r;
	}

	/*
	 * n         l
	 * |\        |\
	 * l r   ->  a n
	 * |\          |\
	 * a b         b r
	 */
	node *spin_right(node *n) {
		node *l = n->left, *r = n->right, *a = l->left, *b = l->right;
		l->left = a;
		l->right = n;
		n->left = b;
		n->right = r;
		return l;
	}

	node *insert_adjust(node *x, node *n) {
		if (x) {
			if (less(n, x)) {
				x->left = insert_adjust(x->left, n);
				if (x->heap < x->left->heap)
					x = spin_right(x);
			} else {
				x->right = insert_adjust(x->right, n);
				if (x->heap < x->right->heap)
					x = spin_left(x);
			}
			return x;
		}
		return n;
	}

public:
	class const_iterator;

	class iterator {
		friend class map;

	private:
		node *p;

		iterator(node *p) : p(p) {};
	public:
		iterator() {
			p = nullptr;
		}

		iterator(const iterator &other) {
			p = other.p;
		}

		iterator operator++(int) {
			if (!p->next) throw index_out_of_bound();
			p = p->next;
			return iterator(p->prev);
		}

		iterator &operator++() {
			if (!p->next) throw index_out_of_bound();
			p = p->next;
			return *this;
		}

		iterator operator--(int) {
			if (!p->prev) throw index_out_of_bound();
			p = p->prev;
			return iterator(p->next);
		}

		iterator &operator--() {
			if (!p->prev) throw index_out_of_bound();
			p = p->prev;
			return *this;
		}

		value_type &operator*() const {
			return *p->value;
		}

		bool operator==(const iterator &rhs) const {
			return p == rhs.p;
		}

		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p;
		}

		bool operator!=(const iterator &rhs) const {
			return p != rhs.p;
		}

		bool operator!=(const const_iterator &rhs) const {
			return p != rhs.p;
		}

		value_type *operator->() const noexcept {
			return p->value;
		}
	};

	class const_iterator {
		friend class map;

	private:
		node *p;

		const_iterator(node *p) : p(p) {}

	public:
		const_iterator() {
			p = nullptr;
		}

		const_iterator(const const_iterator &other) {
			p = other.p;
		}

		const_iterator(const iterator &other) {
			p = other.p;
		}

		const_iterator operator++(int) {
			if (!p->next) throw index_out_of_bound();
			p = p->next;
			return const_iterator(p->prev);
		}

		const_iterator &operator++() {
			if (!p->next) throw index_out_of_bound();
			p = p->next;
			return *this;
		}

		const_iterator operator--(int) {
			if (!p->prev) throw index_out_of_bound();
			p = p->prev;
			return const_iterator(p->next);
		}

		const_iterator &operator--() {
			if (!p->prev) throw index_out_of_bound();
			p = p->prev;
			return *this;
		}

		const value_type &operator*() const {
			return *p->value;
		}

		bool operator==(const iterator &rhs) const {
			return p == rhs.p;
		}

		bool operator==(const const_iterator &rhs) const {
			return p == rhs.p;
		}

		bool operator!=(const iterator &rhs) const {
			return p != rhs.p;
		}

		bool operator!=(const const_iterator &rhs) const {
			return p != rhs.p;
		}

		value_type const *operator->() const noexcept {
			return p->value;
		}
	};

	map() {
		const long long mod = 1E16 + 7, a = 17, b = 5E15 + 13;
		nend = new node(mod);
		nbegin = nend;
		m_size = 0;
	}

	map(const map &other) {
		nend = new node(mod);
		nbegin = nend;
		m_size = 0;
		for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
			insert(*it);
	}

	map &operator=(const map &other) {
		if (this == &other) return *this;
		clear();
		for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
			insert(*it);
		return *this;
	}

	~map() {
		delete nend;
	}

	/**
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T &at(const Key &key) {
		iterator it = find(key);
		if (it == end()) throw index_out_of_bound();
		return it->second;
	}

	const T &at(const Key &key) const {
		const_iterator it = find(key);
		if (it == cend()) throw index_out_of_bound();
		return it->second;
	}

	/**
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T &operator[](const Key &key) {
		return insert(value_type(key, T())).first->second;
	}

	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T &operator[](const Key &key) const {
		const_iterator it = find(key);
		if (it == cend()) throw index_out_of_bound();
		return it->second;
	}

	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		return iterator(nbegin);
	}

	const_iterator cbegin() const {
		return const_iterator(nbegin);
	}

	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		return iterator(nend);
	}

	const_iterator cend() const {
		return const_iterator(nend);
	}

	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return !nend->prev;
	}

	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return m_size;
	}

	/**
	 * clears the contents
	 */
	void clear() {
		delete nend;
		nend = new node(mod);
		nbegin = nend;
		m_size = 0;
	}

	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		node *low = lower_bound(value.first).p, *upp = upper_bound(value.first).p;
		if (less(low, value.first) || less(value.first, low)) {
			node *n = new node(get_rand(), new value_type(value), low->prev, upp);
			if (low->prev)
				low->prev->next = n;
			else
				nbegin = n;
			upp->prev = n;
			insert_adjust(nend, n);
			m_size++;
			return pair<iterator, bool>(iterator(n), true);
		} else
			return pair<iterator, bool>(iterator(low), false);
	}

	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		node *n = pos.p;
		if (n == nend) throw invalid_iterator();
		node *p = nend->left, *q = nend;
		while (p && p != n) {
			if (less(n, p)) {
				q = p;
				p = p->left;
			} else if (less(p, n)) {
				q = p;
				p = p->right;
			} else
				break;
		}
		if (!p || p != n) throw invalid_iterator();
		while (p->left && p->right) {
			if (q->left == p) {
				if (p->left->heap < p->right->heap)
					q->left = spin_left(p);
				else
					q->left = spin_right(p);
				q = q->left;
			} else {
				if (p->left->heap < p->right->heap)
					q->right = spin_left(p);
				else
					q->right = spin_right(p);
				q = q->right;
			}
		}
		if (q->left == p) {
			if (p->left)
				q->left = p->left;
			else
				q->left = p->right;
		} else {
			if (p->left)
				q->right = p->left;
			else
				q->right = p->right;
		}
		p->left = p->right = nullptr;
		if (p->prev)
			p->prev->next = p->next;
		else
			nbegin = p->next;
		p->next->prev = p->prev;
		p->prev = p->next = nullptr;
		delete p;
		m_size--;
	}

	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		return (find(key) == cend() ? 0 : 1);
	}

	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		node *n = nend;
		while (n) {
			if (less(key, n))
				n = n->left;
			else if (less(n, key))
				n = n->right;
			else
				break;
		}
		if (n)
			return iterator(n);
		else
			return iterator(nend);
	}

	const_iterator find(const Key &key) const {
		node *n = nend;
		while (n) {
			if (less(key, n))
				n = n->left;
			else if (less(n, key))
				n = n->right;
			else
				break;
		}
		if (n)
			return const_iterator(n);
		else
			return const_iterator(nend);
	}

	iterator lower_bound(const Key &key) {
		node *n = nend;
		node *ans = n;
		while (n) {
			if (less(n, key))
				n = n->right;
			else if (less(key, n)) {
				if (less(n, ans)) ans = n;
				n = n->left;
			} else {
				ans = n;
				break;
			}
		}
		return iterator(ans);
	}

	iterator upper_bound(const Key &key) {
		node *n = nend;
		node *ans = n;
		while (n) {
			if (less(n, key))
				n = n->right;
			else {
				if (less(key, n) && less(n, ans)) ans = n;
				n = n->left;
			}
		}
		return iterator(ans);
	}

	const_iterator lower_bound(const Key &key) const {
		node *n = nend;
		node *ans = n;
		while (n) {
			if (less(n, key))
				n = n->right;
			else if (less(key, n)) {
				if (less(n, ans)) ans = n;
				n = n->left;
			} else {
				ans = n;
				break;
			}
		}
		return const_iterator(ans);
	}

	const_iterator upper_bound(const Key &key) const {
		node *n = nend;
		node *ans = n;
		while (n) {
			if (less(n, key))
				n = n->right;
			else {
				if (less(key, n) && less(n, ans)) ans = n;
				n = n->left;
			}
		}
		return const_iterator(ans);
	}

	friend std::ostream &operator<<(std::ostream &os, map<Key, T, Compare> &m) {
		m.out(os, m.nend->left);
		return os;
	}

	friend std::istream &operator>>(std::istream &is, map<Key, T, Compare> &m) {
		m.clear();
		pair<node *, node *> n = m.in(is, nullptr);
		m.nend->left = n.first;
		m.nend->prev = n.second;
		n.second->next = m.nend;
		return is;
	}
	friend QDataStream &operator<<(QDataStream &os, map<Key, T, Compare> &m) {
		m.out(os, m.nend->left);
		return os;
	}

	friend QDataStream &operator>>(QDataStream &is, map<Key, T, Compare> &m) {
		m.clear();
		pair<node *, node *> n = m.in(is, nullptr);
		m.nend->left = n.first;
		m.nend->prev = n.second;
		n.second->next = m.nend;
		return is;
	}
};

}

#endif
