#ifndef CONTAINER_DEQUE_H
#define CONTAINER_DEQUE_H

#include "List.h"

namespace container {

    /**
     * @brief A deque container adaptor that provides front and back access to a list-based container.
     *
     * This class provides the basic operations for a deque, such as adding and removing elements from both ends,
     * as well as accessing the front and back elements. The underlying container is customizable and defaults to List<T>.
     *
     * @tparam T Type of elements stored in the deque.
     * @tparam Container Type of the underlying container, defaulting to List<T>.
     */
    template <typename T, class Container = List<T>>
    class Deque {
    public:
        using value_type = typename Container::value_type; ///< Type of element stored in the container.
        using reference = typename Container::reference; ///< Reference type for mutable elements.
        using const_reference = typename Container::const_reference; ///< Const reference type for immutable elements.
        using size_type = typename Container::size_type; ///< Type for the container's size.
        using container_type = Container; ///< Type of the underlying container.

        static_assert(std::is_same_v<T, value_type>, "container adaptors require consistent types");

        /**
         * @brief Default constructor.
         */
        Deque() = default;

        /**
         * @brief Constructor to initialize from an existing container.
         * @param container The container to initialize the deque with.
         */
        explicit Deque(const Container& container) : m_container(container) {}

        /**
         * @brief Constructor to initialize from an rvalue container.
         * @param container The container to move into the deque.
         */
        explicit Deque(Container&& container) noexcept(std::is_nothrow_move_constructible_v<Container>)
            : m_container(std::move(container)) {}

        /**
         * @brief Checks if the deque is empty.
         * @return True if the deque is empty, false otherwise.
         */
        [[nodiscard]] bool empty() const noexcept(noexcept(m_container.empty())) {
            return m_container.empty();
        }

        /**
         * @brief Returns the size of the deque.
         * @return The number of elements in the deque.
         */
        [[nodiscard]] size_type size() const noexcept(noexcept(m_container.size())) {
            return m_container.size();
        }

        /**
         * @brief Returns a reference to the front element.
         * @return A reference to the front element of the deque.
         */
        [[nodiscard]] reference front() noexcept(noexcept(m_container.front())) {
            return m_container.front();
        }

        /**
         * @brief Returns a const reference to the front element.
         * @return A const reference to the front element of the deque.
         */
        [[nodiscard]] const_reference front() const noexcept(noexcept(m_container.front())) {
            return m_container.front();
        }

        /**
         * @brief Returns a reference to the back element.
         * @return A reference to the back element of the deque.
         */
        [[nodiscard]] reference back() noexcept(noexcept(m_container.back())) {
            return m_container.back();
        }

        /**
         * @brief Returns a const reference to the back element.
         * @return A const reference to the back element of the deque.
         */
        [[nodiscard]] const_reference back() const noexcept(noexcept(m_container.back())) {
            return m_container.back();
        }

        /**
         * @brief Adds an element to the front of the deque.
         * @param value The value to be added to the front of the deque.
         */
        void push_front(const T& value) {
            m_container.push_front(value);
        }

        /**
         * @brief Adds an rvalue element to the front of the deque.
         * @param value The value to be moved to the front of the deque.
         */
        void push_front(T&& value) {
            m_container.push_front(std::move(value));
        }

        /**
         * @brief Adds an element to the back of the deque.
         * @param value The value to be added to the back of the deque.
         */
        void push_back(const T& value) {
            m_container.push_back(value);
        }

        /**
         * @brief Adds an rvalue element to the back of the deque.
         * @param value The value to be moved to the back of the deque.
         */
        void push_back(T&& value) {
            m_container.push_back(std::move(value));
        }

        /**
         * @brief Removes the front element of the deque.
         */
        void pop_front() noexcept(noexcept(m_container.pop_front())) {
            m_container.pop_front();
        }

        /**
         * @brief Removes the back element of the deque.
         */
        void pop_back() noexcept(noexcept(m_container.pop_back())) {
            m_container.pop_back();
        }

    protected:
        container_type m_container; ///< The underlying container.
    };

} // namespace container

#endif // CONTAINER_DEQUE_H
