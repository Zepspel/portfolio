//
// Created by pavel on 30.11.2025.
//

#ifndef INC_3_NODE_H
#define INC_3_NODE_H
#include <utility>


template<typename Key, typename Value>
struct Node {
//    Key key;
//    Value value;

    std::pair<const Key, Value> data;

    Node *left;
    Node *right;
    Node *par;

    Node(const Key& key, const Value& value, Node *par) : data(key, value), left(nullptr), right(nullptr), par(par) {}

    /**
     * used for coping orderedTable
     * @param data
     */
    Node(const std::pair<const Key, Value>& data) : data(data), left(nullptr), right(nullptr), par(nullptr) {}


    /**
     * @brief coping other node without left and right
     * used for coping orderedTable
     * @param other
     */
    //Node(const Node &other) : data(other.data), left(nullptr), right(nullptr), par(other.par) {}

};



#endif //INC_3_NODE_H
