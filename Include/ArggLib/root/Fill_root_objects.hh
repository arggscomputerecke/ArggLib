#ifndef ArggLib_Fill_root_objects
#define ArggLib_Fill_root_objects

#include "TH1D.h"
#include "TGraph.h"
#include "ArggLib/type_trates.hh"
#include <type_traits>

namespace ArggLib {
  template <typename T>
  class Fill_root_histogram_impl {
  public:
    T * m_h = nullptr;
    bool do_reset = false;
    Fill_root_histogram_impl& reset_on_start() {
      do_reset = true;
      return *this;
    };
    Fill_root_histogram_impl(T* h) :m_h(h) {}
    template <typename NEXT_T, typename... ARGS>
    auto operator()(NEXT_T&& next, ARGS&&... x) ->decltype(next(x...)) {
      m_h->Fill(x...);
      return next(x...);
    }
    
    procReturn Start() {
      if (do_reset) {
        m_h->Reset();
      }
      return procReturn::success;
    }
    auto End() ->decltype(m_h) {

      return   m_h;
    }

  };
  template <typename T>
  auto Fill_histogramm(T* t) -> decltype(Fill_root_histogram_impl<T>(t)){
    return  Fill_root_histogram_impl<T>(t);
  }



  template < typename T, typename std::enable_if < std::is_base_of<TH1, T>::value , int > ::type = 0 >
  auto __helper_to_proc(T* t) -> decltype(Fill_histogramm(t)) {
    return Fill_histogramm(t);
  }
  template < typename T, typename std::enable_if < std::is_base_of<TH1, T>::value, int > ::type = 0 >
  inline auto __helper_to_proc(T& t) -> decltype(Fill_histogramm(&t)) {
    return Fill_histogramm(&t);
  }

  template <typename T>
  class Fill_root_graph_impl{
  public:
	  Fill_root_graph_impl(T* graph_) :m_graph(graph_) {}
	  T* m_graph;
	  template <typename NEXT_T, typename... ARGS>
	  auto operator()(NEXT_T&& next, ARGS&&... x) ->decltype(next(x...)) {
		  
		  m_graph->SetPoint(m_graph->GetN(), x...);
	
		  return next(x...);
	  }


	  auto End() ->decltype(m_graph) {

		  return   m_graph;
	  }

  };

  template <typename T>
  auto Fill_graph(T* t) -> decltype(Fill_root_graph_impl<T>(t)) {
	  return  Fill_root_graph_impl<T>(t);
  }

  template < typename T, typename std::enable_if < std::is_base_of<TGraph, T>::value, int > ::type = 0 >
  auto __helper_to_proc(T* t) -> decltype(Fill_graph(t)) {
	  return Fill_graph(t);
  }
  template < typename T, typename std::enable_if < std::is_base_of<TGraph, T>::value, int > ::type = 0 >
  inline auto __helper_to_proc(T& t) -> decltype(Fill_graph(&t)) {
	  return Fill_graph(&t);
  }

  class Draw_root_objects {
  public:
    

    Draw_root_objects()  {

    }

    template <typename ARGS>
    auto End(ARGS&& args) ->decltype(args->Draw()) {

      return   args->Draw();
    }

    template <typename NEXT_T, typename... ARGS>
    procReturn operator()(NEXT_T&& next, ARGS&&... args) {


      return next(std::forward<ARGS>(args)...);
    }


  };
}
#endif