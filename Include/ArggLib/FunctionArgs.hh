#ifndef FunctionArgs_h__
#define FunctionArgs_h__
#include <type_traits>
#include "ArggLib/PRECOMPILER_HELPER.hh"

namespace ArggLib {


	template <typename T1, typename T2>
	class FunctionArg_A1_and_B1 {
	public:
		FunctionArg_A1_and_B1(T1 t1, T2 t2) :value_A(t1), value_B(t2) {}
		T1 value_A;
		T2 value_B;

	};
	template <typename T1, typename T2, bool t1_set = false, bool t2_set = false>
	class FunctionArg_A1_and_B0 {
	public:
		FunctionArg_A1_and_B0(T1 t1) :value_A(t1) {}
		auto set_B(T2&& t2) && {
			return FunctionArg_A1_and_B1<T1, T2>(value_A, t2);
		}
		T1 value_A;

	};
	template <typename T1, typename T2, bool t1_set = false, bool t2_set = false>
	class FunctionArg_A0_and_B0 {
	public:
		template <std::enable_if_t<!t1_set> N = 0>
		auto set_A(T1 t1) {
			return FunctionArg_A0_and_B0<T1, T2, true, t2_set>(t1);
		}
		template <std::enable_if_t<!t2_set, int> N = 0>
		auto set_B(T2 t2) {
			return FunctionArg_A0_and_B0<T1, T2, t1_set, true>(t2);
		}
		// 		template <std::enable_if_t<t2_set, int> N = 0>
		// 		auto get_B() const{
		// 			return B;
		// 		}
		// 		template <std::enable_if_t<t1_set, int> N = 0>
		// 		auto get_A() const {
		// 			return A;
		// 		}
		T1 A;
		T2 B;
	};
	// #define  named_arg(name) \
	// 	template <typename RET_T, typename T,bool N> \
	// 	class CONCATENATE(named_arg_,name) { \
	// 	public: \
	// 		auto name(T x) { return 1;} \ 
	// 	}; \
	// 	template <typename RET_T,typename T> \
	// 	class CONCATENATE(named_arg_,name) <RET_T,T,true> { \
	// 	public: \
	// 		CONCATENATE(named_arg_,name)(T x) : X(x) {} \
	// 		auto CONCATENATE(get_,name)() { return X; } \
	// 	private:\
	// 		T X; \
	// 	}
	// 	named_arg(filename);
	// 	named_arg(extension);



	template <typename CURRENT_T, typename RET_T, typename T, bool N> \
		class named_arg_date {
		public:
			const CURRENT_T* m_curr =nullptr;
			named_arg_date(const CURRENT_T& curr) :m_curr(&curr) {}
			named_arg_date(CURRENT_T& curr) :m_curr(&curr) {}
			template <typename T1>
			named_arg_date(T1& t) {}
			named_arg_date() = default;

			template <typename T1, typename T2>
			named_arg_date(const T1* t, T2* t2) {
				auto x = reinterpret_cast<const CURRENT_T*>(&t);
			//	X = t2->get_date();
				//X = x->get_date();
				m_curr = t;
			}
			auto date(T x) { return RET_T(*m_curr, named_arg_date<CURRENT_T, RET_T,T,true>( x)); }
	};
	template <typename CURRENT_T, typename RET_T, typename T>
	class named_arg_date <CURRENT_T, RET_T, T, true > {
	public:
		template <typename T1, typename T2>
		named_arg_date(const T1& t,T2* t2)  {
			auto x = reinterpret_cast<const CURRENT_T*>(&t);
			X = t2->get_date();
			
		}
		template <typename T1, typename T2>
		named_arg_date(const  named_arg_date <T1, T2, T, true > & t){
			X = t.get_date();
		}
// 		template <typename T1, typename T2>
// 		named_arg_date(const  named_arg_date <T1, T2, T, false > & t) {
// 	//		X = t.get_date();
// 			//auto x = reinterpret_cast<const CURRENT_T*>(&t);
// 			//X = x->get_date();
// 		}

		named_arg_date() = default;
		named_arg_date(T x) : X(x) {}
		auto get_date()const { return X; }
	private:
		T X;
	};

	template <typename CURRENT_T, typename RET_T, typename T, bool N> \
		class named_arg_time {
		public:
			const CURRENT_T* m_curr = nullptr;
			named_arg_time(const CURRENT_T& curr) :m_curr(&curr) {}
			named_arg_time( CURRENT_T& curr) :m_curr(&curr) {}
			template <typename T1, typename T2>
			named_arg_time( T1* t,T2* t2) {
			
				auto x = reinterpret_cast<const CURRENT_T*>(&t);
				m_curr = t;
			}
			named_arg_time() = default;
			auto time(T x) { return RET_T(*m_curr, named_arg_time<CURRENT_T, RET_T, T, true>(x)); }
	};
	template <typename CURRENT_T, typename RET_T, typename T>
	class named_arg_time <CURRENT_T, RET_T, T, true > {
	public:
	//	CURRENT_T& m_curr;
	//	named_arg_time(CURRENT_T& curr) :m_curr(curr) {}
		template <typename T1>
		named_arg_time(T1 t) {}
		named_arg_time() = default;
		named_arg_time(T& x) : X(x) {}
		named_arg_time(const T& x) : X(x) {}
		template <typename T1, typename T2>
		named_arg_time(const  named_arg_time <T1, T2, T, true > & t) {
			X = t.get_time();
			//auto x = reinterpret_cast<const CURRENT_T*>(&t);
			//X = x->get_date();
		}
		template <typename T1, typename T2>
		named_arg_time(const  named_arg_time <T1, T2, T, false > & t) {
		//	X = t.get_time();
			//auto x = reinterpret_cast<const CURRENT_T*>(&t);
			//X = x->get_date();
		}
		template <typename T1, typename T2>
		named_arg_time(T1* t, T2* t2) {

			auto x = reinterpret_cast<const CURRENT_T*>(&t);
			//m_curr = t;
			X = t2->get_time();
		}
		auto get_time() const { return X; }
	private:
		T X;
	};

	template <typename T1, typename T2, bool N1, bool N2>
	class test2 {
	public:

		auto set_A(T1 x) {
			return test2<T1, T2, true, false>(x);
		}

		auto set_B(T2 x) {
			return test2<T1, T2, false, true>(x);
		}


	};

	template <typename T1, typename T2>
	class test2<T1, T2, false, true> {
	public:

		test2(T2 x) : B(x) {}

		auto set_A(T1 x) {
			return test2<T1, T2, true, true>(x, B);
		}

		auto get_B() {
			return B;
		}
		T2 B;
	};

	template <typename T1, typename T2>
	class test2<T1, T2, true, false> {
	public:

		test2(T1 x) : A(x) {}

		auto set_B(T1 x) {
			return test2<T1, T2, true, true>(A, x);
		}

		auto get_A() {
			return A;
		}
		T1 A;
	};

	template <typename T1, typename T2>
	class test2<T1, T2, true, true> {
	public:

		test2(T1 x, T2 t2) : A(x), B(t2) {}

		auto get_A() {
			return A;
		}
		auto get_B() {
			return B;
		}
		T2 B;
		T1 A;
	};

	template <bool N1 = false, bool N2 = false>
	class test3 :public named_arg_date<test3<N1, N2>,test3<true, N2>, int, N1>, public named_arg_time<test3<N1, N2>, test3<N1, true>, int, N2> {
	public:
		test3():named_arg_date<test3<N1, N2>, test3<true, N2>, int, N1>(*this),  named_arg_time<test3<N1, N2>, test3<N1, true>, int, N2>(*this) {}
		template <typename T>
		test3(const T& t3, const  named_arg_time<test3<N1, N2>, test3<N1, true>, int, N2>& t2) :
			named_arg_date<test3<N1, N2>, test3<true, N2>, int, N1>(this, &t3),
			named_arg_time<test3<N1, N2>, test3<N1, true>, int, N2>(t2) {}
		
		
		template <typename T,typename T2>
		test3(const T& t3, const named_arg_date<T2, test3<true, N2>, int, N1>& t2) : 
			named_arg_time<test3<N1, N2>, test3<N1, true>, int, N2>(this, &t3),
			named_arg_date<test3<N1, N2>, test3<true, N2>, int, N1>(t2) {}
		
		

	};
}
#endif // FunctionArgs_h__