#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <fstream>
#include "utilities.hpp"

namespace sjtu {

template<class T>
class list {
private:
    struct list_node;
    typedef list_node *link_type;

    struct list_node {
        T *val;
        link_type next;
        link_type prev;

        ~list_node() {
            delete val;
        }
    };

    /**
     *  get_node, 不设置val的值，初始化next，prev
     *  construct_node, 将x的值设置为val
     *  create_node，创造一个新的节点，其值为val
     *  destroy_node，析构一个节点
     */
    link_type get_node();

    void construct_node(link_type x, const T &val);

    link_type create_node(const T &val);

    void destroy_node(link_type x);

    link_type node;

    void init();

    void destroy_list();

    void copy(const list &x);

public:
    list() {
        init();
    }

    list(const list &other) {
        init();
        copy(other);
    }

    ~list() { destroy_list(); }

    list &operator=(const list &other) {
        if (this == &other)
            return *this;
        destroy_list();
        init();
        copy(other);
    }


public:
    class iterator {
        friend class list;

    private:
        link_type iter_node;

    public:
        iterator() {}

        iterator(link_type x) : iter_node(x) {}

        iterator(const iterator &x) : iter_node(x.iter_node) {}

        bool operator==(const iterator &x) const;

        bool operator!=(const iterator &x) const;

        T &operator*();

        T *operator->();

        iterator &operator++();

        iterator operator++(int);

        iterator &operator--();

        iterator operator--(int);
    };

    iterator begin();

    iterator end();

    T &front();

    T &back();

public:
    /// 在pos位置前插入一个节点，返回插入值位置的迭代器
    iterator insert(iterator pos, const T &x);

    void push_back(const T &x);

    void push_front(const T &x);

    /// 移除pos所指节点，返回pos后位置的迭代器
    iterator erase(iterator pos);

    void pop_back();

    void pop_front();

    void clear();

public:
    bool empty() const;

    size_t size() const;

public: // io
    std::streampos save(std::ofstream fout);
    std::streampos load(std::ifstream fin);

};

template<class T>
bool list<T>::iterator::operator==(const list<T>::iterator &x) const {
    return iter_node == x.iter_node;
}

template<class T>
bool list<T>::iterator::operator!=(const list<T>::iterator &x) const {
    return iter_node != x.iter_node;
}

template<class T>
T &list<T>::iterator::operator*() {
    return *(iter_node->val);
}

template<class T>
T *list<T>::iterator::operator->() {
    return &(operator*());
}

template<class T>
typename list<T>::iterator &list<T>::iterator::operator++() {
    iter_node = iter_node->next;
    return *this;
}

template<class T>
typename list<T>::iterator list<T>::iterator::operator++(int) {
    iterator tmp = *this;
    ++*this;
    return tmp;
}

template<class T>
typename list<T>::iterator &list<T>::iterator::operator--() {
    iter_node = iter_node->prev;
    return *this;
}

template<class T>
typename list<T>::iterator list<T>::iterator::operator--(int) {
    iterator tmp = *this;
    --*this;
    return tmp;
}

template<class T>
typename list<T>::iterator list<T>::begin() {
    return iterator(node->next);
}

template<class T>
typename list<T>::iterator list<T>::end() {
    return iterator(node);
}

template<class T>
typename list<T>::link_type list<T>::get_node() {
    link_type tmp = new list_node;
    tmp->val = nullptr;
    tmp->next = tmp->prev = nullptr;
    return tmp;
}

template<class T>
void list<T>::construct_node(list<T>::link_type x, const T &val) {
    delete x->val;
    x->val = new T(val);
}

template<class T>
typename list<T>::link_type list<T>::create_node(const T &val) {
    link_type tmp = get_node();
    construct_node(tmp, val);
    return tmp;
}

template<class T>
void list<T>::destroy_node(list<T>::link_type x) {
    x->~list_node();
}

template<class T>
typename list<T>::iterator list<T>::insert(list<T>::iterator pos, const T &x) {
    link_type tmp = create_node(x);
    tmp->prev = pos->prev;
    tmp->next = pos.iter_node;
    pos->prev->next = tmp;
    pos->prev = tmp;
    return tmp;
}

template<class T>
typename list<T>::iterator list<T>::erase(list::iterator pos) {
    iterator tmp(pos.iter_node->next);
    pos.iter_node->next->prev = pos.iter_node->prev;
    pos.iter_node->prev->next = pos.iter_node->next;
    destroy_node(pos.iter_node);
    return tmp;
}

template<class T>
void list<T>::push_back(const T &x) {
    insert(begin(), x);
}

template<class T>
void list<T>::push_front(const T &x) {
    insert(end(), x);
}

template<class T>
void list<T>::pop_back() {
    iterator tmp = end();
    erase(--end());
}

template<class T>
void list<T>::pop_front() {
    erase(begin());
}

template<class T>
void list<T>::clear() {
    while (!empty())
        pop_back();
}

template<class T>
T &list<T>::front() {
    return *begin();
}

template<class T>
T &list<T>::back() {
    iterator tmp = end();
    --tmp;
    return *tmp;
}

template<class T>
bool list<T>::empty() const {
    return node == node->next;
}

template<class T>
size_t list<T>::size() const {
    size_t sz = 0;
    link_type tmp = node->next;
    while (tmp != node) {
        ++sz;
        tmp = tmp->next;
    }
    return sz;
}

template<class T>
void list<T>::init() {
    destroy_node(node);
    node = get_node();
    node->prev = node->next = node;
}

template<class T>
void list<T>::destroy_list() {
    clear();
    destroy_node(node);
}

template<class T>
void list<T>::copy(const list<T> &x) {
    link_type tmp = x.node->next;
    while (tmp != x.node) {
        push_back(tmp->val);
        tmp = tmp->next;
    }
}

template <class T>
std::streampos list<T>::save(std::ofstream fout) {
    std::streampos result = fout.tellp();
    size_t sz = size();
    fout.write(reinterpret_cast<char*>(&sz), sizeof(sz));
    for (link_type i = node->next; i != node; i = i->next) {

    }

    return result;
}

}
#endif //SJTU_LIST_HPP
