#ifndef SMART_PTR
	#define SMART_PTR
	template <class T>
	class smart_ptr {
		private:
			T *m_ptr;
			int *m_cnt;
			void terminate ();
		public:
			smart_ptr ();
			explicit smart_ptr (T * const ptr);
			smart_ptr (const smart_ptr <T> &ptr);
			~smart_ptr ();
			const smart_ptr <T> &operator = (const smart_ptr <T> &ptr);
			T &operator * () const;
			T *operator -> () const;
			T &operator [] (unsigned int loc) const;
			int get_count () const;
			bool expired () const;
	};
	template <class T>
	smart_ptr <T> make_ptr (T * const ptr);

template <class T>
void smart_ptr <T>::terminate () {
	if (m_cnt != nullptr) {
		--(*m_cnt);
		if (*m_cnt == 0) {
			if (m_ptr != nullptr) delete []m_ptr;
			delete m_cnt;
		}
	}
}

template <class T>
smart_ptr <T>::smart_ptr () {
	m_ptr = nullptr;
	m_cnt = nullptr;
}

template <class T>
smart_ptr <T>::smart_ptr (T * const ptr) {
	if (ptr == nullptr) {
		m_ptr = nullptr;
		m_cnt = nullptr;
	} else {
		m_ptr = ptr;
		m_cnt = new int;
		*m_cnt = 1;
	}
}

template <class T>
smart_ptr <T>::smart_ptr (const smart_ptr <T> &ptr) {
	m_ptr = ptr.m_ptr;
	m_cnt = ptr.m_cnt;
	if (m_cnt != nullptr) ++(*m_cnt);
}

template <class T>
smart_ptr <T>::~smart_ptr () {
	terminate ();
}

template <class T>
const smart_ptr <T> &smart_ptr <T>::operator = (const smart_ptr <T> &ptr) {
	terminate ();
	m_ptr = ptr.m_ptr;
	m_cnt = ptr.m_cnt;
	++(*m_cnt);
	return ptr;
}

template <class T>
T &smart_ptr <T>::operator * () const {
	if (m_ptr == nullptr) throw;
	return *m_ptr;
}

template <class T>
T *smart_ptr <T>::operator -> () const {
	if (m_ptr == nullptr) throw;
	return m_ptr;
}

template <class T>
T &smart_ptr <T>::operator [] (unsigned int loc) const {
	if (m_ptr == nullptr) throw;
	return m_ptr[loc];
}

template <class T>
int smart_ptr <T>::get_count () const {
	if (m_cnt == nullptr) return 0;
	return *m_cnt;
}

template <class T>
bool smart_ptr <T>::expired () const {
	return (m_cnt == nullptr);
}

template <class T>
smart_ptr <T> make_ptr (T * const ptr) {
	return smart_ptr <T> (ptr);
}



#endif

