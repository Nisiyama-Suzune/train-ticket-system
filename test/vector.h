#ifndef VECTOR
	#define VECTOR
	#include "smart_ptr.h"
	#include "smart_ptr.cpp"
	template <class T>
	class vector {
		protected:
			smart_ptr <T> m_ptr;
			unsigned int m_size;
			unsigned int d_size;
			void allocate_space (unsigned int size);
		public:
			vector (unsigned int size = 0);
			vector (const T* x, unsigned int size);
			vector (const vector <T> &rhs);
			~vector ();
			unsigned int size () const;
			const T* c_data () const;
            void push_back (const T &x);
            void pop_back ();
			void resize (unsigned int size);
			void clear ();
			T &operator [] (unsigned int loc);
			const T &operator [] (unsigned int loc) const;
			const vector <T> operator = (const vector <T> &rhs);
			const vector <T> operator + (const vector <T> &rhs) const;
			bool operator < (const vector <T> &rhs);
			bool operator <= (const vector <T> &rhs);
			bool operator > (const vector <T> &rhs);
			bool operator >= (const vector <T> &rhs);
			bool operator == (const vector <T> &rhs);
			bool operator != (const vector <T> &rhs);
			const vector <T> partition (unsigned int loc, unsigned int len) const;
			unsigned int find (const T &x) const;
			unsigned int find (const vector <T> &x) const;
	};
#endif

