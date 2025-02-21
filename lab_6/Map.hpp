#ifndef CONTAINER_MAP_HPP
#define CONTAINER_MAP_HPP

#include <memory>
#include <iterator>
#include <utility>
#include <iostream>
#include <type_traits>
#include <iomanip>

namespace container
{
    namespace detail
    {
        template <typename T>
        concept Tree_type =
            std::is_default_constructible_v<T> &&
            (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>);

        template <typename T, typename... Args>
        concept Tree_NoThrow =
            std::is_nothrow_constructible_v<T, Args&&...> &&
            ((std::is_nothrow_copy_assignable_v<T> && std::is_copy_assignable_v<T>) ||
                (std::is_nothrow_move_assignable_v<T> && std::is_move_assignable_v<T>));


        template <typename Key, typename Value>
        struct Node {
            using value_type = std::pair<Key, Value>;
            value_type data;
            Node* left;
            Node* right;
            Node* parent;
            int height;

            explicit Node(const value_type& val, Node* p = nullptr)
                : data(val), left(nullptr), right(nullptr), parent(p), height(0) {}
        };

        template <typename NodeType>
        struct AVLTreeIterator
        {
            using value_type = typename NodeType::value_type;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator_category = std::bidirectional_iterator_tag;

            explicit AVLTreeIterator(NodeType* current, bool reverse = false)
                : m_current(current), m_reverse(reverse) {}

            bool operator==(const AVLTreeIterator& other) const {
                return other.m_current == m_current && other.m_reverse == m_reverse;
            }

            bool operator!=(const AVLTreeIterator& other) const {
                return !(*this == other);
            }

            AVLTreeIterator& operator++() {
                if (!m_current) return *this;
                if (m_reverse)
                    moveBackward();
                else
                    moveForward();
                return *this;
            }

            AVLTreeIterator& operator++(int) {
                AVLTreeIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            AVLTreeIterator& operator--() {
                if (!m_current) return *this;
                if (m_reverse)
                    moveForward();
                else
                    moveBackward();
                return *this;
            }

            AVLTreeIterator& operator--(int) {
                AVLTreeIterator tmp = *this;
                --(*this);
                return tmp;
            }

            reference operator*()
                requires(!std::is_const_v<value_type>)
            {
                return m_current->data;
            }

            const_reference operator*() const
                requires(std::is_const_v<value_type>)
            {
                return m_current->data;
            }

            pointer operator->()
                requires(!std::is_const_v<value_type>)
            {
                return &m_current->data;
            }

            const_pointer operator->() const
                requires(std::is_const_v<value_type>)
            {
                return &m_current->data;
            }

        private:
            NodeType* m_current = nullptr;
            bool		 m_reverse{};

        private:

            void moveForward() {
                if (m_current->right) {
                    m_current = m_current->right;
                    while (m_current->left) {
                        m_current = m_current->left;
                    }
                }
                else {
                    NodeType* parent = m_current->parent;
                    while (parent && m_current == parent->right) {
                        m_current = parent;
                        parent = parent->parent;
                    }
                    m_current = parent;
                }
            }


            void moveBackward() {
                if (m_current->left) {
                    m_current = m_current->left;
                    while (m_current->right)
                        m_current = m_current->right;
                }
                else {
                    NodeType* parent = m_current->parent;
                    while (parent && m_current == parent->left) {
                        m_current = parent;
                        parent = parent->parent;
                    }
                    m_current = parent;
                }
            }
        };

        template <Tree_type Key, Tree_type Value,
            typename Compare = std::less<Key>,
            typename Allocator = std::allocator<std::pair<Key, Value>>>
        class AVLTree {
        public:
            using key_type = Key;
            using mapped_type = Value;
            using value_type = std::pair<Key, Value>;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using key_compare = Compare;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using node_type = Node<key_type, mapped_type>;
            using iterator = AVLTreeIterator<node_type>;
            using const_iterator = AVLTreeIterator<const node_type>;
            using reverse_iterator = AVLTreeIterator<node_type>;
            using const_reverse_iterator = AVLTreeIterator<const node_type>;

            using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;
            using allocator_type = NodeAllocator;

        private:
            node_type* m_root;
            size_type m_size{};
            Compare m_comp;
            NodeAllocator m_alloc;

        public:
            explicit AVLTree(NodeAllocator allocator = NodeAllocator())
                : m_alloc(allocator), m_root(nullptr) {}
            ~AVLTree() { destroy(m_root); }

            [[nodiscard]] allocator_type get_allocator() const {
                return m_alloc;
            }

            // Element Access
            mapped_type& at(const key_type& key) {
                node_type* node = findNode(key);
                if (!node) {
                    throw std::out_of_range("container::AVLTree::at");
                }
                return node->data.second;
            }

            const mapped_type& at(const key_type& key) const {
                node_type* node = findNode(key);
                if (!node) {
                    throw std::out_of_range("container::AVLTree::at");
                }
                return node->data.second;
            }

            mapped_type& operator[](const key_type& key) {
                node_type* node = findNode(key);
                return node->data.second;
            }

            mapped_type& operator[](key_type&& key) {
                node_type* node = findNode(key);
                return node->data.second;
            }

            void insert(const value_type& val) {
                m_root = insertUtil(m_root, val, nullptr);
            }

            void remove(const key_type& x) {
                m_root = removeUtil(m_root, x);
            }

            iterator find(const key_type& key) {
                node_type* node = findNode(key);
                if (node)
                    return iterator(node);
                else
                    return end();
            }

            const_iterator find(const key_type& key) const {
                node_type* node = findNode(key);
                if (node)
                    return iterator(node);
                else
                    return end();
            }

            // Iterators 

            iterator begin() { return iterator(min()); }

            const_iterator begin() const { return const_iterator(min()); }

            const_iterator cbegin() const noexcept {
                return const_iterator(min());
            }

            iterator end() { return iterator(nullptr); }

            const_iterator end() const { return const_iterator(nullptr); }

            const_iterator cend() const noexcept {
                return const_iterator(nullptr);
            }

            reverse_iterator rbegin() { return iterator(max(), true); }

            const_reverse_iterator rbegin() const {
                return const_iterator(max(), true);
            }

            const_reverse_iterator crbegin() const noexcept {
                return const_iterator(max(), true);
            }

            reverse_iterator rend() { return iterator(nullptr, true); }

            const_reverse_iterator rend() const {
                return const_iterator(nullptr, true);
            }

            const_reverse_iterator crend() const noexcept {
                return const_iterator(nullptr, true);
            }

            // Capacity
            [[nodiscard]] size_type size() const noexcept { return m_size; }

            [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

            void inorder() {
                inorderUtil(m_root);
                std::cout << std::endl;
            }

            void preorder() {
                preorderUtil(m_root);
                std::cout << std::endl;
            }

            void postorder() {
                postorderUtil(m_root);
                std::cout << std::endl;
            }

        private:

            void destroy(node_type* head) {
                if (!head) return;
                destroy(head->left);
                destroy(head->right);
                std::allocator_traits<NodeAllocator>::destroy(m_alloc, head);
                std::allocator_traits<NodeAllocator>::deallocate(m_alloc, head, 1);
            }

            int height(node_type* head) {
                return head ? head->height : 0;
            }

            node_type* rightRotation(node_type* head) {
                node_type* newhead = head->left;
                head->left = newhead->right;
                if (newhead->right) newhead->right->parent = head;
                newhead->right = head;
                head->parent = newhead;
                head->height = 1 + std::max(height(head->left), height(head->right));
                newhead->height = 1 + std::max(height(newhead->left), height(newhead->right));
                return newhead;
            }

            node_type* leftRotation(node_type* head) {
                node_type* newhead = head->right;
                head->right = newhead->left;
                if (newhead->left) newhead->left->parent = head;
                newhead->left = head;
                head->parent = newhead;
                head->height = 1 + std::max(height(head->left), height(head->right));
                newhead->height = 1 + std::max(height(newhead->left), height(newhead->right));
                return newhead;
            }

            void inorderUtil(node_type* head) {
                if (!head) return;
                inorderUtil(head->left);
                std::cout << head->data.first << ": " << head->data.second << std::endl;
                inorderUtil(head->right);
            }

            void preorderUtil(node_type* head) {
                if (!head) return;
                std::cout << head->data.first << ": " << head->data.second << std::endl;
                preorderUtil(head->left);
                preorderUtil(head->right);
            }

            void postorderUtil(node_type* head) {
                if (!head) return;
                postorderUtil(head->left);
                postorderUtil(head->right);
                std::cout << head->data.first << ": " << head->data.second << std::endl;
            }

            node_type* insertUtil(node_type* head, const value_type& val, node_type* parent) {
                if (!head) {
                    ++m_size;
                    node_type* temp = std::allocator_traits<NodeAllocator>::allocate(m_alloc, 1);
                    std::allocator_traits<NodeAllocator>::construct(m_alloc, temp, val);
                    temp->parent = parent;
                    return temp;
                }
                if (m_comp(val.first, head->data.first)) head->left = insertUtil(head->left, val, head);
                else if (m_comp(head->data.first, val.first)) head->right = insertUtil(head->right, val, head);

                head->height = 1 + std::max(height(head->left), height(head->right));
                int bal = height(head->left) - height(head->right);

                if (bal > 1) {
                    if (m_comp(val.first, head->left->data.first)) return rightRotation(head);
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
                if (bal < -1) {
                    if (m_comp(head->right->data.first, val.first)) return leftRotation(head);
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
                return head;
            }

            node_type* removeUtil(node_type* head, const key_type& x) {
                if (!head) return nullptr;
                if (m_comp(x, head->data.first)) head->left = removeUtil(head->left, x);
                else if (m_comp(head->data.first, x)) head->right = removeUtil(head->right, x);
                else {
                    node_type* r = head->right;
                    if (!r) {
                        node_type* l = head->left;
                        std::allocator_traits<NodeAllocator>::destroy(m_alloc, head);
                        std::allocator_traits<NodeAllocator>::deallocate(m_alloc, head, 1);
                        head = l;
                    }
                    else if (!head->left) {
                        std::allocator_traits<NodeAllocator>::destroy(m_alloc, head);
                        std::allocator_traits<NodeAllocator>::deallocate(m_alloc, head, 1);
                        head = r;
                    }
                    else {
                        while (r->left) r = r->left;
                        head->data = r->data;
                        head->right = removeUtil(head->right, r->data.first);
                    }
                }
                if (!head) return head;
                head->height = 1 + std::max(height(head->left), height(head->right));
                int bal = height(head->left) - height(head->right);

                if (bal > 1) {
                    if (height(head->left) >= height(head->right)) return rightRotation(head);
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
                if (bal < -1) {
                    if (height(head->right) >= height(head->left)) return leftRotation(head);
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
                return head;
            }

            node_type* minNode(node_type* head) {
                while (head && head->left) {
                    head = head->left;
                }
                return head;
            }

            node_type* maxNode(node_type* head) {
                while (head && head->right) {
                    head = head->right;
                }
                return head;
            }

            node_type* min() {
                return minNode(m_root);
            }

            node_type* max() {
                return maxNode(m_root);
            }

            node_type* findNode(const key_type& key) {
                node_type* current = m_root;
                while (current) {
                    if (m_comp(key, current->data.first)) {
                        current = current->left;
                    }
                    else if (m_comp(current->data.first, key)) {
                        current = current->right;
                    }
                    else {
                        return current;
                    }
                }
                return nullptr;
            }
        };
    } // namespace detail


    template <detail::Tree_type Key, detail::Tree_type Value,
        typename Compare = std::less<Key>,
        typename Allocator = std::allocator<std::pair<Key, Value>>>
    class Map : public detail::AVLTree<Key, Value, Compare, Allocator> {
        using tree_type = detail::AVLTree<Key, Value, Compare, Allocator>;
        using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<detail::Node<Key, Value>>;
    public:
        explicit Map(NodeAllocator allocator = NodeAllocator())
            : tree_type(allocator) {}
    };

} // namespace container

#endif // CONTAINER_MAP_HPP
