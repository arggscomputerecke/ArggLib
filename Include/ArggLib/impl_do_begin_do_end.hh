#ifndef ArggLib_impl_do_begin_do_end_h__
#define ArggLib_impl_do_begin_do_end_h__
#include "ArggLib/has_member_helper.hh"

namespace ArggLib {

	namespace ArggLib_impl {



		CREATE_TEST_FOR_MEMBER(hasEnd, End());
		CREATE_TEST_FOR_MEMBER(hasStart, Start());

		CREATE_TEST_FOR_MEMBER(has_t, t);
		CREATE_TEST_FOR_MEMBER(has_n, n);


    template <typename... N>
    auto do_end0a_check_for_return(N&&... n) ->decltype(success){
      return success;
    }




    template <typename N, typename T , __ENABLE_IF_V2 (hasEnd<N>)>
    T&& do_end0a_check_for_return(N&& n, T&& value) {
      	n.End(value);
        return value;

    }




		template <typename... N>
		procReturn do_end0(N&&... n) {
			return do_end0a_check_for_return(n...);
		}


    // function has return but no input 
		template <typename N, typename T >
		auto do_end0(N&& n, T&& value) ->decltype(n.End()) {
			return	n.End();

		}


    template <typename N, typename T >
    T&& do_end1a_no_return_value(N&& n, T&& value) {
      n.End(value);
      return value;
    }


		template <typename... N>
		auto do_end1(N&&... n)  -> decltype(do_end0(n...)){
			return do_end0(n...);
		}

    // function hat input und return 
		template <typename N ,typename T >
		auto do_end1(N&& n,T&& value) ->decltype(n.End(value)) {
			return	n.End(value);

		}



    //remaining  element
		template <typename... N>
		auto do_end2(N&&... n) -> decltype(do_end1(n...)){
			return do_end1(n...);
		}


    // last elements 
		template <typename N,typename T>
		auto do_end2(N&& n,T&& value) ->decltype(do_end1(n.n,value)){
			return	do_end1(n.n ,value);

		}
// 
// 		template <typename N>
// 		__ENABLE_IF(has_n<const N>(), procReturn) do_end2(const N& n) {
// 			return	do_end1(n.n);
// 
// 		}
// 
// 		template <typename N>
// 		__ENABLE_IF(has_n< N>(), procReturn) do_end2(N&& n) {
// 			return	do_end1(n.n);
// 
// 		}


		template <typename... P>
		auto unfold_end(P&&... p) -> decltype(do_end2(p..., success)) {
			
			return do_end2(p...,success);;
		}

		template <typename P>
		auto  unfold_end(P&& p) -> decltype(p.t, do_end2(p, unfold_end(p.t))) {

			
	
			return  do_end2(p, unfold_end(p.t));
		}

// 
// 		template <typename P>
// 		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_end(const P& p) {
// 
// 			unfold_end(p.t);
// 			return do_end2(p);
// 		}
// 
// 
// 		template <typename P>
// 		__ENABLE_IF(has_t< P>(), procReturn)  unfold_end(P&& p) {
// 
// 			unfold_end(p.t);
// 			return do_end2(p);
// 		}






		template <typename N>
		procReturn do_Start1(N&& n) {
			return success;
		}

		template <typename N>
		__ENABLE_IF(hasStart<N>(), procReturn) do_Start1(N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart<const N>(), procReturn) do_Start1(const N& n) {
			return	n.Start();

		}

		template <typename N>
		__ENABLE_IF(hasStart< N>(), procReturn) do_Start1(N&& n) {
			return n.Start();

		}



		template <typename N>
		__ENABLE_IF_NOT(has_n<N>(), procReturn)  do_Start2(N&& n) {
			return do_Start1(n);
		}

		template <typename N>
		__ENABLE_IF(has_n<N>(), procReturn) do_Start2(N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n<const N>(), procReturn) do_Start2(const N& n) {
			return	do_Start1(n.n);

		}

		template <typename N>
		__ENABLE_IF(has_n< N>(), procReturn) do_Start2(N&& n) {
			return	do_Start1(n.n);

		}


		template <typename P>
		procReturn unfold_Start(P&& p) {
			do_Start2(p);
			return success;
		}

		template <typename P>
		__ENABLE_IF(has_t<P>(), procReturn)  unfold_Start(P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t<const P>(), procReturn)  unfold_Start(const P& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}


		template <typename P>
		__ENABLE_IF(has_t< P>(), procReturn)  unfold_Start(P&& p) {

			do_Start2(p);
			return unfold_Start(p.t);
		}

	}

}



#endif // ArggLib_impl_do_begin_do_end_h__
