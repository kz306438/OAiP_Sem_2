#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <memory>
#include <iterator>
#include <cstddef>

namespace container
{
    /**
     * @brief A doubly linked list container that provides basic list operations.
     *
     * This container allows for efficient insertion and removal of elements at both ends of the list.
     * It provides bidirectional iteration and supports both const and non-const iterators.
     *
     * @tparam T Type of elements stored in the list.
     * @tparam Allocator Type of allocator used for memory management (default: std::allocator).
     */
    template <typename T, typename Allocator = std::allocator<T>>
    class List {
    private:
        struct Node {
            T data;       ///< Data stored in the node.
            Node* prev;   ///< Pointer to the previous node.
            Node* next;   ///< Pointer to the next node.

            /**
             * @brief Constructs a new node with specified data.
             * @param value The value to store in the node.
             * @param p Pointer to the previous node (default: nullptr).
             * @param n Pointer to the next node (default: nullptr).
             */
            Node(const T& value, Node* p = nullptr, Node* n = nullptr) noexcept : data(value), prev(p), next(n) {}
        };

        using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        NodeAlloc alloc;  ///< Allocator for the nodes.
        Node* head;       ///< Pointer to the first node.
        Node* tail;       ///< Pointer to the last node.
        size_t sz;        ///< Current size of the list.

    public:
        using value_type = T;                ///< The type of elements stored in the list.
        using reference = T&;               ///< A reference to an element in the list.
        using const_reference = const T&;         ///< A const reference to an element in the list.
        using size_type = size_t;           ///< The type for the size of the list.

        /**
         * @brief Iterator for traversing the list.
         */
        class iterator {
            Node* node;   ///< Pointer to the current node.
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            /**
             * @brief Constructs an iterator for a given node.
             * @param n Pointer to the node.
             */
            iterator(Node* n) noexcept : node(n) {}

            iterator& operator++ ()       noexcept { node = node->next; return *this; }
            iterator  operator++ (int)    noexcept { iterator tmp = *this; ++(*this); return tmp; }
            iterator& operator-- ()       noexcept { node = node->prev; return *this; }
            iterator  operator-- (int)    noexcept { iterator tmp = *this; --(*this); return tmp; }

            reference operator* () const noexcept { return node->data; }
            pointer   operator->() const noexcept { return &node->data; }

            bool         operator== (const iterator& other) const noexcept { return node == other.node; }
            bool         operator!= (const iterator& other) const noexcept { return node != other.node; }
        };

        /**
         * @brief Const iterator for traversing the list.
         */
        class const_iterator {
            const Node* node;  ///< Pointer to the current node.
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            /**
             * @brief Constructs a const iterator for a given node.
             * @param n Pointer to the node.
             */
            const_iterator(const Node* n) noexcept : node(n) {}

            const_iterator& operator++()        noexcept { node = node->next; return *this; }
            const_iterator  operator++(int)     noexcept { const_iterator tmp = *this; ++(*this); return tmp; }
            const_iterator& operator--()        noexcept { node = node->prev; return *this; }
            const_iterator  operator--(int)     noexcept { const_iterator tmp = *this; --(*this); return tmp; }

            reference    operator*() const noexcept { return node->data; }
            pointer      operator->() const noexcept { return &node->data; }

            bool operator==(const const_iterator& other) const noexcept { return node == other.node; }
            bool operator!=(const const_iterator& other) const noexcept { return node != other.node; }
        };

        /**
         * @brief Constructs an empty list.
         */
        List() noexcept : head(nullptr), tail(nullptr), sz(0) {}

        /**
         * @brief Destroys the list and releases all resources.
         */
        ~List() { clear(); }

        /**
         * @brief Adds an element to the back of the list.
         * @param value The value to add.
         */
        void push_back(const T& value) {
            Node* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc, 1);
            std::allocator_traits<NodeAlloc>::construct(alloc, newNode, value, tail, nullptr);
            if (!tail) head = newNode;
            else tail->next = newNode;
            tail = newNode;
            ++sz;
        }

        /**
         * @brief Adds an element to the back of the list (move version).
         * @param value The value to add.
         */
        void push_back(T&& value) {
            Node* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc, 1);
            std::allocator_traits<NodeAlloc>::construct(alloc, newNode, std::move(value), tail, nullptr);
            if (!tail) head = newNode;
            else tail->next = newNode;
            tail = newNode;
            ++sz;
        }

        /**
         * @brief Adds an element to the front of the list.
         * @param value The value to add.
         */
        void push_front(const T& value) {
            Node* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc, 1);
            std::allocator_traits<NodeAlloc>::construct(alloc, newNode, value, nullptr, head);
            if (!head) tail = newNode;
            else head->prev = newNode;
            head = newNode;
            ++sz;
        }

        /**
         * @brief Adds an element to the front of the list (move version).
         * @param value The value to add.
         */
        void push_front(T&& value) {
            Node* newNode = std::allocator_traits<NodeAlloc>::allocate(alloc, 1);
            std::allocator_traits<NodeAlloc>::construct(alloc, newNode, std::move(value), nullptr, head);
            if (!head) tail = newNode;
            else head->prev = newNode;
            head = newNode;
            ++sz;
        }

        /**
         * @brief Removes the last element from the list.
         */
        void pop_back() noexcept {
            if (!tail) return;
            Node* tmp = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
            std::allocator_traits<NodeAlloc>::destroy(alloc, tmp);
            std::allocator_traits<NodeAlloc>::deallocate(alloc, tmp, 1);
            --sz;
        }

        /**
         * @brief Removes the first element from the list.
         */
        void pop_front() noexcept {
            if (!head) return;
            Node* tmp = head;
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
            std::allocator_traits<NodeAlloc>::destroy(alloc, tmp);
            std::allocator_traits<NodeAlloc>::deallocate(alloc, tmp, 1);
            --sz;
        }

        /**
         * @brief Removes all elements from the list.
         */
        void clear() noexcept {
            while (head) pop_front();
        }

        /**
         * @brief Gets the size of the list.
         * @return The number of elements in the list.
         */
        [[nodiscard]] size_t size()  const noexcept { return sz; }

        /**
         * @brief Checks if the list is empty.
         * @return true if the list is empty, false otherwise.
         */
        [[nodiscard]] bool empty()   const noexcept { return sz == 0; }

        /**
         * @brief Gets an iterator to the beginning of the list.
         * @return An iterator to the first element.
         */
        [[nodiscard]] iterator begin()   noexcept { return iterator(head); }

        /**
         * @brief Gets an iterator to the end of the list.
         * @return An iterator to one past the last element.
         */
        [[nodiscard]] iterator end()     noexcept { return iterator(nullptr); }

        /**
         * @brief Gets a const iterator to the beginning of the list.
         * @return A const iterator to the first element.
         */
        [[nodiscard]] const_iterator begin() const noexcept { return const_iterator(head); }

        /**
         * @brief Gets a const iterator to the end of the list.
         * @return A const iterator to one past the last element.
         */
        [[nodiscard]] const_iterator end()   const noexcept { return const_iterator(nullptr); }

        /**
         * @brief Gets the first element of the list.
         * @return A reference to the first element.
         */
        [[nodiscard]] reference          front() noexcept { return head->data; }

        /**
         * @brief Gets the first element of the list (const version).
         * @return A const reference to the first element.
         */
        [[nodiscard]] const_reference    front() const noexcept { return head->data; }

        /**
         * @brief Gets the last element of the list.
         * @return A reference to the last element.
         */
        [[nodiscard]] reference          back() noexcept { return tail->data; }

        /**
         * @brief Gets the last element of the list (const version).
         * @return A const reference to the last element.
         */
        [[nodiscard]] const_reference    back() const noexcept { return tail->data; }
    };

} // container

#endif CONTAINER_LIST_H
