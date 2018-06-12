#ifndef reactive_ports_h__
#define reactive_ports_h__


#include "ArggLib/reactive_programming/reactive_signals.hh"
#include "ArggLib/reactive_programming/reactive_entity_base.hh"


namespace ArggLib {


	template<typename reactive_signals_T>
	class active_in_port {
	public:
		active_in_port() :m_base(get_current_reactive_entity_base()) {

		}
		void set_input(reactive_signals_T* input) {
			
			m_input = input;
			input->register_processor_ptr(&m_base->m_process);
		}
		auto value() const {
			return m_input->value();
		}
		reactive_entity_base* m_base = nullptr;
		const reactive_signals_T* m_input = nullptr;
	};
	template<typename reactive_signals_T>
	class passive_in_port {
	public:
		passive_in_port() :m_base(get_current_reactive_entity_base()) {

		}
		void set_input(reactive_signals_T* input) {

			m_input = input;
			
		}
		auto value() const {
			return m_input->value();
		}
		reactive_entity_base* m_base = nullptr;
		const reactive_signals_T* m_input = nullptr;
	};

	template<typename reactive_signals_T>
	class out_port {
	public:
		template <typename T>
		out_port(T&& t) :m_base(get_current_reactive_entity_base()), m_out(std::forward<T>(t),get_current_reactive_entity_base()->m_backend) {

		}

		auto value() const {
			return m_out->value();
		}
		template <typename T>
		void set(T&& t) {
			m_out.set(std::forward<T>(t));
		}
		template <typename T>
		void operator<<=(T&& t) {
			m_out.set(std::forward<T>(t));
		}
		reactive_entity_base* m_base = nullptr;
		reactive_signals_T m_out;
	};
}


#endif // reactive_ports_h__
