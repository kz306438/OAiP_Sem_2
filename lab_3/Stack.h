#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"

namespace container
{
	
	template <typename T, class Container = LinkedList<T>>
	class Stack
	{
	public:
		using value_type		 = typename Container::value_type;
		using reference			 = typename Container::reference;
		using const_reference	 = typename Container::const_reference;
		using size_type			 = typename Container::size_type;
		using container_type	 = Container;

		static_assert(std::is_same_v<T, value_type>, "container adaptors require consistent types");

		Stack() = default;

		explicit Stack(const Container& container) : m_container(container) {}

		explicit Stack(Container&& container) noexcept(is_nothrow_constructible_v(Container))
			: m_container(container) {}

		[[nodiscard]] bool empty() const noexcept(noexcept(m_container.empty())) {
			return m_container.empty();
		}

		[[nodiscard]] size_type size() const noexcept(noexcept(m_container.size())) {
			return m_container.size();
		}

		[[nodiscard]] reference top() noexcept(noexcept(m_container.back())) {
			return m_container.back();
		}

		[[nodiscard]] const_reference top() const noexcept(noexcept(m_container.back())) {
			return m_container.back();
		}

		void push(const value_type& value) {
			m_container.push_back(value);
		}

		void push(value_type&& value) {
			m_container.push_back(std::move(value));
		}

		void pop() noexcept(noexcept(m_container.pop_back())) {
			m_container.pop_back();
		}

		[[nodiscard]] const Container& getContainer() noexcept {
			return m_container;
		}

	protected:
		container_type m_container{};
	};


} // namespace container

#endif STACK_H
