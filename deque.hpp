#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include "utility.hpp"

#include <cmath>
#include <cstddef>

using std::sqrt;

namespace sjtu {

/**链表套链表
 * 设当前总元素为N个，map大小为m，buffer大小为n，
 * 每个buffer内实际元素个数为ni
 * 按照map的情况来决定是否需要维护。
 * 需要满足sqrtN <= m <= 2sqrtN，如不满足当前情况，则维护。
 * n在维护m的时候再修改？
 * 如果ni >= sqrtN，则分裂为两个buffer。
 * 这样可以确保每个buffer不过大，可以使得 m >= sqrtN是成立的。（待证明）
 * 考虑维护 m 的另一边：如果 m > 2sqrtN，
 * 即map过大，而buffer过小，这一情况由多次删除导致。
 * 注：从中间插入导致分裂的情况，每一块的大小是sqrtN/2，不会产生buffer过小的情况。
 * 则需要合并部分的buffer，将buffer的大小重新设为sqrtN。（为什么是sqrtN？）
 * 考虑一次遍历，如果当前块的大小不足sqrtN，则从后一个buffer中截取足够的元素。
 * （时间复杂度？？）
 */

/**需要记录的内容
 * map的链表header，map的size；
 * buffer的size，注：buffer的header在map里。
 */

/**                      Structure
 *   +-------------------------------------------------+
 *   |                                                 |
 *   +-> {map_header} -- {map_1} ----------- {map_2} --+
 *                          |                   |
 *                   [buffer_head_1]     [buffer_head_2]  <- end()
 *                          |                   |
 *                       @begin()               X
 *                                              |
 *                                            back()
 */

/**已确认过的内容：
 *  Y，阅读过；
 *  YYY，debug过；
 *  DDD, 一定炸了
 * find_from       Y
 * transparent     YY
 * split           YY
 * absorb          Y
 *
 * pos_in_buffer   Y
 * iter+           Y
 *
 * iter++/--       YY
 * push_back       YY
 * push_front      YY
 *
 * insert          DDD
 *               （容器为空时的插入，或许可以顺道修改push_back写法）
 */

template<class T>
class deque {
public:
    class iterator;
    class const_iterator;
    friend class iterator;
    friend class const_iterator;

private:
    typedef T value_type;

    struct map_node;
    struct buffer_node;
    typedef map_node* map_ptr;
    typedef buffer_node* buffer_ptr;
    typedef deque* deque_ptr;

    struct map_node {
        buffer_ptr buffer_header;
        map_ptr next, prev;
        size_t sz; // the size of this particular buffer

        ~map_node() {
            delete buffer_header;
        }
    };

    struct buffer_node {
        value_type* val;
        buffer_ptr next, prev;

        ~buffer_node() {
            delete val;
        }
    };

    map_ptr get_map_node() {
        map_ptr tmp = new map_node;
        tmp->buffer_header = get_buffer_node();
        tmp->next = tmp->prev = tmp;
        tmp->sz = 0;
        return tmp;
    }
    buffer_ptr get_buffer_node() {
        buffer_ptr tmp = new buffer_node;
        tmp->val = nullptr;
        tmp->next = tmp->prev = tmp;
        return tmp;
    }
    void delete_buffer_node(buffer_ptr p) {
        delete p;
        p = nullptr;
    }
    void delete_map_node(map_ptr p) {
        delete p;
        p = nullptr;
    }
    void delete_empty_map_node(map_ptr p) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete_map_node(p);
    }

    bool buffer_full(map_ptr p) {
        return p->sz >= buffer_sz;
    }
    bool buffer_empty(map_ptr p) {
        return !p->sz;
    }

private:
    size_t sz; // the total size of the deque
    size_t map_sz; // the size of the map
    size_t buffer_sz; // the max size the buffer

    map_ptr map_header;

private:
    // increment, decrement
    // split, absorb, maintain

    // 返回p的后继，以及是否进入到了下一个buffer，如果进入，则为true
    pair<buffer_ptr, bool> increment(buffer_ptr p, const map_ptr & m_p) {
        // p is the last element in this buffer
        if (p->next == m_p->buffer_header && m_p->next != map_header) {
            return make_pair(increment_aux(m_p), 1);
        }
        return make_pair(p->next, 0);
    }
    // p is the last element in buffer m_p
    buffer_ptr increment_aux(const map_ptr & m_p) {
        return m_p->next->buffer_header->next;
    }

    // 返回p的前驱，以及是否进入到了前一个buffer，如果进入，则为true
    pair<buffer_ptr, bool> decrement(buffer_ptr p, const map_ptr & m_p) {
        if (p->prev == m_p->buffer_header) {
            return make_pair(decrement_aux(m_p), 1);
        }
        return make_pair(p->prev, 0);
    }
    // p is the first element in buffer m_p
    buffer_ptr decrement_aux(const map_ptr & m_p) {
        return m_p->prev->buffer_header->prev;
    }

    bool pos_out_of_bound(size_t pos) const {
        return pos >= sz;
    }
    // find
    pair<map_ptr, buffer_ptr> find_from(int pos, map_ptr cur_buffer) const {
        /*   [k-2]----[k-1]----[k]----[k+1]
         *     |        |       |       |
         *     O        O       A       O
         *              |       |       |
         *              C       B       O
         *                      |
         *                      O
         * 假设先在cur_buffer为k，表示从A开始往后数pos位，即
         * 若pos=0，则返回A，若pos=1，则返回B，若pos=-1，则返回C。
         * 实现如下：
         * 对于pos>=0的情况：
         *  考虑当前所在的buffer，如果pos\in [0, cur_sz)，则意味这在当前这buffer里，
         *  所以需要一个循环，当pos<cur_sz时候终止，每次pos-=cur_zs。
         *  考虑已确定节点在这个buffer里，则只需要找到第pos个位置即可。
         *  一个特殊情况是，传进来的pos指向end，则此时应为pos \in [0, cur_sz]
         * 对于pos<0的情况：
         *  首先把pos取绝对值，并且把cur_buffer置为cur_buffer->prev。
         *  为了使得是0-based的，应当先--pos。
         *  同样的，如果pos\in [0, cur_sz)，则意味这在当前buffer中，循环是同样的。
         *  只是最后确定位置时，从后往前找。
         */

        if (pos >= 0) {
            // 这里假设如果下一个buffer是map_header了，但pos依然不在
            // 当前buffer中的情况只可能是end()的情况
            while (pos >= cur_buffer->sz && cur_buffer->next != map_header) {
                pos -= cur_buffer->sz;
                cur_buffer = cur_buffer->next;
            }
            if (pos == cur_buffer->sz)
                return make_pair(map_header->prev, map_header->prev->buffer_header);

            buffer_ptr result = cur_buffer->buffer_header->next;
            while (pos--) {
                result = result->next;
            }
            return make_pair(cur_buffer, result);
        }
        // pos < 0
        pos = -pos;
        --pos;
        cur_buffer = cur_buffer->prev;
        while (pos >= cur_buffer->sz) {
            pos -= cur_buffer->sz;
            cur_buffer = cur_buffer->prev;
        }
        buffer_ptr result = cur_buffer->buffer_header->prev;
        while (pos--) {
            result = result->prev;
        }
        return make_pair(cur_buffer, result);
    }
    // get pos in buffer, 0-based
    size_t pos_in_buffer(buffer_ptr p, map_ptr buf) {
        /* 获得p在buffer中的下标（0-based）
         * [k]--A--B--C--D--E
         * 假设现在的p是D，它的下标应为3。
         */

        size_t result = 0;
        while (p->prev != buf->buffer_header) {
            ++result;
            p = p->prev;
        }
        return result;
    }

    bool invalid_iter(const iterator & p) {
        // TODO
        return p.dek != this;
    }

    void init() {
        sz = 0;
        map_sz = 0;
        buffer_sz = 10;
        map_header = get_map_node();
    }
    void destroy_buffer(map_ptr buf) {
        buffer_ptr tmp;
        while (buf->buffer_header->next != buf->buffer_header) {
            tmp = buf->buffer_header->next;
            buf->buffer_header->next = tmp->next;
            delete_buffer_node(tmp);
        }
        delete_empty_map_node(buf);
    }
    void destroy() {
        while (map_header->next != map_header) {
            destroy_buffer(map_header->next);
        }
        delete_map_node(map_header);
    }

    map_ptr clone_buffer(const map_ptr &q) {
        map_ptr result = get_map_node();
        result->sz = q->sz;
        buffer_ptr tmp;
        for (buffer_ptr iter = q->buffer_header->next;
             iter != q->buffer_header;
             iter = iter->next) {
            tmp = get_buffer_node();
            tmp->val = new value_type(*iter->val);
            result->buffer_header->prev->next = tmp;
            tmp->prev = result->buffer_header->prev;
            tmp->next = result->buffer_header;
            result->buffer_header->prev = tmp;
        }
        return result;
    }
    map_ptr clone_map(const map_ptr &q_header) {
        map_ptr header = get_map_node();
        map_ptr tmp;
        for (map_ptr iter = q_header->next; iter != q_header; iter = iter->next) {
            tmp = clone_buffer(iter);
            header->prev->next = tmp;
            tmp->prev = header->prev;
            tmp->next = header;
            header->prev = tmp;
        }
        return header;
    }
    void copy(const deque & other) {
        sz = other.sz;
        map_sz = other.map_sz;
        buffer_sz = other.buffer_sz;
        map_header = clone_map(other.map_header);
    }

private:
    /* cut off the first sz buffer_node from p
     * and link the last of the buffer_node to the first one,
     * so that only a buffer_ptr is needed.
     */
    /// !!! the size should be maintain manually
    buffer_ptr cut_buffer(map_ptr p, size_t sz) {
        /*   [k]       [k]
         *    |         |
         *    A         D    A--+
         *    |              |  |
         *    B    -->       B  |
         *    |              |  |
         *    C              C--+
         *    |              |
         *    D
         *  操作如上图所示，返回A。
         *  注意：A的prev被修改为C，但C的next并没有被修改。
         *  实现如下：
         *   由于要求sz>0，所以取出A，作为初始节点，那么接下来
         *   只需要取sz-1个。一直从队首取，直到取满所需的节点
         *   为止。
         */

        if (!sz)
            throw exception();

        buffer_ptr result = p->buffer_header->next;
        result->prev = result;
        p->buffer_header->next = p->buffer_header->next->next;
        p->buffer_header->next->prev = p->buffer_header;
        --sz;

        buffer_ptr tmp = result;
        while (sz--) {
            // 取出最前面的节点
            tmp = p->buffer_header->next;
            // 维护result
            result->prev->next = tmp;
            tmp->prev = result->prev;
            result->prev = tmp;

            //维护buffer
            p->buffer_header->next = p->buffer_header->next->next;
            p->buffer_header->next->prev = p->buffer_header;
        }
        return result;
    }
    /// !!! the size should be maintain manually
    void transplant(map_ptr p, buffer_ptr q) {
        /*   [k]               [k]
         *    |   A--+          |
         *    D   |  |          D
         *        B  |   -->    |
         *        |  |          A
         *        C--+          +-- B -- C
         *        |
         * 操作如图所示，其中q要求经过cut_buffer中的特殊处理。
         * 实现如下：
         *  首先记录C，使得在修改了A的prev后，访问C仍是O(1)的，记为r。
         *  接着修改D和A的关系，将buffer接上去。
         *  注意这里D是buffer的最后一个，所以应该是buffer_head->prev
         *  接下来修改C，也就是r，使buffer成为一个循环链表。
         */
        buffer_ptr r = q->prev;
        q->prev = p->buffer_header->prev;
        p->buffer_header->prev->next = q;

        r->next = p->buffer_header;
        p->buffer_header->prev = r;
    }
    /* split p;
     * p: 1,2,3,4  ->  prev:1,2; p:3,4
     */
    bool need_split(map_ptr p) {
        if (p->sz > buffer_sz)
            return true;
        return false;
    }
    void split(map_ptr p) {
        /*   [k]     [k-1]    [k]
         *    |        |       |
         *    O        O       O
         *    |   -->          |
         *    O                O
         *    |
         *    O
         *  设p所指的buffer为p，则此函数将k分为两个，并将新分出的buffer
         *  插入到k之前，它的大小为k.sz/2。
         *  实现如下：
         *
         */

        ++map_sz;
        map_ptr prev_m_p = get_map_node();
        prev_m_p->prev = p->prev;
        p->prev->next = prev_m_p;
        prev_m_p->next = p;
        p->prev = prev_m_p;
        prev_m_p->sz = p->sz / 2;
        p->sz -= prev_m_p->sz;

        buffer_ptr tmp = cut_buffer(p, prev_m_p->sz);
        transplant(prev_m_p, tmp);
    }

    /*
     * p:1,2; next:3,4,5 -> p:1,2,3,4; next:5
     */
    void absorb(map_ptr p) {
        /*   [k]  [k+1]     [k]  [k+1]
         *    |     |        |     |
         *    A     B        A     D
         *          |   -->  |
         *          C        B
         *          |        |
         *          D        C
         * 操作如图所示。
         * 实现如下：
         *  首先确定需要确认需要从[k+1]中取多少个节点，即left。
         *  并且需要注意[k+1]中节点不够的情况，最后确定的取的节点数记为cap。
         *  维护一下两个buffer的大小。
         *  从[k+1]中切下前cap个，再transparent到[k]上
         */
        size_t left = buffer_sz - p->sz;
        size_t cap = left < p->next->sz ? left : p->next->sz;

        p->sz += cap;
        p->next->sz -= cap;

        buffer_ptr tmp = cut_buffer(p->next, cap);
        transplant(p, tmp);
    }

    bool need_maintenance() {
        return map_sz > 2 * sqrt(sz);
    }
    void maintain() {
        buffer_sz = (size_t)sqrt(sz);
        map_sz = 0;
        /* 根据新的buffer_sz维护每个buffer，
         * 如果当前buffer不满，则从后一个buffer中
         * 取足够多的元素填满当前buffer，把后面取空了，
         * 则直接删除后面的buffer，继续取。
         */
        for (map_ptr iter = map_header->next; iter != map_header; iter = iter->next) {
            while (!buffer_full(iter) && iter->next != map_header) {
                absorb(iter);
                if (buffer_empty(iter->next))
                    delete_empty_map_node(iter->next);
            }
            ++map_sz;
        }
    }


public:
    class iterator {
        friend class deque;
        friend class const_iterator;

    private:
        buffer_ptr node;
        map_ptr buffer;
        deque_ptr dek;

        iterator(buffer_ptr _node, map_ptr _buf, deque_ptr _dek)
                : node(_node), buffer(_buf), dek(_dek) {}

    public:
        iterator(){};
        iterator(const iterator &other)
                : node(other.node), buffer(other.buffer), dek(other.dek) {}
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            /*  [k]  [k+1]  [k+2]
             *   |     |      |
             *   *     C      F
             *   |     |      |
             *   A     D      G
             *   |     |
             *   B     E
             *  考虑当前在A，需要+6，则应该会到G的位置。
             *  思路：首先回退到[k]，然后利用find_from。
             *  假设A在以[k]为起始的坐标系中的下标为p（0-based），
             *  则G的坐标就是p+6，所以只需找出A的下标即可，
             *  调用pos_in_buffer函数得到下标。
             */
            if (!n)
                return *this;

            int pos = (int)dek->pos_in_buffer(node, buffer);
            pos += n;
            sjtu::pair<map_ptr, buffer_ptr> result = dek->find_from(pos, buffer);
            return iterator(result.second, result.first, dek);
        }
        iterator operator-(const int &n) const {
            if (!n)
                return *this;

            int pos = (int)dek->pos_in_buffer(node, buffer);
            pos -= n;
            sjtu::pair<map_ptr, buffer_ptr> result = dek->find_from(pos, buffer);
            return iterator(result.second, result.first, dek);
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            if (dek != rhs.dek)
                throw invalid_iterator();
            int pos = (int)dek->pos_in_buffer(node, buffer);
            map_ptr tmp = buffer->prev;
            while (tmp != dek->map_header) {
                pos += tmp->sz;
                tmp = tmp->prev;
            }

            pos -= dek->pos_in_buffer(rhs.node, rhs.buffer);
            tmp = rhs.buffer->prev;
            while (tmp != dek->map_header) {
                pos -= tmp->sz;
                tmp = tmp->prev;
            }
            return pos;
        }
        iterator operator+=(const int &n) {
            *this = *this + n;
            return *this;
        }
        iterator operator-=(const int &n) {
            *this = *this - n;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        iterator& operator++() {
            if (*this == dek->end())
                throw invalid_iterator();

            pair<buffer_ptr, bool> tmp = dek->increment(node, buffer);
            node = tmp.first;
            if (tmp.second)
                buffer = buffer->next;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        iterator& operator--() {
            if (*this == dek->begin())
                throw invalid_iterator();

            pair<buffer_ptr, bool> tmp = dek->decrement(node, buffer);
            node = tmp.first;
            if (tmp.second)
                buffer = buffer->prev;
            return *this;
        }


        T& operator*() const {
            if (node == dek->end().node)
                throw invalid_iterator();
            return *node->val;
        }
        T* operator->() const noexcept {
            return &(operator*());
        }

        bool operator==(const iterator &rhs) const {
            return node == rhs.node;
        }
        bool operator==(const const_iterator &rhs) const {
            return node == rhs.node;
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
        friend class deque;
        friend class iterator;

        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:
        buffer_ptr node;
        map_ptr buffer;
        deque_ptr dek;

        const_iterator(buffer_ptr _node, map_ptr _buf, deque_ptr _dek)
                : node(_node), buffer(_buf), dek(_dek) {}

    public:
        const_iterator(){}
        const_iterator(const const_iterator &other)
                : node(other.node), buffer(other.buffer), dek(other.dek) {}
        const_iterator(const iterator &other)
                : node(other.node), buffer(other.buffer), dek(other.dek) {}


        const_iterator operator+(const int &n) const {
            if (!n)
                return *this;

            int pos = (int)dek->pos_in_buffer(node, buffer);
            pos += n;
            sjtu::pair<map_ptr, buffer_ptr> result = dek->find_from(pos, buffer);
            return const_iterator(result.second, result.first, dek);
        }
        const_iterator operator-(const int &n) const {
            if (!n)
                return *this;

            int pos = (int)dek->pos_in_buffer(node, buffer);
            pos -= n;
            sjtu::pair<map_ptr, buffer_ptr> result = dek->find_from(pos, buffer);
            return const_iterator(result.second, result.first, dek);
        }

        int operator-(const const_iterator &rhs) const {
            if (dek != rhs.dek)
                throw invalid_iterator();
            int pos = (int)dek->pos_in_buffer(node, buffer);
            map_ptr tmp = buffer->prev;
            while (tmp != dek->map_header) {
                pos += tmp->sz;
                tmp = tmp->prev;
            }

            pos -= dek->pos_in_buffer(rhs.node, rhs.buffer);
            tmp = rhs.buffer->prev;
            while (tmp != dek->map_header) {
                pos -= tmp->sz;
                tmp = tmp->prev;
            }
            return pos;
        }
        const_iterator operator+=(const int &n) {
            *this = *this + n;
            return *this;
        }
        const_iterator operator-=(const int &n) {
            *this = *this - n;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        const_iterator& operator++() {
            pair<buffer_ptr, bool> tmp = dek->increment(node, buffer);
            node = tmp.first;
            if (tmp.second)
                buffer = buffer->next;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        const_iterator& operator--() {
            pair<buffer_ptr, bool> tmp = dek->decrement(node, buffer);
            node = tmp.first;
            if (tmp.second)
                buffer = buffer->prev;
            return *this;
        }


        const T& operator*() const {
            if (node == dek->end().node)
                throw invalid_iterator();
            return *node->val;
        }
        const T* operator->() const noexcept {
            return &(operator*());
        }

        bool operator==(const iterator &rhs) const {
            return node == rhs.node;
        }
        bool operator==(const const_iterator &rhs) const {
            return node == rhs.node;
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

    deque() {
        init();
    }
    deque(const deque &other) {
        copy(other);
    }

    ~deque() {
        destroy();
    }
    deque &operator=(const deque &other) {
        if (this == &other)
            return *this;
        destroy();
        copy(other);
        return *this;
    }
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T & at(const size_t &pos) {
        if (pos_out_of_bound(pos))
            throw index_out_of_bound();
        return *find_from(pos, map_header->next).second->val;
    }
    const T & at(const size_t &pos) const {
        if (pos_out_of_bound(pos))
            throw index_out_of_bound();
        return *find_from(pos, map_header->next).second->val;
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
        if (!sz)
            throw container_is_empty();
        return *map_header->next->buffer_header->next->val;
    }
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T & back() const {
        if (!sz)
            throw container_is_empty();
        return *map_header->prev->buffer_header->prev->val;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(map_header->next->buffer_header->next,
                        map_header->next,
                        this);
    }
    const_iterator cbegin() const {
        return const_iterator(map_header->next->buffer_header->next,
                              map_header->next,
                              (deque_ptr)this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(map_header->prev->buffer_header,
                        map_header->prev,
                        this);
    }
    const_iterator cend() const {
        return const_iterator(map_header->prev->buffer_header,
                              map_header->prev,
                              (deque_ptr)this);
    }
    /**
     * checks whether the container is empty.
     */
    bool empty() const {
        return !sz;
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        return sz;
    }
    /**
     * clears the contents
     */
    void clear() {
        destroy();
        init();
    }
    /**
     * inserts elements at the specified locate on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {
        if (invalid_iter(pos))
            throw invalid_iterator();

        // 容器为空的情况
        if (pos.buffer == map_header) {
            map_ptr map_tmp = get_map_node();
            ++map_sz;
            map_tmp->next = pos.buffer;
            map_tmp->prev = pos.buffer;
            pos.buffer->next = map_tmp;
            pos.buffer->prev = map_tmp;
            pos = iterator(map_tmp->buffer_header, map_tmp, this);
        }

        ++sz;
        ++pos.buffer->sz;
        // 设值
        buffer_ptr tmp = get_buffer_node();
        tmp->val = new T(value);
        // 插入
        pos.node->prev->next = tmp;
        tmp->prev = pos.node->prev;
        tmp->next = pos.node;
        pos.node->prev = tmp;

        // 不需要split情况，直接返回插入位置，也就是前一个位置的迭代器
        --pos;
        if (!need_split(pos.buffer))
            return pos;

        // split
        split(pos.buffer);
        if (need_maintenance()) {
            // 如果需要maintain，那么迭代器会完全失效(buffer部分)，
            // 需要重新找到该点
            buffer_ptr target = pos.node;
            maintain();
            iterator ret;
            for (ret = begin(); ret != end(); ++ret) {
                if (ret.node == target)
                    return ret;
            }
        }

        // 无需maintain的情况，只需确认新插入的节点在
        // 被切割出的前半部分，还是后半部分中。
        map_ptr buffer_of_node = nullptr;
        // 检查是否在后半部分，即pos原来所指buffer
        for (buffer_ptr iter = pos.buffer->buffer_header->next;
             iter != pos.buffer->buffer_header;
             iter = iter->next) {
            if (iter == pos.node) {
                buffer_of_node = pos.buffer;
            }
        }
        if (buffer_of_node == nullptr) {
            buffer_of_node = pos.buffer->prev;
        }
        return iterator(tmp, buffer_of_node, this);

    }
    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid or it points to a wrong place.
     */
    iterator erase(iterator pos) {
        if (empty())
            throw container_is_empty();
        if (invalid_iter(pos) || pos == end())
            throw invalid_iterator();
        iterator ret = pos;
        ++ret;
        --sz;
        --pos.buffer->sz;
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        delete_buffer_node(pos.node);

        if (buffer_empty(pos.buffer)) {
            /* 删除该位置元素后，那一块buffer为空，需要删除。
             * 如果原本该位置元素并非最后的元素，则迭代器ret不会失效。
             * 如果是最后元素，则ret会指向pos.buffer->buffer_header.
             * 删除当前元素后，应当指向它上一个节点的buffer->header
             */
            --map_sz;
            if (ret == end()) {
                ret = iterator(pos.buffer->prev->buffer_header, pos.buffer->prev, this);
            }
            delete_empty_map_node(pos.buffer);
        }

        if (!need_maintenance())
            return ret;

        maintain();
        iterator tar;
        for (tar = begin(); tar != end(); ++tar) {
            if (tar.node == ret.node)
                return tar;
        }
        return tar;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(end(), value);
    }
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {
        erase(--end());
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        insert(begin(), value);
    }
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {
        erase(begin());
    }
};

}

#endif
