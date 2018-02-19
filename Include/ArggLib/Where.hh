#ifndef ArggLibWhere_h__
#define ArggLibWhere_h__
#include "ArggLib/procReturn.hh"
#include "ArggLib/proc.hh"

namespace ArggLib {

	template<typename T>
	class Where_impl {
	public:
		T m_fun;

		Where_impl(T fun) :m_fun(std::move(fun)) {

		}

		template <typename NEXT_T, typename... ARGS>
		procReturn operator()(NEXT_T&& next, ARGS&&... args) {

			if (m_fun(args...)) {
				return next(std::forward<ARGS>(args)...);
			}

			return procReturn::success;

			
		}


	};
	template <typename T>
	auto Where(T fun) {
		return proc()>> Where_impl<T>(std::move(fun));
	}

  enum where_begin_e{
  __whereBegin_enum
  };

  template<typename T>
  auto operator*(where_begin_e, T&& t) {
    return Where(std::forward<T>(t));
  }
#define _where __whereBegin_enum * [&](cautor _x)
}

#endif // ArggLibWhere_h__
