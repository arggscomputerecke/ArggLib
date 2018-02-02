#ifndef procMultiThreading_h__
#define procMultiThreading_h__
#include <mutex>
#include <vector>
#include <thread>


#include "proc.hh"
#include "procReturn.hh"
#include <algorithm>
#include <atomic>

class thread_handler {
public:
  void update() {

    {
      std::unique_lock<std::mutex> lock(m);
      m_ids.erase(std::remove_if(m_ids.begin(), m_ids.end(), [](auto& e) {return e == std::this_thread::get_id(); }), m_ids.end());
      m_ids.push_back(std::this_thread::get_id());
    }
    con.notify_all();

  }
  void push(std::thread::id id__) {

    std::unique_lock<std::mutex> lock(m);
    m_ids.push_back(id__);

  }

  auto wait() {
    std::unique_lock<std::mutex> lock(m);
    auto this_id = std::this_thread::get_id();
    while (m_ids[0] != this_id) {
      con.wait(lock);
    }
    return lock;
  }

  std::condition_variable con;
  std::vector<std::thread::id> m_ids;
  std::mutex m;

};








class par_for_with_thread_handle {
public:
  par_for_with_thread_handle(int numOfThreads, thread_handler* th) :m_num(numOfThreads), m_th(th) {}

  template<typename Next_t>
  auto operator()(Next_t&& next_, int start_, int end_, int step_) {
    std::vector<std::thread> threads;

    for (int j = 0; j < m_num; ++j) {
      threads.push_back(std::thread([next_, start_, step_, end_, j, numOfThreads = m_num, thread_handler__ = m_th]() mutable {
        for (auto i = start_ + step_ * j; i < end_; i += (step_ * numOfThreads)) {
          next_(i);
          thread_handler__->update();
        }
      }));

      m_th->push(threads.back().get_id());
    }

    for (auto&e : threads) {
      e.join();
    }

    return success;
  }

  template<typename Next_t>
  auto operator()(Next_t&& next_, int start_, int end_) {
    return operator()(std::forward<Next_t>(next_), start_, end_, 1);
  }

  template<typename Next_t>
  auto operator()(Next_t&& next_, int end_) {
    return operator()(std::forward<Next_t>(next_), 0, end_, 1);
  }
private:
  const  int m_num;
  thread_handler* m_th;
};

class wait_for_start {
  std::atomic<int> m_start = 0;
public: 
  void wait() {
    while (m_start == 0){}
    m_start = 0;
  }
  void done() {
    m_start = 1;
    while (m_start == 1){}
  }
};

class par_for_with_thread_handle_from_to {
public:
  par_for_with_thread_handle_from_to(int numOfThreads, thread_handler* th, int start, int stop_) :m_num(numOfThreads), m_th(th), m_start(start), m_stop(stop_) {}

  template<typename Next_t>
  auto operator()(Next_t&& next_) {
    std::vector<std::thread> threads;
    int start_ = m_start;
    int step_ = 1;
    int end_ = m_stop;

    for (int j = 0; j < m_num; ++j) {

      wait_for_start start{};
      threads.emplace_back([next_, start_, step_, end_, j, numOfThreads = m_num, thread_handler__ = m_th, &start]() mutable {
        start.wait();
        for (auto i = start_ + step_ * j; i < end_; i += (step_ * numOfThreads)) {
          next_(i);
          thread_handler__->update();
        }
      });

      m_th->push(threads.back().get_id());
      start.done();
    }

    for (auto&e : threads) {
      e.join();
    }

    return success;
  }

  template<typename Next_t>
  auto operator()(Next_t&& next_, int start_, int end_) {
    return operator()(std::forward<Next_t>(next_), start_, end_, 1);
  }

  template<typename Next_t>
  auto operator()(Next_t&& next_, int end_) {
    return operator()(std::forward<Next_t>(next_), 0, end_, 1);
  }
private:
  const  int m_num, m_start, m_stop;
  thread_handler* m_th;

};



class par_for_without_thread_handle {
public:
  par_for_without_thread_handle(int numOfThreads) :m_num(numOfThreads) {}

  template<typename Next_t>
  auto operator()(Next_t&& next_, int start_, int end_, int step_) {
    std::vector<std::thread> threads;

    for (int j = 0; j < m_num; ++j) {
      threads.push_back(
        std::thread([next_, start_, step_, end_, j, numOfThreads = m_num]() mutable {
        for (auto i = start_ + step_*j; i < end_; i += (step_ * numOfThreads)) {
          next_(i);
        }
      }
      ));

    }

    for (auto&e : threads) {
      e.join();
    }

    return success;
  }
  template<typename Next_t>
  auto operator()(Next_t&& next_, int start_, int end_) {
    return operator()(std::forward<Next_t>(next_), start_, end_, 1);
  }
  template<typename Next_t>
  auto operator()(Next_t&& next_, int end_) {
    return operator()(std::forward<Next_t>(next_), 0, end_, 1);
  }
private:
  const  int m_num;
};



auto par_for(int numOfThreads, thread_handler* th) {
  return par_for_with_thread_handle(numOfThreads, th);
}

auto par_for(int numOfThreads, thread_handler* th, int start_, int stop_) {
  return par_for_with_thread_handle_from_to(numOfThreads, th, start_, stop_);
}
auto par_for(int numOfThreads) {
  return par_for_without_thread_handle(numOfThreads);
}



class de_randomize {
public:
  de_randomize(thread_handler* mt) :m_th(mt) {}


  template<typename NEXT_T, typename... ARGS>
  auto operator()(NEXT_T&& next_, ARGS&&... args) {

    auto lock = m_th->wait();
    return  next_(args...);

  }
private:
  thread_handler* m_th;
};
#endif // procMultiThreading_h__
