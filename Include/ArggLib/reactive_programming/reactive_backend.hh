#ifndef reactive_backend_h__
#define reactive_backend_h__

#include "ArggLib/reactive_programming/reactive_processor.hh"
#include "ArggLib/contains.hh"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>


namespace ArggLib {


	class reactive_backend {
	public:
		reactive_backend() :m_worker([this]() mutable {return this->run(); }) {

		}
		void run() {

			while ((m_running == running || !m_notify.empty()) && m_running != force_stopping) {

				std::unique_lock<std::mutex> lock(m);
				if (m_notify.empty()) cond_var.wait(lock);
				auto l_notify = m_notify;
				m_notify.clear();
				lock.unlock();
				for (auto& e : l_notify) {
					e.begin();
				}
				for (auto& e : l_notify) {
					e.process();
				}
				for (auto& e : l_notify) {
					e.end();
				}
			}

			m_running = stopped;
		}
		void push_back(reactive_processor_impl_c& f) {
			{

				std::unique_lock<std::mutex> lock(m);
				if (!contains(m_notify, f))
				{
					m_notify.push_back(f);
				}
			}
			cond_var.notify_one();
		}
		void append(const std::vector<reactive_processor_impl_c> &  app) {
			{

				std::unique_lock<std::mutex> lock(m);
				for (auto f : app) {
					if (!contains(m_notify, f))
					{
						m_notify.push_back(f);
					}
				}
			}
			cond_var.notify_one();

		}

		// 		void append_ref(const std::vector<reactive_processor_impl_c&> & app) {
		// 			{
		// 
		// 				std::unique_lock<std::mutex> lock(m);
		// 				for (auto f : app) {
		// 					m_notify.push_back(f);
		// 				}
		// 			}
		// 			cond_var.notify_one();
		// 
		// 		}
		void stop() {
			m_running = stopping;

			m_worker.join();

		}
		void force_stop() {
			m_running = force_stopping;

			m_worker.join();

		}
	private:
		enum state {
			running,
			stopping,
			force_stopping,
			stopped
		};
#ifndef  WIN32 
		state m_running = running;
#else
		std::atomic<state> m_running = running;
#endif
		std::condition_variable cond_var;
		std::mutex m;
		std::thread m_worker;
		std::vector<reactive_processor_impl_c> m_notify;
	};

	reactive_backend& get_reactive_backend() {
		static reactive_backend g_reactive_backend;
		return g_reactive_backend;
	}

}

#endif // reactive_backend_h__