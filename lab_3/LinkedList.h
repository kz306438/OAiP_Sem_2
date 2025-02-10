#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iterator>  // For iterator support
#include <memory>    // For smart pointers

namespace container
{
	template <typename T>
	class LinkedList {
	private:
		// Node structure containing data and pointer to next node
		struct Node {
			T data;
			std::unique_ptr<Node> next;
			Node(const T& value) : data(value) {}
		};

		std::unique_ptr<Node> head;  // Pointer to the first element
		size_t count;  // Number of elements

	public:
		using value_type		 = T;
		using reference			 = T&;
		using const_reference	 = const T&;
		using size_type			 = size_t;

		LinkedList() : head(nullptr), count(0) {}

		LinkedList				(const LinkedList& other)		 = delete;  // Copy constructor deleted
		LinkedList& operator=	(const LinkedList& other)		 = delete;  // Assignment operator deleted
		LinkedList				(LinkedList&& other) noexcept	 = default;  // Move constructor
		LinkedList& operator=	(LinkedList&& other) noexcept	 = default;  // Move assignment operator
		~LinkedList				() { clear(); }

		// Add element to the begin of the list
		void push_front(const T& value) {
			auto newNode = std::make_unique<Node>(value);
			newNode->next = std::move(head);
			head = std::move(newNode);
			++count;
		}

		// Remove first element 
		void pop_front() noexcept {
			if (head) {
				head = std::move(head->next);
				--count;
			}
		}

		// Add element to the end of the list
		void push_back(const T& value) {
			auto newNode = std::make_unique<Node>(value);
			if (!head) {
				head = std::move(newNode);
			}
			else {
				Node* current = head.get();
				while (current->next) {
					current = current->next.get();
				}
				current->next = std::move(newNode);
			}
			++count;
		}

		// Remove last element
		void pop_back() noexcept {
			if (!head) return;
			if (!head->next) {
				head.reset();
			}
			else {
				Node* current = head.get();
				while (current->next->next) {
					current = current->next.get();
				}
				current->next.reset();
			}
			--count;
		}

		// Get first element
		[[nodiscard]] reference			 front() noexcept		 { return head->data; }
		[[nodiscard]] const_reference	 front() const noexcept	 { return head->data; }

		// Get last element
		[[nodiscard]] reference back() noexcept {
			Node* current = head.get();
			while (current->next) {
				current = current->next.get();
			}
			return current->data;
		}

		[[nodiscard]] const_reference back() const noexcept {
			Node* current = head.get();
			while (current->next) {
				current = current->next.get();
			}
			return current->data;
		}

		// Get size of list
		[[nodiscard]] size_type size() const noexcept { return count; }
		
		// Check for empty
		[[nodiscard]] bool empty() const noexcept { return count == 0; }

		// Clear list
		void clear() noexcept {
			while (!empty()) pop_back();
		}

		// Iterator class
		class iterator {
			Node* current;
		public:
			using iterator_category	 = std::forward_iterator_tag;
			using value_type		 = T;
			using difference_type	 = std::ptrdiff_t;
			using pointer			 = T*;
			using reference			 = T&;

			explicit iterator(Node* node) : current(node) {}

			reference operator*() { return current->data; }
			
			iterator& operator++()	 { current = current->next.get(); return *this; }
			iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
			
			bool operator==(const iterator& other) const { return current == other.current; }
			bool operator!=(const iterator& other) const { return !(*this == other); }
		};

		iterator begin()	 { return iterator(head.get()); }
		iterator end()		 { return iterator(nullptr); }


		// Const iterator class
		class const_iterator {
			const Node* current;
		public:
			using iterator_category		 = std::forward_iterator_tag;
			using value_type			 = T;
			using difference_type		 = std::ptrdiff_t;
			using pointer				 = const T*;
			using reference				 = const T&;

			explicit const_iterator(const Node* node) : current(node) {}

			reference operator*() const { return current->data; }

			const_iterator& operator++()	 { current = current->next.get(); return *this; }
			const_iterator operator++(int)	 { const_iterator tmp = *this; ++(*this); return tmp; }

			bool operator==(const const_iterator& other) const { return current == other.current; }
			bool operator!=(const const_iterator& other) const { return !(*this == other); }
		};

		const_iterator begin()	 const { return const_iterator(head.get()); }
		const_iterator end()	 const { return const_iterator(nullptr); }
	};	

} // namespace container

#endif // LINKED_LIST_H