//
// Created by pavel on 21.11.2025.
//

#ifndef INC_3_TABLE_H
#define INC_3_TABLE_H

#include <cstddef>
#include <string>
#include <limits>
#include <stack>
#include <concepts>
#include <stdexcept>

#include "node.h"
#include "tableIterator.h"

#include "tableIterator.h"




template<typename Key, typename Value, bool isConst>
class TableIterator;

template<typename Key, typename Value, bool isConst>
class ReverseIterator;

/**
 * @brief ordered table based on binary tree
 * @tparam Key
 * @tparam Value
 */


template<typename Key, typename Value>
class OrderedTable {

public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<const key_type, mapped_type>;
    using pointer = Value*;
    using const_pointer = const Value*;
    using reference = Value&;
    using const_reference = const Value&;
    using iterator = TableIterator<Key, Value, false>;
    using const_iterator = TableIterator<Key, Value, true>;
    using reverse_iterator = ReverseIterator<Key, Value, false>;
    using const_reverse_iterator = ReverseIterator<Key, Value, true>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

private:
    size_type size_;
    Node<Key, Value> *root;



    Node<Key, Value> *min;    // кэш минимального элемента для того чтобы .begin() было O(1)
    Node<Key, Value> *max;

    friend TableIterator<Key, Value, true>;
    friend TableIterator<Key, Value, false>;
    friend ReverseIterator<Key, Value, true>;
    friend ReverseIterator<Key, Value, false>;

    /**
     * @brief find node by key
     * @param key for searching
     * @return Node with such key
     */
    const Node<Key, Value>* find_node(const key_type& key) const;
    Node<Key, Value>* find_node(const key_type& key);

    Node<Key, Value>* find_pos(const key_type& key);

    const Node<Key, Value>* find_next(const Node<Key,Value> *node) const;
    Node<Key, Value>* find_next(Node<Key, Value>* node);

    const Node<Key, Value>* find_min(const Node<Key, Value> *node) const;
    Node<Key, Value>* find_min(Node<Key, Value>* node);


    const Node<Key, Value>* find_prev(const Node<Key, Value> *node) const;
    Node<Key, Value>* find_prev(const Node<Key, Value> *node);
    const Node<Key, Value>* find_max(const Node<Key, Value> *node) const;

    Node<Key, Value>* find_larger(const key_type& key, bool equal);

    /**
     * @brief used in copy constructor and copy assigment
     * @param other
     */
    void copy(const OrderedTable &other);

    /**
     * @brief reconnect nodes for deliting
     * @param node which will be deleted
     */
    void disconnect(Node<Key, Value> *node);

    void swap(OrderedTable& other) noexcept;


public:
    ~OrderedTable();


    OrderedTable() : size_(0), root(nullptr), min(nullptr), max(nullptr) {}
    OrderedTable(const OrderedTable& other);
    OrderedTable(OrderedTable&& other) noexcept : size_(other.size_), root(other.root), min(other.min), max(other.max) {other.size_ = 0; other.root = nullptr; other.min = nullptr; other.max = nullptr;}

    OrderedTable& operator=(const OrderedTable& other);
    OrderedTable& operator=(OrderedTable&& other) noexcept;

    iterator begin() noexcept {return iterator(min, this);}
    iterator end() noexcept {return iterator(nullptr, this);}
    const_iterator begin() const noexcept {return const_iterator(min, this);}
    const_iterator end() const noexcept {return const_iterator(nullptr, this);}

    const_iterator cbegin() const noexcept {return begin();}        //нужен, если хотим получить константный итератор у неконстантного объекта
    const_iterator cend() const noexcept {return end();}

    reverse_iterator rbegin() noexcept {return reverse_iterator(max, this);}
    reverse_iterator rend() noexcept {return reverse_iterator(nullptr, this);}
    const_reverse_iterator rbegin() const noexcept {return const_reverse_iterator(max, this);}
    const_reverse_iterator rend() const noexcept {return const_reverse_iterator(nullptr, this);}


    bool empty() const noexcept {return size_ == 0;}
    size_type size() const noexcept {return size_;}
    size_type max_size() const noexcept {return std::numeric_limits<size_type>::max();}

    void clear() noexcept;
    std::pair<iterator, bool> insert(const value_type& value);
    iterator erase(iterator pos);
    size_type erase(const key_type& key);

    iterator find(const key_type& key) {return iterator(find_node(key), this);}
    const_iterator find(const key_type& key) const {return const_iterator(find_node(key), this);}
    size_type count(const key_type& key) const {return find_node(key) != nullptr;}

    mapped_type& operator[](const key_type& key);
    mapped_type& at(const key_type& key);
    const mapped_type& at(const key_type& key) const;

    iterator lower_bound(const key_type& key);      // fisrt ≥ key
    iterator upper_bound(const key_type& key);      // first > key

    std::pair<iterator, bool> insert_or_assign(const key_type& k, const mapped_type& v);


     /**
     * @details constructs an object inside a container, and an object can have different constructors,
     * so the number of arguments is variable
     */
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
};









template<typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_node(const key_type& key) {
    return const_cast<Node<Key, Value>*>(
       static_cast<const OrderedTable*>(this)->find_node(key)
   );
}

template<typename Key, typename Value>
const Node<Key, Value>* OrderedTable<Key, Value>::find_node(const key_type& key) const {
    Node<Key, Value>* curr = root;
    while (curr != nullptr && curr->data.first != key) {
        if (key < curr->data.first) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return curr;
}

/**
 * @brief find position for insertion
 * @param key
 * @return if no such key --> pointer to par for new node
 * else node with such key already exists --> pointer to this node
 */
template<typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_pos(const key_type& key){
    Node<Key, Value> *child = root;
    Node<Key, Value> *pos = root;

    while (child != nullptr && key != pos->data.first) {
        pos = child;
        if (key < child->data.first) {
            child = child->left;
        }
        else {
            child = child->right;
        }
    }
    return pos;
}


template <typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_next(Node<Key, Value>* node) {
    return const_cast<Node<Key, Value>*>(
        static_cast<const OrderedTable*>(this)->find_next(node)
    );
}

template <typename Key, typename Value>
const Node<Key, Value>* OrderedTable<Key, Value>::find_next(const Node<Key, Value>* node) const {
    if (node == nullptr)
        return nullptr;

    //smallest in the right subtree
    if (node->right != nullptr) {
        node = find_min(node->right);
        return node;
    }

    //searching parent that contains current node in the left subtree
    auto tmp = node;
    auto par_tmp = tmp->par;

    while (par_tmp != nullptr && par_tmp->left != tmp) {
        tmp = par_tmp;
        par_tmp = par_tmp->par;
    }

    return par_tmp;
}


template <typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_min(Node<Key, Value>* node) {
    return const_cast<Node<Key, Value>*>(
        static_cast<const OrderedTable*>(this)->find_min(node)
    );
}

template <typename Key, typename Value>
const Node<Key, Value>* OrderedTable<Key, Value>::find_min(const Node<Key, Value>* node) const {
    if (node == nullptr)
        return nullptr;

    auto curr = node;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}

template <typename Key, typename Value>
const Node<Key, Value>* OrderedTable<Key, Value>::find_prev(const Node<Key, Value>* node) const {

    if (node == nullptr) { //if iterator points to the end we need to go to the last node
        return max;
    }

    //largest in the left subtree
    if (node->left != nullptr) {
        node = find_max(node->left);
        return node;
    }

    //searching parent that contains current node in the right subtree
    auto tmp = node;
    auto par_tmp = tmp->par;

    while (par_tmp != nullptr && par_tmp->right != tmp) {
        tmp = par_tmp;
        par_tmp = par_tmp->par;
    }

    return par_tmp;
}

template <typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_prev(const Node<Key, Value>* node) {
    return const_cast<Node<Key, Value>*>(
    static_cast<const OrderedTable*>(this)->find_prev(node)
);
}


template <typename Key, typename Value>
const Node<Key, Value>* OrderedTable<Key, Value>::find_max(const Node<Key, Value>* node) const {
    if (node == nullptr)
        return nullptr;

    auto curr = node;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr;
}


/*template <typename Key, typename Value>
void OrderedTable<Key, Value>::clear() noexcept {
    if (root == nullptr) return;

    std::stack<Node<Key, Value>*> stack;
    Node<Key, Value>* curr = root;
    Node<Key, Value>* last = nullptr;

    while (!stack.empty() || curr != nullptr) {
        if (curr) {
            stack.push(curr);
            curr = curr->left;
        }
        else {
            Node<Key, Value>* node = stack.top();
            if (node->right && last != node->right) { //if we already was in the right subtree then skip
                curr = node->right;
            }
            else {
                delete node;
                last = node;                //remember that we processed this node
                stack.pop();
            }
        }
    }

    root = nullptr;
    size_ = 0;
    min = nullptr;
    max = nullptr;
}*/

template <typename Key, typename Value>
void OrderedTable<Key, Value>::clear() noexcept {
    if (root == nullptr) return;

    Node<Key, Value>* curr = root;
    Node<Key, Value>* last = nullptr;

    while (curr) {
        if (!curr->left && !curr->right) {
            last = curr;
            curr = curr->par;
            if (curr)
                curr->left = nullptr;
            delete last;
        } else if (curr->left) {
            curr = curr->left;
        } else if (curr->right && last != curr->right) {
            curr = curr->right;
        } else {
            last = curr;
            curr = curr->par;
            if (curr)
                curr->left = nullptr;
            delete last;
        }
    }
    size_ = 0;
    root = nullptr;
    min = max = nullptr;
}

template <typename Key, typename Value>
OrderedTable<Key, Value>& OrderedTable<Key, Value>::operator=(const OrderedTable& other) {
    if (this == &other)
        return *this;

    clear();
    copy(other);
    return *this;
}

template <typename Key, typename Value>
OrderedTable<Key, Value>& OrderedTable<Key, Value>::operator=(OrderedTable&& other) noexcept {
    if (this == &other)
        return *this;

    swap(other);
    other.clear();

    return *this;
}

template <typename Key, typename Value>
void OrderedTable<Key, Value>::copy(const OrderedTable& other) {
    if (other.root == nullptr) return;


    Node<Key, Value>* curr = other.root;
    Node<Key, Value>* parent = nullptr;
    Node<Key, Value>* node_new = nullptr;

    bool its_root = true;
    while (curr) {
        node_new = new Node<Key, Value>(curr->data);
        node_new->par = parent;

        if (parent) {
            if (node_new->data.first < parent->data.first)
                parent->left = node_new;
            else
                parent->right = node_new;
        }
        parent = node_new;

        if (its_root) {
            root = node_new;
            its_root = false;
        }

        if (curr->left && !parent->left) {  //идем максимально влево
            curr = curr->left;
        }
        else {                              //пробуем уйти вправо, если так и не смогли уйти, то стоп
            if (!min) {
                min = node_new;
            }
            while (curr) {
                if (curr->right && !parent->right) {
                    curr = curr->right;
                    break;
                }
                curr = curr->par;
                if (parent)
                    parent = parent->par;
            }
        }
    }
    max = node_new;
    size_ = other.size_;
}


template <typename Key, typename Value>
OrderedTable<Key, Value>::OrderedTable(const OrderedTable& other): size_(other.size_), root(nullptr), min(nullptr), max(nullptr) {
    copy(other);
}




template<typename Key, typename Value>
std::pair<typename OrderedTable<Key, Value>::iterator, bool> OrderedTable<Key, Value>::insert(const value_type& value) {

    Node<Key, Value> *pos = find_pos(value.first);
    Node<Key, Value> *new_node = pos;


    if (pos == nullptr) {                                                       //empty tree
        root = new Node<Key, Value>(value.first, value.second, nullptr);
        min = root;
        max = root;
        ++size_;
        return {iterator(root, this), true};
    }

    if (value.first == pos->data.first) {                   //if value.first == pos->key (key already exists)
        return {iterator(pos, this), false};
    }

    if (value.first < pos->data.first) {
        pos->left = new Node<Key, Value>(value.first, value.second, pos);
        new_node = pos->left;
    }
    else {
        pos->right = new Node<Key, Value>(value.first, value.second, pos);
        new_node = pos->right;
    }


    if (value.first < min->data.first) {
        min = new_node;
    }
    else if (value.first > max->data.first) {
        max = new_node;
    }

    ++size_;
    return {iterator(new_node, this), true};
}




template<typename Key, typename Value>
std::pair<typename OrderedTable<Key, Value>::iterator, bool> OrderedTable<Key, Value>::insert_or_assign(const key_type& key, const mapped_type& value) {
    auto [it, inserted] = insert({key, value});

    if (!inserted)
        it->second = value;

    return {it, inserted};
}

template <typename Key, typename Value>
template <typename ... Args>
std::pair<typename OrderedTable<Key, Value>::iterator, bool> OrderedTable<Key, Value>::emplace(Args&&... args) {

    value_type value(std::forward<Args>(args)...);

    Node<Key, Value> *pos = find_pos(value.first);
    Node<Key, Value> *new_node = pos;


    if (pos == nullptr) {                                                       //empty tree
        root = new Node<Key, Value>(value.first, value.second, nullptr);
        min = root;
        max = root;
        ++size_;
        return {iterator(root, this), true};
    }

    if (value.first == pos->data.first) {                   //if value.first == pos->key (key already exists)
        return {iterator(pos, this), false};
    }

    if (value.first < pos->data.first) {
        pos->left = new Node<Key, Value>(value.first, value.second, pos);
        new_node = pos->left;
    }
    else {
        pos->right = new Node<Key, Value>(value.first, value.second, pos);
        new_node = pos->right;
    }


    if (value.first < min->data.first) {
        min = new_node;
    }
    else if (value.first > max->data.first) {
        max = new_node;
    }

    ++size_;
    return {iterator(new_node, this), true};
}



template <typename Key, typename Value>
OrderedTable<Key, Value>::iterator OrderedTable<Key, Value>::erase(iterator pos) {
    --size_;
    auto ret = pos;
    ++ret;
    Node<Key, Value>* node = pos.node;

    if (node == root) {
        root = find_next(node);
    }
    if (node == min) {
        min = find_next(node);
    }
    if (node == max) {
        max = find_prev(node);
    }

    if (node->right == nullptr || node->left == nullptr) {
        disconnect(node);
        delete node;
        return ret;
    }

    //case when pos node has 2 child
    auto actual = find_next(node);
    disconnect(actual);          //disconnect and move actual to the node place

    if (node->par->right == node) {
        node->par->right = actual;
    }
    else {
        node->par->left = actual;
    }

    actual->left = node->left;
    actual->right = node->right;

    delete node;

    return ret;
}

template<typename Key, typename Value>
OrderedTable<Key, Value>::size_type OrderedTable<Key, Value>::erase(const key_type& key) {
    Node<Key, Value>* pos = find_node(key);
    if (pos == nullptr)
        return 0;

    erase(iterator(pos, this));
    return 1;
}



template <typename Key, typename Value>
void OrderedTable<Key, Value>::disconnect(Node<Key, Value>* node) {

    if (node->right && node->par) {         //in the second condition check if it's root
        if (node->par->right == node) {
            node->par->right = node->right;

        }
        else {
            node->par->left = node->right;
        }
        node->right->par = node->par;
    }
    else if (node->left && node->par) {
        if (node->par->right == node) {
            node->par->right = node->left;
        }
        else {
            node->par->left = node->left;
        }
        node->left->par = node->par;
    }
    else if (node->par) {
        if (node->par->right == node) {
            node->par->right = nullptr;
        }
        else
            node->par->left = nullptr;
    }

}

template <typename Key, typename Value>
void OrderedTable<Key, Value>::swap(OrderedTable& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(root,  other.root);
    std::swap(min,   other.min);
    std::swap(max,   other.max);
}

template <typename Key, typename Value>
OrderedTable<Key, Value>::~OrderedTable() {
    clear();
}


template <typename Key, typename Value>
OrderedTable<Key, Value>::mapped_type& OrderedTable<Key, Value>::operator[](const key_type& key) {
    if (Node<Key, Value>* node = find_node(key)) {
        return node->data.second;
    }

    auto ret = insert({key, mapped_type{}});
    return ret.first->second;
}

template <typename Key, typename Value>
OrderedTable<Key, Value>::mapped_type& OrderedTable<Key, Value>::at(const key_type& key) {
    if (Node<Key, Value>* node = find_node(key)) {
        return node->data.second;
    }
    throw std::out_of_range("Key not found");
}

template <typename Key, typename Value>
const OrderedTable<Key, Value>::mapped_type& OrderedTable<Key, Value>::at(const key_type& key) const {
    if (auto node = find_node(key)) {
        return node->data.second;
    }
    throw std::out_of_range("Key not found");
}

template <typename Key, typename Value>
OrderedTable<Key, Value>::iterator OrderedTable<Key, Value>::lower_bound(const key_type& key) {
    auto node = find_larger(key, true);
    return iterator(node, this);
}

template <typename Key, typename Value>
OrderedTable<Key, Value>::iterator OrderedTable<Key, Value>::upper_bound(const key_type& key) {
    auto node = find_larger(key, false);
    return iterator(node, this);
}


template <typename Key, typename Value>
Node<Key, Value>* OrderedTable<Key, Value>::find_larger(const key_type& key, bool equal) {
    Node<Key, Value>* curr = root;
    Node<Key, Value>* candidate = nullptr;
    while (curr != nullptr) {
        if (equal && curr->data.first == key) {
            return curr;
        }
        if (key < curr->data.first) {
            candidate = curr;
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return candidate;
}




#endif //INC_3_TABLE_H
