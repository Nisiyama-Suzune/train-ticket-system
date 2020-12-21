#include "vector.h"

template <class T>
void vector <T>::allocate_space (unsigned int size) {
    if (size <= m_size + 1) return;
    smart_ptr <T> ptr (new T[size] ());
	for (unsigned int i = 0; i < m_size; i++)
		ptr[i] = m_ptr[i];
	m_ptr = ptr;
	d_size = size;
}

template <class T>
vector <T>::vector (unsigned int size) {
	clear ();
	resize (size);
}

template <class T>
vector <T>::vector (const T* x, unsigned int size) {
	clear ();
	resize (size);
	for (int i = 0; i < m_size; i++)
		m_ptr[i] = x[i];
}

template <class T>
vector <T>::vector (const vector &rhs) {
	clear ();
	resize (rhs.size ());
	for (int i = 0; i < m_size; i++)
		m_ptr[i] = rhs[i];
}

template <class T>
vector <T>::~vector () {
	return;
}

template <class T>
unsigned int vector <T>::size () const {
	return m_size;
}

template <class T>
const T* vector <T>::c_data () const {
	return &(*m_ptr); 
}

template <class T>
void vector <T>::push_back (const T &x) {
    if (++m_size >= d_size) allocate_space (d_size * 2 + 2);
	m_ptr[m_size - 1] = x;
}

template <class T>
void vector <T>::pop_back () {
    if (m_size > 0) resize (m_size - 1);
}

template <class T>
void vector <T>::resize (unsigned int size) {
	unsigned int p_size = d_size;
    while (p_size <= size)
        p_size = p_size * 2 + 2;
	allocate_space (p_size);
	m_size = size;
}

template <class T>
void vector <T>::clear () {
	m_ptr = nullptr;
	m_size = 0;
	d_size = 0;
}

template <class T>
T &vector <T>::operator [] (unsigned int loc) {
	if (loc >= m_size) throw;
	return m_ptr[loc];
}

template <class T>
const T &vector <T>::operator [] (unsigned int loc) const {
	if (loc >= m_size) throw;
	return m_ptr[loc];
}

template <class T>
const vector <T> vector <T>::operator = (const vector <T> &rhs) {
	resize (rhs.size ());
	for (int i = 0; i < m_size; i++)
		m_ptr[i] = rhs[i];
}

template <class T>
const vector <T> vector <T>::operator + (const vector <T> &rhs) const {
	vector <T> ans (m_size + rhs.size ());
	for (unsigned int i = 0; i < m_size; i++)
		ans[i] = m_ptr[i];
	for (unsigned int i = m_size; i < m_size + rhs.size (); i++)
		ans[i] = rhs[i - m_size];
	return ans;
}

template <class T>
bool vector <T>::operator < (const vector <T> &rhs) {
	unsigned int size = (m_size < rhs.size () ? m_size : rhs.size ());
	for (int i = 0; i < size; i++) {
		if (m_ptr[i] < rhs[i]) return true;
		if (rhs[i] < m_ptr[i]) return false;
	}
	if (size < rhs.size ()) return true;
	return false;
}

template <class T>
bool vector <T>::operator <= (const vector <T> &rhs) {
	unsigned int size = (m_size < rhs.size () ? m_size : rhs.size ());
	for (int i = 0; i < size; i++) {
		if (m_ptr[i] < rhs[i]) return true;
		if (rhs[i] < m_ptr[i]) return false;
	}
	if (size < m_size) return false;
	return true;
}

template <class T>
bool vector <T>::operator > (const vector <T> &rhs) {
	unsigned int size = (m_size < rhs.size () ? m_size : rhs.size ());
	for (int i = 0; i < size; i++) {
		if (m_ptr[i] < rhs[i]) return false;
		if (rhs[i] < m_ptr[i]) return true;
	}
	if (size < m_size) return true;
	return false;
}

template <class T>
bool vector <T>::operator >= (const vector <T> &rhs) {
	unsigned int size = (m_size < rhs.size () ? m_size : rhs.size ());
	for (int i = 0; i < size; i++) {
		if (m_ptr[i] < rhs[i]) return false;
		if (rhs[i] < m_ptr[i]) return true;
	}
	if (size < rhs.size ()) return false;
	return true;
}


template <class T>
bool vector <T>::operator == (const vector <T> &rhs) {
	if (m_size != rhs.size ()) return false;
	for (int i = 0; i < m_size; i++) {
		if (m_ptr[i] < rhs[i]) return false;
		if (rhs[i] < m_ptr[i]) return false;
	}
	return true;
}

template <class T>
bool vector <T>::operator != (const vector <T> &rhs) {
	return !((*this) == rhs);
}

template <class T>
const vector <T> vector <T>::partition (unsigned int loc, unsigned int len) const {
	if (loc >= m_size || len == 0) return vector <T> (0);
	if (loc + len > m_size) len = m_size - loc;
	vector <T> ans (len);
	for (unsigned int i = loc; i < loc + len; i++)
		ans[i - loc] = m_ptr[i];
	return ans;
}

template <class T>
unsigned int vector <T>::find (const T &x) const {
	for (int i = 0; i < m_size; i++)
        if (!(m_ptr[i] < x || x < m_ptr[i])) return i;
	return m_size;
}

template <class T>
unsigned int vector <T>::find (const vector <T> &x) const {
	for (unsigned int i = 0; i < m_size - x.size (); i++) {
		bool is_match = true;
		for (unsigned int j = 0; j < x.size (); j++)
			if (m_ptr[i + j] < x[j] || x[j] < m_ptr[i + j]) {
				is_match = false;
				break;
			}
		if (is_match) return i;
	}
	return m_size;
}

