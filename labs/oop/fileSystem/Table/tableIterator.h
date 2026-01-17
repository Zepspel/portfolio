//
// Created by pavel on 29.11.2025.
//

#ifndef INC_3_TABLEITERATOR_H
#define INC_3_TABLEITERATOR_H

#include "node.h"
#include "orderedTable.h"
//#include "tableIterator.cpp"
//#include "tableIterator.h"

template<typename Key, typename Value>
class OrderedTable;

template<typename Key, typename Value, bool isConst>
class ReverseIterator;

template<typename Key, typename Value, bool isConst>
class TableIterator {

    using node_ptr_type = std::conditional_t<isConst, const Node<Key, Value>, Node<Key, Value>>*;
    node_ptr_type node;             //pointer to the node

    using Table = std::conditional_t<isConst, const OrderedTable<Key, Value>, OrderedTable<Key, Value>>;
    //using Table = OrderedTable<Key, Value>;
    Table* table;

    friend class OrderedTable<Key, Value>;

    friend class TableIterator<Key, Value, !isConst>;
    friend ReverseIterator<Key, Value, true>;
    friend ReverseIterator<Key, Value, false>;


public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::pair<const Key, Value>;
    using pointer = std::conditional_t<isConst, const value_type, value_type>*;
    using reference = std::conditional_t<isConst, const value_type, value_type>&;
    using iterator_category = std::bidirectional_iterator_tag;



private:
    //explicit TableIterator(const node_ptr_type node, const Table* table) : node(node), table(table) {}
    explicit TableIterator(node_ptr_type node, Table* table) : node(node), table(table) {}


public:
    TableIterator(): node(nullptr), table(nullptr) {}


    template <bool otherConst>
    explicit TableIterator(TableIterator<Key, Value, otherConst> &o) noexcept requires(isConst >= otherConst): node(o.node), table(o.table) {}

    template<bool otherConst>
    TableIterator& operator =(TableIterator<Key, Value, otherConst> &o) noexcept requires(isConst >= otherConst);

    bool operator ==(const TableIterator &o) const noexcept {return node == o.node;}


    TableIterator& operator ++ () noexcept;
    TableIterator operator ++ (int) noexcept;

    TableIterator& operator -- () noexcept;
    TableIterator operator -- (int) noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;


private:
    //node_ptr_type findMin(node_ptr_type node);
    //node_ptr_type findMax(node_ptr_type node);

};



template<typename Key, typename Value, bool isConst>
template <bool otherConst>
TableIterator<Key, Value, isConst>& TableIterator<Key, Value, isConst>::operator =(TableIterator<Key, Value, otherConst> &o) noexcept
requires(isConst >= otherConst) {
    node = o.node;
    return *this;
}

/*template <typename Key, typename Value, bool isConst>
typename TableIterator<Key, Value, isConst>::node_ptr_type TableIterator<Key, Value, isConst>::findMin(node_ptr_type node) {

    if (node == nullptr)
        return nullptr;

    node_ptr_type curr = node;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}*/


template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst>& TableIterator<Key, Value, isConst>::operator++() noexcept {
    node = table->find_next(node);
    return *this;
}

template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst> TableIterator<Key, Value, isConst>::operator++(int) noexcept {

    TableIterator ret = *this;
    node = table->find_next(node);
    return ret;
}

/*template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst>::node_ptr_type TableIterator<Key, Value, isConst>::findMax(node_ptr_type node) {

    if (node == nullptr)
        return nullptr;

    node_ptr_type curr = node;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr;
}*/


template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst>& TableIterator<Key, Value, isConst>::operator--() noexcept {
    node = table->find_prev(node);
    return *this;
}

template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst> TableIterator<Key, Value, isConst>::operator--(int) noexcept {

    TableIterator ret = *this;
    node = table->find_prev(node);
    return ret;
}


template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst>::reference TableIterator<Key, Value, isConst>::operator*() const noexcept {
    return node->data;
}

template <typename Key, typename Value, bool isConst>
TableIterator<Key, Value, isConst>::pointer TableIterator<Key, Value, isConst>::operator->() const noexcept {
    return &node->data;
}


template<typename Key, typename Value, bool isConst>
class ReverseIterator: public TableIterator<Key, Value, isConst> {
private:
    explicit ReverseIterator(TableIterator<Key, Value, isConst>::node_ptr_type node, TableIterator<Key, Value, isConst>::Table* table) : TableIterator<Key, Value, isConst>(node, table)  {}

    friend class OrderedTable<Key, Value>;
public:
    template <bool otherConst>
    explicit ReverseIterator(ReverseIterator<Key, Value, otherConst> &o) noexcept requires(isConst >= otherConst): TableIterator<Key, Value, isConst>(o) {}


    //auto& operator*() { Iterator tmp = base_it; return *--tmp; }
    ReverseIterator& operator++();
    ReverseIterator operator ++ (int) noexcept;

    ReverseIterator& operator--();
    ReverseIterator operator -- (int) noexcept;
};

template <typename Key, typename Value, bool isConst>
ReverseIterator<Key, Value, isConst>& ReverseIterator<Key, Value, isConst>::operator++() {
    this->node = this->table->find_prev(this->node);
    return *this;
}

template <typename Key, typename Value, bool isConst>
ReverseIterator<Key, Value, isConst> ReverseIterator<Key, Value, isConst>::operator++(int) noexcept {
    ReverseIterator ret = *this;
    this->node = this->table->find_prev(this->node);
    return ret;
}

template <typename Key, typename Value, bool isConst>
ReverseIterator<Key, Value, isConst>& ReverseIterator<Key, Value, isConst>::operator--() {
    this->node = this->table->find_next(this->node);
    return *this;
}

template <typename Key, typename Value, bool isConst>
ReverseIterator<Key, Value, isConst> ReverseIterator<Key, Value, isConst>::operator--(int) noexcept {
    ReverseIterator ret = *this;
    this->node = this->table->find_next(this->node);
    return ret;
}


#endif //INC_3_TABLEITERATOR_H
