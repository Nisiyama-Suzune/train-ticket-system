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
#endif

